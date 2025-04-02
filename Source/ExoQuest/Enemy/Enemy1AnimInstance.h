#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "Enemy1AnimInstance.generated.h"


UCLASS()
class EXOQUEST_API UEnemy1AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	class AEnemy1* enemyOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	EEnemyState animState;


	UFUNCTION()
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	bool bAttackPlay{false};

	UFUNCTION(BlueprintCallable, Category = FSMEvent)
	void OnEndAttackAnimation();


	UFUNCTION(BlueprintCallable)
	void AnimNotify_E1AttackStart();

	UFUNCTION(BlueprintCallable)
	void AnimNotify_E1AttackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* HitMontage;

	UFUNCTION(BlueprintCallable)
	void PlayHitMontage();



};
