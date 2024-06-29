// Copyright Denys Kryvytskyi. All Rights Reserved.

#include "CombatComponent.h"

#include "Spark/Character/SparkCharacter.h"
#include "Spark/Weapon/Weapon.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

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
}

void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UCombatComponent, EquippedWeapon);
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
