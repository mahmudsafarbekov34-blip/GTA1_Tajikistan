#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GTA1Character.generated.h"

class AGTA1MissionActor;
class AGTA1VehiclePawn;
class UCameraComponent;
class UStaticMesh;
class UStaticMeshComponent;
class UMaterialInterface;
class USpringArmComponent;

UCLASS()
class GTA1_API AGTA1Character : public ACharacter
{
    GENERATED_BODY()

public:
    AGTA1Character();

    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    void SetNearbyMission(AGTA1MissionActor* Mission);
    void SetNearbyVehicle(AGTA1VehiclePawn* Vehicle);
    bool CompleteDeliveryMission(FName MissionId, int32 Reward);

protected:
    virtual void BeginPlay() override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void TurnCamera(float Value);
    void LookCamera(float Value);
    void StartRunning();
    void StopRunning();
    void Interact();
    void EnterVehicle(AGTA1VehiclePawn* Vehicle);
    void SaveProgress();
    void LoadProgress();
    void ApplyBlockoutMaterials();

    UPROPERTY(VisibleAnywhere, Category = "Камера")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, Category = "Камера")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, Category = "Қаҳрамон")
    TObjectPtr<UStaticMeshComponent> TorsoBlockout;

    UPROPERTY(VisibleAnywhere, Category = "Қаҳрамон")
    TObjectPtr<UStaticMeshComponent> HeadBlockout;

    UPROPERTY(VisibleAnywhere, Category = "Қаҳрамон")
    TObjectPtr<UStaticMeshComponent> LeftArmBlockout;

    UPROPERTY(VisibleAnywhere, Category = "Қаҳрамон")
    TObjectPtr<UStaticMeshComponent> RightArmBlockout;

    UPROPERTY(VisibleAnywhere, Category = "Қаҳрамон")
    TObjectPtr<UStaticMeshComponent> LeftLegBlockout;

    UPROPERTY(VisibleAnywhere, Category = "Қаҳрамон")
    TObjectPtr<UStaticMeshComponent> RightLegBlockout;

    UPROPERTY()
    TObjectPtr<UMaterialInterface> BlockoutBaseMaterial;

    UPROPERTY(VisibleAnywhere, Category = "Қаҳрамон")
    float Health = 100.0f;

    UPROPERTY(VisibleAnywhere, Category = "Қаҳрамон")
    float Stamina = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Танзимот", meta = (AllowPrivateAccess = "true", ClampMin = "0.1", ClampMax = "3.0"))
    float CameraSensitivity = 1.0f;

    TWeakObjectPtr<AGTA1MissionActor> NearbyMission;
    TWeakObjectPtr<AGTA1VehiclePawn> NearbyVehicle;
    bool bWantsToRun = false;
    float WalkSpeed = 450.0f;
    float RunSpeed = 700.0f;
};
