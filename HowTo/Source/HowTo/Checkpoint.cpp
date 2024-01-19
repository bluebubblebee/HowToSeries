
#include "Checkpoint.h"
#include "HowToCharacter.h"


void ACheckpoint::OnInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnInteract Checkpoint"));

	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->TriggerCheckPoint();
	}
}



FName ACheckpoint::GetName()
{
	return "";
}

FName ACheckpoint::GetQuestID()
{
	return "";
}

