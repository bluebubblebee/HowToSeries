// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Character.h"
#include "CatCharacter.generated.h"

UCLASS()
class HOWTO_API ACatCharacter : public ACharacter, public IInteractable
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cat")
	class UBoxComponent* Trigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cat")
	FName CharacterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cat")
	FName QuestID;

	bool QuestActivated = false;

public:

	ACatCharacter();

	

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cat")
	class UComplexAnimInstance* Animator = nullptr;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Cat")
	void OnPlayerEnter();

	class AHowToCharacter* PlayerCharacter;
public:

	class UComplexAnimInstance* GetAnimator() { return Animator; }

public:

	FName GetName() override;

	// Interaface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void OnInteract();

	//Actual implementation of the Interact
	void OnInteract_Implementation();


	FName GetQuestID() override;
};
