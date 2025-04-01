// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy1HPBar.h"
#include "Enemy/EnemyBase.h"
#include "Components/ProgressBar.h"


void UEnemy1HPBar::NativeConstruct()
{
	percent = 1;
}

void UEnemy1HPBar::Init(AEnemyCharacter* OwnerCharacter)
{
	Owner = OwnerCharacter;
}

void UEnemy1HPBar::UpdateHP(float Percent)
{
	// HP_ProgressBar->SetPercent(Percent);
	// HP_ProgressBar = Percent;
	percent = Percent;
}
