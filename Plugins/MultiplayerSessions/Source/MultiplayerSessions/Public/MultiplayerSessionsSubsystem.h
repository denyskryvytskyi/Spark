// Copyright Denys Kryvytskyi. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "MatchType.h"

#include "Subsystems/GameInstanceSubsystem.h"

#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MultiplayerSessionsSubsystem.generated.h"

/**
 * Custom delegates to be broadcasted from subsystem
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSucessfull);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSucessfull);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResult, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionsComplete, const EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSucessfull);

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

private:
    struct SessionToCreateInfo {
        int32 NumPublicConnections;
        EMatchType MatchType;
    };

public:
    UMultiplayerSessionsSubsystem();

    /** API for managing sessions */
    void CreateSession(const int32 NumPublicConnections, const EMatchType MatchType);
    void StartSession();
    void FindSessions(const int32 MaxSearchResult);
    void JoinSession(const FOnlineSessionSearchResult& Result);
    void DestroySession();

protected:
    /** Callbacks for the delegates */
    void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
    void OnFindSessionsComplete(bool bWasSuccessful);
    void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
    void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

public:
    FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
    FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;
    FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;
    FMultiplayerOnJoinSessionsComplete MultiplayerOnJoinSessionComplete;
    FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;

private:
    IOnlineSessionPtr OnlineSessionInterface;
    TSharedPtr<FOnlineSessionSearch> SessionSearch;

    /** Delegates to be added to delegates list of the OnlineSessionInterface */
    FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
    FDelegateHandle CreateSessionCompleteDelegateHandle;

    FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
    FDelegateHandle StartSessionCompleteDelegateHandle;

    FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
    FDelegateHandle FindSessionCompleteDelegateHandle;

    FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
    FDelegateHandle JoinSessionCompleteDelegateHandle;

    FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
    FDelegateHandle DestroySessionCompleteDelegateHandle;

    SessionToCreateInfo QueuedSessionToCreateInfo;
    bool bCreateNewSessionOnDestroy { false };
};
