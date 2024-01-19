// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ComplexAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HOWTO_API UComplexAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	APawn * OwningPawn;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	float Speed;


protected:

	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	FRotator RotationHead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	float RotationHeadAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	float MaxRotationHead = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	float AlphaLeftArm = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	float AlphaRigthArm = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK Settings")
	FName CurrentAnimation = "IDLE";

protected:

	bool bEnableLookAt;

	bool bResetLookAt;

	AActor* ActorToLookAt;

	USkeletalMeshComponent* MeshParent;

public:

	void StartLookAtActor(AActor* ActorTarget, USkeletalMeshComponent* MeshParentRef);

	void StopLookAt();

	void SetAlphaRightArm(bool Value) { AlphaRigthArm = Value;  }
	void SetAlphaLeftArm(bool Value) { AlphaLeftArm = Value; }

public:

	void ChangeAnimation(FName NewAnimation);

};
