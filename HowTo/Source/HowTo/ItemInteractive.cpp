// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInteractive.h"
#include "HowToCharacter.h"


void AItemInteractive::BeginPlay()
{
	Super::BeginPlay();

	bItemCollected = false;
}


void AItemInteractive::OnInteract_Implementation()
{
	if (bItemCollected) return;

	if (PlayerCharacter != nullptr)
	{
		if (PlayerCharacter->HasFreeInventorySlots())
		{
			PlayerCharacter->AddItem(ItemID);

			bItemCollected = true;

			OnItemCollected();
		}
	}
}

FName AItemInteractive::GetName()
{
	return InteractiveName;
}

FName AItemInteractive::GetQuestID()
{
	return QuestID;
}



void AItemInteractive::OnPlayerBeginOverlap()
{
	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->OnShowUI(InteractiveName);
	}
}


void AItemInteractive::OnPlayerEndOverlap()
{
	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->OnHideUI();
	}
}
