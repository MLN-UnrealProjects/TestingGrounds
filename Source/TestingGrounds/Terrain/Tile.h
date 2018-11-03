// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//TODO: Move all blueprints logic to c++ for exercise
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT(BlueprintType)
struct FTileSpawnParametersBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	int32 MaxSpawnNumber = 1;
	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	int32 MinSpawnNumber = 1;
	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	float Radius = 50.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	int32 MaxTries = 200;
};
USTRUCT(BlueprintType)
struct FTileAIPawnSpawnParameters
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	FTileSpawnParametersBase BaseParams;
	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	FName Tag = "Enemy";
};
USTRUCT(BlueprintType)
struct FTileSpawnParameters
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	FTileSpawnParametersBase BaseParams;
	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	float MinScale = 1.f;
	UPROPERTY(BlueprintReadWrite, Category = "Parameters")
	float MaxScale = 1.f;
};
USTRUCT(BlueprintType)
struct FTileSpawnData
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	FVector Location = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	float Rotation = 0.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	float Scale = 1.0f;
};

UCLASS()
class TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ATile();
	UFUNCTION(BlueprintCallable , Category = "Spawn")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, const FTileSpawnParameters& Parameters);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlaceAIPawns(TSubclassOf<class APawn> ToSpawn, const FTileAIPawnSpawnParameters& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetNavMeshBoundsPool(class UActorPool* NavMeshBoundsPool);
private:
	const float MaxRotation = 180.0f;
	const float MinRotation = -180.0f;
	const float PawnScale = 1.0f;


	UPROPERTY(EditDefaultsOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	FVector MinSpawnBound = FVector{ 0.0f,-2000.0f, 100.0f };

	UPROPERTY(EditDefaultsOnly, Category = "Setup", meta = (AllowPrivateAccess = "true"))
	FVector MaxSpawnBound = FVector{ 4000.0f, 2000.0f, 100.0f };

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	bool IsSphereColliding(FVector WorldCenterLocation, float Radius = 500.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetEmptyLocalLocationOnTerrain(bool& bValidLocation, float EmptyRadius = 500.0f, int32 MaxTries = 200) const;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	AActor* PlaceActor(const TSubclassOf<AActor>& ToSpawn, const FTileSpawnData& SpawnData);

	void GetAndSetNavMeshBoundsLocation();
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Pool")
	class UActorPool* NavMeshBoundsPool = nullptr;
	UPROPERTY()
	AActor* NavMeshVolume = nullptr;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
