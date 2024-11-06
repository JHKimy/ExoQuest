#include "Enemy/EnemyFSM.h"
#include "Enemy/EnemyBase.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>		// 맵에 있는 인스턴스 가져오기
#include "Components/CapsuleComponent.h"
#include "Character/CharacterBase.h"
#include "Weapon/Rifle.h"
#include "Weapon/Shotgun.h"
#include "Weapon/RocketLauncher.h"
#include "Weapon/RocketProjectile.h"
#include "Weapon/Sword.h"


UEnemyFSM::UEnemyFSM()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass());

	target = Cast<ACharacterBase>(actor);

	enemy = Cast<AEnemyBase>(GetOwner());

	
	// 데미지 초기화
	//switch (target->PrimaryWeapon)
	//{
	//case EWeaponType::Rifle:
	//	rifleInstance = Cast<ARifle>(UGameplayStatics::GetActorOfClass(GetWorld(), ARifle::StaticClass()));
	//	rifleDamage = rifleInstance->damage;
	//	break;

	////case EWeaponType::Shotgun:
	////	shotgunInstance = Cast<AShotgun>(UGameplayStatics::GetActorOfClass(GetWorld(), ARifle::StaticClass()));
	////	shotgunDamage = shotgunInstance->damage;
	////	break;

	////case EWeaponType::RocketLauncher:
	////	rocketLauncherInstance = Cast<ARocketLauncher>(UGameplayStatics::GetActorOfClass(GetWorld(), ARifle::StaticClass()));
	////	rocketLaunchetDamage = rocketLauncherInstance->damage;
	////	break;

	////case EWeaponType::Sword:
	////	swordInstance = Cast<ASword>(UGameplayStatics::GetActorOfClass(GetWorld(), ARifle::StaticClass()));
	////	swordDamage = swordInstance->damage;
	////	break;

	//default:
	//	break;
	//}

}


void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (EState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}

	// 획득무기에 따른 데미지
	UpdateWeaponDamage();

	// 캐릭터의 주무기가 이전 무기와 다를때
	// 즉, 무기가 바뀌면
	if (target && target->PrimaryWeapon != target->PreviousWeaponType)
	{
		UpdateWeaponDamage();
		target->PreviousWeaponType = target->PrimaryWeapon;
	}
}

void UEnemyFSM::IdleState()
{
	// 누적 시간 계산
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > idleDelayTIme) {

		EState = EEnemyState::Move;
		currentTime = 0;
	}

}

void UEnemyFSM::MoveState()
{
	// 타겟위치
	FVector destination = target->GetActorLocation();

	// 움직일 방향 = 타겟 위치 - 적위치 
	FVector dir = destination - enemy->GetActorLocation();

	// 벡터의 크기를 1로 정규화하여, 방향벡터만 반환
	enemy->AddMovementInput(dir.GetSafeNormal());

	// 적이 계속 움직이는데 타겟과의 거리가 범위보다 작아지면
	if (dir.Size() < attackRange)
	{
		EState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 공격 대기 시간 지나면 시간 초기화
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
	}

	// 적과 플레이어 거리
	float distance = FVector::Distance(target->GetActorLocation(),
		enemy->GetActorLocation());

	if (distance > attackRange)
	{
		EState = EEnemyState::Move;
	}

}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	// 피격 대기 시간 넘기면 아이들상태
	if (currentTime > damageDelayTime)
	{
		EState = EEnemyState::Idle;
		currentTime = 0;
	}

}

void UEnemyFSM::DieState()
{
	// 등속운동으로 밑으로 내려가기
	FVector P0 = enemy->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	enemy->SetActorLocation(P);

	if (P.Z < -200.f)
	{
		enemy->Destroy();
	}

}

void UEnemyFSM::OnDamageProcess()
{
	if (target->PrimaryWeapon != EWeaponType::None) {
		// 캐릭터의 무기에 따라 데미지 다르게
		switch (target->PrimaryWeapon)
		{
		case EWeaponType::Rifle:
			enemy->health -= rifleDamage;
			break;

		case EWeaponType::Shotgun:
			enemy->health -= shotgunDamage;
			break;

		case EWeaponType::RocketLauncher:
			enemy->health -= rocketLaunchetDamage;

			break;

		case EWeaponType::Sword:

			break;
		}
	}

	// 체력 다 떨어지면
	if (enemy->health > 0)
	{
		EState = EEnemyState::Damage;
	}
	else
	{
		EState = EEnemyState::Die;
		enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UEnemyFSM::UpdateWeaponDamage()
{
	// 현재 캐릭터의 무기 타입에 따라 해당 무기 인스턴스와 데미지 값을 캐싱
	if (target)
	{
		switch (target->PrimaryWeapon)
		{
		case EWeaponType::Rifle:
			rifleInstance = Cast<ARifle>(UGameplayStatics::GetActorOfClass(GetWorld(), ARifle::StaticClass()));
			if (rifleInstance)
			{
				rifleDamage = rifleInstance->damage;
			}
			break;

		case EWeaponType::Shotgun:
			shotgunInstance = Cast<AShotgun>(UGameplayStatics::GetActorOfClass(GetWorld(), AShotgun::StaticClass()));
			if (shotgunInstance)
			{
				shotgunDamage = shotgunInstance->damage;
			}
			break;

		case EWeaponType::RocketLauncher:
			// 여기서만 발사체로 넣어주기
			rocketLauncherInstance = Cast<ARocketProjectile>(UGameplayStatics::GetActorOfClass(GetWorld(), ARocketProjectile::StaticClass()));
			if (rocketLauncherInstance)
			{
				rocketLaunchetDamage = rocketLauncherInstance->damage;
			}
			break;

			//case EWeaponType::Sword:
			//	swordInstance = Cast<ASword>(UGameplayStatics::GetActorOfClass(GetWorld(), ASword::StaticClass()));
			//	if (swordInstance)
			//	{
			//		swordDamage = swordInstance->damage;
			//	}
			//	break;

		default:
			break;
		}
	}
}
