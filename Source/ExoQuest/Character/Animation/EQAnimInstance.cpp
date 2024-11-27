#include "Character/Animation/EQAnimInstance.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Weapon/Rifle.h"
#include "Weapon/Shotgun.h"
#include "Weapon/RocketLauncher.h"
#include "Weapon/Sword.h"

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
	AnimCharacter = Cast<ACharacterBase>(TryGetPawnOwner());

	// 무기를 다시 오른손 소켓으로 이동
	FName LeftHandSocket = FName(TEXT("WeaponLeft"));
	FName LeftSwordSocket = FName(TEXT("SwordLeft"));
	

	FName RifleSocket = FName(TEXT("Rifle"));
	FName ShotgunSocket = FName(TEXT("Shotgun"));
	FName RocketLauncherSocket = FName(TEXT("RocketLauncher"));
	FName SwordSocket = FName(TEXT("Sword"));


	// 오른손 소켓에 부착된 액터 가져오기
	AActor* attachedActor = nullptr;

	for (USceneComponent* ChildComp : AnimCharacter->GetMesh()->GetAttachChildren())
	{
		if (ChildComp) 
		{
			if (ChildComp->GetAttachSocketName() == LeftHandSocket 
				|| ChildComp->GetAttachSocketName() == LeftSwordSocket)
			{
				attachedActor = Cast<AActor>(ChildComp->GetOwner());
				
				break;
			}
		}
	}

	if (attachedActor)
	{
		// 무기 타입과 소켓 이름을 매핑
		TMap<UClass*, FName> WeaponSocketMap;
		WeaponSocketMap.Add(ARifle::StaticClass(), RifleSocket);
		WeaponSocketMap.Add(AShotgun::StaticClass(), ShotgunSocket);
		WeaponSocketMap.Add(ARocketLauncher::StaticClass(), RocketLauncherSocket);
		WeaponSocketMap.Add(ASword::StaticClass(), SwordSocket);

		// 무기 타입에 맞는 소켓 이름 가져오기
		UClass* WeaponClass = attachedActor->GetClass();

		if (FName* TargetSocket = WeaponSocketMap.Find(WeaponClass))
		{
			// 부착
			attachedActor->AttachToComponent(
				AnimCharacter->GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				*TargetSocket
			);
		}
	}
}