// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "OnlineSessionSettings.h"

#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer) 
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WBP_ServerRowClass(TEXT("/Game/MenuSystem/UI/WBP_ServerRow"));
    if (!ensure(WBP_ServerRowClass.Class != nullptr)) return;
    ServerRowClass = WBP_ServerRowClass.Class;
}


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
    if (MenuInterface != nullptr)
    {
        MenuInterface->RefreshServerList();
    }
}


void UMainMenu::SwitchToMainMenu() 
{
    if (!ensure(MenuSwitcher != nullptr)) return;
    if (!ensure(MainMenu != nullptr)) return;
    MenuSwitcher->SetActiveWidget(MainMenu);
}


void UMainMenu::SetServerList(TArray<FString>& ServerNames) 
{
    JoinServerListScrollBox->ClearChildren();

    uint32 count = 0;

    for (const FString& ServerName : ServerNames)
    {
        UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
        if (!ensure(ServerRow != nullptr)) return;

        ServerRow->ServerName->SetText(FText::FromString(ServerName));
        ServerRow->Setup(this, count);
        ++count;

        JoinServerListScrollBox->AddChild(ServerRow);
    }
}


void UMainMenu::SelectIndex(uint32 Index) 
{
    SelectedIndex = Index;
}


void UMainMenu::JoinServer() 
{
    if (SelectedIndex.IsSet())
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectedIndex.GetValue());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected Index no set."));
    }

    if (MenuInterface != nullptr)
    {
        // const FString& IpAddress = IpAddressTextBox->GetText().ToString();
        MenuInterface->Join("");
    }
}


void UMainMenu::QuitGame() 
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand(TEXT("Quit"));
}
