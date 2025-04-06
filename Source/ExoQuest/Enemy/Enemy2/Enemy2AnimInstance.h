#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy/EnemyFSM.h"
#include "Enemy2AnimInstance.generated.h"

UCLASS()
class EXOQUEST_API UEnemy2AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class AEnemy2* enemyOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	EEnemyState animState;

	UFUNCTION(BlueprintCallable, Category = "AnimState")
	void SetAnimState(EEnemyState newState);

	UFUNCTION(BlueprintCallable, Category = "Anim")
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* HitMontage;

	UFUNCTION(BlueprintCallable)
	void PlayHitMontage();
};
