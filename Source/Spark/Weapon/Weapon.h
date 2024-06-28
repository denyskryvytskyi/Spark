// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8 {
    EWS_Initial UMETA(DisplayName = "Initial State"),
    EWS_Equipped UMETA(DisplayName = "Equipped"),
    EWS_Dropped UMETA(DisplayName = "Dropped"),

    EWS_Max UMETA(DisplayName = "DefaultMaX")
};

UCLASS()
class SPARK_API AWeapon : public AActor {
    GENERATED_BODY()

public:
    AWeapon();

public:
    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
    class USphereComponent* AreaSphere;

    UPROPERTY(VisibleAnywhere)
    EWeaponState WeaponState { EWeaponState::EWS_Initial };
};
