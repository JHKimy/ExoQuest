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
		return; // AnimCharacter가 nullptr이면 함수 종료
	}

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
	if (!AnimCharacter) return; // 캐릭터가 없으면 종료

	// 부착된 컴포넌트 중 "Grenade" 소켓에 있는 StaticMeshComponent 제거
	if (AnimCharacter->GrenadeMeshComponent)
	{
		AnimCharacter->GrenadeMeshComponent->DestroyComponent();
		AnimCharacter->GrenadeMeshComponent = nullptr;
	}

	// 기존 부착된 수류탄 찾기
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

	// 기존 수류탄이 있으면 제거
	if (EquippedGrenade)
	{
		EquippedGrenade->collisionComponent->IgnoreActorWhenMoving(AnimCharacter, true);
		EquippedGrenade->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedGrenade->collisionComponent->SetSimulatePhysics(true);
		EquippedGrenade->Destroy();
	}

	// 새 수류탄 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = AnimCharacter; // 소유자 설정

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
	UKismetSystemLibrary::PrintString(this, TEXT("Pause 실행됨!"), true, true, FLinearColor::Red, 3.0f);

	Montage_Pause(nullptr);
	
	// 0.5초 후에 애니메이션 다시 재생
	GetWorld()->GetTimerManager().SetTimer(PauseTimerHandle, this, &UEQAnimInstance::ResumeMontage, 0.5f, false);
	
}

void UEQAnimInstance::ResumeMontage()
{
	Montage_Resume(nullptr);
	UKismetSystemLibrary::PrintString(this, TEXT("Pause 끝, 애니메이션 재생"), true, true, FLinearColor::Green, 3.0f);
	
}