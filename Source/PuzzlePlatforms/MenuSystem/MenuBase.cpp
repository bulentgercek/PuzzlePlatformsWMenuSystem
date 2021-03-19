// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuBase.h"

#include "GameFramework/PlayerController.h"


void UMenuBase::SetMenuInterface(IMenuInterface* MMenuInterface) 
{
    this->MenuInterface = MMenuInterface;
}


void UMenuBase::Setup() 
{
    this->AddToViewport();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(this->TakeWidget());
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}


void UMenuBase::Teardown() 
{
    this->RemoveFromViewport();

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!ensure(PlayerController != nullptr)) return;

    FInputModeGameOnly InputModeData;
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = false;
}
