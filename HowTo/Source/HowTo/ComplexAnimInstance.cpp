// Fill out your copyright notice in the Description page of Project Settings.


#include "ComplexAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"


void UComplexAnimInstance::NativeInitializeAnimation()
{

	Super::NativeInitializeAnimation();

	OwningPawn = TryGetPawnOwner();
}

void UComplexAnimInstance::NativeUpdateAnimation(float DeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);

	if (!OwningPawn)
	{
		return;
	}

	if (!bEnableLookAt) return;

	if (bResetLookAt) // Reset
	{
		RotationHead = FMath::RInterpTo(RotationHead, FRotator::ZeroRotator, DeltaTimeX, 6.0f);

		if (RotationHead.IsNearlyZero())
		{
			RotationHead = FRotator::ZeroRotator;

			RotationHeadAlpha = 0.0f;

			bResetLookAt = false;
			bEnableLookAt = false;
		}
	}
	else
	{
		FVector TargetLocation = FVector::ZeroVector;

		TargetLocation = ActorToLookAt->GetActorLocation();

		FVector DeltaLocation = TargetLocation - OwningPawn->GetActorLocation();
		DeltaLocation.Normalize();
		float DotProduct = FVector::DotProduct(DeltaLocation, OwningPawn->GetActorForwardVector());

		if (DotProduct < 0.0f)
		{
			// Reset
			RotationHead = FMath::RInterpTo(RotationHead, FRotator::ZeroRotator, DeltaTimeX, 6.0f);
		}
		else
		{
			FVector HeadMeshParentLocation = MeshParent->GetSocketLocation("NeckSocket");

			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(HeadMeshParentLocation, TargetLocation);

			FRotator TargetRotationHead = FRotator::ZeroRotator;
			TargetRotationHead.Roll = LookAtRotation.Pitch * (-1.0f);

			// Normalize the rotation
			FRotator Delta = LookAtRotation - OwningPawn->GetActorRotation();

			Delta.Normalize();
			TargetRotationHead.Yaw = Delta.Yaw;
			TargetRotationHead.Pitch = 0.0f;

			float Roll = FMath::Abs(TargetRotationHead.Roll);
			float Yaw = FMath::Abs(TargetRotationHead.Yaw);
			if ((Roll > MaxRotationHead) || (Yaw > MaxRotationHead))
			{
				TargetRotationHead.Roll = 0.0f;
				TargetRotationHead.Yaw = 0.0f;
			}


			RotationHead = FMath::RInterpTo(RotationHead, TargetRotationHead, DeltaTimeX, 6.0f);
		}
	}

}


void UComplexAnimInstance::StartLookAtActor(AActor* ActorTarget, USkeletalMeshComponent* MeshParentRef)
{
	ActorToLookAt = ActorTarget;

	MeshParent = MeshParentRef;

	bResetLookAt = false;

	RotationHeadAlpha = 1.0f;

	bEnableLookAt = true;
}

void UComplexAnimInstance::StopLookAt()
{
	ActorToLookAt = nullptr;

	RotationHeadAlpha = 1.0f;

	bEnableLookAt = true;

	bResetLookAt = true;
}


void UComplexAnimInstance::ChangeAnimation(FName NewAnimation)
{
	if (CurrentAnimation != NewAnimation)
	{
		CurrentAnimation = NewAnimation;
	}
}


