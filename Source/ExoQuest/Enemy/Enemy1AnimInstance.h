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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	EEnemyState animState;

	UFUNCTION()
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
	bool bAttackPlay{false};

	UFUNCTION(BlueprintCallable, Category = FSMEvent)
	void OnEndAttackAnimation();

};
