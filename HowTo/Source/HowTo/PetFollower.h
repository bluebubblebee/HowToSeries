// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PetFollower.generated.h"

UCLASS()
class HOWTO_API APetFollower : public ACharacter
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pet Settings")
	float SpeedMovement = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pet Settings")
	float MinimunDistanceToActor = 130.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pet Settings")
	float StopRadius = 160.0f; // Needs to be  greater than the MinimunDistanceToActor

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Pet Settings")
	AActor* ActorToFollow;


protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pet Settings")
	void StartFollow(AActor* Actor);

	void StartFollow_Implementation(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Pet Settings")
	void StopFollow();

	void StopFollow_Implementation();

public:

	APetFollower(); 
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;


protected:

	class UPetAnimInstance* Animator;

	int32 CurrentState;

	FVector LastLocation;
};
