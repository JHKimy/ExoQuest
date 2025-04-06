// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "Enemy/EnemyFSM.h"
#include "Enemy2.generated.h"


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	class UEnemy2AnimInstance* anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class AAIController* AI;
	

	//  Enemy2의 라이플 무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle")
	TObjectPtr<class AEnemyRifle> EnemyRifle;


	// 총을 부착할 소켓 이름 (스켈레탈 메시 본 기준)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle")
	FName RifleSocketName = "RightHandSocket";


	 UFUNCTION(BlueprintCallable, Category = "Animation")
	 void SetAnimState(EEnemyState NewState);


	virtual float TakeDamage
	(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	void Death() override;

};
