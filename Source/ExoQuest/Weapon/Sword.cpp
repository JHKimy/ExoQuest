#include "Weapon/Sword.h"
#include "Components/BoxComponent.h"
#include "Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"


ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/Sword/SM_Sword.SM_Sword'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = meshComp;

	attackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	attackCollision->SetRelativeLocationAndRotation(
		FVector(6.7f, 0.f, 40.f), FRotator(0.f, -2.5f, 0.f));
	attackCollision->SetRelativeScale3D(FVector(0.65f, 0.25f, 1.9f));
	attackCollision->SetupAttachment(RootComponent);
	attackCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));  // �浹 �������� ����
	attackCollision->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnWeaponOverlapBegin);

	// ������
	damage = 30.f;
	// ��Ÿ��
	coolTime = 0.2f;
}

void ASword::BeginPlay()
{
	Super::BeginPlay();
}

void ASword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASword::OnWeaponOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsAttacking) return; // ���� ���� �ƴ� ��� �������� ������ ����

	APawn* OwnerPawn = Cast<APawn>(GetOwner());  // ���� ������ Ȯ��

	// �� ĳ���Ϳ� ���� Ÿ�� ����
	if (OtherActor && OtherActor != this)
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);

		if (Enemy && Enemy->health > 0)  // ���� ����ִ��� Ȯ��
		{
			// UGameplayStatics::ApplyDamage ���
			UGameplayStatics::ApplyDamage(Enemy, damage, OwnerPawn->GetController(), this, UDamageType::StaticClass());
		}
	}

}

void ASword::Slash()
{
	// ���� ���¸� Ȱ��ȭ
	bIsAttacking = true;

	// ���� �ð� �� ���� ���� ���� (Ÿ�̸� ���)
	GetWorld()->GetTimerManager().SetTimer(
		SalshTimer, this, &ASword::ResetSlash, coolTime, false); // 0.5�� �� ����
	
}

void ASword::ResetSlash()
{
	bIsAttacking = false;
}

