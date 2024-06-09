// Copyright Denys Kryvytskyi. All Rights Reserved.

#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (GameState->PlayerArray.Num() == MinPlayerAmountToStart) {
        if (UWorld* World = GetWorld()) {
            bUseSeamlessTravel = true;
            World->ServerTravel(FString(TEXT("/Game/Maps/TestGameMap?listen")));
        }
    }
}
