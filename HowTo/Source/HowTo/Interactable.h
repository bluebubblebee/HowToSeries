
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HOWTO_API IInteractable
{
	GENERATED_BODY()

public:

	virtual FName GetName() = 0;

	virtual FName GetQuestID() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void OnInteract();
	
};
