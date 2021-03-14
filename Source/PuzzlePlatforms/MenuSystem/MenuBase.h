// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuBase.generated.h"


UCLASS()
class PUZZLEPLATFORMS_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	IMenuInterface* MenuInterface;

public:
	void SetMenuInterface(IMenuInterface* MMenuInterface);
	
	void Setup();

	void Teardown();
};
