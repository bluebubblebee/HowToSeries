// Fill out your copyright notice in the Description page of Project Settings.


#include "CatCharacter.h"
#include "Components/BoxComponent.h"
#include "HowTo/ComplexAnimInstance.h"
#include "HowTo/HowToGameMode.h"
#include "HowTo/QuestSystem/QuestManager.h"
#include "HowTo/HowToCharacter.h"


ACatCharacter::ACatCharacter()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

	Trigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACatCharacter::BeginOverlap);
	Trigger->OnComponentEndOverlap.AddUniqueDynamic(this, &ACatCharacter::EndOverlap);

	PrimaryActorTick.bCanEverTick = true;
}

void ACatCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetMesh() != nullptr)
	{
		Animator = Cast<UComplexAnimInstance>(GetMesh()->GetAnimInstance());
	}

	QuestActivated = true;

	SetActorTickEnabled(true);
}


void ACatCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (PlayerCharacter != nullptr) return;

	AHowToCharacter* OtherCharacter = Cast<AHowToCharacter>(OtherActor);

	if (OtherCharacter != nullptr)
	{
		// Check Dot Product between forward vectors, 
		float DotProduct = FVector::DotProduct(OtherCharacter->GetActorForwardVector(), GetActorForwardVector());

		UE_LOG(LogTemp, Warning, TEXT("Dot Product CatCharacter and Player: %f "), DotProduct);

		// DotProduct > 0.0f Same direction
		// DotProduct == 0.0f Perpendicular direction
		// DotProduct < 0.0f Opposite direction
		if (DotProduct < 0.0f)
		{
			PlayerCharacter = OtherCharacter;

			if (Animator != nullptr)
			{
				Animator->StartLookAtActor(PlayerCharacter, GetMesh());
			}

			PlayerCharacter->StartLookAt(this);

			PlayerCharacter->OnEnterActor(this);

			PlayerCharacter->OnShowUI(CharacterName);

			OnPlayerEnter();
		}
	}
}


void ACatCharacter::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Animator != nullptr)
	{
		Animator->StopLookAt();
	}

	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->StopLookAt();

		PlayerCharacter->OnLeaveActor();

		PlayerCharacter->OnHideUI();
	}

	PlayerCharacter = nullptr;
}

FName ACatCharacter::GetName()
{
	return CharacterName;
}

FName ACatCharacter::GetQuestID()
{
	return QuestID;
}

void ACatCharacter::OnInteract_Implementation()
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



