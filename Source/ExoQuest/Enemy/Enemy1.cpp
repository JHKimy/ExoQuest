#include "Enemy/Enemy1.h"
#include "Enemy/EnemyFSM.h"	// AI

AEnemy1::AEnemy1()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Enemy/Enemy1/SM_Enemy1.SM_Enemy1'"));

	if (TempMesh.Succeeded())
	{
		// 스켈레탈 메쉬 생성
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 위치 조정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// FSM의 주인으로 설정
	FSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));
}

void AEnemy1::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* localMesh = GetMesh();
	// StaticLoadClass를 사용하여 애니메이션 블루프린트 클래스 로드
	AnimBPClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr,
		TEXT("/Game/BluePrint/Enemy/Enemy1/ABP_Enemy1.ABP_Enemy1_C"));

	if (AnimBPClass) {
		UE_LOG(LogTemp, Warning, TEXT("load! BP!!!!!!"));
	}

	localMesh->SetAnimInstanceClass(AnimBPClass);
}

void AEnemy1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AEnemy1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}