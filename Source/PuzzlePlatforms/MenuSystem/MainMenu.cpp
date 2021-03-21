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
    HostButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchToHostMenu);

    if (!ensure(JoinButton != nullptr)) return false;
    HostCancelButton->OnClicked.AddDynamic(this, &UMainMenu::SwitchToMainMenu);

    if (!ensure(JoinButton != nullptr)) return false;
    HostStartButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

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
        FName HostName = FName(*HostNameEditableBox->GetText().ToString());
        MenuInterface->Host(HostName);
    }
}


void UMainMenu::SwitchToHostMenu()
{
    if (!ensure(MenuSwitcher != nullptr)) return;
    if (!ensure(HostMenu != nullptr)) return;
    MenuSwitcher->SetActiveWidget(HostMenu);
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


void UMainMenu::SetServerList(TArray<FServerData>& ServerNames) 
{
    JoinServerListScrollBox->ClearChildren();

    uint32 count = 0;

    for (const FServerData& ServerData : ServerNames)
    {
        UServerRow* ServerRow = CreateWidget<UServerRow>(this, ServerRowClass);
        if (!ensure(ServerRow != nullptr)) return;

        ServerRow->ServerName->SetText(FText::FromString(ServerData.ServerName));
        ServerRow->HostUser->SetText(FText::FromString(ServerData.HostUsername));
        FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
        ServerRow->ConnectionFraction->SetText(FText::FromString(FractionText));
        ServerRow->Setup(this, count);
        ++count;

        JoinServerListScrollBox->AddChild(ServerRow);
    }
}


void UMainMenu::SelectIndex(uint32 Index) 
{
    SelectedIndex = Index;
    UpdateChildren();
}


void UMainMenu::UpdateChildren()
{
    for (int32 i = 0; i < JoinServerListScrollBox->GetChildrenCount(); ++i)
    {
        UServerRow* ServerRow = Cast<UServerRow>(JoinServerListScrollBox->GetChildAt(i));
        if (ServerRow != nullptr)
        {
            ServerRow->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
        }

    }
}


void UMainMenu::JoinServer() 
{
    if (SelectedIndex.IsSet() && MenuInterface != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected Index %d"), SelectedIndex.GetValue());
        MenuInterface->Join(SelectedIndex.GetValue());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected Index no set."));
    }
}

void UMainMenu::QuitGame() 
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    PlayerController->ConsoleCommand(TEXT("Quit"));
}
