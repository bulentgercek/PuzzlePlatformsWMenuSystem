// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/GameMenu.h"
#include "MenuSystem/ServerRow.h"


UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer) 
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WBP_MainMenuClass(TEXT("/Game/MenuSystem/UI/WBP_MainMenu"));
    if (!ensure(WBP_MainMenuClass.Class != nullptr)) return;
    MainMenuClass = WBP_MainMenuClass.Class;

    static ConstructorHelpers::FClassFinder<UUserWidget> WBP_GameMenuClass(TEXT("/Game/MenuSystem/UI/WBP_GameMenu"));
    if (!ensure(WBP_GameMenuClass.Class != nullptr)) return;
    GameMenuClass = WBP_GameMenuClass.Class;
}


void UPuzzlePlatformsGameInstance::Init() 
{
    IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
    if (!ensureAlways(OnlineSubsystem != nullptr)) return;

    UE_LOG(LogTemp, Warning, TEXT("Found Online Subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString());

    SessionInterface = OnlineSubsystem->GetSessionInterface();
    if (SessionInterface.IsValid())
    {
        SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
        SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
        SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
        SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);

        RefreshServerList();
    }
}


void UPuzzlePlatformsGameInstance::LoadMainMenu() 
{
    MainMenu = CreateWidget<UMainMenu>(this, MainMenuClass);
    if (!ensure(MainMenu != nullptr)) return;

    MainMenu->Setup();
    MainMenu->SetMenuInterface(this);
}


void UPuzzlePlatformsGameInstance::LoadGameMenu()
{
    UGameMenu* GameMenu = CreateWidget<UGameMenu>(this, GameMenuClass);
    if (!ensure(GameMenu != nullptr)) return;

    GameMenu->Setup();
    GameMenu->SetMenuInterface(this);
}


void UPuzzlePlatformsGameInstance::Host(FName HostName)
{
    if (SessionInterface.IsValid())
    {
        HostNameFromMainMenu = HostName;
        FNamedOnlineSession* ExistedSession = SessionInterface->GetNamedSession(HostNameFromMainMenu);
        if (ExistedSession != nullptr)
        {
            SessionInterface->DestroySession(HostNameFromMainMenu);
        }
        else
        {
            CreateSession();
        }
    }
}


void UPuzzlePlatformsGameInstance::CreateSession() 
{
    if (SessionInterface.IsValid())
    {
        FOnlineSessionSettings SessionSettings;
        if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
        {
            SessionSettings.bIsLANMatch = true;
        }
        else
        {
            SessionSettings.bIsLANMatch = false;
        }

        SessionSettings.NumPublicConnections = 2;
        SessionSettings.bShouldAdvertise = true;
        SessionSettings.bUsesPresence = true;
        SessionSettings.Set(TEXT("ServerName"), HostNameFromMainMenu.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

        SessionInterface->CreateSession(0, HostNameFromMainMenu, SessionSettings);
    }
}


void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) 
{
    if (!Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
        return;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("%s named session created."), *SessionName.ToString());
    }

    if (MainMenu != nullptr)
    {
        MainMenu->Teardown();
    }

    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Hosting %s"), *SessionName.ToString()));

    GetWorld()->ServerTravel("/Game/PuzzlePlatforms/Maps/Lobby?listen");
}


void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) 
{
    if (Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s named session already found and destroyed."), *SessionName.ToString());
        CreateSession();
    }
}


void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{
    if (Success && SessionSearch.IsValid() && MainMenu != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Finished find sessions."));
        
        TArray<FServerData> ServerList;

        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
            UE_LOG(LogTemp, Warning, TEXT("Found session name : %s"), *SearchResult.GetSessionIdStr());
            
            FServerData ServerData;

            FString SessionSettingsServerName;

            if (SearchResult.Session.SessionSettings.Get(TEXT("ServerName"), SessionSettingsServerName))
            {
                ServerData.ServerName = SessionSettingsServerName;
            }
            else
            {
                ServerData.ServerName = "No Name";
            }

            ServerData.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
            ServerData.CurrentPlayers = ServerData.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
            ServerData.HostUsername = SearchResult.Session.OwningUserName;
            
            ServerList.Add(ServerData);
        }

        MainMenu->SetServerList(ServerList);
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Server list refreshed."));
    }
}


void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) 
{
    if (!SessionInterface.IsValid()) return;

    UE_LOG(LogTemp, Warning, TEXT("Join Session : %s"), *SessionName.ToString());

    FString URL;
    if (!SessionInterface->GetResolvedConnectString(SessionName, URL))
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
        return;
    }

    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Joining session %s"), *SessionName.ToString()));

    APlayerController* PlayerController = UGameInstance::GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;
    PlayerController->ClientTravel(URL, ETravelType::TRAVEL_Absolute);

    UE_LOG(LogTemp, Warning, TEXT("Session joined."));
}


void UPuzzlePlatformsGameInstance::Join(uint32 Index) 
{
    if (!SessionInterface.IsValid()) return;
    if (!SessionSearch.IsValid()) return;

    if (MainMenu != nullptr)
    {
        MainMenu->Teardown();
    }

    FString SessionSettingsServerName;
    if (SessionSearch->SearchResults[Index].Session.SessionSettings.Get(TEXT("ServerName"), SessionSettingsServerName))
    {
        SessionInterface->JoinSession(0, *SessionSettingsServerName, SessionSearch->SearchResults[Index]);
    }
}


void UPuzzlePlatformsGameInstance::RefreshServerList() 
{
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (SessionSearch.IsValid())
    {
        //SessionSearch->bIsLanQuery = true;
        SessionSearch->MaxSearchResults = 100;
        SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
        UE_LOG(LogTemp, Warning, TEXT("Starting Find Sessions..."));
        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }
}