#include "Character/Animation/EQAnimInstance.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimNotifies/AnimNotify.h"

void UEQAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//Super::NativeUpdateAnimation(DeltaSeconds);

	AnimCharacter = Cast<ACharacterBase>(TryGetPawnOwner());

	if (AnimCharacter)
	{
		// 마우스 클릭 스피드 정리
		mouseClickMoveSpeed = AnimCharacter->GetVelocity().Size();

		// 속도와 우벡터 내적 -> 음수면 왼쪽 / 양수면 오른쪽
		RLSpeed = FVector::DotProduct
		(AnimCharacter->GetActorRightVector(), AnimCharacter->GetVelocity());

		// 속도와 전방벡터 방향계산
		FBSpeed = FVector::DotProduct
		(AnimCharacter->GetActorForwardVector(), AnimCharacter->GetVelocity());


		// 점프중인지 확인
		isFalling = AnimCharacter->GetCharacterMovement()->IsFalling();
		
		// 속력 구하기
		velocity = AnimCharacter->GetVelocity().Size();



		// 정지상태인지 파악
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


void UEQAnimInstance::AnimNotify_EndThrow()
{
	UE_LOG(LogTemp, Warning, TEXT("EndThrow Notify Triggered"));

	AnimCharacter = Cast<ACharacterBase>(TryGetPawnOwner());

	// 무기를 다시 오른손 소켓으로 이동
	FName RifleSocket = FName(TEXT("WeaponLeft"));
	//FName ShotgunSocket = FName(TEXT("Shotgun"));
	//FName RocketLauncherSocket = FName(TEXT("RocketLauncher"));
	//FName SwordSocket = FName(TEXT("Sword"));

	FName RightHandSocket = FName(TEXT("Rifle"));

	// 오른손 소켓에 부착된 액터 가져오기
	AActor* attachedActor = nullptr;

	for (USceneComponent* ChildComp : AnimCharacter->GetMesh()->GetAttachChildren())
	{
		if (ChildComp && ChildComp->GetAttachSocketName() == RifleSocket)
		{
			attachedActor = Cast<AActor>(ChildComp->GetOwner());
			break;
		}
	}

	if (attachedActor)
	{
		// 오른손에 붙어 있는 무기를 왼손 소켓으로 이동
		attachedActor->AttachToComponent(
			AnimCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			RightHandSocket
		);
	}
}