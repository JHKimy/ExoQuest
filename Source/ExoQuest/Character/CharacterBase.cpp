#include "Character/CharacterBase.h"
#include <GameFramework/SpringArmComponent.h>	//스프링 팔
#include <Camera/CameraComponent.h>				// 카메라

#include "Components/SceneCaptureComponent2D.h"

#include "GameFramework/CharacterMovementComponent.h" // 캐릭터 움직임

#include "NavigationSystem.h"		// 마우스 움직임 네비
#include "Blueprint/AIBlueprintHelperLibrary.h" // AI 관련 블루프린트 함수들을 C++ 코드에서 사용

#include "Player/EQPlayerController.h"		// 애니메이션 관련 헤더
#include "Weapon/WeaponForEquip.h"


#include "Weapon/Rifle.h"
#include "Weapon/Shotgun.h"
#include "Weapon/RocketLauncher.h"
#include "Weapon/Sword.h"			// 무기


#include "Character/Animation/EQAnimInstance.h"

#include <Blueprint/UserWidget.h>	// 위젯
#include "Enemy/EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyBase.h"

#include "Kismet/KismetSystemLibrary.h" // PrintString 함수 사용을 위해 포함

//#include "GameFramework/PlayerController.h"	// 컨트롤러

#include "Animation/AnimMontage.h"	// 몽타주
#include "Animation/AnimInstance.h"	// 애니메이션인스턴스

#include "PaperSpriteComponent.h"

#include "Map/EQGameInstance.h"	// 맵 이동 인스턴스

#include "Animation/AnimNotifies/AnimNotify.h" // Notify 관련 처리

#include "Weapon/Grenade/BasicGrenade.h"

#include "Components/ChildActorComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/ArrowComponent.h"

#include "Item/Starflux.h"

