#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Pawn.h"
#include "GTA1VehiclePawn.generated.h"

class AGTA1Character;
class UBoxComponent;
class UStaticMesh;
class UStaticMeshComponent;

UCLASS()
class GTA1_API AGTA1VehiclePawn : public APawn
{
    GENERATED_BODY()

public:
    AGTA1VehiclePawn();

    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    bool CanEnter() const;
    void Enter(AGTA1Character* Character);
    void Exit();

protected:
    virtual void BeginPlay() override;

private:
    void SetThrottle(float Value);
    void SetSteering(float Value);
    void SetBrake(bool bPressed);
    void ToggleCamera();

    UFUNCTION()
    void OnVehicleBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UFUNCTION()
    void OnVehicleEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex);

    UPROPERTY(VisibleAnywhere, Category = "Мошин")
    TObjectPtr<UStaticMeshComponent> Body;

    UPROPERTY(VisibleAnywhere, Category = "Мошин")
    TObjectPtr<UBoxComponent> EntryTrigger;

    UPROPERTY(VisibleAnywhere, Category = "Камера")
    TObjectPtr<class USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, Category = "Камера")
    TObjectPtr<class UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, Category = "Мошин")
    TArray<TObjectPtr<UStaticMeshComponent>> Wheels;

    UPROPERTY()
    TObjectPtr<UStaticMesh> WheelMesh;

    UPROPERTY(EditAnywhere, Category = "Мошин")
    float MaximumSpeed = 3200.0f;

    UPROPERTY(EditAnywhere, Category = "Мошин")
    float EngineForce = 60000.0f;

    UPROPERTY(EditAnywhere, Category = "Мошин")
    float SteeringTorque = 12000.0f;

    UPROPERTY(EditAnywhere, Category = "Мошин")
    float BrakeForce = 140000.0f;

    TWeakObjectPtr<AGTA1Character> NearbyCharacter;
    TWeakObjectPtr<AGTA1Character> PreviousDriver;
    float ThrottleInput = 0.0f;
    float SteeringInput = 0.0f;
    bool bBrakePressed = false;
    bool bInteriorCamera = false;
};
