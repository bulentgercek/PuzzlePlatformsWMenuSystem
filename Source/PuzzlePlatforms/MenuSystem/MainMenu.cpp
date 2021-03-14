// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"


bool UMainMenu::Initialize() 
{
    bIsFocusable = true;

    bool Success = Super::Initialize();
    if (!Success) return false;

    // Assign Menu Buttons
    if (!ensure(HostButton != nullptr)) return false;
    HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

    if (!ensure(JoinButton != nullptr)) return false;
    JoinButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchToJoinMenu);

    if (!ensure(JoinCancelButton != nullptr)) return false;
    JoinCancelButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchToMainMenu);

    if (!ensure(JoinGameButton != nullptr)) return false;
    JoinGameButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

    if (!ensure(MainMenuQuitButton != nullptr)) return false;
    MainMenuQuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

    return true;
}


void UMainMenu::HostServer() 
{
    if (MenuInterface != nullptr)
    {
        MenuInterface->Host();
    }
}


void UMainMenu::SwitchToJoinMenu() 
{
    if (!ensure(MenuSwitcher != nullptr)) return;
    if (!ensure(JoinMenu != nullptr)) return;
    MenuSwitcher->SetActiveWidget(JoinMenu);
}


void UMainMenu::SwitchToMainMenu() 
{
    if (!ensure(MenuSwitcher != nullptr)) return;
    if (!ensure(MainMenu != nullptr)) return;
    MenuSwitcher->SetActiveWidget(MainMenu);
}


void UMainMenu::JoinServer() 
{
    if (MenuInterface != nullptr)
    {
        const FString& IpAddress = IpAddressTextBox->GetText().ToString();
        MenuInterface->Join(IpAddress);
    }
}


void UMainMenu::QuitGame() 
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand(TEXT("Quit"));
}