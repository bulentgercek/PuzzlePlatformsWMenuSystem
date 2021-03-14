// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"
#include "Components/Button.h"
#include "../PuzzlePlatformsGameInstance.h"
#include "Kismet/GameplayStatics.h"


bool UGameMenu::Initialize() 
{
    bIsFocusable = true;

    bool Success = Super::Initialize();
    if (!Success) return false;

    if (!ensure(GameMenuCancelButton != nullptr)) return false;
    GameMenuCancelButton->OnClicked.AddDynamic(this, &UGameMenu::CloseGameMenu);

    if (!ensure(GameMenuMainMenuButton != nullptr)) return false;
    GameMenuMainMenuButton->OnClicked.AddDynamic(this, &UGameMenu::QuitToMainMenu);

    return true;
}


void UGameMenu::CloseGameMenu() 
{
    UMenuBase::Teardown();
}


void UGameMenu::QuitToMainMenu() 
{
    if (MenuInterface != nullptr)
    {
        UMenuBase::Teardown();
        MenuInterface->QuitToMainMenu();
    }
}
