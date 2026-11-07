#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GTA1SaveGame.generated.h"

UCLASS()
class GTA1_API UGTA1SaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Пешрафт", SaveGame)
    int32 Money = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Пешрафт", SaveGame)
    float Health = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Пешрафт", SaveGame)
    float Stamina = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Пешрафт", SaveGame)
    FTransform PlayerTransform = FTransform::Identity;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Пешрафт", SaveGame)
    bool bHasPlayerTransform = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Пешрафт", SaveGame)
    TArray<FName> CompletedMissions;
};