#include "Blueprint/UserWidget.h"
#include "UI/InventoryUI.h"
#include "UI/InventoryComponent.h"

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

	// 미니맵
	miniMapSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapSpringArm"));
	miniMapSpringArmComp->SetupAttachment(RootComponent);
	miniMapSpringArmComp->SetRelativeLocationAndRotation(FVector(0, 0, 60.f), FRotator(-45.f, 0.f, 0.f));
	miniMapSpringArmComp->TargetArmLength = 400;
	miniMapSpringArmComp->SocketOffset.Z = 400;

	// 화살표가 캐릭터 따라 회전
	miniMapSpringArmComp->bInheritPitch = false;
	miniMapSpringArmComp->bInheritYaw = false;
	miniMapSpringArmComp->bInheritRoll = false;

	miniMapCam = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MiniMapCam"));
	miniMapCam->SetupAttachment(miniMapSpringArmComp); // RootComponent와의 연결
	miniMapCam->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	miniMapCam->ProjectionType = ECameraProjectionMode::Orthographic;
	miniMapCam->OrthoWidth = 1024.f;


	GrenadeLaunchPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Grenade"));
	GrenadeLaunchPosition->SetupAttachment(GetCapsuleComponent());
	GrenadeLaunchPosition->SetRelativeLocation(FVector(60.f, 0.f, 60.f));

	//characterPositionArrow->OwnerNosee

	// grenadePos 생성
	// grenadePos = CreateDefaultSubobject<UChildActorComponent>(TEXT("Grenade Launch Position"));
	// 캐릭터의 Mesh에 부착
	// grenadePos->SetupAttachment(GetMesh(), FName(TEXT("Grenade"))); // 소켓 이름 Grenade 확인 필요
	


	bmouseMoveMode = true;
	maxHealth = 100.f;
	health = 50.f;

	maxStamina = 100.f;
	stamina = 100.f;
	staminaDrainRate = 15.0f; // 초당 감소하는 값


	// 수류탄
	GrenadeLaunchPower = 1000.f;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	RestoreStateAfterLevelChange();  // 레벨 시작 시 상태 복원

	//EQPlayerController = Cast<AEQPlayerController>(GetController());

	// 상태가 복원되지 않았다면 기본 상태로 설정
	if (EQCharacterState == ECharacterState::NoWeaponMode && EquippedWeapons.Num() == 0)
	{
		EQCharacterState = ECharacterState::NoWeaponMode;
	}	ChangeState();




	USkeletalMeshComponent* localMesh = GetMesh();
	// StaticLoadClass를 사용하여 애니메이션 블루프린트 클래스 로드
	AnimBP = StaticLoadClass(UAnimInstance::StaticClass(), nullptr,
		TEXT("/Game/BluePrint/Character/ABP_EQAnimation.ABP_EQAnimation_C"));

	localMesh->SetAnimInstanceClass(AnimBP);





	// EnemyFSM 컴포넌트를 가져와 초기화
	enemyFSM = Cast<UEnemyFSM>(UGameplayStatics::GetActorOfClass(GetWorld(), UEnemyFSM::StaticClass()));



	//// AEnemyBase 객체를 가져오기
	//enemy = Cast<AEnemyBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBase::StaticClass()));
	//if (enemy)
	//{
	//	// UEnemyFSM 컴포넌트를 가져오기
	//	enemyFSM = enemy->FindComponentByClass<UEnemyFSM>();
	//	if (enemyFSM)
	//	{
	//		// enemyFSM 초기화 관련 작업
	//		UE_LOG(LogTemp, Warning, TEXT("Enemy FSM Found!"));
	//	}
	//	// PaperSprite 숨기기
	//	if (enemy->enemyPosition)
	//	{
	//		enemy->enemyPosition->SetVisibility(false);
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("enemyPosition is null!"));
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Enemy not found!"));
	//}


	///////////////////////////////////////////////////
	currentCombo = 0;






















	
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	// 무기 확인과 상태 업데이트가 필요할 때만 실행
	if (EquippedWeapons.Num() > 0)
	{
		// 무기 확인
		CheckEquipWeapon();
		// 무기에 따른 설정 변경
		ChangeState();
	}







	//FString WeaponsList = "Equipped Weapons : ";

	//for (EWeaponType WeaponType : EquippedWeapons)
	//{
	//	switch (WeaponType)
	//	{
	//	case EWeaponType::Rifle:
	//		WeaponsList += "Rifle / ";
	//		break;
	//	case EWeaponType::Shotgun:
	//		WeaponsList += "Shotgun / ";
	//		break;
	//	case EWeaponType::RocketLauncher:
	//		WeaponsList += "RocketLauncher / ";
	//		break;
	//	case EWeaponType::Sword:
	//		WeaponsList += "Sword / ";
	//		break;
	//	default:
	//		WeaponsList += "Unknown / ";
	//		break;
	//	}
	//}

	//// 화면 출력
	//UKismetSystemLibrary::PrintString(this, WeaponsList, true, true, FColor::Green, 2.0f);

	FString PrimaryWeaponName;
	switch (PrimaryWeapon)
	{
	case EWeaponType::Rifle:
		PrimaryWeaponName = "Rifle";
		break;
	case EWeaponType::Shotgun:
		PrimaryWeaponName = "Shotgun";
		break;
	case EWeaponType::RocketLauncher:
		PrimaryWeaponName = "RocketLauncher";
		break;
	case EWeaponType::Sword:
		PrimaryWeaponName = "Sword";
		break;
	default:
		PrimaryWeaponName = "Unknown";
		break;
	}
	// UKismetSystemLibrary::PrintString(this, PrimaryWeaponName, true, true, FColor::Red, 2.0f);




	// 스태미나 관리 함수 호출
	HandleStamina(DeltaTime);









	// 전방벡터
	FVector GrenadeForwardVector = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);

	GrenadeForwardVector.Z = 0;

	GrenadeForwardVector.Normalize();


	//FRotator ControlRotation = GetControlRotation();
	//FVector ForwardVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);

	//GrenadeLaunchVelocity = GrenadeForwardVector * GrenadeLaunchPower;
	GrenadeLaunchVelocity = GrenadeLaunchPosition->GetForwardVector() * GrenadeLaunchPower;
	// 10도 회전 추가 (Z축 기준)
	FRotator RotationOffset = FRotator(0.0f, -10.0f, 0.0f); // Yaw에 -10도를 추가
	GrenadeLaunchVelocity = RotationOffset.RotateVector(GrenadeLaunchVelocity);



}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ACharacterBase::CheckEquipWeapon()
{
	// 무기가 하나도 없으면 실행 중단
	if (EquippedWeapons.IsEmpty()) return;

	// 무기 하나라도 있으면 
	PrimaryWeapon = EquippedWeapons[0];
	bmouseMoveMode = false;


	{

		if (enemyFSM)
		{
			enemyFSM->UpdateWeaponDamage();
		}

		// 조준 UI 크로스 헤어 인스턴스 생성
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController && !crosshairUI)
		{
			crosshairUI = CreateWidget<UUserWidget>(PlayerController, crosshairUIFactory);
			if (crosshairUI)
			{
				crosshairUI->AddToViewport();
			}
		}
	}

	if (EquippedWeapons.Num()<=2) {


		// WeaponForEquip으로 획득한 무기 확인
		for (EWeaponType WeaponType : EquippedWeapons)
		{
			FName SocketName;
			AActor* WeaponActor = nullptr;

			switch (WeaponType)
			{
			case EWeaponType::Rifle:
				if (playerRifle) continue;
				playerRifle = GetWorld()->SpawnActor<ARifle>();
				WeaponActor = playerRifle;
				SocketName = (WeaponType == PrimaryWeapon) ? FName(TEXT("Rifle")) : FName(TEXT("RifleBack"));
				EQCharacterState = (PrimaryWeapon == EWeaponType::Rifle) ? ECharacterState::RifleMode : EQCharacterState;
				break;



			case EWeaponType::Shotgun:
				if (playerShotgun) continue;
				playerShotgun = GetWorld()->SpawnActor<AShotgun>();
				WeaponActor = playerShotgun;
				SocketName = (WeaponType == PrimaryWeapon) ? FName(TEXT("Shotgun")) : FName(TEXT("ShotgunBack"));
				EQCharacterState = (PrimaryWeapon == EWeaponType::Shotgun) ? ECharacterState::ShotgunMode : EQCharacterState;
				break;


			case EWeaponType::RocketLauncher:
				if (playerRocketLauncher) continue;
				playerRocketLauncher = GetWorld()->SpawnActor<ARocketLauncher>();
				WeaponActor = playerRocketLauncher;
				SocketName = (WeaponType == PrimaryWeapon) ? FName(TEXT("RocketLauncher")) : FName(TEXT("RocketLauncherBack"));
				EQCharacterState = (PrimaryWeapon == EWeaponType::RocketLauncher) ? ECharacterState::RocketLauncherMode : EQCharacterState;
				break;

			case EWeaponType::Sword:
				if (playerSword) continue;
				playerSword = GetWorld()->SpawnActor<ASword>();
				WeaponActor = playerSword;
				SocketName = (WeaponType == PrimaryWeapon) ? FName(TEXT("Sword")) : FName(TEXT("SwordBack"));
				EQCharacterState = (PrimaryWeapon == EWeaponType::Sword) ? ECharacterState::SwordMode : EQCharacterState;
				break;


			default:
				break;
			}

			if (WeaponActor)
			{
				WeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
				// 데미지 시스템때문에 주인 정해줘야함
				WeaponActor->SetOwner(this);  // 'this'는 무기의 소유자가 될 액터

			}
		}
	}


}

