#include "GTA1MissionActor.h"

#include "GTA1Character.h"
#include "GTA1SaveGame.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

namespace
{
    const TCHAR* SaveSlotName = TEXT("GTA1_Player");
}

AGTA1MissionActor::AGTA1MissionActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("MissionTrigger"));
    Trigger->SetupAttachment(SceneRoot);
    Trigger->SetBoxExtent(FVector(180.0f, 180.0f, 150.0f));
    Trigger->SetCollisionProfileName(TEXT("Trigger"));

    Marker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissionMarker"));
    Marker->SetupAttachment(SceneRoot);
    Marker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    Marker->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    Marker->SetRelativeScale3D(FVector(0.25f, 0.25f, 1.8f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded())
    {
        Marker->SetStaticMesh(CubeMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BaseMaterial(
        TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (BaseMaterial.Succeeded())
    {
        Marker->SetMaterial(0, BaseMaterial.Object);
    }
}

void AGTA1MissionActor::ConfigureMission(FName InMissionId, int32 InReward, FText InTitle)
{
    MissionId = InMissionId;
    Reward = FMath::Max(0, InReward);
    MissionTitle = MoveTemp(InTitle);
}

void AGTA1MissionActor::BeginPlay()
{
    Super::BeginPlay();
    if (UMaterialInterface* BaseMaterial = Marker->GetMaterial(0))
    {
        UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(BaseMaterial, this);
        if (Material)
        {
            Material->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.72f, 0.54f, 0.20f));
            Marker->SetMaterial(0, Material);
        }
    }
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &AGTA1MissionActor::OnMissionBeginOverlap);
    Trigger->OnComponentEndOverlap.AddDynamic(this, &AGTA1MissionActor::OnMissionEndOverlap);

    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        if (const UGTA1SaveGame* SaveData = Cast<UGTA1SaveGame>(
                UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
        {
            if (SaveData->CompletedMissions.Contains(MissionId))
            {
                Marker->SetVisibility(false);
                Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
        }
    }
}

void AGTA1MissionActor::OnMissionBeginOverlap(
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
        Character->SetNearbyMission(this);
        if (GEngine)
        {
            const FString Prompt = FString::Printf(
                TEXT("E-ро пахш кунед: %s"),
                *MissionTitle.ToString());
            GEngine->AddOnScreenDebugMessage(
                -1, 4.0f, FColor::White, Prompt);
        }
    }
}

void AGTA1MissionActor::OnMissionEndOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
    if (AGTA1Character* Character = Cast<AGTA1Character>(OtherActor))
    {
        if (NearbyCharacter.Get() == Character)
        {
            Character->SetNearbyMission(nullptr);
            NearbyCharacter.Reset();
        }
    }
}

void AGTA1MissionActor::CompleteFor(AGTA1Character* Character)
{
    if (!Character || NearbyCharacter.Get() != Character)
    {
        return;
    }

    if (Character->CompleteDeliveryMission(MissionId, Reward))
    {
        Marker->SetVisibility(false);
        Trigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Character->SetNearbyMission(nullptr);
        NearbyCharacter.Reset();
    }
}
