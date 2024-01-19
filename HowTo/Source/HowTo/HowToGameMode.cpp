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
}




FItem AHowToGameMode::FindItem_Implementation(FName ItemID, bool& Success)
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


FQuest AHowToGameMode::FindQuest_Implementation(FName QuestID, bool& Success)
{
	Success = false;

	FQuest Quest;
	if (QuestDatabase == nullptr) { return Quest; }

	for (int i = 0; i < QuestDatabase->QuestData.Num(); i++)
	{
		if (QuestDatabase->QuestData[i].QuestID == QuestID)
		{
			Success = true;
			return QuestDatabase->QuestData[i];
			
		}
	}

	return Quest;
}
