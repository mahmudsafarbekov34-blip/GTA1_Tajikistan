#include "GTA1GameMode.h"

#include "GTA1Character.h"
#include "GTA1District.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"

AGTA1GameMode::AGTA1GameMode()
{
    DefaultPawnClass = AGTA1Character::StaticClass();
}

void AGTA1GameMode::InitGame(
    const FString& MapName,
    const FString& Options,
    FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World барои сохтани минтақаи прототип дастрас нест."));
        return;
    }

    if (!World->SpawnActor<AGTA1District>(AGTA1District::StaticClass()))
    {
        UE_LOG(LogTemp, Error, TEXT("Минтақаи прототип сохта нашуд."));
    }

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    PrototypePlayerStart = World->SpawnActor<APlayerStart>(
        APlayerStart::StaticClass(),
        FVector(0.0f, 0.0f, 120.0f),
        FRotator::ZeroRotator,
        SpawnParameters);
    if (!PrototypePlayerStart)
    {
        UE_LOG(LogTemp, Error, TEXT("Нуқтаи оғози прототип сохта нашуд."));
    }
}

AActor* AGTA1GameMode::ChoosePlayerStart_Implementation(AController* Player)
{
    if (PrototypePlayerStart)
    {
        return PrototypePlayerStart;
    }

    if (AActor* ExistingStart = Super::ChoosePlayerStart_Implementation(Player))
    {
        return ExistingStart;
    }

    FActorSpawnParameters SpawnParameters;
    SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return GetWorld()->SpawnActor<APlayerStart>(
        APlayerStart::StaticClass(),
        FVector(0.0f, 0.0f, 120.0f),
        FRotator::ZeroRotator,
        SpawnParameters);
}
