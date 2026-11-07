#include "GTA1Character.h"

#include "GTA1MissionActor.h"
#include "GTA1SaveGame.h"
#include "GTA1VehiclePawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/SaveGame.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/StaticMesh.h"
#include "InputCoreTypes.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Math/RotationMatrix.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
    const TCHAR* SaveSlotName = TEXT("GTA1_Player");
}

AGTA1Character::AGTA1Character()
{
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 520.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 350.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BaseMaterial(
        TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));

    if (BaseMaterial.Succeeded())
    {
        BlockoutBaseMaterial = BaseMaterial.Object;
    }

    const auto ConfigureBlockoutPart = [this, &CubeMesh](
        UStaticMeshComponent* Part,
        const FVector& Location,
        const FVector& Scale)
    {
        Part->SetupAttachment(GetCapsuleComponent());
        Part->SetRelativeLocation(Location);
        Part->SetRelativeScale3D(Scale);
        Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Part->SetGenerateOverlapEvents(false);
        if (CubeMesh.Succeeded())
        {
            Part->SetStaticMesh(CubeMesh.Object);
        }
        if (BlockoutBaseMaterial)
        {
            Part->SetMaterial(0, BlockoutBaseMaterial);
        }
    };

    TorsoBlockout = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockoutTorso"));
    ConfigureBlockoutPart(
        TorsoBlockout,
        FVector(0.0f, 0.0f, 8.0f),
        FVector(0.38f, 0.30f, 0.58f));

    HeadBlockout = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockoutHead"));
    ConfigureBlockoutPart(
        HeadBlockout,
        FVector(0.0f, 0.0f, 78.0f),
        FVector(0.24f, 0.24f, 0.26f));

    LeftArmBlockout = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockoutLeftArm"));
    ConfigureBlockoutPart(
        LeftArmBlockout,
        FVector(0.0f, -42.0f, 13.0f),
        FVector(0.18f, 0.16f, 0.50f));

    RightArmBlockout = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockoutRightArm"));
    ConfigureBlockoutPart(
        RightArmBlockout,
        FVector(0.0f, 42.0f, 13.0f),
        FVector(0.18f, 0.16f, 0.50f));

    LeftLegBlockout = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockoutLeftLeg"));
    ConfigureBlockoutPart(
        LeftLegBlockout,
        FVector(0.0f, -19.0f, -50.0f),
        FVector(0.19f, 0.18f, 0.48f));

    RightLegBlockout = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockoutRightLeg"));
    ConfigureBlockoutPart(
        RightLegBlockout,
        FVector(0.0f, 19.0f, -50.0f),
        FVector(0.19f, 0.18f, 0.48f));
}

void AGTA1Character::BeginPlay()
{
    Super::BeginPlay();
    ApplyBlockoutMaterials();

    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        if (const UGTA1SaveGame* SaveData = Cast<UGTA1SaveGame>(
                UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
        {
            Health = FMath::Clamp(SaveData->Health, 0.0f, 100.0f);
            Stamina = FMath::Clamp(SaveData->Stamina, 0.0f, 100.0f);
            if (SaveData->bHasPlayerTransform)
            {
                SetActorTransform(SaveData->PlayerTransform, false, nullptr, ETeleportType::TeleportPhysics);
            }
        }
    }

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        PlayerController->bShowMouseCursor = false;
        PlayerController->SetInputMode(FInputModeGameOnly());
    }
}

void AGTA1Character::ApplyBlockoutMaterials()
{
    if (!BlockoutBaseMaterial)
    {
        UE_LOG(LogTemp, Warning, TEXT("BasicShapeMaterial барои ранги персонажи blockout ёфт нашуд."));
        return;
    }

    const TPair<TObjectPtr<UStaticMeshComponent>, FLinearColor> Parts[] = {
        {TorsoBlockout, FLinearColor(0.12f, 0.28f, 0.62f)},
        {HeadBlockout, FLinearColor(0.72f, 0.53f, 0.37f)},
        {LeftArmBlockout, FLinearColor(0.12f, 0.28f, 0.62f)},
        {RightArmBlockout, FLinearColor(0.12f, 0.28f, 0.62f)},
        {LeftLegBlockout, FLinearColor(0.08f, 0.10f, 0.14f)},
        {RightLegBlockout, FLinearColor(0.08f, 0.10f, 0.14f)}
    };

    for (const TPair<TObjectPtr<UStaticMeshComponent>, FLinearColor>& Part : Parts)
    {
        if (!Part.Key)
        {
            continue;
        }

        UMaterialInstanceDynamic* Material =
            UMaterialInstanceDynamic::Create(BlockoutBaseMaterial, this);
        if (Material)
        {
            Material->SetVectorParameterValue(TEXT("Color"), Part.Value);
            Part.Key->SetMaterial(0, Material);
        }
    }
}

void AGTA1Character::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bWantsToRun && Stamina > 0.0f)
    {
        Stamina = FMath::Max(0.0f, Stamina - 22.0f * DeltaSeconds);
        GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
        if (Stamina <= 0.0f)
        {
            bWantsToRun = false;
        }
    }
    else
    {
        Stamina = FMath::Min(100.0f, Stamina + 14.0f * DeltaSeconds);
        GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    }
}

