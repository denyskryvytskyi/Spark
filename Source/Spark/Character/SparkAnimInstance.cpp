// Copyright Denys Kryvytskyi. All Rights Reserved.

#include "SparkAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "SparkCharacter.h"

#include "Kismet/KismetMathLibrary.h"

#include "Spark/Weapon/Weapon.h"

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

    bIsAiming = SparkCharacter->IsAiming();

    const FRotator AimRotation = SparkCharacter->GetBaseAimRotation();
    const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(SparkCharacter->GetVelocity());
    const FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
    DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaSeconds, 6.f);
    YawOffset = DeltaRotation.Yaw;

    CharacterRotationLastFrame = CharacterRotation;
    CharacterRotation = SparkCharacter->GetActorRotation();

    const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotationLastFrame, CharacterRotation);

    const float Target = Delta.Yaw / DeltaSeconds;
    const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.0f);
    Lean = FMath::Clamp(Interp, -180.0f, 180.0f);

    AO_Yaw = SparkCharacter->GetAOYaw();
    AO_Pitch = SparkCharacter->GetAOPitch();

    const AWeapon* const EquippedWeapon = SparkCharacter->GetEquippedWeapon();

    if (bIsWeaponEquipped && EquippedWeapon && EquippedWeapon->GetMeshComponent() && SparkCharacter->GetMesh()) {
        UE_LOG(LogTemp, Warning, TEXT("TEST"));
        LeftHandTransform = EquippedWeapon->GetMeshComponent()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
        FVector OutPosition;
        FRotator OutRotation;
        SparkCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
        LeftHandTransform.SetLocation(OutPosition);
        LeftHandTransform.SetRotation(FQuat(OutRotation));
    }
}
