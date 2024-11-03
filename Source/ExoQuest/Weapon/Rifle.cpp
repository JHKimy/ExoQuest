#include "Weapon/Rifle.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Character/CharacterBase.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyFSM.h"


// y���� �չ��� �Ǿ��ִ� ����

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/RIfle/SM_Rifle.SM_Rifle'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetupAttachment(RootComponent); // �޽��� ĸ���� ����
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	RootComponent = meshComp;

	// MuzzleLocation Scene Component ���� �� �ʱ�ȭ
	muzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	muzzleLocation->SetupAttachment(RootComponent);

	// �⺻ ��ġ ���� (�ѱ� �պκ����� ��ġ ����)
	muzzleLocation->SetRelativeLocation(FVector(0.f, 40.f, 2.f)); // X, Y, Z ��ǥ ���� ����

	damage = 5;
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();

}

void ARifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARifle::Fire()
{
	// ���� Ʈ���̽�
	FVector startPos = muzzleLocation->GetComponentLocation();
	FVector endPos = muzzleLocation->GetComponentLocation() + muzzleLocation->GetForwardVector() * 5000;

	// �浹 ��ġ
	FHitResult hitInfo;
	// �浹 �ɼ� ����
	FCollisionQueryParams params;
	// �ڽ��� ����
	params.AddIgnoredActor(ownerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	// ����� ���� �߰�
	DrawDebugLine
	(
		GetWorld(),
		startPos,         // ������
		endPos,           // ����
		FColor::Red,    // �� ���� (���)
		false,            // ���� ���� (false�� �Ͻ������θ� ǥ�õ�)
		1.0f,             // ���� �ð� (1�� ���� ǥ��)
		0,                // �β� (����� ������ ä��)
		1.0f              // ���� �β�
	);

	if (bHit)
	{
		// �Ѿ� ���� ȿ�� Ʈ������
		FTransform bulletTrans;
		// �ε��� ��ġ �Ҵ�
		bulletTrans.SetLocation(hitInfo.ImpactPoint);
		// �Ѿ� ���� ȿ�� �ν��Ͻ� ����
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

		// �Ѿ˿� �浹�� ������ �Ǵ�
		auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));

		if (enemy)
		{
			auto enemyFSM = Cast<UEnemyFSM>(enemy);
			enemyFSM->OnDamageProcess();
		}

	}


}