void AGTA1Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AGTA1Character::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AGTA1Character::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AGTA1Character::TurnCamera);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AGTA1Character::LookCamera);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AGTA1Character::StartRunning);
    PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AGTA1Character::StopRunning);
    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AGTA1Character::Interact);
    PlayerInputComponent->BindAction(TEXT("SaveProgress"), IE_Pressed, this, &AGTA1Character::SaveProgress);
    PlayerInputComponent->BindAction(TEXT("LoadProgress"), IE_Pressed, this, &AGTA1Character::LoadProgress);
}

void AGTA1Character::MoveForward(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
    }
}

void AGTA1Character::MoveRight(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value))
    {
        const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
        AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
    }
}

void AGTA1Character::StartRunning()
{
    bWantsToRun = true;
}

void AGTA1Character::StopRunning()
{
    bWantsToRun = false;
}

void AGTA1Character::TurnCamera(float Value)
{
    AddControllerYawInput(Value * CameraSensitivity);
}

void AGTA1Character::LookCamera(float Value)
{
    AddControllerPitchInput(Value * CameraSensitivity);
}

void AGTA1Character::SetNearbyMission(AGTA1MissionActor* Mission)
{
    NearbyMission = Mission;
}

void AGTA1Character::SetNearbyVehicle(AGTA1VehiclePawn* Vehicle)
{
    NearbyVehicle = Vehicle;
}

void AGTA1Character::Interact()
{
    if (AGTA1VehiclePawn* Vehicle = NearbyVehicle.Get())
    {
        EnterVehicle(Vehicle);
        return;
    }

    if (AGTA1MissionActor* Mission = NearbyMission.Get())
    {
        Mission->CompleteFor(this);
    }
}

void AGTA1Character::EnterVehicle(AGTA1VehiclePawn* Vehicle)
{
    AController* PlayerController = GetController();
    if (!Vehicle || !PlayerController || !Vehicle->CanEnter())
    {
        return;
    }

    Vehicle->Enter(this);
}

void AGTA1Character::SaveProgress()
{
    UGTA1SaveGame* SaveData = nullptr;
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        SaveData = Cast<UGTA1SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    }
    if (!SaveData)
    {
        SaveData = Cast<UGTA1SaveGame>(
            UGameplayStatics::CreateSaveGameObject(UGTA1SaveGame::StaticClass()));
    }
    if (!SaveData)
    {
        UE_LOG(LogTemp, Error, TEXT("Захираи бозӣ сохта нашуд."));
        return;
    }

    SaveData->Health = Health;
    SaveData->Stamina = Stamina;
    SaveData->PlayerTransform = GetActorTransform();
    SaveData->bHasPlayerTransform = true;
    if (!UGameplayStatics::SaveGameToSlot(SaveData, SaveSlotName, 0))
    {
        UE_LOG(LogTemp, Error, TEXT("Пешрафт сабт нашуд."));
        return;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Пешрафт сабт шуд."));
    }
}

void AGTA1Character::LoadProgress()
{
    if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Silver, TEXT("Захираи бозӣ ёфт нашуд."));
        }
        return;
    }

    const UGTA1SaveGame* SaveData = Cast<UGTA1SaveGame>(
        UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    if (!SaveData)
    {
        UE_LOG(LogTemp, Error, TEXT("Захираи бозӣ хонда нашуд."));
        return;
    }

    Health = FMath::Clamp(SaveData->Health, 0.0f, 100.0f);
    Stamina = FMath::Clamp(SaveData->Stamina, 0.0f, 100.0f);
    if (SaveData->bHasPlayerTransform)
    {
        SetActorTransform(SaveData->PlayerTransform, false, nullptr, ETeleportType::TeleportPhysics);
    }
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Пешрафт бор карда шуд."));
    }
}

bool AGTA1Character::CompleteDeliveryMission(FName MissionId, int32 Reward)
{
    UGTA1SaveGame* SaveData = nullptr;
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        SaveData = Cast<UGTA1SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
    }
    if (!SaveData)
    {
        SaveData = Cast<UGTA1SaveGame>(UGameplayStatics::CreateSaveGameObject(UGTA1SaveGame::StaticClass()));
    }
    if (!SaveData)
    {
        UE_LOG(LogTemp, Error, TEXT("Захираи бозӣ сохта нашуд."));
        return false;
    }

    if (SaveData->CompletedMissions.Contains(MissionId))
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Silver, TEXT("Ин супориш аллакай иҷро шудааст."));
        }
        return false;
    }

    SaveData->Money += FMath::Max(0, Reward);
    SaveData->Health = Health;
    SaveData->Stamina = Stamina;
    SaveData->PlayerTransform = GetActorTransform();
    SaveData->bHasPlayerTransform = true;
    SaveData->CompletedMissions.Add(MissionId);
    if (!UGameplayStatics::SaveGameToSlot(SaveData, SaveSlotName, 0))
    {
        UE_LOG(LogTemp, Error, TEXT("Пешрафти бозӣ сабт нашуд."));
        return false;
    }

    if (GEngine)
    {
        const FString Message = FString::Printf(TEXT("Супориш иҷро шуд! Мукофот: %d сомонӣ"), Reward);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
    }
    return true;
}
