// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "PatrollingGuard.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"//TODO: check if any includes can be removed
#include "Runtime/AIModule/Classes/AIController.h"
EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BlackboardComponent{ OwnerComp.GetBlackboardComponent() };

	int32 CurrentIndex{ BlackboardComponent->GetValueAsInt(NextWaypointIndexKey.SelectedKeyName) };

	auto AIController{ OwnerComp.GetAIOwner() };
	if (AIController)
	{
		auto PatrollingGuard{ Cast<APatrollingGuard>(AIController->GetPawn()) };
		if (PatrollingGuard)
		{
			auto Array{ PatrollingGuard->GetPatrolWaypointsCPP() };

			if (Array.Num() > 0)
			{
				if (CurrentIndex >= Array.Num())
					CurrentIndex = 0;

				BlackboardComponent->SetValueAsObject(WaypointKey.SelectedKeyName, Array[CurrentIndex]);
			}
		}
	}

	CurrentIndex++;
	BlackboardComponent->SetValueAsInt(NextWaypointIndexKey.SelectedKeyName, CurrentIndex);

	return EBTNodeResult::Type::Succeeded;
}