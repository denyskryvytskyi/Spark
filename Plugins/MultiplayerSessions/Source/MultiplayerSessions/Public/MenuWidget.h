// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"

#include "MatchType.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "MenuWidget.generated.h"

/**
 *
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenuWidget : public UUserWidget {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void MenuSetup(const int32 NumPublicConnections = 4, const EMatchType MatchType = EMatchType::FFA, const FString& LobbyLevelPath = TEXT("/Game/ThirdPerson/Maps/Lobby"));

protected:
    bool Initialize() override;
    void NativeDestruct() override;

    UFUNCTION()
    void OnCreateSession(bool bWasSuccessful);
    UFUNCTION()
    void OnStartSession(bool bWasSuccessful);
    void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
    void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
    UFUNCTION()
    void OnDestroySession(bool bWasSuccessful);

private:
    UFUNCTION()
    void OnHostButtonClicked();

    UFUNCTION()
    void OnJoinButtonClicked();

private:
    UPROPERTY(meta = (BindWidget))
    class UButton* HostButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* JoinButton;

    class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

    int32 NumPublicConnections { 4 };
    EMatchType MatchType { EMatchType::FFA };
    FString LobbyUrl { TEXT("") };
};
