// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ATile();
	UFUNCTION(BlueprintCallable)
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MaxSpawnNumber = 1, int32 MinSpawnNumber = 1, float Radius = 500.0f, int32 MaxTries = 200, float MinScale = 1.f, float MaxScale = 1.f);

	UFUNCTION(BlueprintCallable)
	void SetNavMeshBoundsPool(class UActorPool* NavMeshBoundsPool);
private:
	const float MaxRotation = 180.0f;
	const float MinRotation = -180.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	FVector MinSpawnBound = FVector{ 0.0f,-2000.0f, 100.0f };

	UPROPERTY(EditDefaultsOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	FVector MaxSpawnBound = FVector{ 4000.0f, 2000.0f, 100.0f };

	UFUNCTION(BlueprintCallable)
	bool IsSphereColliding(FVector WorldCenterLocation, float Radius = 500.0f) const;

	UFUNCTION(BlueprintCallable)
	FVector GetEmptyLocalLocationOnTerrain(bool& bValidLocation, float EmptyRadius = 500.0f, int32 MaxTries = 200) const;

	UFUNCTION(BlueprintCallable)
	AActor* PlaceActor(const TSubclassOf<AActor>& ToSpawn, FVector SpawnPoint, float Rotation, float Scale);

	void GetAndSetNavMeshBoundsLocation();
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Pool")
	class UActorPool* NavMeshBoundsPool = nullptr;
	UPROPERTY()
	AActor* NavMeshVolume = nullptr;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
