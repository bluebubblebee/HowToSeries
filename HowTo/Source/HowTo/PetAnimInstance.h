
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PetAnimInstance.generated.h"


UCLASS()
class HOWTO_API UPetAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float Speed;
	
};
