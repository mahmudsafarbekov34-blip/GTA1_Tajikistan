#include "GTA1District.h"

#include "GTA1MissionActor.h"
#include "GTA1VehiclePawn.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AGTA1District::AGTA1District()
{
    PrimaryActorTick.bCanEverTick = false;
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
        TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeAsset.Succeeded())
    {
        CubeMesh = CubeAsset.Object;
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(
        TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (MaterialAsset.Succeeded())
    {
        BaseMaterial = MaterialAsset.Object;
    }
}

void AGTA1District::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    BuildDistrict();
}

void AGTA1District::AddBox(FVector Location, FVector Scale, FLinearColor Color)
{
    if (!CubeMesh)
    {
        UE_LOG(LogTemp, Error, TEXT("Меши асосии Engine барои сохтани минтақа дастрас нест."));
        return;
    }
    if (!GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("World барои сохтани минтақа дастрас нест."));
        return;
    }

    UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this);
    if (!MeshComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("Компоненти бино сохта нашуд."));
        return;
    }

    AddInstanceComponent(MeshComponent);
    MeshComponent->SetupAttachment(SceneRoot);
    MeshComponent->SetStaticMesh(CubeMesh);
    MeshComponent->SetRelativeLocation(Location);
    MeshComponent->SetRelativeScale3D(Scale);
    MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
    MeshComponent->SetMobility(EComponentMobility::Static);
    if (BaseMaterial)
    {
        UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(BaseMaterial, this);
        if (Material)
        {
            Material->SetVectorParameterValue(TEXT("Color"), Color);
            MeshComponent->SetMaterial(0, Material);
        }
    }
    MeshComponent->RegisterComponent();
    DistrictMeshes.Add(MeshComponent);
}

void AGTA1District::BuildDistrict()
{
    if (!GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("World барои сохтани минтақаи Душанбе дастрас нест."));
        return;
    }

    const FLinearColor Grass(0.18f, 0.34f, 0.18f);
    const FLinearColor Asphalt(0.16f, 0.16f, 0.16f);
    const FLinearColor Sidewalk(0.42f, 0.40f, 0.36f);
    const FLinearColor BuildingColors[] = {
        FLinearColor(0.48f, 0.42f, 0.34f),
        FLinearColor(0.54f, 0.54f, 0.54f),
        FLinearColor(0.42f, 0.35f, 0.29f)
    };

    AddBox(FVector(0.0f, 0.0f, -20.0f), FVector(35.0f, 35.0f, 0.2f), Grass);

    for (const float RoadPosition : {-1400.0f, -500.0f, 500.0f, 1400.0f})
    {
        AddBox(FVector(RoadPosition, 0.0f, 2.0f), FVector(1.05f, 35.0f, 0.04f), Asphalt);
        AddBox(FVector(0.0f, RoadPosition, 2.0f), FVector(35.0f, 1.05f, 0.04f), Asphalt);
        AddBox(FVector(RoadPosition - 120.0f, 0.0f, 3.0f), FVector(0.10f, 35.0f, 0.04f), Sidewalk);
        AddBox(FVector(RoadPosition + 120.0f, 0.0f, 3.0f), FVector(0.10f, 35.0f, 0.04f), Sidewalk);
        AddBox(FVector(0.0f, RoadPosition - 120.0f, 3.0f), FVector(35.0f, 0.10f, 0.04f), Sidewalk);
        AddBox(FVector(0.0f, RoadPosition + 120.0f, 3.0f), FVector(35.0f, 0.10f, 0.04f), Sidewalk);
    }

    int32 BuildingIndex = 0;
    for (const float X : {-2050.0f, -950.0f, 250.0f, 1050.0f, 2050.0f})
    {
        for (const float Y : {-2050.0f, -950.0f, 250.0f, 1050.0f, 2050.0f})
        {
            if (FMath::Abs(X) < 500.0f && FMath::Abs(Y) < 500.0f)
            {
                continue;
            }

            const float Width = 2.2f + static_cast<float>(BuildingIndex % 3) * 0.35f;
            const float Depth = 2.0f + static_cast<float>((BuildingIndex + 1) % 3) * 0.3f;
            const float Height = 2.8f + static_cast<float>(BuildingIndex % 4) * 0.7f;
            AddBox(
                FVector(X, Y, Height * 50.0f),
                FVector(Width, Depth, Height),
                BuildingColors[BuildingIndex % UE_ARRAY_COUNT(BuildingColors)]);
            ++BuildingIndex;
        }
    }

    AddBox(FVector(-1800.0f, 1700.0f, 8.0f), FVector(3.0f, 2.0f, 0.08f), Grass);
    AddBox(FVector(-1800.0f, 1700.0f, 12.0f), FVector(0.12f, 2.0f, 0.04f), Sidewalk);

    const FVector MissionLocations[] = {
        FVector(500.0f, 500.0f, 0.0f),
        FVector(-1400.0f, 500.0f, 0.0f),
        FVector(1400.0f, -1400.0f, 0.0f)
    };
    const FName MissionIds[] = {
        TEXT("delivery_01"),
        TEXT("delivery_02"),
        TEXT("delivery_03")
    };
    const int32 MissionRewards[] = {100, 150, 200};
    const TCHAR* MissionTitles[] = {
        TEXT("Бори аввалро расонед"),
        TEXT("Фармоиши бозорро расонед"),
        TEXT("Бори охиринро расонед")
    };

    for (int32 Index = 0; Index < UE_ARRAY_COUNT(MissionLocations); ++Index)
    {
        const FTransform MissionTransform(FRotator::ZeroRotator, MissionLocations[Index]);
        AGTA1MissionActor* Mission = GetWorld()->SpawnActorDeferred<AGTA1MissionActor>(
            AGTA1MissionActor::StaticClass(),
            MissionTransform,
            this,
            nullptr,
            ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
        if (!Mission)
        {
            UE_LOG(LogTemp, Error, TEXT("Супориши %d сохта нашуд."), Index + 1);
            continue;
        }

        Mission->ConfigureMission(
            MissionIds[Index],
            MissionRewards[Index],
            FText::FromString(MissionTitles[Index]));
        UGameplayStatics::FinishSpawningActor(Mission, MissionTransform);
    }

    FActorSpawnParameters VehicleParameters;
    VehicleParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (!GetWorld()->SpawnActor<AGTA1VehiclePawn>(
            AGTA1VehiclePawn::StaticClass(),
            FVector(-500.0f, -800.0f, 35.0f),
            FRotator(0.0f, 90.0f, 0.0f),
            VehicleParameters))
    {
        UE_LOG(LogTemp, Error, TEXT("Мошини прототипӣ сохта нашуд."));
    }
}
