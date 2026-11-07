#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GTA1GameMode.generated.h"

UCLASS()
class GTA1_API AGTA1GameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGTA1GameMode();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
    UPROPERTY(Transient)
    TObjectPtr<class APlayerStart> PrototypePlayerStart;
};
