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
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void ShowPickupWidget(bool bIsVisible);
    void SetWeaponState(const EWeaponState State);

    FORCEINLINE USkeletalMeshComponent* GetMeshComponent() const { return WeaponMesh; }

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex,
                                      bool bFromSweep,
                                      const FHitResult& SweepResult);

    UFUNCTION()
    virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp,
                                    int32 OtherBodyIndex);

private:
    UFUNCTION()
    void OnRep_WeaponState();

private:
    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
    class USphereComponent* AreaSphere;

    UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, Category = "Weapon Properties")
    EWeaponState WeaponState { EWeaponState::EWS_Initial };

    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
    class UWidgetComponent* PickupWidget;
};
