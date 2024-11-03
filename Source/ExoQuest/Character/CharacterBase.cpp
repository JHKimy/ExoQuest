#include "Character/CharacterBase.h"
#include <GameFramework/SpringArmComponent.h>	//스프링 팔
#include <Camera/CameraComponent.h>				// 카메라
#include "GameFramework/CharacterMovementComponent.h" // 캐릭터 움직임

#include "NavigationSystem.h"		// 마우스 움직임 네비
#include "Blueprint/AIBlueprintHelperLibrary.h" // AI 관련 블루프린트 함수들을 C++ 코드에서 사용

#include "Player/EQPlayerController.h"		// 애니메이션 관련 헤더
#include "Weapon/WeaponForEquip.h"

#include "Weapon/Rifle.h"

#include "Character/Animation/EQAnimInstance.h"

//#include "GameFramework/PlayerController.h"	// 컨트롤러



ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Character/Character3/SM_Character3.SM_Character3'"));
	
	if (TempMesh.Succeeded())
	{
		// 1. 스켈레탈 메쉬 생성
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 2. 위치 조정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 3. 스프링 암 컴포넌트 붙이기
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 0, 30));
	springArmComp->TargetArmLength = 400;
	springArmComp->SocketOffset.Z = 400;

	// 3-2. 카메라 컴포넌트 붙이기
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->SetRelativeRotation(FRotator(-40, 0, 0));

}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//EQPlayerController = Cast<AEQPlayerController>(GetController());

	EQCharacterState = ECharacterState::NoWeaponMode;
	ChangeState();


	USkeletalMeshComponent* localMesh = GetMesh();
	UClass* AnimBP;
	// StaticLoadClass를 사용하여 애니메이션 블루프린트 클래스 로드
	AnimBP = StaticLoadClass(UAnimInstance::StaticClass(), nullptr, 
		TEXT("/Game/BluePrint/Character/ABP_EQAnimation.ABP_EQAnimation_C"));
	
	if (AnimBP)
	{
		UE_LOG(LogTemp, Warning, TEXT("ANi Connnecting"));

	}
	localMesh->SetAnimInstanceClass(AnimBP);

}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 무기 확인
	CheckEquipWeapon();
	// 무기에 따른 설정 변경
	ChangeState();


}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::CheckEquipWeapon()
{
	// 무기 하나라도 있으면 
	if (EquippedWeapons.Num() > 0) 
	{
		PrimaryWeapon = EquippedWeapons[0];
		
	}
	else {
		// 없으면 무시
		return;
	}

	// WeaponForEquip으로 주무기 소켓에 붙이기
	switch (PrimaryWeapon)
	{
	//case EWeaponType::None:

	//	break;

	
	case EWeaponType::Rifle:
	{
		FName RifleSocket(TEXT("Rifle"));
		auto RifleforEquip = GetWorld()->SpawnActor<ARifle>(FVector::ZeroVector, FRotator::ZeroRotator);

		// 소켓에 부착
		if (RifleforEquip)
		{
			RifleforEquip->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				RifleSocket);
		}
		EQCharacterState = ECharacterState::RifleMode;

	}
		break;

	case EWeaponType::Shotgun:

		break;

	case EWeaponType::RocketLauncher:

		break;

	case EWeaponType::Sword:

		break;


	default:
		break;
	}
}

void ACharacterBase::ChangeState()
{

	// 애니메이션 블루프린트에 현재 상태 전달
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		UEQAnimInstance* EQAnimInstance = Cast<UEQAnimInstance>(AnimInstance);
		if (EQAnimInstance)
		{
			EQAnimInstance->SetCharacterState(EQCharacterState);
		}
	}

	switch (EQCharacterState)
	{
	case ECharacterState::NoWeaponMode:

		// 캐릭터와 컨트롤러의 회전 따로
		bUseControllerRotationYaw = false;
		// 이동 방향으로 캐릭터만 회전
		GetCharacterMovement()->bOrientRotationToMovement = true;
		// 스프링팔이 그만큼 상쇄해줌
		springArmComp->bUsePawnControlRotation = true;
		break;

	case ECharacterState::RifleMode:
		springArmComp->TargetArmLength = 200;
		springArmComp->SocketOffset.Z = 200;
		springArmComp->SocketOffset.Y = 70;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;

		
		break;

	case ECharacterState::ShotgunMode:


		break;

	case ECharacterState::SwordMode:


		break;

	default:
		break;
	}

	if (GetVelocity().Size() >= 800.f)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}

	//// 만약 달리고 있다면
	//if (GetVelocity().Size() >= 800.f) {
	//	GetCharacterMovement()->bOrientRotationToMovement = true;
	//	bUseControllerRotationYaw = false;
	//}
	//// 달리기 멈추면
	//else
	//{
	//	bUseControllerRotationYaw = true;
	//	/*무기 있을때*/if (EquippedWeapons.Num() > 0) GetCharacterMovement()->bOrientRotationToMovement = false;
	//	/*무기 없을때*/else if (EquippedWeapons.Num() == 0) GetCharacterMovement()->bOrientRotationToMovement = true;
	//}

}

