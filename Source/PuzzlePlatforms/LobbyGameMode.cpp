// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++NumberOfPlayers;

	if (NumberOfPlayers >= 2)
	{
		bUseSeamlessTravel = true;
		GetWorld()->ServerTravel("/Game/PuzzlePlatforms/Maps/Game?listen");
	}

	UE_LOG(LogTemp, Warning, TEXT("PostLogin->Number of Players : %i"), NumberOfPlayers);
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	--NumberOfPlayers;

	UE_LOG(LogTemp, Warning, TEXT("Logout->Number of Players : %i"), NumberOfPlayers);
}