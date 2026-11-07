#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GTA1MissionActor.generated.h"

class AGTA1Character;
class UBoxComponent;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class GTA1_API AGTA1MissionActor : public AActor
{
    GENERATED_BODY()

public:
    AGTA1MissionActor();

    void ConfigureMission(FName InMissionId, int32 InReward, FText InTitle);
    void CompleteFor(AGTA1Character* Character);

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnMissionBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnMissionEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    UPROPERTY(VisibleAnywhere, Category = "Супориш")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, Category = "Супориш")
    TObjectPtr<UBoxComponent> Trigger;

    UPROPERTY(VisibleAnywhere, Category = "Супориш")
    TObjectPtr<UStaticMeshComponent> Marker;

    UPROPERTY(EditAnywhere, Category = "Супориш")
    FName MissionId = TEXT("delivery_01");

    UPROPERTY(EditAnywhere, Category = "Супориш")
    int32 Reward = 100;

    UPROPERTY(EditAnywhere, Category = "Супориш")
    FText MissionTitle = FText::FromString(TEXT("Расонидани бор"));

    TWeakObjectPtr<AGTA1Character> NearbyCharacter;
};
