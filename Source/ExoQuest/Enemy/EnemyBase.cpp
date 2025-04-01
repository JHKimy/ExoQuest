#include "Enemy/EnemyBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyFSM.h"	// AI
#include "GameFramework/CharacterMovementComponent.h" // 움직임
#include "Kismet/KismetSystemLibrary.h"		// 화면 텍스츠 출력 
#include "Item/Starflux.h"

#include "PaperSpriteComponent.h"
#include "Components/WidgetComponent.h"
#include  "UI/Enemy1HPBar.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetRelativeScale3D(FVector(2.5f, 2.5f, 1.5f));
	RootComponent = GetCapsuleComponent();

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
	fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBar")); // 원래대로
	HPBarWidget->SetupAttachment(RootComponent);
	HPBarWidget->SetWidgetSpace(EWidgetSpace::World);
	HPBarWidget->SetDrawSize(FVector2D(200.f, 20.f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarClass(TEXT("/Game/BluePrint/Enemy/Enemy1/WBP_Enemy1HPBar.WBP_Enemy1HPBar_C"));
	if (HPBarClass.Succeeded())
	{
		HPBarWidget->SetWidgetClass(HPBarClass.Class);  // WBP 클래스 지정
	}


}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	USkeletalMeshComponent* localMesh = GetMesh();
	// StaticLoadClass를 사용하여 애니메이션 블루프린트 클래스 로드
	AnimBP = StaticLoadClass(UAnimInstance::StaticClass(), nullptr,
		TEXT("/Game/BluePrint/Enemy/Enemy1/ABP_Enemy1.ABP_Enemy1_C"));
	
	if (AnimBP) {
		UE_LOG(LogTemp, Warning, TEXT("load! BP!!!!!!"));
	}

	localMesh->SetAnimInstanceClass(AnimBP);

	if (enemyPosition) {
		enemyPosition->bVisibleInSceneCaptureOnly = true;
	}
	//enemyPosition->bRenderCustomDepth = true;


	// 메인 카메라에서 PaperSprite 숨기기
	//enemyPosition->SetVisibility(false);

	// 미니맵 카메라에서 PaperSprite 보이게 하기
	//enemyPosition->SetVisibility(true);




	// 미니맵 카메라에서 PaperSprite 보이게 하기
	//enemyPosition->SetVisibility(true);
	//enemyPosition->SetHiddenInGame(true);








	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 스탯
	health = 100.f;
	attackPower = 10.f;
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



// ## 어플라이 테이크 데미지 사용해 보고 싶어서 스워드 시스템만 이걸로 작성 ##
// 칼로 할때 데미지
float AEnemyBase::TakeDamage(
	float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	// FSM 가져오기
	auto fsmForDamage = Cast<UEnemyFSM>(GetComponentByClass(UEnemyFSM::StaticClass()));

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
			fsmForDamage->EState = EEnemyState::Die;  // FSM 상태 전환
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



	return DamageAmount;
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
				EnemyHP->UpdateHP(health / 100.f);  // 체력 비율 전달
			}
		}
	}
}
