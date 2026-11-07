#include "GTA1VehiclePawn.h"

#include "GTA1Character.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AGTA1VehiclePawn::AGTA1VehiclePawn()
{
    PrimaryActorTick.bCanEverTick = true;

    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleBody"));
    SetRootComponent(Body);
    Body->SetCollisionProfileName(TEXT("BlockAll"));
    Body->SetSimulatePhysics(true);
    Body->SetLinearDamping(0.18f);
    Body->SetAngularDamping(2.4f);
    Body->SetMassOverrideInKg(NAME_None, 4200.0f, true);
    Body->SetRelativeScale3D(FVector(2.6f, 1.25f, 0.62f));
    Body->SetNotifyRigidBodyCollision(true);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        Body->SetStaticMesh(CubeMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(
        TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (CylinderMesh.Succeeded())
    {
        WheelMesh = CylinderMesh.Object;
    }

    EntryTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("EntryTrigger"));
    EntryTrigger->SetupAttachment(Body);
    EntryTrigger->SetBoxExtent(FVector(45.0f, 90.0f, 120.0f));
    EntryTrigger->SetRelativeLocation(FVector(35.0f, 0.0f, 0.0f));
    EntryTrigger->SetCollisionProfileName(TEXT("Trigger"));
    EntryTrigger->SetGenerateOverlapEvents(true);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("VehicleCameraBoom"));
    CameraBoom->SetupAttachment(Body);
    CameraBoom->TargetArmLength = 250.0f;
    CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
    CameraBoom->SetAbsolute(false, false, true);
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VehicleCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    const FVector WheelPositions[] = {
        FVector(58.0f, -52.0f, -40.0f),
        FVector(58.0f, 52.0f, -40.0f),
        FVector(-58.0f, -52.0f, -40.0f),
        FVector(-58.0f, 52.0f, -40.0f)
    };

    for (int32 Index = 0; Index < UE_ARRAY_COUNT(WheelPositions); ++Index)
    {
        UStaticMeshComponent* Wheel = CreateDefaultSubobject<UStaticMeshComponent>(
            *FString::Printf(TEXT("Wheel_%d"), Index));
        Wheel->SetupAttachment(Body);
        Wheel->SetRelativeLocation(WheelPositions[Index]);
        Wheel->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
        Wheel->SetRelativeScale3D(FVector(0.27f, 0.14f, 0.7f));
        Wheel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        if (WheelMesh)
        {
            Wheel->SetStaticMesh(WheelMesh);
        }
        Wheels.Add(Wheel);
    }
}

void AGTA1VehiclePawn::BeginPlay()
{
    Super::BeginPlay();
    EntryTrigger->OnComponentBeginOverlap.AddDynamic(this, &AGTA1VehiclePawn::OnVehicleBeginOverlap);
    EntryTrigger->OnComponentEndOverlap.AddDynamic(this, &AGTA1VehiclePawn::OnVehicleEndOverlap);
}

void AGTA1VehiclePawn::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!Body->IsSimulatingPhysics())
    {
        return;
    }

    const FVector Velocity = Body->GetPhysicsLinearVelocity();
    const float Speed = Velocity.Size();
    if (Speed > MaximumSpeed)
    {
        Body->SetPhysicsLinearVelocity(Velocity.GetSafeNormal() * MaximumSpeed);
    }

    if (FMath::Abs(ThrottleInput) > KINDA_SMALL_NUMBER)
    {
        Body->AddForce(GetActorForwardVector() * ThrottleInput * EngineForce);
    }

    if (bBrakePressed && Speed > 1.0f)
    {
        Body->AddForce(-Velocity.GetSafeNormal() * BrakeForce);
    }

    if (FMath::Abs(SteeringInput) > KINDA_SMALL_NUMBER && Speed > 80.0f)
    {
        const float SpeedFactor = FMath::Clamp(Speed / 700.0f, 0.2f, 1.0f);
        Body->AddTorqueInRadians(
            FVector(0.0f, 0.0f, SteeringInput * SteeringTorque * SpeedFactor),
            NAME_None,
            false);
    }

    if (GetController())
    {
        if (GEngine)
        {
            const float Kph = Speed * 0.036f;
            const FString HudText = FString::Printf(TEXT("Суръат: %.0f км/соат  |  E: баромадан  |  C: камера"), Kph);
            GEngine->AddOnScreenDebugMessage(GetUniqueID(), 0.0f, FColor::White, HudText);
        }
    }
}

