// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"

#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString ServerName;
	uint16 CurrentPlayers;
	uint32 MaxPlayers;
	FString HostUsername;

};

UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* HostNameEditableBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostCancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostStartButton;
	
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

	TOptional<uint32> SelectedIndex;

	TSubclassOf<class UUserWidget> ServerRowClass;

private:
	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void SwitchToHostMenu();

	UFUNCTION()
	void SwitchToJoinMenu();

	UFUNCTION()
	void SwitchToMainMenu();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void QuitGame();

	void UpdateChildren();

protected:
	virtual bool Initialize();

public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData>& ServerNames);

	void SelectIndex(uint32 Index);

};
