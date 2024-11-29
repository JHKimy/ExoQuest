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


void UEQAnimInstance::AnimNotify_EndThrow()
{
	AnimCharacter = Cast<ACharacterBase>(TryGetPawnOwner());

	// ���⸦ �ٽ� ������ �������� �̵�
	FName LeftHandSocket = FName(TEXT("WeaponLeft"));
	FName LeftSwordSocket = FName(TEXT("SwordLeft"));
	

	FName RifleSocket = FName(TEXT("Rifle"));
	FName ShotgunSocket = FName(TEXT("Shotgun"));
	FName RocketLauncherSocket = FName(TEXT("RocketLauncher"));
	FName SwordSocket = FName(TEXT("Sword"));


	// ������ ���Ͽ� ������ ���� ��������
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
		// ���� Ÿ�԰� ���� �̸��� ����
		TMap<UClass*, FName> WeaponSocketMap;
		WeaponSocketMap.Add(ARifle::StaticClass(), RifleSocket);
		WeaponSocketMap.Add(AShotgun::StaticClass(), ShotgunSocket);
		WeaponSocketMap.Add(ARocketLauncher::StaticClass(), RocketLauncherSocket);
		WeaponSocketMap.Add(ASword::StaticClass(), SwordSocket);

		// ���� Ÿ�Կ� �´� ���� �̸� ��������
		UClass* WeaponClass = attachedActor->GetClass();

		if (FName* TargetSocket = WeaponSocketMap.Find(WeaponClass))
		{
			// ����
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
	// Ư�� ���Ͽ��� ������ ���� ��������
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
	



	// ����ź�� ���� ���� ǥ��
	// EquippedGrenade->PredictGrenadePath(AnimCharacter->GrenadeLaunchVelocity);

	// �����ڿ��� �浹 ���� ����
	EquippedGrenade->collisionComponent->IgnoreActorWhenMoving(AnimCharacter, true);

	// ����ź ���Ͽ��� �и�
	EquippedGrenade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	EquippedGrenade->collisionComponent->SetSimulatePhysics(true);
	

	// �ʱ� �߻� �ӵ� ���� (������ ȿ���� ���� Z�� �ӵ� �߰�)
	FVector LaunchVelocity = AnimCharacter->GrenadeLaunchVelocity;
	LaunchVelocity.Z += 500.0f; // Z�� ���� �ӵ� �߰�
	
	// ������ ������
	EquippedGrenade->collisionComponent
		->AddImpulse(LaunchVelocity, NAME_None, true);
	// �ʿ�� ���� ��ü ����� �ٽ� ���� �ؼ� ������Ÿ�Ϸ� ������ ���

}
