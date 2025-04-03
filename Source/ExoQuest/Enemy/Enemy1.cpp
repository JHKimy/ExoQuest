#include "Enemy/Enemy1.h"
//#include "Enemy/EnemyFSM.h"	// AI

#include "Enemy/Enemy1FSM.h"	// AI
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/CharacterBase.h"              // 플레이어에게 데미지 줄 때 필요
#include "GameFramework/DamageType.h"      // 이건 타입을 정할 때 필요
#include "Engine/EngineTypes.h"         // FDamageEvent 포함됨!!
#include "Enemy/Enemy1AnimInstance.h"
#include "GameFramework/Controller.h"  // AController 정의 필요 시
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

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

	AttackCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetMesh(), FName("RightHandSocket")); // 본에 붙이기
	AttackCollision->SetCollisionProfileName("OverlapAll");
	AttackCollision->SetGenerateOverlapEvents(false); // 기본은 꺼둠
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy1::OnHandOverlap);

	
	// FSM1의 주인으로 설정
	FSM = CreateDefaultSubobject<UEnemy1FSM>(TEXT("FSM"));
}

void AEnemy1::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* localMesh = GetMesh();
	// StaticLoadClass를 사용하여 애니메이션 블루프린트 클래스 로드
	AnimBPClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr,
		TEXT("/Game/BluePrint/Enemy/Enemy1/ABP_Enemy1.ABP_Enemy1_C"));


	localMesh->SetAnimInstanceClass(AnimBPClass);

	//if (AnimBPClass) {
	//	UE_LOG(LogTemp, Warning, TEXT("load! BP!!!!!!"));
	//}

	anim = Cast<UEnemy1AnimInstance>(GetMesh()->GetAnimInstance());

	AI = Cast<AAIController>(GetController());

	InitialPosition = GetActorLocation();

	// 테스트용 패트롤 포인트 지정 (레벨에서 직접 할 수도 있음)
	PatrolPoints.Add(InitialPosition + FVector(500, 0, 0));
	PatrolPoints.Add(InitialPosition + FVector(500, 500, 0));
	PatrolPoints.Add(InitialPosition + FVector(0, 500, 0));

	//for (int32 i = 0; i < PatrolPoints.Num(); ++i)
	//{
	//	const FVector& pt = PatrolPoints[i];
	//	FString msg = FString::Printf(TEXT("[%d] → X=%.1f, Y=%.1f, Z=%.1f"), i, pt.X, pt.Y, pt.Z);

	//	// 화면 출력
	//	UKismetSystemLibrary::PrintString(GetWorld(), msg, true, true, FLinearColor::Green, 2.0f);

	//	// 로그 출력
	//	UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
	//}
}

void AEnemy1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy1::SetAnimState(EEnemyState newState)
{
	if (anim)
	{
		anim->animState = newState;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemy1::SetAnimState 실패: anim이 nullptr입니다."));
	}
}


void AEnemy1::SetAttackPlay(bool b)
{
	anim->bAttackPlay = b;
}

void AEnemy1::OnHandOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bCanDamage) return;
	ACharacterBase* player = Cast<ACharacterBase>(OtherActor);
	if (player)
	{
		// ApplyDamage: 데미지를 플레이어에게 적용
		UGameplayStatics::ApplyDamage(
			player,					// 데미지를 받을 대상
			10.f,					// 데미지 양
			GetController(),		// 데미지 인스티게이터 (공격자 컨트롤러)
			this,					// 데미지 유발자 (적 액터 자신)
			UDamageType::StaticClass() // 데미지 타입
		);

		UE_LOG(LogTemp, Warning, TEXT("ApplyDamage !"));
	}
}

float AEnemy1::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float actualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	anim->PlayHitMontage();

	return actualDamage;
}

void AEnemy1::Death()
{
	Super::Death();
}

void AEnemy1::MoveToTarget()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	Super::MoveToTarget();
}

void AEnemy1::MoveToPatrolPoint()
{
	if (PatrolPoints.Num() == 0) return;

	FVector targetLocation = PatrolPoints[CurrentPatrolIndex];
	FVector direction = targetLocation - GetActorLocation();

	if (direction.Size() < 100.f)
	{
		CurrentPatrolIndex = (CurrentPatrolIndex + 1) % PatrolPoints.Num();
		return;
	}
	// 걷는 속도 일시적으로 줄이기
	GetCharacterMovement()->MaxWalkSpeed = 150.f;
	// GetCharacterMovement()->Max
	//if (!AI)
	//{
	//	AI = Cast<AAIController>(GetController());
	//}

	//if (AI)
	//{
	//	//FString msg = FString::Printf(TEXT("Move Patrol: %s"), *targetLocation.ToString());

	//	//UKismetSystemLibrary::PrintString(GetWorld(), msg, true, true, FLinearColor::Green, 2.0f);

	//	AI->MoveToLocation(targetLocation, 1.f);
	//}

	AddMovementInput(direction.GetSafeNormal());
}

bool AEnemy1::IsPlayerInSight()
{
	if (!target) return false;

	// 1) 시야각 확인
	FVector toTarget = target->GetActorLocation() - GetActorLocation();
	toTarget.Normalize();

	FVector forward = GetActorForwardVector();
	float dot = FVector::DotProduct(forward, toTarget);
	float angleDegrees = FMath::RadiansToDegrees(FMath::Acos(dot));
	if (angleDegrees > ViewAngle * 0.5f)
	{
		return false; // 시야 밖
	}

	// 2) 라인트레이스로 가려졌는지 확인
	FHitResult hitResult;
	FVector start = GetActorLocation() + FVector(0, 0, 80); // 적 눈높이
	FVector end = target->GetActorLocation() + FVector(0, 0, 30); // 플레이어 눈높이

	FCollisionQueryParams params;
	params.AddIgnoredActor(this); // 자기 자신 제외
	params.AddIgnoredActor(target); // 플레이어는 무시

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		hitResult, start, end, ECC_Visibility, params
	);

	DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 1.f, 0, 1.f);
	// true: 무언가 맞았다 → 벽 있음
	// false: 아무것도 안 맞았다 → 벽 없음
	return !bHit;
}

void AEnemy1::ReturnToPatrolLocation()
{
	if (!Controller) return;

	bReturningToOrigin = true;
	if (!AI)
	{
		AI = Cast<AAIController>(GetController());
	}
	if (AI){
		AI->MoveToLocation(InitialPosition, 5.f); // 5cm 오차까지 이동 허용
	}
}
