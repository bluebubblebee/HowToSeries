// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestTypes.h"
#include "QuestManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class HOWTO_API UQuestManager : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest System")
	TObjectPtr<class UQuestData> QuestDatabase;

public:

	UFUNCTION(BlueprintCallable, Category = "Quest System")
	FQuest FindQuest(FName QuestID, bool& Success);
	
};
