// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "Enemy2.generated.h"

/**
 * 
 */
UCLASS()
class EXOQUEST_API AEnemy2 : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemy2();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Animation)
	class UEnemy1AnimInstance* anim;

	virtual float TakeDamage
	(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	void Death() override;
};
