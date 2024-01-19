// Copyright Epic Games, Inc. All Rights Reserved.

#include "HowToCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "ComplexAnimInstance.h"
#include "HowToGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HowToGameInstance.h"
#include "Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// AHowToCharacter

AHowToCharacter::AHowToCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	CarryItemPoint = CreateDefaultSubobject<USceneComponent>(TEXT("CarryItemPoint"));
	CarryItemPoint->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AHowToCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetMesh() != nullptr)
	{
		Animator = Cast<UComplexAnimInstance>(GetMesh()->GetAnimInstance());
	}


	OnRefreshInventory();


	QuestList.Empty();
	// Load game
	UHowToGameInstance* GameInstance = Cast<UHowToGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if ((GameInstance != nullptr) && (GameInstance->LoadGame()))
	{
		if (GameInstance->CurrentSaveGame != nullptr)
		{
			// Retrieve the quest list
			QuestList = GameInstance->CurrentSaveGame->QuestStatus;

			UpdateAndShowQuestList();
		}
	}
	
}



void AHowToCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHowToCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHowToCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHowToCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHowToCharacter::LookUpAtRate);

}


void AHowToCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHowToCharacter::LookUpAtRate(float Rate)
{

	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHowToCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHowToCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


void AHowToCharacter::OnEnterActor(AActor* InteractiveActor)
{
	if (InteractiveActor != nullptr)
	{
		bool IsInterface = UKismetSystemLibrary::DoesImplementInterface(InteractiveActor, UInteractable::StaticClass());
		if (IsInterface)
		{
			CurrentInteractiveActor = InteractiveActor;

			CurrentInteractive = Cast<IInteractable>(InteractiveActor);
		}
	}	
}


void AHowToCharacter::OnLeaveActor()
{
	CurrentInteractive = nullptr;
	CurrentInteractiveActor = nullptr;
}

void AHowToCharacter::Action()
{
	if (CurrentInteractive != nullptr)
	{
		// Execute the interact event
		IInteractable::Execute_OnInteract(CurrentInteractiveActor); 		
	}
}

bool AHowToCharacter::FindQuest(FName QuestID, FQuestItem& Quest)
{
	for (int i = 0; i < QuestList.Num(); i++)
	{
		if (QuestList[i].QuestID == QuestID)
		{
			Quest = QuestList[i];
			return true;
		}
	}
	return false;
}


void AHowToCharacter::AcceptQuest(FName QuestID)
{
	bool QuestFound = false;
	for (int i = 0; i < QuestList.Num(); i++)
	{
		if (QuestList[i].QuestID == QuestID)
		{
			QuestFound = true;
			break;
		}
	}

	if (!QuestFound)
	{
		FQuestItem NewQuest;
		NewQuest.QuestID = QuestID;
		NewQuest.IsCompleted = false;
		QuestList.Add(NewQuest);

		UpdateAndShowQuestList();		
	}
}

void AHowToCharacter::MarkQuestCompleted(FName QuestID)
{	
	for (int i = 0; i < QuestList.Num(); i++)
	{
		if ((QuestList[i].QuestID == QuestID) && (!QuestList[i].IsCompleted))
		{
			QuestList[i].IsCompleted = true;
			break;
		}
	}

	UpdateAndShowQuestList();
}

void AHowToCharacter::UpdateAndShowQuestList()
{
	// Prepare list of quest, to show on the UI
	AHowToGameMode* GameMode = Cast<AHowToGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode != nullptr)
	{
		TArray<FText> QuestTextList;
		for (int i = 0; i < QuestList.Num(); i++)
		{
			if (!QuestList[i].IsCompleted)
			{
				bool Success = false;
				// Find the quest on the game mode
				FQuest Quest = GameMode->FindQuest(QuestList[i].QuestID, Success);

				if (Success)
				{
					QuestTextList.Add(Quest.SortDescription);
				}
			}
		}

		OnShowUpdatedQuestList(QuestTextList);
	}
}



void AHowToCharacter::StartLookAt(AActor* ActorTarget)
{
	if (Animator != nullptr)
	{
		Animator->StartLookAtActor(ActorTarget, GetMesh());
	}
}

void AHowToCharacter::StopLookAt()
{
	if (Animator != nullptr)
	{
		Animator->StopLookAt();
	}
}


void AHowToCharacter::TriggerCheckPoint_Implementation()
{
	// Save current game
	UHowToGameInstance* GameInstance = Cast<UHowToGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));	
	
	if ((GameInstance != nullptr) && (GameInstance->CurrentSaveGame != nullptr))
	{
		GameInstance->CurrentSaveGame->QuestStatus.Empty();
		GameInstance->CurrentSaveGame->QuestStatus = QuestList;

		if (GameInstance->SaveGame())
		{
			UE_LOG(LogTemp, Warning, TEXT("[AHowToCharacter::TriggerCheckPoint] Success saving game"));

			return;
		}	
	}

	UE_LOG(LogTemp, Warning, TEXT("[AHowToCharacter::TriggerCheckPoint] Fail saving game"));
}



//// Inventory ///////

