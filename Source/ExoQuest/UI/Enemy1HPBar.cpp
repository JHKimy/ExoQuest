// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy1HPBar.h"
#include "Enemy/EnemyBase.h"
#include "Components/ProgressBar.h"


void UEnemy1HPBar::NativeConstruct()
{
	healthPercent = maxHealthPercent;

}

void UEnemy1HPBar::UpdateHP(float Percent)
{
	healthPercent = Percent;
}
