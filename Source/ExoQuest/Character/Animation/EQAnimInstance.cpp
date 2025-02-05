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
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"


#include "Kismet/KismetSystemLibrary.h"

void UEQAnimInstance::NativeInitializeAnimation()
{
	AnimCharacter = Cast<ACharacterBase>(TryGetPawnOwner());


}

void UEQAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//Super::NativeUpdateAnimation(DeltaSeconds);

	

	if (!AnimCharacter)
	{
		return; // AnimCharacter�� nullptr�̸� �Լ� ����
	}

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


	pitch = AnimCharacter->GetBaseAimRotation().Pitch;



	if (bIsMoving) 
	{
		rootYawOffset = 0;
	}

	else if (!bIsMoving) 
	{
		prevCharacterYaw = characterYaw;

		characterYaw = AnimCharacter->GetActorRotation().Yaw;
	
		rootYawOffset = UKismetMathLibrary::NormalizeAxis
		(rootYawOffset - (characterYaw - prevCharacterYaw));
	
		rootYawOffset = FMath::Clamp(rootYawOffset, -90.0f, 90.0f);
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
	if (!AnimCharacter) return; // ĳ���Ͱ� ������ ����

	// ������ ������Ʈ �� "Grenade" ���Ͽ� �ִ� StaticMeshComponent ����
	if (AnimCharacter->GrenadeMeshComponent)
	{
		AnimCharacter->GrenadeMeshComponent->DestroyComponent();
		AnimCharacter->GrenadeMeshComponent = nullptr;
	}

	// ���� ������ ����ź ã��
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

	// ���� ����ź�� ������ ����
	if (EquippedGrenade)
	{
		EquippedGrenade->collisionComponent->IgnoreActorWhenMoving(AnimCharacter, true);
		EquippedGrenade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedGrenade->collisionComponent->SetSimulatePhysics(true);
		EquippedGrenade->Destroy();
	}

	// �� ����ź ����
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = AnimCharacter; // ������ ����

	GetWorld()->SpawnActor<ABasicGrenade>(
		ABasicGrenade::StaticClass(),
		AnimCharacter->LaunchPosition->GetComponentLocation(),
		AnimCharacter->LaunchPosition->GetComponentRotation(),
		SpawnParams
	);

}

void UEQAnimInstance::AnimNotify_Aim()
{
	if (AnimCharacter->bIsThrowingGrenade) {
		Montage_Pause(ThrowGrenadeMontage);
	}
	else {
		return;
	}
	//else {
	//	Montage_Resume(ThrowGrenadeMontage);
	//}
}

void UEQAnimInstance::AnimNotify_Pause()
{
	UKismetSystemLibrary::PrintString(this, TEXT("Pause �����!"), true, true, FLinearColor::Red, 3.0f);

	Montage_Pause(nullptr);
	
	// 0.5�� �Ŀ� �ִϸ��̼� �ٽ� ���
	GetWorld()->GetTimerManager().SetTimer(PauseTimerHandle, this, &UEQAnimInstance::ResumeMontage, 0.5f, false);
	
}

void UEQAnimInstance::ResumeMontage()
{
	Montage_Resume(nullptr);
	UKismetSystemLibrary::PrintString(this, TEXT("Pause ��, �ִϸ��̼� ���"), true, true, FLinearColor::Green, 3.0f);
	
}