void ACharacterBase::ChangeState()
{
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
	case ECharacterState::ShotgunMode:
	case ECharacterState::RocketLauncherMode:
	case ECharacterState::SwordMode:
		springArmComp->TargetArmLength = 200;
		springArmComp->SocketOffset.Z = 200;
		springArmComp->SocketOffset.Y = 70;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bUseControllerRotationYaw = true;
		break;

	default:
		break;
	}

	// 애니메이션 블루프린트에 현재 상태 전달
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		UEQAnimInstance* EQAnimInstance = Cast<UEQAnimInstance>(AnimInstance);
		if (EQAnimInstance)
		{
			// 애니메이션 상태 변수
			EQAnimInstance->SetCharacterState(EQCharacterState);
		}
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

	bIsRunning = true;
}

void ACharacterBase::RunStop()
{
	// 무기가 없으면 무시
	if (EquippedWeapons.Num() == 0) return;

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

	bIsRunning = false;
}

// 대쉬 스킬
void ACharacterBase::DashStart()
{
	// 무기가 없으면 무시
	if (EquippedWeapons.Num() == 0) return;

	// 대쉬가 쿨타임 중이면 함수 종료
	if (!bCanDash) return;


	bIsDashing = true;  // 대쉬 시작 시 상태 설정

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

	bIsDashing = false;

	// 2초 후에 대쉬 가능 상태로 복구
	GetWorld()->GetTimerManager().SetTimer
	(DashTimer, this, &ACharacterBase::ResetDash, dashCoolTime, false);
}

