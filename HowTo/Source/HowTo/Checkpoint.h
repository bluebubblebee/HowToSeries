// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "BasicInteractive.h"
#include "Checkpoint.generated.h"

/**
 * 
 */
UCLASS()
class HOWTO_API ACheckpoint : public ABasicInteractive, public IInteractable
{
	GENERATED_BODY()


	//////////// INTERFACE IInteractable //////////////////
public:

	FName GetName() override;

	// Interaface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void OnInteract();

	//Actual implementation of the Interact
	void OnInteract_Implementation();

	FName GetQuestID() override;

	//////////// INTERFACE IInteractable //////////////////	
};
