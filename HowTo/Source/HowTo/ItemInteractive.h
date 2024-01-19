// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "BasicInteractive.h"
#include "ItemInteractive.generated.h"

/**
 * 
 */
UCLASS()
class HOWTO_API AItemInteractive : public ABasicInteractive, public IInteractable
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bItemCollected;

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
	void OnItemCollected();

protected:

	virtual void BeginPlay() override;


//////////// ABasicInteractive override methods //////////////////
protected:

	void OnPlayerBeginOverlap() override;

	void OnPlayerEndOverlap() override;

//////////// ABasicInteractive override methods //////////////////

	

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
