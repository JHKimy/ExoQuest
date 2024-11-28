#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNotifies/AnimNotify.h"	// ��Ƽ����
#include "EQAnimInstance.generated.h"

//// ĳ���� ����
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

	//// �ִϸ��̼� ����
	//EAnimState EQAnimState;

	// �÷��̾� �̵� �ӵ�
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

	// ĳ���� ����
	UPROPERTY(BlueprintReadOnly, Category = "Character State")
	ECharacterState CharacterState;


	virtual void NativeUpdateAnimation(float DeltaSeconds)override;


	UFUNCTION(BlueprintCallable, Category = "Character State")
	void SetCharacterState(ECharacterState NewState);



	UFUNCTION()
	void AnimNotify_EndThrow();


	UFUNCTION()
	void AnimNotify_Throw();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Grenade")
	class ABasicGrenade* EquippedGrenade;

};