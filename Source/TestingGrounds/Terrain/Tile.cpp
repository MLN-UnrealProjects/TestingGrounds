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

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MaxSpawnNumber, int32 MinSpawnNumber, float Radius, int32 MaxTries, float MinScale, float MaxScale)
{
	int32 NumToSpawn{ FMath::RandRange(MinSpawnNumber,MaxSpawnNumber) };
	for (size_t i = 0; i < NumToSpawn; i++)
	{
		bool ValidLocation{ true };
		float Scale = FMath::RandRange(MinScale, MaxScale);
		FVector SpawnLocation{ GetEmptyLocalLocationOnTerrain(ValidLocation, Radius * Scale, MaxTries) };
		if (ValidLocation)
		{
			PlaceActor(ToSpawn, SpawnLocation, FMath::RandRange(MinRotation, MaxRotation), Scale);
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

AActor* ATile::PlaceActor(const TSubclassOf<AActor>& ToSpawn, FVector SpawnPoint, float Rotation, float Scale)
{
	AActor* SpawnedProp{ GetWorld()->SpawnActor(ToSpawn) };

	SpawnedProp->SetActorRelativeTransform(FTransform(FRotator{ 0.0f,Rotation,0.0f }, SpawnPoint, FVector(Scale)));

	SpawnedProp->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));

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
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The navmeshboundspool does not contain enough elements!!"));
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
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Error occurred while destroying Tile obj, navmeshbounds is invalid"));
	}
	NavMeshVolume = nullptr;
	NavMeshBoundsPool = nullptr;
	Super::EndPlay(EndPlayReason);
}

