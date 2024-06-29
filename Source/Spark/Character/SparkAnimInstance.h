// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "Animation/AnimInstance.h"
#include "CoreMinimal.h"

#include "SparkAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class SPARK_API USparkAnimInstance : public UAnimInstance {
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
    UPROPERTY(BlueprintReadOnly, category = Character, meta = (AllowPrivateAccess = "true"))
    class ASparkCharacter* SparkCharacter { nullptr };

    UPROPERTY(BlueprintReadOnly, category = Movement, meta = (AllowPrivateAccess = "true"))
    float Speed { 0.0f };

    UPROPERTY(BlueprintReadOnly, category = Movement, meta = (AllowPrivateAccess = "true"))
    bool bIsInAir { false };

    UPROPERTY(BlueprintReadOnly, category = Movement, meta = (AllowPrivateAccess = "true"))
    bool bIsAccelerating { false };

    UPROPERTY(BlueprintReadOnly, category = Combat, meta = (AllowPrivateAccess = "true"))
    bool bIsWeaponEquipped { false };
};
