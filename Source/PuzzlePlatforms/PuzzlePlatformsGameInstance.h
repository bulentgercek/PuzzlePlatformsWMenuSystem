// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"


UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	class UMainMenu* MainMenu;

	TSubclassOf<class UUserWidget> GameMenuClass;
	class UGameMenu* GameMenu;

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void LoadGameMenu();

	UFUNCTION(Exec)
	virtual void Host() override;
	
	UFUNCTION(Exec)
	virtual void Join(const FString& IpAddress) override;

	UFUNCTION(Exec)
	virtual void QuitToMainMenu() override;
};
