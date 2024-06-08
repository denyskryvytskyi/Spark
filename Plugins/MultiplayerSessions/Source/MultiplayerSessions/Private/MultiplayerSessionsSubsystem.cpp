// Copyright Denys Kryvytskyi. All Rights Reserved.

#include "MultiplayerSessionsSubsystem.h"

#include "Online/OnlineSessionNames.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
    : CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete))
    , StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
    , FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete))
    , JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete))
    , DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete))
{
    if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get()) {
        OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();

        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Green,
                FString::Printf(TEXT("Found online subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()));
        }
    }
}

void UMultiplayerSessionsSubsystem::CreateSession(const int32 NumPublicConnections, const EMatchType MatchType)
{
    if (!OnlineSessionInterface.IsValid()) {
        MultiplayerOnCreateSessionComplete.Broadcast(false);
        return;
    }

    auto ExistingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
    if (ExistingSession != nullptr) {
        bCreateNewSessionOnDestroy = true;
        QueuedSessionToCreateInfo.NumPublicConnections = NumPublicConnections;
        QueuedSessionToCreateInfo.MatchType = MatchType;
        DestroySession();
    } else {
        CreateSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

        FOnlineSessionSettings Settings;
        Settings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName().IsEqual("NULL") ? true : false;
        Settings.NumPublicConnections = NumPublicConnections;
        Settings.bAllowJoinInProgress = true;
        Settings.bAllowJoinViaPresence = true;
        Settings.bShouldAdvertise = true;
        Settings.bUsesPresence = true;
        Settings.bUseLobbiesIfAvailable = true;
        Settings.Set(FName("MatchType"), static_cast<int32>(MatchType), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

        if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController()) {
            if (!OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Settings)) {
                OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
                MultiplayerOnCreateSessionComplete.Broadcast(false);
            }
        }
    }
}

void UMultiplayerSessionsSubsystem::StartSession()
{
    if (!OnlineSessionInterface.IsValid()) {
        MultiplayerOnStartSessionComplete.Broadcast(false);
        return;
    }

    StartSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

    if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController()) {
        if (!OnlineSessionInterface->StartSession(NAME_GameSession)) {
            OnlineSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
            MultiplayerOnStartSessionComplete.Broadcast(false);
        }
    }
}

void UMultiplayerSessionsSubsystem::FindSessions(const int32 MaxSearchResult)
{
    if (!OnlineSessionInterface.IsValid()) {
        MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
        return;
    }

    FindSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    SessionSearch->MaxSearchResults = MaxSearchResult;
    SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName().IsEqual("NULL") ? true : false;
    SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

    if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController()) {
        if (!OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef())) {
            OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);

            MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
        }
    }
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& Result)
{
    if (!OnlineSessionInterface.IsValid()) {
        MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
        return;
    }

    JoinSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

    if (ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController()) {
        if (!OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, Result)) {
            OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);

            MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
        }
    }
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
    if (!OnlineSessionInterface.IsValid()) {
        MultiplayerOnDestroySessionComplete.Broadcast(false);
        return;
    }

    DestroySessionCompleteDelegateHandle = OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

    if (!OnlineSessionInterface->DestroySession(NAME_GameSession)) {
        OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
        MultiplayerOnDestroySessionComplete.Broadcast(false);
    }
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (OnlineSessionInterface) {
        OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
    }
    MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (OnlineSessionInterface) {
        OnlineSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
    }
    MultiplayerOnStartSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
    if (OnlineSessionInterface) {
        OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
    }

    if (SessionSearch->SearchResults.IsEmpty()) {
        MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
    } else {
        MultiplayerOnFindSessionsComplete.Broadcast(SessionSearch->SearchResults, bWasSuccessful);
    }
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
    if (OnlineSessionInterface) {
        OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
    }

    MultiplayerOnJoinSessionComplete.Broadcast(Result);
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (OnlineSessionInterface) {
        OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
    }

    MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);

    if (bWasSuccessful && bCreateNewSessionOnDestroy) {
        bCreateNewSessionOnDestroy = false;

        CreateSession(QueuedSessionToCreateInfo.NumPublicConnections, QueuedSessionToCreateInfo.MatchType);
    }
}
