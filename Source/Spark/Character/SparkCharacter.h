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

protected:
    virtual void BeginPlay() override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void ShootFromInput(const FInputActionValue& Value);
    void OnEquipPressed(const FInputActionValue& Value);

private:
    UFUNCTION()
    void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

    UFUNCTION(Server, Reliable)
    void ServerOnEquipPressed();

public:
    UPROPERTY(VisibleAnywhere, Category = Camera)
    class USpringArmComponent* CameraBoom { nullptr };

    UPROPERTY(VisibleAnywhere, Category = Camera)
    class UCameraComponent* Camera { nullptr };

    /** Input */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    class UInputMappingContext* DefaultMappingContext { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    class UInputAction* JumpAction { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    class UInputAction* MoveAction { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    class UInputAction* LookAction { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    class UInputAction* ShootAction { nullptr };

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    class UInputAction* EquipAction { nullptr };

    /** HUD */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UWidgetComponent* OverheadWidget { nullptr };

    /** Pickups */
    UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
    AWeapon* OverlappingWeapon { nullptr };

    /** Components*/
    UPROPERTY(VisibleAnywhere)
    class UCombatComponent* Combat { nullptr };
};