void ACharacterBase::ResetDash()
{
	// 대쉬 가능 
	bCanDash = true;

}

void ACharacterBase::WeaponAttack()
{
	// 대쉬나 달리기 중일 때는 발사하지 않음
	// 콤보 공격을 위한 공격가능 변수
	if (bIsDashing || bIsRunning) return;

	switch (PrimaryWeapon)
	{

	case EWeaponType::None:
		break;

	case EWeaponType::Rifle:
		playerRifle->Fire();
		break;

	case EWeaponType::Shotgun:
		playerShotgun->Fire();
		break;

	case EWeaponType::RocketLauncher:
		playerRocketLauncher->Fire();
		break;

	case EWeaponType::Sword:
	{
		//playerSword->Slash();
		//
		//// 섹션
		//const char* comboList[] = { "RIghtSlash","LeftSlash"};

		//// UAnimInstance를 올바르게 가져옴
		//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		//if (AnimInstance && swordComboMontage)
		//{


		//	if (currentCombo == 0)
		//	{
		//		AnimInstance->Montage_Play(swordComboMontage);
		//		AnimInstance->Montage_JumpToSection(FName("RightSlash"));
		//		currentCombo = 1;
		//		// 타이머 시작 - 0.5초 내에 두 번째 입력이 없으면 콤보 초기화
		//		GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ACharacterBase::ResetCombo, 0.1f, false);
		//	}


		//	// 두 번째 섹션으로 이동
		//	else if (currentCombo == 1)
		//	{
		//		//bCanAttack = false; // 다음 입력 방지
		//		
		//		currentCombo = 0;
		//		AnimInstance->Montage_JumpToSection(FName("LeftSlash"));
		//		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
		//		// 타이머 초기화 (두 번째 공격이 성공적으로 연결되었기 때문에 초기화)
		//	}


		// }
		break;
	}

	default:
		break;
	}
}

void ACharacterBase::HandleStamina(float DeltaTime)
{
	if (bIsRunning && stamina > 0.0f)
	{
		stamina -= staminaDrainRate * DeltaTime;
		if (stamina <= 0.0f)
		{
			stamina = 0.0f;
			RunStop(); // 스태미나가 다 닳으면 자동으로 달리기 멈춤
		}
	}
	else if (!bIsRunning && stamina < maxStamina) // 달리기 멈췄을 때 서서히 회복
	{
		stamina += staminaDrainRate * 0.5f * DeltaTime;
		if (stamina > maxStamina)
		{
			stamina = maxStamina;
		}
	}

}

void ACharacterBase::SwordAttack()
{
	if (bIsDashing || bIsRunning || PrimaryWeapon != EWeaponType::Sword) return;

	playerSword->Slash();

	// 섹션
	//const char* comboList[] = { "RIghtSlash","LeftSlash" };

	// UAnimInstance를 올바르게 가져옴
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && swordComboMontage)
	{
		AnimInstance->Montage_Play(swordComboMontage);
		if (currentCombo == 0)
		{
			AnimInstance->Montage_JumpToSection(FName("RightSlash"));
			currentCombo = 1;
			// 타이머 시작 - 일정 시간 내에 두 번째 입력이 없으면 콤보 초기화
			GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ACharacterBase::ResetCombo, 0.5f, false);
		}
		else if (currentCombo == 1)
		{
			AnimInstance->Montage_JumpToSection(FName("LeftSlash"));
			currentCombo = 0;
			// 타이머 초기화 (두 번째 공격이 성공적으로 연결되었기 때문에 초기화)
			GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
		}

	}
}

