// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "BasicInteractive.h"
#include "Chest.generated.h"

UCLASS()
class HOWTO_API AChest :  public ABasicInteractive, public IInteractable
{
	GENERATED_BODY()
	
public:	

	AChest();


protected:

	void OnPlayerBeginOverlap() override;

	void OnPlayerEndOverlap() override;

	//////////// INTERFACE IInteractable //////////////////
public:

	FName GetName() override;

	// Interaface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void OnInteract();

	//Actual implementation of the Interact event
	void OnInteract_Implementation();

	FName GetQuestID() override;

	//////////// INTERFACE IInteractable //////////////////


};
