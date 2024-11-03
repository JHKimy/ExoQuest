#include "Enemy/EnemyBase.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT(""));

	if (TempMesh.Succeeded())
	{
		// 1. 스켈레탈 메쉬 생성
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 2. 위치 조정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

