// Copyright Denys Kryvytskyi. All Rights Reserved.

#include "CombatComponent.h"

#include "Spark/Character/SparkCharacter.h"
#include "Spark/Weapon/Weapon.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

#include "GameFramework/CharacterMovementComponent.h"

UCombatComponent::UCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
    if (Character == nullptr || WeaponToEquip == nullptr) {
        return;
    }

    EquippedWeapon = WeaponToEquip;
    EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
    auto* CharacterMesh = Character->GetMesh();
    if (const USkeletalMeshSocket* HandSocket = CharacterMesh->GetSocketByName(FName("Right_Hand_Socket"))) {
        HandSocket->AttachActor(EquippedWeapon, CharacterMesh);
    }
    EquippedWeapon->SetOwner(Character);

    Character->GetCharacterMovement()->bOrientRotationToMovement = false;
    Character->bUseControllerRotationYaw = true;
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCombatComponent::SetAiming(bool bAiming)
{
    bIsAiming = bAiming;
    ServerSetAiming(bAiming);
}

void UCombatComponent::OnRep_EquippedWeapon()
{
    if (EquippedWeapon && Character) {
        Character->GetCharacterMovement()->bOrientRotationToMovement = false;
        Character->bUseControllerRotationYaw = true;
    }
}

void UCombatComponent::ServerSetAiming_Implementation(bool bAiming)
{
    bIsAiming = bAiming;
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCombatComponent, EquippedWeapon);
    DOREPLIFETIME(UCombatComponent, bIsAiming);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