void ACharacterBase::MouseClickMove()
{
	// 무기 획득하면 무시
	if (EquippedWeapons.Num() > 0) return;
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{

		// 월드 위치, 월드 방향
		FVector WorldLocation, WorldDirection;
		//
		const float TraceDistance = 10000.f;

		// 2D화면 좌표 가져와서 3D 좌표로 역투영
		PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
		
		// 라인 트레이스를 통해 클릭한 위치를 계산
		FVector TraceEnd = WorldLocation + (WorldDirection * TraceDistance);

		// 충돌 정보 저장 구조체
		FHitResult HitResult;
		// 라인 트레이스를 수행 
		// WorldLocation -> TraceEnd 까지 
		// ECC_Visibility 충돌채널의 충돌 지점을 HitResult에 저장
		GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility);
		

		if (HitResult.IsValidBlockingHit())
		{
			// 이 경로로 컨트롤러에 명렬
			// 내부에서 캐릭터 간접 참조해서 정해진 위치로 움직임
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), HitResult.Location);
		}
	}


}

void ACharacterBase::WASDClick(const FInputActionValue& InputValue)
{
	// 무기가 없으면 무시
	if (EquippedWeapons.Num() == 0) return;

	// 입력된 값에서 X, Y 값을 추출
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	float ValueX = MovementVector.X;
	float ValueY = MovementVector.Y;


	// 현재 컨트롤러 회전값
	FRotator LocalControlRotation = GetControlRotation();
	// 전방벡터
	FVector ForwardVector = FRotationMatrix(LocalControlRotation).GetUnitAxis(EAxis::X);
	// 우벡터
	FVector RightVector = FRotationMatrix(LocalControlRotation).GetUnitAxis(EAxis::Y);


	// https://leekangw.github.io/posts/73/
	// Z 방향 회전을 빼줘야 2D인 장판 맵에서 깔끔한 속도가 나옴
	ForwardVector.Z = 0;
	RightVector.Z = 0;

	ForwardVector.Normalize();
	RightVector.Normalize();

	AddMovementInput(RightVector, ValueX);
	AddMovementInput(ForwardVector, ValueY);

}

void ACharacterBase::Rotate(const FInputActionValue& InputValue)
{	
	// 무기가 없으면 무시
	if (EquippedWeapons.Num() == 0) return;

	APlayerController* EQPlayerController = Cast<APlayerController>(GetController());

	// 입력된 값에서 X, Y 값을 추출
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	float ValueX = -MovementVector.X; // ?
	float ValueY = MovementVector.Y;
	
	EQPlayerController->AddYawInput(ValueX);
	EQPlayerController->AddPitchInput(ValueY);
}

void ACharacterBase::CustomJump()
{
	// 무기가 없으면 무시
	if (EquippedWeapons.Num() == 0) return;

	Jump();
}

void ACharacterBase::RunStart()
{
	// 무기가 없으면 무시
	if (EquippedWeapons.Num() == 0) return;

	GetCharacterMovement()->MaxWalkSpeed = 1000.f;
}

void ACharacterBase::RunStop()
{
	// 무기가 없으면 무시
	if (EquippedWeapons.Num() == 0) return;

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

// 대쉬 스킬
void ACharacterBase::DashStart()
{
	// 무기가 없으면 무시
	if (EquippedWeapons.Num() == 0) return;

	// 대쉬가 쿨타임 중이면 함수 종료
	if (!bCanDash) return; 
	



	// 대쉬 가능
	bCanDash = false;

	// 캐릭터의 이동 방향 가져오기
	FVector CurrentVelocity = GetVelocity();
	
	// 대쉬 전 속도 저장
	beforeDashVelocity = CurrentVelocity;

	// 이동 방향이 없는 경우(정지 상태), 기본적으로 바라보는 방향으로 대쉬
	if (CurrentVelocity.IsNearlyZero())
	{
		CurrentVelocity = GetActorForwardVector();
	}

	// 대쉬 방향을 현재 이동 방향으로 설정 (정규화하여 크기를 1로 만듦)
	FVector DashDirection = CurrentVelocity.GetSafeNormal();
	DashDirection.Z = 0;

	// 대쉬 거리
	float DashDistance = 20000.0f;

	// 대쉬 작동
	GetMovementComponent()->Velocity = DashDirection * DashDistance;

	// 대쉬 작동 후
	GetWorld()->GetTimerManager().SetTimer
	(DashTimer, this, &ACharacterBase::StopDash, 0.1f, false);
	
}

void ACharacterBase::StopDash()
{
	// 대쉬 전 속도로 돌아가기
	GetMovementComponent()->Velocity = beforeDashVelocity;

	// 2초 후에 대쉬 가능 상태로 복구
	GetWorld()->GetTimerManager().SetTimer
	(DashTimer, this, &ACharacterBase::ResetDash, dashCoolTime, false);
}

void ACharacterBase::ResetDash()
{
	// 대쉬 가능 
	bCanDash = true;

}