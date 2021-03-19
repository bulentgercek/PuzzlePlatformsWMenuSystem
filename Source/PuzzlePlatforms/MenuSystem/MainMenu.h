// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"

#include "MainMenu.generated.h"


UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* JoinServerListScrollBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinGameButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinCancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuQuitButton;

	TSubclassOf<class UUserWidget> ServerRowClass;

private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void SwitchToJoinMenu();

	UFUNCTION()
	void SwitchToMainMenu();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void QuitGame();

	TOptional<uint32> SelectedIndex;

protected:
	virtual bool Initialize();

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FString>& ServerNames);

	void SelectIndex(uint32 Index);

};
