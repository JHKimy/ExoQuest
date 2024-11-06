#include "Enemy/EnemyFSM.h"
#include "Enemy/EnemyBase.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>		// �ʿ� �ִ� �ν��Ͻ� ��������
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

	
	// ������ �ʱ�ȭ
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

	// ȹ�湫�⿡ ���� ������
	UpdateWeaponDamage();

	// ĳ������ �ֹ��Ⱑ ���� ����� �ٸ���
	// ��, ���Ⱑ �ٲ��
	if (target && target->PrimaryWeapon != target->PreviousWeaponType)
	{
		UpdateWeaponDamage();
		target->PreviousWeaponType = target->PrimaryWeapon;
	}
}

void UEnemyFSM::IdleState()
{
	// ���� �ð� ���
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > idleDelayTIme) {

		EState = EEnemyState::Move;
		currentTime = 0;
	}

}

void UEnemyFSM::MoveState()
{
	// Ÿ����ġ
	FVector destination = target->GetActorLocation();

	// ������ ���� = Ÿ�� ��ġ - ����ġ 
	FVector dir = destination - enemy->GetActorLocation();

	// ������ ũ�⸦ 1�� ����ȭ�Ͽ�, ���⺤�͸� ��ȯ
	enemy->AddMovementInput(dir.GetSafeNormal());

	// ���� ��� �����̴µ� Ÿ�ٰ��� �Ÿ��� �������� �۾�����
	if (dir.Size() < attackRange)
	{
		EState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	// ���� ��� �ð� ������ �ð� �ʱ�ȭ
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
	}

	// ���� �÷��̾� �Ÿ�
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

	// �ǰ� ��� �ð� �ѱ�� ���̵����
	if (currentTime > damageDelayTime)
	{
		EState = EEnemyState::Idle;
		currentTime = 0;
	}

}

void UEnemyFSM::DieState()
{
	// ��ӿ���� ������ ��������
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
		// ĳ������ ���⿡ ���� ������ �ٸ���
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

	// ü�� �� ��������
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
	// ���� ĳ������ ���� Ÿ�Կ� ���� �ش� ���� �ν��Ͻ��� ������ ���� ĳ��
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
			// ���⼭�� �߻�ü�� �־��ֱ�
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