void AHowToCharacter::AddItem(FName ItemID)
{
	// Find the item on the inventory
	for (int i = 0; i < EquipmentInventory.Num(); i++)
	{
		if (EquipmentInventory[i].ItemID == ItemID)
		{
			EquipmentInventory[i].Quantity += 1;	

			if (!bHasItemOnHands)
			{
				IndexItemOnHands = i;
				ItemIDOnHands = ItemID;
				bHasItemOnHands = true;

				EquipmentInventory[i].SpawnedItem->SetActorHiddenInGame(false);

				Animator->SetAlphaRightArm(true);
				Animator->SetAlphaLeftArm(true);
			}

			OnRefreshInventory();
			return;
		}
	}

	if (EquipmentInventory.Num() == TotalEquipmentSlots)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AHowToCharacter::AddItem] EquipmentInventory full: %d / %d "), EquipmentInventory.Num(), TotalEquipmentSlots);
		return;
	}

	// Find the item on the table on the game mode to get the information	
	AHowToGameMode* GameMode = Cast<AHowToGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode != nullptr)
	{
		bool Found = false;
		FItem ItemFound = GameMode->FindItem(ItemID, Found);

		if (Found && (ItemFound.ItemActor != nullptr))
		{
			AActor* SpawnItem = GetWorld()->SpawnActor<AActor>(ItemFound.ItemActor, FVector::ZeroVector, FRotator::ZeroRotator);

			// Spawn the item and add it to the list of elements
			if (SpawnItem != nullptr)
			{
				FItem NewItem;
				NewItem.ItemID = ItemID;
				NewItem.Name = ItemFound.Name;
				NewItem.Description = ItemFound.Description;
				NewItem.Quantity = 1;
				NewItem.ItemIcon = ItemFound.ItemIcon;
				NewItem.SpawnedItem = SpawnItem;

				SpawnItem->AttachToComponent(CarryItemPoint, FAttachmentTransformRules::KeepWorldTransform);
				SpawnItem->SetActorLocation(CarryItemPoint->GetComponentLocation());
				SpawnItem->SetActorRotation(CarryItemPoint->GetComponentRotation());

				EquipmentInventory.Add(NewItem);

				// Nothing on hands, we add this item on hands
				if (!bHasItemOnHands)
				{
					Animator->SetAlphaRightArm(true);
					Animator->SetAlphaLeftArm(true);

					// THe index is the last one on the list
					IndexItemOnHands = EquipmentInventory.Num() - 1;
					ItemIDOnHands = ItemID;
					bHasItemOnHands = true;
					SpawnItem->SetActorHiddenInGame(false);
				}
				else
				{
					// Hide item if exists
					SpawnItem->SetActorHiddenInGame(true);					
				}
			}
		}
	}

	OnRefreshInventory();
}


void AHowToCharacter::RemoveItem(FName ItemID, bool RemoveItemFromHands)
{
	// Find the item on the inventory
	int32 ItemIndexToRemove = -1;
	for (int i = 0; i < EquipmentInventory.Num(); i++)
	{
		if (EquipmentInventory[i].ItemID == ItemID)
		{
			EquipmentInventory[i].Quantity -= 1;

			if (EquipmentInventory[i].Quantity <= 0) // No more units
			{
				ItemIndexToRemove = i;

				// Remove item from hands
				if (bHasItemOnHands && (ItemIDOnHands == ItemID))
				{
					IndexItemOnHands = -1;
					bHasItemOnHands = false;
					ItemIDOnHands = "";					

					Animator->SetAlphaRightArm(false);
					Animator->SetAlphaLeftArm(false);
				}
			}
			else if (RemoveItemFromHands) // If we hare more units, we hide it from hands
			{
				FreeHands();
			}

			break;
		}
	}

	// If we remove all units from the item, we remove it from the list
	if (ItemIndexToRemove >= 0)
	{
		EquipmentInventory[ItemIndexToRemove].SpawnedItem->Destroy();
		EquipmentInventory.RemoveAt(ItemIndexToRemove);
	}

	OnRefreshInventory();

}

bool AHowToCharacter::HasFreeInventorySlots()
{
	return (EquipmentInventory.Num() < TotalEquipmentSlots);
}

bool AHowToCharacter::HasItemOnHands(FName ItemID)
{
	if (bHasItemOnHands)
	{
		return (ItemID == ItemIDOnHands);
	}

	return false;	
}


void AHowToCharacter::SwitchItem()
{
	if (EquipmentInventory.Num() == 0) return;

	if (!bHasItemOnHands) // No items on hands
	{
		IndexItemOnHands = 0;
		EquipmentInventory[IndexItemOnHands].SpawnedItem->SetActorHiddenInGame(false);		

		bHasItemOnHands = true;
		ItemIDOnHands = EquipmentInventory[IndexItemOnHands].ItemID;

		Animator->SetAlphaRightArm(true);
		Animator->SetAlphaLeftArm(true);

		return;
	}	

	// Items on hands, we switch we have at least 1
	if (EquipmentInventory.Num() > 1)
	{
		// Hide the current one
		EquipmentInventory[IndexItemOnHands].SpawnedItem->SetActorHiddenInGame(true);

		IndexItemOnHands += 1;
		if (IndexItemOnHands >= EquipmentInventory.Num()) // Last item on the inventory
		{
			IndexItemOnHands = 0;
		}
		EquipmentInventory[IndexItemOnHands].SpawnedItem->SetActorHiddenInGame(false);
		ItemIDOnHands = EquipmentInventory[IndexItemOnHands].ItemID;
	}	
}

void AHowToCharacter::FreeHands()
{
	if (bHasItemOnHands && (IndexItemOnHands >= 0) && (IndexItemOnHands < EquipmentInventory.Num()))
	{		
		EquipmentInventory[IndexItemOnHands].SpawnedItem->SetActorHiddenInGame(true);
	}

	IndexItemOnHands = -1;
	bHasItemOnHands = false;
	ItemIDOnHands = "";

	Animator->SetAlphaRightArm(false);
	Animator->SetAlphaLeftArm(false);
}

//// Inventory ///////
