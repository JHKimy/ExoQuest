// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyFSM.h"
#include "Enemy1FSM.generated.h"

class Enemy1DeathState : public EnemyDeathState {
public:
	void Enter(UEnemyFSM* FSM) override;
	void Update(UEnemyFSM* FSM, float DeltaTime) override;
};

UCLASS()
class EXOQUEST_API UEnemy1FSM : public UEnemyFSM
{
	GENERATED_BODY()

public:
	UEnemy1FSM();
	Enemy1DeathState Enemy1DeathState;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};


