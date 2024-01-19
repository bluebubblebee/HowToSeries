#include "PetFollower.h"
#include "Kismet/GameplayStatics.h"
#include "PetAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"


APetFollower::APetFollower()
{ 	
	PrimaryActorTick.bCanEverTick = true;
}


void APetFollower::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	if (GetMesh() != nullptr)
	{
		Animator = Cast<UPetAnimInstance>(GetMesh()->GetAnimInstance());
	}

	if (Animator != nullptr)
	{
		Animator->Speed = 0.0f;
	}

	CurrentState = 0;

	LastLocation = GetActorLocation();	
}


void APetFollower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = 0.0f;

	FVector OtherActorPosition = ActorToFollow->GetActorLocation();
	OtherActorPosition.Z = 0.0f;

	float DistanceToActor = FVector::Dist(OtherActorPosition, CurrentLocation);	

	if (CurrentState == 1) // Initial State
	{
		if (DistanceToActor < 50)
		{
			LastLocation = GetActorLocation();

			CurrentState = 2;

		}

		if (Animator != nullptr)
		{
			Animator->Speed = 0.0f;
		}
	}

	else if (CurrentState == 2) // Follow player State
	{
		// Check if pet is close to the player
		float Speed = SpeedMovement;
		if (DistanceToActor < StopRadius)
		{
			float SpeedFactor = UKismetMathLibrary::NormalizeToRange(DistanceToActor, MinimunDistanceToActor, StopRadius);
			Speed *= SpeedFactor;
		}

		if (Speed > 5.0f)
		{
			// Calculate Direction and rotation
			FVector DirectionToFollow = OtherActorPosition - CurrentLocation;
			DirectionToFollow = DirectionToFollow.GetSafeNormal();
			DirectionToFollow.Z = 0.0f;

			FRotator TargetRotation = DirectionToFollow.Rotation();
			TargetRotation.Pitch = 0.0f;
			TargetRotation.Roll = 0.0f;

			// Move and rotate the pet
			FVector NewLocation = GetActorLocation();
			NewLocation += DirectionToFollow * Speed * DeltaTime;
			FRotator CurrentRotation = GetActorRotation();
			CurrentRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 5.0f);

			// Third paramenter make the pet collide with other elements
			SetActorLocationAndRotation(NewLocation, CurrentRotation, true);

			float DistanceToLastLoc = FVector::Dist(GetActorLocation(), LastLocation);

			// Stop completely if current speed is very small
			float CurrentSpeed = DistanceToLastLoc / DeltaTime;
			if (CurrentSpeed < 20.0f)
			{
				Speed = 0.0f;
				CurrentState = 3;
			}
		}	

		if (Animator != nullptr)
		{
			Animator->Speed = Speed;
		}

	}

	else if (CurrentState == 3)
	{
		if (DistanceToActor >= StopRadius) // Resume movement
		{
			CurrentState = 2;
		}
	}


	LastLocation = GetActorLocation();
}


void APetFollower::StartFollow_Implementation(AActor* Actor)
{
	ActorToFollow = Actor;	

	if (ActorToFollow != nullptr)
	{
		CurrentState = 1;
		SetActorTickEnabled(true);
	}
}


void APetFollower::StopFollow_Implementation()
{
	CurrentState = 0;

	if (Animator != nullptr)
	{
		Animator->Speed = 0.0f;
	}
}


