// Copyright Epic Games, Inc. All Rights Reserved.

#include "HowToGameMode.h"
#include "HowToCharacter.h"
#include "HowToGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AHowToGameMode::AHowToGameMode()
{
}

void AHowToGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (QuestManagerObject)
	{
		// Create an instance of UClueObjectManager using ClueObjectManagerTest
		QuestManager = NewObject<UQuestManager>(this, *QuestManagerObject);
	}
}

FItem AHowToGameMode::FindItem(FName ItemID, bool& Success)
{
	Success = false;

	FItem Item;
	if (ItemDatabase == nullptr) { return Item; }

	for (int i = 0; i < ItemDatabase->Data.Num(); i++)
	{
		if (ItemDatabase->Data[i].ItemID == ItemID)
		{
			Success = true;
			return ItemDatabase->Data[i];

		}
	}

	return Item;
}
