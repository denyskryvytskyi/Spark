// Copyright Denys Kryvytskyi. All Rights Reserved.

#include "OverheadWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

FString NetRoleToString(const ENetRole Role)
{
    switch (Role) {
    case ENetRole::ROLE_Authority:
        return FString("Authority");
    case ENetRole::ROLE_AutonomousProxy:
        return FString("Autonomous Proxy");
    case ENetRole::ROLE_SimulatedProxy:
        return FString("Simulated Proxy");
    }

    return FString("None");
}

void UOverheadWidget::SetDisplayText(const FString& Text)
{
    if (!Text.IsEmpty()) {
        DisplayText->SetText(FText::FromString(Text));
    }
}

void UOverheadWidget::ShowPlayerNetRole(APawn* Pawn)
{
    const ENetRole LocalRole = Pawn->GetLocalRole();
    const ENetRole RemoteRole = Pawn->GetRemoteRole();

    const FString LocalRoleStr = FString::Printf(TEXT("Local role: %s"), *NetRoleToString(LocalRole));
    const FString RemoteRoleStr = FString::Printf(TEXT("Remote role: %s"), *NetRoleToString(RemoteRole));

    SetDisplayText(RemoteRoleStr);
}

void UOverheadWidget::NativeDestruct()
{
    RemoveFromParent();

    Super::NativeDestruct();
}
