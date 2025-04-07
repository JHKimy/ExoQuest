#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy/EnemyFSM.h"
#include "Enemy2AnimInstance.generated.h"

UENUM(BlueprintType)
enum class EIdleActionType : uint8
{
	Default       UMETA(DisplayName = "Default"),
	LookAround    UMETA(DisplayName = "Look Around"),
	Stretching    UMETA(DisplayName = "Stretching"),
	
	
	Relaxed       UMETA(DisplayName = "Relaxed"),
	Alert         UMETA(DisplayName = "Alert"),
};



UCLASS()
class EXOQUEST_API UEnemy2AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Idle")
	EIdleActionType IdleActionType = EIdleActionType::Default;

	//UFUNCTION(BlueprintCallable)
	//void SetIdleActionType(int num);

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


	//// IK
	//UPROPERTY(BlueprintReadOnly, Category = "IK")
	//bool bEnableIK = true;

	//UFUNCTION(BlueprintCallable)
	//void SetEnableIK();

	UPROPERTY(BlueprintReadOnly, Category = "IK")
	FVector LeftHandEffector;

	UPROPERTY(BlueprintReadOnly, Category = "IK")
	FRotator LeftHandRotation;

	UFUNCTION(BlueprintCallable, Category = "IK")
	void SetLeftHandIKTransform(const FTransform& InTransform);
};
