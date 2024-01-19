// Fill out your copyright notice in the Description page of Project Settings.


#include "PushInteractive.h"
#include "HowToCharacter.h"


void APushInteractive::OnInteract_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnInteract"));

	float dotForward = FMath::Clamp(FVector::DotProduct(PlayerCharacter->GetActorForwardVector(), GetActorForwardVector()), 0.0f, 1.0f);
	float dotBack = FMath::Clamp(FVector::DotProduct(PlayerCharacter->GetActorForwardVector(), -GetActorForwardVector()), 0.0f, 1.0f);
	float dotRight = FMath::Clamp(FVector::DotProduct(PlayerCharacter->GetActorForwardVector(), GetActorRightVector()), 0.0f, 1.0f);
	float dotLeft = FMath::Clamp(FVector::DotProduct(PlayerCharacter->GetActorForwardVector(), -GetActorRightVector()), 0.0f, 1.0f);



	// Get the max to get the right directon to push
	FVector Direction(0, 0, 0);

	if ((dotForward > dotBack) && (dotForward > dotRight) && ((dotForward > dotLeft))) // DOWN
	{
		UE_LOG(LogTemp, Warning, TEXT("Push direction is Box To Forward"));

		Direction.X = 1;

	}

	else if ((dotBack > dotForward) && (dotBack > dotRight) && ((dotBack > dotLeft))) // To Top
	{
		UE_LOG(LogTemp, Warning, TEXT("Push direction is Box To Backward"));

		Direction.X = -1;

	}

	else if ((dotRight > dotForward) && (dotRight > dotBack) && ((dotRight > dotLeft)))// To Left
	{

		UE_LOG(LogTemp, Warning, TEXT("Push direction is Box To Right"));

		Direction.Y = 1;

	}

	else if ((dotLeft > dotForward) && (dotLeft > dotBack) && ((dotLeft > dotRight))) // To Right
	{
		UE_LOG(LogTemp, Warning, TEXT("Push direction is Box To Left"));

		Direction.Y = -1;
	}

	// Move the box on that direction
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation += (30.0f * Direction);
	SetActorLocation(CurrentLocation);
}



FName APushInteractive::GetName()
{
	return "Box";
}

FName APushInteractive::GetQuestID()
{
	return "";
}