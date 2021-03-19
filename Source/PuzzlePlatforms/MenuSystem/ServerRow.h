// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"


UCLASS()
class PUZZLEPLATFORMS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ServerRowButton;

	UPROPERTY()
	class UMainMenu* Parent;

	uint32 Index;

private:
	UFUNCTION()
	void OnClicked();

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

public:
	void Setup(class UMainMenu* InParent, uint32 InIndex);

};