void ACharacterBase::ZoomIn()
{
	if (bmouseMoveMode) return;
	tpsCamComp->SetFieldOfView(45.f);
	springArmComp->SetRelativeLocation(FVector(0, 0, 30));
	springArmComp->SocketOffset.Y = 30;
}

void ACharacterBase::ZoomOut()
{
	if (bmouseMoveMode) return;

	tpsCamComp->SetFieldOfView(90.f);
	springArmComp->SetRelativeLocation(FVector(0, 0, 30));
	//springArmComp->TargetArmLength = 400;
	//springArmComp->SocketOffset.Z = 400;

	//springArmComp->TargetArmLength = 200;
	//springArmComp->SocketOffset.Z = 200;
	springArmComp->SocketOffset.Y = 70;
}

void ACharacterBase::SaveStateBeforeLevelChange()
{
	UEQGameInstance* MyGameInstance = Cast<UEQGameInstance>(GetGameInstance());

	if (MyGameInstance)
	{
		MyGameInstance->SavedEquippedWeapons = EquippedWeapons;  // 현재 무기 상태 저장

		// PrimaryWeapon 복원
		if (!EquippedWeapons.IsEmpty())
		{
			MyGameInstance->SavedPrimaryWeapon = PrimaryWeapon;  // 첫 번째 무기를 주무기로 설정
		}

		// 캐릭터 상태 복원 및 애니메이션 동기화
		MyGameInstance->SavedState = EQCharacterState;

		//// 체력 및 스태미나 저장
		MyGameInstance->SavedHealth = health;
		MyGameInstance->SavedStamina = stamina;
	}
}

void ACharacterBase::RestoreStateAfterLevelChange()
{
	UEQGameInstance* MyGameInstance = Cast<UEQGameInstance>(GetGameInstance());

	if (MyGameInstance)
	{
		// 저장된 무기 상태 복원
		EquippedWeapons = MyGameInstance->SavedEquippedWeapons;

		// PrimaryWeapon 복원
		if (!EquippedWeapons.IsEmpty())
		{
			PrimaryWeapon = MyGameInstance->SavedPrimaryWeapon;  // 첫 번째 무기를 주무기로 설정
		}

		// 무기 부착 로직 - CheckEquipWeapon 호출로 재활용
		CheckEquipWeapon();

		// 캐릭터 상태 복원
		EQCharacterState = MyGameInstance->SavedState;
		ChangeState();  // 캐릭터 상태에 따라 로직 업데이트

		// 체력, 스태미나 등 추가 상태 복원 (필요 시)
		health = MyGameInstance->SavedHealth;
		stamina = MyGameInstance->SavedStamina;
	}

	if (springArmComp)
	{
		// 스프링암 설정 복원
		springArmComp->bUsePawnControlRotation = true;  // 컨트롤러 회전을 따라감
		//springArmComp->bInheritPitch = true;           // 상하 회전 허용
		//springArmComp->bInheritYaw = true;             // 좌우 회전 허용
	}








	//FName SocketName;
	//AActor* WeaponActor = nullptr;


	//switch (PrimaryWeapon)
	//{

	//case EWeaponType::Rifle:

	//	SocketName = FName(TEXT("Rifle"));

	//	playerRifle = GetWorld()->SpawnActor<ARifle>();
	//	WeaponActor = playerRifle;
	//	EQCharacterState = PrimaryWeapon == EWeaponType::Rifle ? ECharacterState::RifleMode : EQCharacterState;

	//	break;
	//case EWeaponType::Shotgun:
	//	break;
	//case EWeaponType::RocketLauncher:
	//	break;
	//case EWeaponType::Sword:

	//	SocketName = FName(TEXT("Sword"));

	//	playerSword = GetWorld()->SpawnActor<ASword>();
	//	WeaponActor = playerSword;
	//	EQCharacterState = PrimaryWeapon == EWeaponType::Sword ? ECharacterState::SwordMode : EQCharacterState;

	//	break;
	//default:
	//	break;


	//}

	//if (WeaponActor)
	//{
	//	WeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	//	// 데미지 시스템때문에 주인 정해줘야함
	//	WeaponActor->SetOwner(this);  // 'this'는 무기의 소유자가 될 액터

	//}

}

