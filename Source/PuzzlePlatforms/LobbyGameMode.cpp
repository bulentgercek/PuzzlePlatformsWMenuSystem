// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameMapsSettings.h"
#include "TimerManager.h"
#include "PuzzlePlatformsGameInstance.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 2)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::StartGame, 10.0f);
	}

	UE_LOG(LogTemp, Warning, TEXT("PostLogin->Number of Players : %i"), NumberOfPlayers);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("Logout->Number of Players : %i"), NumberOfPlayers);
}

void ALobbyGameMode::StartGame()
{
	bUseSeamlessTravel = true;

	UPuzzlePlatformsGameInstance* GameInstance = Cast<UPuzzlePlatformsGameInstance>(AActor::GetGameInstance());

	if (GameInstance == nullptr) return;
	GameInstance->StartSession();

	GetWorld()->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
}