// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HowTo/Interfaces/Interactable.h"
#include "Definitions.h"
#include "HowToCharacter.generated.h"

UCLASS(config=Game)
class AHowToCharacter : public ACharacter
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* CarryItemPoint;

public:

	AHowToCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Camera")
	float BaseLookUpRate;

protected:	

	void MoveForward(float Value);

	void MoveRight(float Value);
	
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

public:
	void Action();

protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	class UComplexAnimInstance* Animator = nullptr;	

protected:	

	TArray<FQuestItem> QuestList;	

public:

	bool FindQuest(FName QuestID, FQuestItem& Quest);

	void AcceptQuest(FName QuestID);

	void MarkQuestCompleted(FName QuestID);

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnShowQuestInfo(FQuest Quest);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnShowQuestCompleted(const FText& Message);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnShowUpdatedQuestList(const TArray<FText>& QuestTextList);

protected:

	void UpdateAndShowQuestList();	

//// Inventory ///////
protected:

	// Equipment inventory on UI and hands
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FItem> EquipmentInventory;

	// Maximun slots 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 TotalEquipmentSlots = 6;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bHasItemOnHands = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 IndexItemOnHands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	FName ItemIDOnHands;


public:

	void AddItem(FName ItemID);

	void RemoveItem(FName ItemID, bool RemoveItemFromHands);

	bool HasFreeInventorySlots();

	bool HasItemOnHands(FName ItemID);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnRefreshInventory();

public:

	void SwitchItem();

	void FreeHands();

//// Interactives ///////



//// Interactives ///////

protected:

	AActor* CurrentInteractiveActor;

	IInteractable* CurrentInteractive;
	

public:

	void OnEnterActor(AActor* InteractiveActor);

	void OnLeaveActor();

//// Interactives ///////


public:
	void StartLookAt(AActor* ActorTarget);

	void StopLookAt();

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnShowUI(FName Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnHideUI();	

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnSpawnItem(TSubclassOf<class AActor> ItemToGrab);


public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Game Events")
	void TriggerCheckPoint();

	void TriggerCheckPoint_Implementation();
};

