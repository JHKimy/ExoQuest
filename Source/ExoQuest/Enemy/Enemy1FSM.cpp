// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy1FSM.h"
#include "Enemy/EnemyFSM.h"
#include "Enemy/Enemy1.h"
#include "Enemy/Enemy1AnimInstance.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>

UEnemy1FSM::UEnemy1FSM()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemy1FSM::BeginPlay()
{
	Super::BeginPlay();

	StateMap.Add(EEnemyState::Death, &Enemy1DeathState);

	// enemy = Cast<AEnemy1>(GetOwner());

	//Enemy1DeathState CustomDeathState;
	//StateMap.Add(EEnemyState::Death, &CustomDeathState);
	
	// target = Cast<ACharacterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass()));
	// enemy = Cast<AEnemy1>(GetOwner());
	// anim = Cast<UEnemy1AnimInstance>(enemy->GetMesh()->GetAnimInstance()) : nullptr;

	//StateMap.Add(EEnemyState::Idle, &IdleState);
	//StateMap.Add(EEnemyState::Move, &MoveState);
	//StateMap.Add(EEnemyState::Attack, &AttackState);
	//StateMap.Add(EEnemyState::Damage, &DamageState);
	//StateMap.Add(EEnemyState::Death, &DeathState);

	//ChangeState(EEnemyState::Idle);
}

void UEnemy1FSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if (CurrentState)
	//{
	//	CurrentState->Update(this, DeltaTime);
	//}
}

//void Enemy1DamageState::Enter(UEnemyFSM* FSM)
//{
//	Super::Enter(FSM); // 공통 동작 유지
//	AEnemy1* enemy1 = Cast<AEnemy1>(FSM->enemy);
//	enemy1->Death();
//}

void Enemy1DeathState::Enter(UEnemyFSM* FSM)

{    // 부모 인스턴스를 가리키는 포인터가 필요
	EnemyDeathState::Enter(FSM);

	AEnemy1* enemy1 = Cast<AEnemy1>(FSM->enemy);
	enemy1->Death();
}

void Enemy1DeathState::Update(UEnemyFSM* FSM, float DeltaTime)
{
	AEnemy1* enemy1 = Cast<AEnemy1>(FSM->enemy);
	if (enemy1)
	{
		enemy1->Death(); // 매 프레임 호출됨
	}
}