// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/GameMenu.h"
#include "MenuSystem/ServerRow.h"

const static FName SESSION_NAME = TEXT("My Session");

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


void UPuzzlePlatformsGameInstance::Host()
{
    if (SessionInterface.IsValid())
    {
        FNamedOnlineSession* ExistedSession = SessionInterface->GetNamedSession(SESSION_NAME);
        if (ExistedSession != nullptr)
        {
            SessionInterface->DestroySession(SESSION_NAME);
        }
        else
        {
            CreateSession(SESSION_NAME);
        }
    }
}


void UPuzzlePlatformsGameInstance::CreateSession(FName SessionName) 
{
    if (SessionInterface.IsValid())
    {
        FOnlineSessionSettings SessionSettings;
        SessionSettings.bIsLANMatch = true;
        SessionSettings.NumPublicConnections = 2;
        SessionSettings.bShouldAdvertise = true;
        SessionInterface->CreateSession(0, SessionName, SessionSettings);
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

    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Hosting"));

    GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}


void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) 
{
    if (Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s named session already found and destroyed."), *SessionName.ToString());
        CreateSession(SessionName);
    }
}


void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool Success)
{
    if (Success && SessionSearch.IsValid() && MainMenu != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Finished find sessions."));
        
        TArray<FString> ServerList;

        for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
        {
           UE_LOG(LogTemp, Warning, TEXT("Found session name : %s"), *SearchResult.GetSessionIdStr());
           ServerList.Add(SearchResult.GetSessionIdStr());
        }
        MainMenu->SetServerList(ServerList);
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Server list refreshed."));
    }
}


void UPuzzlePlatformsGameInstance::Join(const FString& IpAddress) 
{
    if (MainMenu != nullptr)
    {
        //MainMenu->Teardown();
    }

    // GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Joining %s"), *IpAddress));

    // APlayerController* PlayerController = UGameInstance::GetFirstLocalPlayerController();
    // if (!ensure(PlayerController != nullptr)) return;
    // PlayerController->ClientTravel(IpAddress, ETravelType::TRAVEL_Absolute);
}


void UPuzzlePlatformsGameInstance::RefreshServerList() 
{
    SessionSearch = MakeShareable(new FOnlineSessionSearch());
    if (SessionSearch.IsValid())
    {
        SessionSearch->bIsLanQuery = true;
        UE_LOG(LogTemp, Warning, TEXT("Starting Find Sessions..."));
        SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
    }
}