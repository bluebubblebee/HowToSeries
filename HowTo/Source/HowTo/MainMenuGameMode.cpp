// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "HowToGameInstance.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UHowToGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GameInstance != nullptr)
	{
		bool bIsNewGame = GameInstance->IsNewGame();

		OnShowMainMenu(bIsNewGame);
	}	
}


void AMainMenuGameMode::OnNewGame_Implementation(FName MapNameToLoad)
{
	if (GameInstance == nullptr) return;	

	if (GameInstance->CreateNewSaveGame())
	{
		UGameplayStatics::OpenLevel(GetWorld(), MapNameToLoad, true);
	}	
}


void AMainMenuGameMode::OnLoadGame_Implementation(FName MapNameToLoad)
{
	UGameplayStatics::OpenLevel(GetWorld(), MapNameToLoad, true);
}

