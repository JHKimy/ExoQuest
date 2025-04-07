#include "Enemy/Enemy2/Enemy2AIController.h"
#include "BehaviorTree//BehaviorTree.h"

AEnemy2AIController::AEnemy2AIController()
{
	//static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("/Game/BluePrint/Enemy/Enemy2/BT/IDLE/BT_Enemy2Idle.BT_Enemy2Idle"));
	//if (BT.Succeeded())
	//{
	//	BT_Enemy2Idle = BT.Object;
	//}
	BBComponent = GetBlackboardComponent();  //  부모 클래스에서 가져오기

}

void AEnemy2AIController::RunIdleBT()
{

	if (UseBlackboard(BT_Enemy2Idle->BlackboardAsset, BBComponent))
	{
		RunBehaviorTree(BT_Enemy2Idle);
	}
}

void AEnemy2AIController::RunPatrolBT()
{
	if (UseBlackboard(BT_Enemy2Patrol->BlackboardAsset, BBComponent))
	{
		RunBehaviorTree(BT_Enemy2Patrol);
	}
}

void AEnemy2AIController::RunChaseBT()
{
	if (UseBlackboard(BT_Enemy2Chase->BlackboardAsset, BBComponent))
	{
		RunBehaviorTree(BT_Enemy2Chase);
	}
}

void AEnemy2AIController::RunAttackBT()
{
	if (UseBlackboard(BT_Enemy2Attack->BlackboardAsset, BBComponent))
	{
		RunBehaviorTree(BT_Enemy2Attack);
	}
}