void ACharacterBase::ThrowGrenade()
{
	if (bIsDashing || bIsRunning || !bCanThrowGrenade || EquippedWeapons.IsEmpty()) return;

	// 수류탄 던지기 불가능 상태로 전환
	bCanThrowGrenade = false;
	
	// 메쉬에서 "Grenade" 소켓의 월드 위치와 회전값 가져오기
	FVector GrenadeSpawnLocation = GetMesh()->GetSocketLocation(FName(TEXT("Grenade")));
	FRotator GrenadeSpawnRotation = GetMesh()->GetSocketRotation(FName(TEXT("Grenade")));

	// 수류탄 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // 수류탄의 소유자 설정


	// 수류탄 스폰
	playerBasicGrenade = GetWorld()->SpawnActor<ABasicGrenade>(
		GrenadeSpawnLocation,
		GrenadeSpawnRotation,
		SpawnParams
	);

	//********************
	playerBasicGrenade->GetMesh()->SetSimulatePhysics(false);
	playerBasicGrenade->collisionComponent->SetSimulatePhysics(false);
	playerBasicGrenade->collisionComponent->SetGenerateOverlapEvents(true);







	playerBasicGrenade->AttachToComponent(GetMesh(), 
		FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
		FName(TEXT("Grenade")));
	
	playerBasicGrenade->SetOwner(this);




	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && ThrowGrenadeMontage)
		{
			AnimInstance->Montage_Play(ThrowGrenadeMontage);

			// 오른손 소켓에 부착된 액터 가져오기
			AActor* attachedActor = nullptr;

			for (USceneComponent* ChildComp : GetMesh()->GetAttachChildren())
			{
				if (ChildComp)
				{
					auto cg = ChildComp->GetAttachSocketName();

					if (cg == RifleSocket || cg == ShotgunSocket || cg == RocketLauncherSocket || cg == SwordSocket)
					{
						attachedActor = Cast<AActor>(ChildComp->GetOwner());
						break;
					}
				}
			}

			if (attachedActor)
			{
				// 루트 컴포넌트를 가져옴

				if (attachedActor->IsA(ASword::StaticClass()))
				{
					attachedActor->AttachToComponent(
						GetMesh(),
						FAttachmentTransformRules::SnapToTargetNotIncludingScale,
						LeftSwordSocket);
				}
				else
				{
					// 오른손에 붙어 있는 무기를 왼손 소켓으로 이동
					attachedActor->AttachToComponent(
						GetMesh(),
						FAttachmentTransformRules::SnapToTargetNotIncludingScale,
						LeftHandSocket
					);
				}
			}
		}
	}

	// 쿨타임 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(GrenadeCooldownTimer, this, &ACharacterBase::ResetGrenadeCooldown, 1.0f, false);
}

// 쿨타임 해제 함수
void ACharacterBase::ResetGrenadeCooldown()
{
	bCanThrowGrenade = true; // 수류탄 던질 수 있는 상태로 전환
}

