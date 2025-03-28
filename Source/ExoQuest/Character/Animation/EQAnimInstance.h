#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNotifies/AnimNotify.h"	// 노티파이






#include "Kismet/KismetSystemLibrary.h"



#include "EQAnimInstance.generated.h"








//// 캐릭터 상태
//UENUM(BlueprintType)
//enum class EAnimState : uint8
//{
//	NoWeaponMode,
//	RifleMode,
//	ShotgunMode,
//	SwordMode
//};

UCLASS()
class EXOQUEST_API UEQAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Character)
	class ACharacterBase* AnimCharacter;




	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Animation")
	class UAnimMontage* ThrowGrenadeMontage;



	//// 애니메이션 상태
	//EAnimState EQAnimState;

	// 플레이어 이동 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = MouseClickMovement)
	float mouseClickMoveSpeed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BS)
	float RLSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BS)
	float FBSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	bool isFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Velocity)
	float velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Move)
	bool bIsMoving;

	// 캐릭터 상태
	UPROPERTY(BlueprintReadOnly, Category = "Character State")
	ECharacterState CharacterState;

	virtual void NativeInitializeAnimation() override;


	virtual void NativeUpdateAnimation(float DeltaSeconds)override;


	UFUNCTION(BlueprintCallable, Category = "Character State")
	void SetCharacterState(ECharacterState NewState);



	UFUNCTION()
	void AnimNotify_EndThrow();


	UFUNCTION()
	void AnimNotify_Throw();






	UFUNCTION()
	void AnimNotify_Aim();


	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Grenade")
	class ABasicGrenade* EquippedGrenade;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	float characterYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	float prevCharacterYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	float rootYawOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AimOffset")
	float pitch;


	UFUNCTION()
	void AnimNotify_Pause();

	UFUNCTION()
	void ResumeMontage();

	FTimerHandle PauseTimerHandle;  // 타이머 핸들 추가

};