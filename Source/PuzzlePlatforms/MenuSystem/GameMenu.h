// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "GameMenu.generated.h"


UCLASS()
class PUZZLEPLATFORMS_API UGameMenu : public UMenuBase
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* GameMenuMainMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* GameMenuCancelButton;

private:
	UFUNCTION()
	void CloseGameMenu();

	UFUNCTION()
	void QuitToMainMenu();

protected:
	virtual bool Initialize();
};