void ACharacterBase::ChangeWeapon()
{
	// 배열이 두 개의 무기를 가지고 있는지 확인
	if (EquippedWeapons.Num() < 2)
	{
		return; // 무기가 두 개 미만이면 함수 종료
	}
	
	// 주무기가 있으면
	if (EquippedWeapons[0] != EWeaponType::None) 
	{
		AActor* HandAttachedWeapon = nullptr;
		FName BackSocketName;

		if (PrimaryWeapon == EWeaponType::Rifle)
		{
			BackSocketName = FName(TEXT("RifleBack"));
			HandAttachedWeapon = GetAttachedActorAtSocket(FName(TEXT("Rifle")));
		}
		else if (PrimaryWeapon == EWeaponType::Shotgun)
		{
			BackSocketName = FName(TEXT("ShotgunBack"));
			HandAttachedWeapon = GetAttachedActorAtSocket(FName(TEXT("Shotgun")));
		}
		else if (PrimaryWeapon == EWeaponType::RocketLauncher)
		{
			BackSocketName = FName(TEXT("RocketLauncherBack"));
			HandAttachedWeapon = GetAttachedActorAtSocket(FName(TEXT("RocketLauncher")));

		}
		else if (PrimaryWeapon == EWeaponType::Sword)
		{
			BackSocketName = FName(TEXT("SwordBack"));
			HandAttachedWeapon = GetAttachedActorAtSocket(FName(TEXT("Sword")));

		}

		HandAttachedWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			BackSocketName
		);

	}
	// 보조 무기를 손에 부착
	if (EquippedWeapons[1] != EWeaponType::None)
	{
		AActor* BackAttachWeapon = nullptr;
		FName HandSocketName;

		if (EquippedWeapons[1] == EWeaponType::Rifle)
		{
			BackAttachWeapon = GetAttachedActorAtSocket(FName(TEXT("RifleBack")));

			HandSocketName = FName(TEXT("Rifle")); // 보조 무기가 Shotgun이라고 가정
		}
		else if (EquippedWeapons[1] == EWeaponType::Shotgun)
		{
			BackAttachWeapon = GetAttachedActorAtSocket(FName(TEXT("ShotgunBack")));

			HandSocketName = FName(TEXT("Shotgun")); // 보조 무기가 Shotgun이라고 가정
		}
		else if (EquippedWeapons[1] == EWeaponType::RocketLauncher)
		{
			BackAttachWeapon = GetAttachedActorAtSocket(FName(TEXT("RocketLauncherBack")));

			HandSocketName = FName(TEXT("RocketLauncher")); // 보조 무기가 Shotgun이라고 가정
		}
		else if (EquippedWeapons[1] == EWeaponType::Sword)
		{
			BackAttachWeapon = GetAttachedActorAtSocket(FName(TEXT("SwordBack")));

			HandSocketName = FName(TEXT("Sword")); // 보조 무기가 Shotgun이라고 가정
		}

		BackAttachWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			HandSocketName
		);
	}

	// 배열의 0번째와 1번째 요소 교환
	Swap(EquippedWeapons[0], EquippedWeapons[1]);

	// 주 무기를 업데이트
	PrimaryWeapon = EquippedWeapons[0];
}

AActor* ACharacterBase::GetAttachedActorAtSocket(FName SocketName)
{
	// 캐릭터의 SkeletalMeshComponent를 가져오기
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	
	if (!SkeletalMesh)
	{
		return nullptr;
	}

	// 부착된 모든 컴포넌트를 가져옵니다.
	const TArray<USceneComponent*>& AttachedComponents = SkeletalMesh->GetAttachChildren();

	for (USceneComponent* Component : AttachedComponents)
	{
		// 부착된 컴포넌트의 소켓 이름을 확인
		if (Component && Component->GetAttachSocketName() == SocketName)
		{
			// 해당 컴포넌트의 소유자(부착된 액터)를 반환
			return Component->GetOwner();
		}
	}

	// 소켓에 부착된 액터가 없으면 nullptr 반환
	return nullptr;

}

void ACharacterBase::ToggleInventory()
{
	// InventoryUI가 초기화되지 않았다면 생성
	if (!InventoryUI)
	{
		InventoryUI = CreateWidget<UInventoryUI>(GetWorld()->GetFirstPlayerController(), InventoryUIClass);

	}

	// 인벤토리 UI 표시/숨김 처리
	if (bIsInventoryOpen)
	{
		InventoryUI->RemoveFromViewport();
	}
	else
	{
		InventoryUI->AddToViewport();
	}

	bIsInventoryOpen = !bIsInventoryOpen;
}