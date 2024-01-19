// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "BasicInteractive.h"
#include "PushInteractive.generated.h"

/**
 * 
 */
UCLASS()
class HOWTO_API APushInteractive : public ABasicInteractive, public IInteractable
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

	//void Interact() override;

	FName GetQuestID() override;

//////////// INTERFACE IInteractable //////////////////
	
};
