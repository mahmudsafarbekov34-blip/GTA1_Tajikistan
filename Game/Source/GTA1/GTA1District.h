#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GTA1District.generated.h"

class UMaterialInterface;
class UStaticMesh;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class GTA1_API AGTA1District : public AActor
{
    GENERATED_BODY()

public:
    AGTA1District();

protected:
    virtual void PostInitializeComponents() override;

private:
    void AddBox(FVector Location, FVector Scale, FLinearColor Color);
    void BuildDistrict();

    UPROPERTY(VisibleAnywhere, Category = "Минтақа")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY()
    TObjectPtr<UStaticMesh> CubeMesh;

    UPROPERTY()
    TObjectPtr<UMaterialInterface> BaseMaterial;

    UPROPERTY()
    TArray<TObjectPtr<UStaticMeshComponent>> DistrictMeshes;
};
