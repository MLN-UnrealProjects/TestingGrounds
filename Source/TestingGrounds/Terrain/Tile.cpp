// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavMeshBoundsVolume.h"
#include "ActorPool.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"
// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
}
void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, const FTileSpawnParameters& Parameters)
{
	int32 NumToSpawn{ FMath::RandRange(Parameters.BaseParams.MinSpawnNumber,Parameters.BaseParams.MaxSpawnNumber) };
	for (size_t i = 0; i < NumToSpawn; i++)
	{
		bool ValidLocation{ true };
		FTileSpawnData Data;
		Data.Scale = FMath::RandRange(Parameters.MinScale, Parameters.MaxScale);
		Data.Location = GetEmptyLocalLocationOnTerrain(ValidLocation, Parameters.BaseParams.Radius * Data.Scale, Parameters.BaseParams.MaxTries);
		if (ValidLocation)
		{
			Data.Rotation = FMath::RandRange(MinRotation, MaxRotation);
			PlaceActor(ToSpawn, Data);
		}
	}
}

void ATile::PlaceAIPawns(TSubclassOf<class APawn> ToSpawn, const FTileAIPawnSpawnParameters& Parameters)
{
	//TODO: Remove duplicated code between placeactors and placeaipawns
	int32 NumToSpawn{ FMath::RandRange(Parameters.BaseParams.MinSpawnNumber,Parameters.BaseParams.MaxSpawnNumber) };
	for (size_t i = 0; i < NumToSpawn; i++)
	{
		bool ValidLocation{ true };
		FTileSpawnData Data;
		Data.Scale = PawnScale;
		Data.Location = GetEmptyLocalLocationOnTerrain(ValidLocation, Parameters.BaseParams.Radius * Data.Scale, Parameters.BaseParams.MaxTries);
		if (ValidLocation)
		{
			Data.Rotation = FMath::RandRange(MinRotation, MaxRotation);
			APawn* SpawnedPawn{ Cast<APawn>(PlaceActor(ToSpawn, Data)) };
			if (SpawnedPawn)
			{
				SpawnedPawn->SpawnDefaultController();
				SpawnedPawn->Tags.Push(Parameters.Tag);
			}
		}
	}
}

void ATile::SetNavMeshBoundsPool(UActorPool * NavMeshBoundsPool)
{
	this->NavMeshBoundsPool = NavMeshBoundsPool;
	GetAndSetNavMeshBoundsLocation();
}

bool ATile::IsSphereColliding(FVector WorldCenterLocation, float Radius) const
{
	FHitResult res;
	return GetWorld()->SweepSingleByChannel(res, WorldCenterLocation, WorldCenterLocation, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));;
}

FVector ATile::GetEmptyLocalLocationOnTerrain(bool& bValidLocation, float EmptyRadius, int32 MaxTries) const
{
	bValidLocation = true;
	for (size_t i = 0; i < MaxTries; i++)
	{
		FBox Bounds{ MinSpawnBound,MaxSpawnBound };
		FVector CandidateSpawnPoint{ FMath::RandPointInBox(Bounds) };
		FVector CandidateSpawnWorldPoint = ActorToWorld().TransformPosition(CandidateSpawnPoint);
		if (!IsSphereColliding(CandidateSpawnWorldPoint, EmptyRadius))
		{
			return CandidateSpawnPoint;
		}
	}
	bValidLocation = false;
	return FVector::ZeroVector;
}

AActor* ATile::PlaceActor(const TSubclassOf<AActor>& ToSpawn, const FTileSpawnData& Data)
{
	FRotator Rotation{ 0.0f,Data.Rotation,0.0f };
	AActor* SpawnedProp{ GetWorld()->SpawnActor(ToSpawn, &Data.Location, &Rotation) };

	if (SpawnedProp)
	{
		SpawnedProp->SetActorScale3D(FVector(Data.Scale));

		SpawnedProp->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

		ChildActors.Push(SpawnedProp);
	}

	return SpawnedProp;
}

void ATile::GetAndSetNavMeshBoundsLocation()
{
	if (NavMeshBoundsPool)
	{
		NavMeshVolume = NavMeshBoundsPool->Checkout();
		if (NavMeshVolume)
		{
			NavMeshVolume->SetActorLocation(FVector{ GetActorLocation() + (MinSpawnBound + MaxSpawnBound) * 0.5f });
			GetWorld()->GetNavigationSystem()->Build();
		}
	}
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (NavMeshVolume)
	{
		if (NavMeshBoundsPool)
		{
			NavMeshBoundsPool->Return(NavMeshVolume);
		}
	}
	NavMeshVolume = nullptr;
	NavMeshBoundsPool = nullptr;
	while (ChildActors.Num() != 0)
	{
		ChildActors.Pop()->Destroy();
	}
	OnTileDestroyed.Broadcast();
	Super::EndPlay(EndPlayReason);
}

