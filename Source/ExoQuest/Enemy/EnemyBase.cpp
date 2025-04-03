#include "Enemy/EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyFSM.h"	// AI
#include "GameFramework/CharacterMovementComponent.h" // 움직임
#include "Kismet/KismetSystemLibrary.h"		// 화면 텍스츠 출력 
#include "Item/Starflux.h"

#include "Character/CharacterBase.h"

#include "PaperSpriteComponent.h"
#include "Components/WidgetComponent.h"
#include  "UI/Enemy1HPBar.h"
#include "Kismet/GameplayStatics.h"

#include "AIController.h"


AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetRelativeScale3D(FVector(2.5f, 2.5f, 1.5f));
	RootComponent = GetCapsuleComponent();

	//ConstructorHelpers::FObjectFinder<USkeletalMesh>
	//	TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Enemy/Enemy1/SM_Enemy1.SM_Enemy1'"));

	//if (TempMesh.Succeeded())
	//{
	//	// 스켈레탈 메쉬 생성
	//	GetMesh()->SetSkeletalMesh(TempMesh.Object);
	//	// 위치 조정
	//	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	//}

	//// FSM의 주인으로 설정
	//FSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar")); // 원래대로
	HPBarWidget->SetupAttachment(RootComponent);
	HPBarWidget->SetWidgetSpace(EWidgetSpace::World);
	HPBarWidget->SetDrawSize(FVector2D(200.f, 20.f));
	HPBarWidget->SetVisibility(false); // 시작 시 비활성화

	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarClass(TEXT("/Game/BluePrint/Enemy/Enemy1/WBP_Enemy1HPBar.WBP_Enemy1HPBar_C"));
	if (HPBarClass.Succeeded())
	{
		HPBarWidget->SetWidgetClass(HPBarClass.Class);  // WBP 클래스 지정
	}


}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	//USkeletalMeshComponent* localMesh = GetMesh();
	//// StaticLoadClass를 사용하여 애니메이션 블루프린트 클래스 로드
	//AnimBPClass = StaticLoadClass(UAnimInstance::StaticClass(), nullptr,
	//	TEXT("/Game/BluePrint/Enemy/Enemy1/ABP_Enemy1.ABP_Enemy1_C"));
	//
	//if (AnimBPClass) {
	//	UE_LOG(LogTemp, Warning, TEXT("load! BP!!!!!!"));
	//}

	//localMesh->SetAnimInstanceClass(AnimBPClass);

	if (EnemyPosition) {
		EnemyPosition->bVisibleInSceneCaptureOnly = true;
	}
	GetCharacterMovement()->bOrientRotationToMovement = true;


	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterBase::StaticClass());

	target = Cast<ACharacterBase>(actor);

	//// 스탯
	//health = 100.f;
	//attackPower = 10.f;
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovementHealthBar();
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyBase::TakeDamage(
	float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	// FSM 가져오기
	auto fsmForDamage = Cast<UEnemyFSM>(GetComponentByClass(UEnemyFSM::StaticClass()));

	fsmForDamage->ChangeState(EEnemyState::Damage);

	// 체력 
	health -= DamageAmount;

	// 현재 체력 출력
	//FString HealthString = FString::Printf(TEXT("Current Health: %f"), health);
	//UKismetSystemLibrary::PrintString(GetWorld(), HealthString, true, false, FLinearColor::Blue, 2.f);

	// 체력이 0 이하가 되면 사망 처리
	if (health <= 0.0f)
	{
		// FSM이 nullptr인지 확인
		if (fsmForDamage)
		{
			fsmForDamage->ChangeState(EEnemyState::Death);  // FSM 상태 전환
			// 처치시 충돌체 지우기
			fsmForDamage->enemy->GetCapsuleComponent()->
				SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//UKismetSystemLibrary::PrintString
			//(GetWorld(), TEXT("oops!!"), true, false, FLinearColor::Green, 2.f);
		}
		else
		{
			// FSM이 nullptr인 경우 에러 처리
			//UKismetSystemLibrary::PrintString
			//(GetWorld(), TEXT("NULL!!"), true, false, FLinearColor::Red, 2.f);
		}
	}

	UpdateHealthBar();

	// TakeDamage 내부에서
	if (health <= 0.f)
	{
		bIsAlive = false;
	}




	return DamageAmount;
}

void AEnemyBase::Attack()
{
}

void AEnemyBase::Death()
{
	// Destroy();

	HPBarWidget->SetVisibility(false);

	// 등속운동으로 밑으로 내려가기
	FVector P0 = GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	SetActorLocation(P);

	if (P.Z < -200.f)
	{
		Destroy();
	}
	if (!bSpawnStarflux) {
		bSpawnStarflux = true;
		GetWorld()->SpawnActor<AStarflux>
			(AStarflux::StaticClass(),
				GetActorLocation(),
				GetActorRotation());
	}
}

void AEnemyBase::MoveToTarget() 
{
	//FVector dir = target->GetActorLocation() - GetActorLocation();
	//AddMovementInput(dir.GetSafeNormal());

	AAIController* ai = Cast<AAIController>(GetController());
	if (ai)
	{
		ai->MoveToActor(target, 5.f); // 5cm까지 근접하면 멈춤
	}
}


void AEnemyBase::UpdateHealthBar()
{
	if (HPBarWidget)
	{
		UUserWidget* Widget = HPBarWidget->GetUserWidgetObject();
		if (Widget)
		{
			UEnemy1HPBar* EnemyHP = Cast<UEnemy1HPBar>(Widget);
			if (EnemyHP)
			{
				EnemyHP->UpdateHP(health / maxHealth);  // 체력 비율 전달
			}
		}
	}
}

void AEnemyBase::MovementHealthBar()
{	
	// 체력바가 플레이어를 바라보게 회전하고 일정 거리 내일 때만 표시
	if (HPBarWidget && GetWorld())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		APawn* PlayerPawn = PC ? PC->GetPawn() : nullptr;
		if (PC && PlayerPawn && PC->PlayerCameraManager)
		{
			FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
			FVector WidgetLocation = HPBarWidget->GetComponentLocation();
			FRotator LookAtRotation = (CameraLocation - WidgetLocation).Rotation();
			HPBarWidget->SetWorldRotation(LookAtRotation);

			// 거리 계산 후 체력바 표시 여부 결정
			float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
			bool bShouldShow = Distance <= healthBarRange;
			HPBarWidget->SetVisibility(bShouldShow);
		}
	}
}