void AGTA1VehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("VehicleThrottle"), this, &AGTA1VehiclePawn::SetThrottle);
    PlayerInputComponent->BindAxis(TEXT("VehicleSteer"), this, &AGTA1VehiclePawn::SetSteering);
    PlayerInputComponent->BindAction(TEXT("VehicleBrake"), IE_Pressed, this, &AGTA1VehiclePawn::SetBrake, true);
    PlayerInputComponent->BindAction(TEXT("VehicleBrake"), IE_Released, this, &AGTA1VehiclePawn::SetBrake, false);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AGTA1VehiclePawn::Exit);
    PlayerInputComponent->BindAction(TEXT("VehicleCamera"), IE_Pressed, this, &AGTA1VehiclePawn::ToggleCamera);
}

bool AGTA1VehiclePawn::CanEnter() const
{
    return !PreviousDriver.IsValid() && NearbyCharacter.IsValid();
}

void AGTA1VehiclePawn::Enter(AGTA1Character* Character)
{
    if (!Character || NearbyCharacter.Get() != Character || !CanEnter())
    {
        return;
    }

    AController* PlayerController = Character->GetController();
    if (!PlayerController)
    {
        return;
    }

    PreviousDriver = Character;
    Character->SetNearbyVehicle(nullptr);
    PlayerController->Possess(this);
}

void AGTA1VehiclePawn::Exit()
{
    AGTA1Character* Character = PreviousDriver.Get();
    AController* PlayerController = GetController();
    if (!Character || !PlayerController)
    {
        return;
    }

    const FVector ExitLocation = GetActorLocation() - GetActorForwardVector() * 260.0f +
        GetActorRightVector() * 190.0f + FVector(0.0f, 0.0f, 120.0f);
    Character->SetActorLocation(ExitLocation, false, nullptr, ETeleportType::TeleportPhysics);
    PlayerController->Possess(Character);
    Character->SetNearbyVehicle(this);
    PreviousDriver.Reset();
    ThrottleInput = 0.0f;
    SteeringInput = 0.0f;
    bBrakePressed = false;
}

void AGTA1VehiclePawn::SetThrottle(float Value)
{
    ThrottleInput = FMath::Clamp(Value, -1.0f, 1.0f);
}

void AGTA1VehiclePawn::SetSteering(float Value)
{
    SteeringInput = FMath::Clamp(Value, -1.0f, 1.0f);
}

void AGTA1VehiclePawn::SetBrake(bool bPressed)
{
    bBrakePressed = bPressed;
}

void AGTA1VehiclePawn::ToggleCamera()
{
    bInteriorCamera = !bInteriorCamera;
    CameraBoom->TargetArmLength = bInteriorCamera ? 14.0f : 250.0f;
    CameraBoom->SetRelativeLocation(
        bInteriorCamera ? FVector(45.0f, 0.0f, 95.0f) : FVector(0.0f, 0.0f, 120.0f));
}

void AGTA1VehiclePawn::OnVehicleBeginOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (AGTA1Character* Character = Cast<AGTA1Character>(OtherActor))
    {
        NearbyCharacter = Character;
        Character->SetNearbyVehicle(this);
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 3.0f, FColor::White, TEXT("E-ро пахш кунед: ба мошин нишастан"));
        }
    }
}

void AGTA1VehiclePawn::OnVehicleEndOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (AGTA1Character* Character = Cast<AGTA1Character>(OtherActor))
    {
        if (NearbyCharacter.Get() == Character)
        {
            Character->SetNearbyVehicle(nullptr);
            NearbyCharacter.Reset();
        }
    }
}
