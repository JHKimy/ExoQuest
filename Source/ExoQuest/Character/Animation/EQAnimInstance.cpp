#include "Character/Animation/EQAnimInstance.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEQAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//Super::NativeUpdateAnimation(DeltaSeconds);

	AnimCharacter = Cast<ACharacterBase>(TryGetPawnOwner());

	if (AnimCharacter)
	{
		// ���콺 Ŭ�� ���ǵ� ����
		mouseClickMoveSpeed = AnimCharacter->GetVelocity().Size();

		// �ӵ��� �캤�� ���� -> ������ ���� / ����� ������
		RLSpeed = FVector::DotProduct
		(AnimCharacter->GetActorRightVector(), AnimCharacter->GetVelocity());

		// �ӵ��� ���溤�� ������
		FBSpeed = FVector::DotProduct
		(AnimCharacter->GetActorForwardVector(), AnimCharacter->GetVelocity());


		// ���������� Ȯ��
		isFalling = AnimCharacter->GetCharacterMovement()->IsFalling();
		
		// �ӷ� ���ϱ�
		velocity = AnimCharacter->GetVelocity().Size();



		// ������������ �ľ�
		if (velocity > 0) {
			bIsMoving = true;
		}
		else if (velocity == 0) {
			bIsMoving = false;
		}
	}



	switch (AnimCharacter->EQCharacterState)
	{
	case ECharacterState::NoWeaponMode:

		break;

	case ECharacterState::RifleMode:

		break;

	case ECharacterState::ShotgunMode:
		break;

	case ECharacterState::SwordMode:
		break;
	}

}

void UEQAnimInstance::SetCharacterState(ECharacterState NewState)
{
	CharacterState = NewState;
}
