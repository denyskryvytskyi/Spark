// Copyright Denys Kryvytskyi. All Rights Reserved.

#include "SparkAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SparkCharacter.h"

void USparkAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    SparkCharacter = Cast<ASparkCharacter>(TryGetPawnOwner());
}

void USparkAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!SparkCharacter) {
        SparkCharacter = Cast<ASparkCharacter>(TryGetPawnOwner());
    }

    if (!SparkCharacter) {
        return;
    }

    FVector Velocity = SparkCharacter->GetVelocity();
    Velocity.Z = 0.0f;

    Speed = Velocity.Size();

    bIsInAir = SparkCharacter->GetCharacterMovement()->IsFalling();

    bIsAccelerating = SparkCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.0f;

    bIsWeaponEquipped = SparkCharacter->IsWeaponEquipped();

    bIsCrouched = SparkCharacter->bIsCrouched;
}
