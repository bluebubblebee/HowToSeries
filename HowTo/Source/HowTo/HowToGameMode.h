// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Definitions.h"
#include "HowToGameMode.generated.h"

UCLASS(minimalapi)
class AHowToGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHowToGameMode();

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest System")
	TSubclassOf<class UQuestManager> QuestManagerObject;

	TObjectPtr<class UQuestManager> QuestManager;

public:

	FORCEINLINE UQuestManager* GetQuestManager(){ return QuestManager; }


///////////////////////// Inventory ////////////////////////////////
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item System")
	TObjectPtr<class UItemData> ItemDatabase;

public:

	UFUNCTION(BlueprintCallable,Category = "Item System")
	FItem FindItem(FName ItemID, bool& Success);

	///////////////////////// Inventory ////////////////////////////////

};



