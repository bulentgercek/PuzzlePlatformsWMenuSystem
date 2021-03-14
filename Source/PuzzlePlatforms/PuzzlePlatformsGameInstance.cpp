// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/GameMenu.h"


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
    UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MainMenuClass->GetName());
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
    GameMenu = CreateWidget<UGameMenu>(this, GameMenuClass);
    if (!ensure(GameMenu != nullptr)) return;

    GameMenu->Setup();
    GameMenu->SetMenuInterface(this);
}


void UPuzzlePlatformsGameInstance::Host()
{
    if (MainMenu != nullptr)
    {
        MainMenu->Teardown();
    }

    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Hosting"));

    GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}


void UPuzzlePlatformsGameInstance::Join(const FString& IpAddress) 
{
    if (MainMenu != nullptr)
    {
        MainMenu->Teardown();
    }

    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Joining %s"), *IpAddress));

    APlayerController* PlayerController = UGameInstance::GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;
    PlayerController->ClientTravel(IpAddress, ETravelType::TRAVEL_Absolute);
}


void UPuzzlePlatformsGameInstance::QuitToMainMenu() 
{
    APlayerController* PlayerController = UGameInstance::GetFirstLocalPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ClientTravel("/Game/MenuSystem/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}
