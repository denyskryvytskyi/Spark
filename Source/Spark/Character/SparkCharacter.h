// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "SparkCharacter.generated.h"

UCLASS()
class SPARK_API ASparkCharacter : public ACharacter {
    GENERATED_BODY()

public:
    ASparkCharacter();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PostInitializeComponents() override;

    void SetOverlappingWeapon(class AWeapon* Weapon);
    bool IsWeaponEquipped() const;
    bool IsAiming() const;

    FORCEINLINE float GetAOYaw() const { return AO_Yaw; }
    FORCEINLINE float GetAOPitch() const { return AO_Pitch; }
    AWeapon* GetEquippedWeapon() const;

protected:
    virtual void BeginPlay() override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void ShootFromInput(const FInputActionValue& Value);
    void OnEquipPressed(const FInputActionValue& Value);
    void OnCrouchPressed(const FInputActionValue& Value);
    void OnAimTriggered(const FInputActionValue& Value);

    void AimOffset(float DeltaTime);

private:
    UFUNCTION()
    void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

    UFUNCTION(Server, Reliable)
    void ServerOnEquipPressed();

private:
    UPROPERTY(VisibleAnywhere, Category = Camera)
    class USpringArmComponent* CameraBoom { nullptr };

    UPROPERTY(VisibleAnywhere, Category = Camera)
    class UCameraComponent* Camera { nullptr };

    /** Input */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputMappingContext* DefaultMappingContext { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* JumpAction { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* MoveAction { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* CrouchAction { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* LookAction { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* ShootAction { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* EquipAction { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    class UInputAction* AimAction { nullptr };

    /** HUD */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = "true"))
    class UWidgetComponent* OverheadWidget { nullptr };

    /** Pickups */
    UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
    AWeapon* OverlappingWeapon { nullptr };

    /** Components*/
    UPROPERTY(VisibleAnywhere)
    class UCombatComponent* Combat { nullptr };

private:
    float AO_Yaw { 0.0f };
    float AO_Pitch { 0.0f };
    FRotator StartingAimRotation;
};
