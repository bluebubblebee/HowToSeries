// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "HowToCharacter.h"


void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	MyCharacter = Cast<AHowToCharacter>(GetPawn());
}


void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayerController::Action).bConsumeInput = false;

	InputComponent->BindAction("SwitchItem", IE_Pressed, this, &AMainPlayerController::SwitchItem).bConsumeInput = false;

	InputComponent->BindAction("FreeHands", IE_Pressed, this, &AMainPlayerController::FreeHands).bConsumeInput = false;

}

void AMainPlayerController::Action()
{
	if (MyCharacter != nullptr)
	{
		MyCharacter->Action();
	}
}


void AMainPlayerController::SwitchItem()
{
	if (MyCharacter != nullptr)
	{
		MyCharacter->SwitchItem();
	}
}

void AMainPlayerController::FreeHands()
{
	if (MyCharacter != nullptr)
	{
		MyCharacter->FreeHands();
	}
}
