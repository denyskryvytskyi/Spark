// Copyright Denys Kryvytskyi. All Rights Reserved.

#include "SparkCharacter.h"

#include "Spark/Components/CombatComponent.h"
#include "Spark/Weapon/Weapon.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Net/UnrealNetwork.h"

constexpr float kTargetSpringArmLength = 600.0f;

ASparkCharacter::ASparkCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
    CameraBoom->SetupAttachment(GetMesh());
    CameraBoom->TargetArmLength = kTargetSpringArmLength;
    CameraBoom->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    // workaround to rotate character toward the direction of acceleration (movement input)
    bUseControllerRotationYaw = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;

    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Overhead Widget"));
    OverheadWidget->SetupAttachment(RootComponent);

    Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
    Combat->SetIsReplicated(true);
}

void ASparkCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    // workaround to hide default wepon bone of the character skeleton
    GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
    //
}

void ASparkCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr) {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ASparkCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr) {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ASparkCharacter::ShootFromInput(const FInputActionValue& Value)
{
    // shooting...
}

void ASparkCharacter::OnEquipPressed(const FInputActionValue& Value)
{
    if (Combat) {
        if (HasAuthority()) {
            Combat->EquipWeapon(OverlappingWeapon);
        } else {
            ServerOnEquipPressed();
        }
    }
}

void ASparkCharacter::OnCrouchPressed(const FInputActionValue& Value)
{
    if (bIsCrouched) {
        UnCrouch();
    } else {
        Crouch();
    }
}

void ASparkCharacter::OnAimTriggered(const FInputActionValue& Value)
{
    const bool isPressed = Value.Get<bool>();

    Combat->SetAiming(isPressed);
}

void ASparkCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
    if (OverlappingWeapon) {
        OverlappingWeapon->ShowPickupWidget(true);
    } else {
        LastWeapon->ShowPickupWidget(false);
    }
}

void ASparkCharacter::ServerOnEquipPressed_Implementation()
{
    if (Combat) {
        Combat->EquipWeapon(OverlappingWeapon);
    }
}

void ASparkCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ASparkCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ASparkCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASparkCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASparkCharacter::Move);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ASparkCharacter::OnCrouchPressed);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASparkCharacter::Look);
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ASparkCharacter::ShootFromInput);
        EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASparkCharacter::OnEquipPressed);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &ASparkCharacter::OnAimTriggered);
    }
}

void ASparkCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (Combat) {
        Combat->Character = this;
    }
}

void ASparkCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
    if (Weapon == nullptr && OverlappingWeapon) {
        OverlappingWeapon->ShowPickupWidget(false);
    }
    OverlappingWeapon = Weapon;

    if (IsLocallyControlled() && OverlappingWeapon) {
        OverlappingWeapon->ShowPickupWidget(true);
    }
}

bool ASparkCharacter::IsWeaponEquipped() const
{
    return Combat && Combat->EquippedWeapon;
}

bool ASparkCharacter::IsAiming() const
{
    return Combat && Combat->bIsAiming;
}
