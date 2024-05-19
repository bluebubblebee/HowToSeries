

#include "Chest.h"
#include "HowTo/HowToGameMode.h"
#include "HowTo/HowToCharacter.h"
#include "HowTo/QuestSystem/QuestManager.h"


AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AChest::OnPlayerBeginOverlap()
{
	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->OnShowUI(InteractiveName);
	}
}

void AChest::OnPlayerEndOverlap()
{
	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->OnHideUI();
	}
}

void AChest::OnInteract_Implementation()
{
	// Retrieve info quest from game mode
	AHowToGameMode* GameMode = Cast<AHowToGameMode>(GetWorld()->GetAuthGameMode());
	if ((GameMode == nullptr) || (GameMode->GetQuestManager() == nullptr)) return;

	if (!QuestActivated) return;

	bool Success = false;
	// Find the quest on the game mode
	FQuest Quest = GameMode->GetQuestManager()->FindQuest(QuestID, Success);

	if (!Success) return;

	// Check if player has already accepted the quest
	bool bQuestAccepted = false;
	FQuestItem QuestInfo;
	bQuestAccepted = PlayerCharacter->FindQuest(QuestID, QuestInfo);

	// Quest not accepted, show info quest mark quest as a accepted
	if (!bQuestAccepted)
	{
		PlayerCharacter->OnShowQuestInfo(Quest);
		PlayerCharacter->AcceptQuest(QuestID);
	}
	else
	{
		// If quest is not completed yet, check if player has the item
		if (!QuestInfo.IsCompleted)
		{
			if (PlayerCharacter->HasItemOnHands(Quest.ItemID))
			{
				PlayerCharacter->RemoveItem(Quest.ItemID, true);
				PlayerCharacter->MarkQuestCompleted(QuestID);

				PlayerCharacter->OnShowQuestCompleted(Quest.CompleteMessage);
				QuestActivated = false;

			}
			else
			{
				PlayerCharacter->OnShowQuestInfo(Quest);
			}
		}
		else
		{
			PlayerCharacter->OnShowQuestCompleted(Quest.PostCompletionMessage);

		}
	}
}

FName AChest::GetName()
{
	return InteractiveName;
}

FName AChest::GetQuestID()
{
	return QuestID;
}
