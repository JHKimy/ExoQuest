#include "Character/Animation/EQAnimInstance.h"
#include "Character/CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Weapon/Rifle.h"
#include "Weapon/Shotgun.h"
#include "Weapon/RocketLauncher.h"
#include "Weapon/Sword.h"
#include "Weapon/Grenade/BasicGrenade.h"
#include "Components/SphereComponent.h"
#include <Camera/CameraComponent.h>


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

void UEQAnimInstance::AnimNotify_Throw()
{
	// 특정 소켓에서 부착된 액터 가져오기
	TArray<USceneComponent*> AttachedComponents = AnimCharacter->GetMesh()->GetAttachChildren();

	for (USceneComponent* Component : AttachedComponents) 
	{
		if (Component->GetAttachSocketName() == TEXT("Grenade"))
		{
			AActor* AttachedActor = Component->GetOwner();
			EquippedGrenade = Cast<ABasicGrenade>(AttachedActor);
			break;
		}
	}
	



	// 수류탄의 예상 궤적 표시
	// EquippedGrenade->PredictGrenadePath(AnimCharacter->GrenadeLaunchVelocity);

	// 소유자와의 충돌 무시 설정
	EquippedGrenade->collisionComponent->IgnoreActorWhenMoving(AnimCharacter, true);

	// 수류탄 소켓에서 분리
	EquippedGrenade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	EquippedGrenade->collisionComponent->SetSimulatePhysics(true);
	

	// 초기 발사 속도 설정 (포물선 효과를 위한 Z축 속도 추가)
	FVector LaunchVelocity = AnimCharacter->GrenadeLaunchVelocity;
	LaunchVelocity.Z += 500.0f; // Z축 방향 속도 추가
	
	// 실제로 던지기
	EquippedGrenade->collisionComponent
		->AddImpulse(LaunchVelocity, NAME_None, true);
	// 필요시 붙은 객체 지우고 다시 스폰 해서 프로젝타일로 던지는 방식

}
