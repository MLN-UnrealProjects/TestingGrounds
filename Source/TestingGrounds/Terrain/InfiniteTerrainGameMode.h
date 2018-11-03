// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TESTINGGROUNDS_API AInfiniteTerrainGameMode : public AGameModeBase
{
	GENERATED_BODY()
	AInfiniteTerrainGameMode();
protected:
	UFUNCTION(BlueprintCallable, Category = "Nav Bounds Pool")
	void PopulateBoundsInPool();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	class UActorPool* NavMeshBoundsPool = nullptr;
private:
	void AddToPool(class ANavMeshBoundsVolume* ToAdd);
};
