// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "PatrolRoute.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BlackboardComponent{ OwnerComp.GetBlackboardComponent() };

	int32 CurrentIndex{ BlackboardComponent->GetValueAsInt(NextWaypointIndexKey.SelectedKeyName) };

	bool success{ false };

	auto AIController{ OwnerComp.GetAIOwner() };
	if (ensure(AIController))
	{
		auto AIPawn{ (AIController->GetPawn()) };
		if (ensure(AIPawn))
		{
			auto PatrolRoute{ AIPawn->FindComponentByClass<UPatrolRoute>() };

			if (ensure(PatrolRoute))
			{
				auto Routes{ PatrolRoute->GetPatrolLocations() };
				if (Routes.Num() > 0)
				{
					if (CurrentIndex >= Routes.Num())
						CurrentIndex = 0;

					BlackboardComponent->SetValueAsVector(WaypointKey.SelectedKeyName, Routes[CurrentIndex]->GetActorLocation());

					success = true;
				}
			}
		}
	}

	CurrentIndex++;
	BlackboardComponent->SetValueAsInt(NextWaypointIndexKey.SelectedKeyName, CurrentIndex);

	if (success)
	{
		return EBTNodeResult::Type::Succeeded;
	}
	return EBTNodeResult::Type::Failed;
}