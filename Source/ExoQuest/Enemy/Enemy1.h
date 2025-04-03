#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.h"
#include "Enemy1.generated.h"

UCLASS()
class EXOQUEST_API AEnemy1 : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemy1();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 손에 충돌체
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* AttackCollision;

	UPROPERTY(EditAnywhere, Category = Animation)
	class UEnemy1AnimInstance* anim;

	UFUNCTION()
	void SetAnimState(EEnemyState NewState);

	UFUNCTION()
	void SetAttackPlay(bool b);

	// 손에 맞으면 실행
	UFUNCTION()
	void OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	virtual float TakeDamage
	(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category = Damage)
	bool bCanDamage = true;


	void Death() override;

	void MoveToTarget() override;
};
