// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicInteractive.h"
#include "Components/BoxComponent.h"
#include "HowToCharacter.h"


ABasicInteractive::ABasicInteractive()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootScene;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

	Trigger->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABasicInteractive::BeginOverlap);
	Trigger->OnComponentEndOverlap.AddUniqueDynamic(this, &ABasicInteractive::EndOverlap);

	PrimaryActorTick.bCanEverTick = false;
}


void ABasicInteractive::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	PlayerCharacter = Cast<AHowToCharacter>(OtherActor);

	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->OnEnterActor(this);	

		OnPlayerBeginOverlap();
	}
}

void ABasicInteractive::OnPlayerBeginOverlap()
{

}


void ABasicInteractive::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->OnLeaveActor();

		OnPlayerEndOverlap();
	}
}

void ABasicInteractive::OnPlayerEndOverlap()
{

}




