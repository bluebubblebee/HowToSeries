
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicInteractive.generated.h"

UCLASS()
class HOWTO_API ABasicInteractive : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
	class UBoxComponent* Trigger;

	class AHowToCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive")
	FName InteractiveName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive")
	FName QuestID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactive")
	bool QuestActivated;
	
public:	

	ABasicInteractive();

protected:

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult &SweepResult);

	virtual void OnPlayerBeginOverlap();

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void OnPlayerEndOverlap();

};
