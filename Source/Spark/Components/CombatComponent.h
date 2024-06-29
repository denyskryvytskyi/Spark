// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#include "CombatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPARK_API UCombatComponent : public UActorComponent {
    GENERATED_BODY()

public:
    friend class ASparkCharacter;
    UCombatComponent();

    void EquipWeapon(class AWeapon* WeaponToEquip);

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void BeginPlay() override;

private:
    class ASparkCharacter* Character { nullptr };

    UPROPERTY(Replicated)
    AWeapon* EquippedWeapon { nullptr };
};
