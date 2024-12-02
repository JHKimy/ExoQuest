#include "Character/CharacterBase.h"
#include <GameFramework/SpringArmComponent.h>	//������ ��
#include <Camera/CameraComponent.h>				// ī�޶�

#include "Components/SceneCaptureComponent2D.h"

#include "GameFramework/CharacterMovementComponent.h" // ĳ���� ������

#include "NavigationSystem.h"		// ���콺 ������ �׺�
#include "Blueprint/AIBlueprintHelperLibrary.h" // AI ���� �������Ʈ �Լ����� C++ �ڵ忡�� ���

#include "Player/EQPlayerController.h"		// �ִϸ��̼� ���� ���
#include "Weapon/WeaponForEquip.h"


#include "Weapon/Rifle.h"
#include "Weapon/Shotgun.h"
#include "Weapon/RocketLauncher.h"
#include "Weapon/Sword.h"			// ����


#include "Character/Animation/EQAnimInstance.h"

#include <Blueprint/UserWidget.h>	// ����
#include "Enemy/EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "Enemy/EnemyBase.h"

#include "Kismet/KismetSystemLibrary.h" // PrintString �Լ� ����� ���� ����

//#include "GameFramework/PlayerController.h"	// ��Ʈ�ѷ�

#include "Animation/AnimMontage.h"	// ��Ÿ��
#include "Animation/AnimInstance.h"	// �ִϸ��̼��ν��Ͻ�

#include "PaperSpriteComponent.h"

#include "Map/EQGameInstance.h"	// �� �̵� �ν��Ͻ�

#include "Animation/AnimNotifies/AnimNotify.h" // Notify ���� ó��

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
		// 1. ���̷�Ż �޽� ����
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 2. ��ġ ����
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 3. ������ �� ������Ʈ ���̱�
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 0, 30));
	springArmComp->TargetArmLength = 400;
	springArmComp->SocketOffset.Z = 400;

	// 3-2. ī�޶� ������Ʈ ���̱�
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->SetRelativeRotation(FRotator(-40, 0, 0));

	// �̴ϸ�
	miniMapSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapSpringArm"));
	miniMapSpringArmComp->SetupAttachment(RootComponent);
	miniMapSpringArmComp->SetRelativeLocationAndRotation(FVector(0, 0, 60.f), FRotator(-45.f, 0.f, 0.f));
	miniMapSpringArmComp->TargetArmLength = 400;
	miniMapSpringArmComp->SocketOffset.Z = 400;

	// ȭ��ǥ�� ĳ���� ���� ȸ��
	miniMapSpringArmComp->bInheritPitch = false;
	miniMapSpringArmComp->bInheritYaw = false;
	miniMapSpringArmComp->bInheritRoll = false;

	miniMapCam = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MiniMapCam"));
	miniMapCam->SetupAttachment(miniMapSpringArmComp); // RootComponent���� ����
	miniMapCam->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	miniMapCam->ProjectionType = ECameraProjectionMode::Orthographic;
	miniMapCam->OrthoWidth = 1024.f;


	GrenadeLaunchPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Grenade"));
	GrenadeLaunchPosition->SetupAttachment(GetCapsuleComponent());
	GrenadeLaunchPosition->SetRelativeLocation(FVector(60.f, 0.f, 60.f));

	//characterPositionArrow->OwnerNosee

	// grenadePos ����
	// grenadePos = CreateDefaultSubobject<UChildActorComponent>(TEXT("Grenade Launch Position"));
	// ĳ������ Mesh�� ����
	// grenadePos->SetupAttachment(GetMesh(), FName(TEXT("Grenade"))); // ���� �̸� Grenade Ȯ�� �ʿ�
	


	bmouseMoveMode = true;
	maxHealth = 100.f;
	health = 50.f;

	maxStamina = 100.f;
	stamina = 100.f;
	staminaDrainRate = 15.0f; // �ʴ� �����ϴ� ��


	// ����ź
	GrenadeLaunchPower = 1000.f;
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	RestoreStateAfterLevelChange();  // ���� ���� �� ���� ����

	//EQPlayerController = Cast<AEQPlayerController>(GetController());

	// ���°� �������� �ʾҴٸ� �⺻ ���·� ����
	if (EQCharacterState == ECharacterState::NoWeaponMode && EquippedWeapons.Num() == 0)
	{
		EQCharacterState = ECharacterState::NoWeaponMode;
	}	ChangeState();




	USkeletalMeshComponent* localMesh = GetMesh();
	// StaticLoadClass�� ����Ͽ� �ִϸ��̼� �������Ʈ Ŭ���� �ε�
	AnimBP = StaticLoadClass(UAnimInstance::StaticClass(), nullptr,
		TEXT("/Game/BluePrint/Character/ABP_EQAnimation.ABP_EQAnimation_C"));

	localMesh->SetAnimInstanceClass(AnimBP);





	// EnemyFSM ������Ʈ�� ������ �ʱ�ȭ
	enemyFSM = Cast<UEnemyFSM>(UGameplayStatics::GetActorOfClass(GetWorld(), UEnemyFSM::StaticClass()));



	//// AEnemyBase ��ü�� ��������
	//enemy = Cast<AEnemyBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyBase::StaticClass()));
	//if (enemy)
	//{
	//	// UEnemyFSM ������Ʈ�� ��������
	//	enemyFSM = enemy->FindComponentByClass<UEnemyFSM>();
	//	if (enemyFSM)
	//	{
	//		// enemyFSM �ʱ�ȭ ���� �۾�
	//		UE_LOG(LogTemp, Warning, TEXT("Enemy FSM Found!"));
	//	}
	//	// PaperSprite �����
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



	// ���� Ȯ�ΰ� ���� ������Ʈ�� �ʿ��� ���� ����
	if (EquippedWeapons.Num() > 0)
	{
		// ���� Ȯ��
		CheckEquipWeapon();
		// ���⿡ ���� ���� ����
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

	//// ȭ�� ���
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




	// ���¹̳� ���� �Լ� ȣ��
	HandleStamina(DeltaTime);









	// ���溤��
	FVector GrenadeForwardVector = FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X);

	GrenadeForwardVector.Z = 0;

	GrenadeForwardVector.Normalize();


	//FRotator ControlRotation = GetControlRotation();
	//FVector ForwardVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X);

	//GrenadeLaunchVelocity = GrenadeForwardVector * GrenadeLaunchPower;
	GrenadeLaunchVelocity = GrenadeLaunchPosition->GetForwardVector() * GrenadeLaunchPower;
	// 10�� ȸ�� �߰� (Z�� ����)
	FRotator RotationOffset = FRotator(0.0f, -10.0f, 0.0f); // Yaw�� -10���� �߰�
	GrenadeLaunchVelocity = RotationOffset.RotateVector(GrenadeLaunchVelocity);



}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ACharacterBase::CheckEquipWeapon()
{
	// ���Ⱑ �ϳ��� ������ ���� �ߴ�
	if (EquippedWeapons.IsEmpty()) return;

	// ���� �ϳ��� ������ 
	PrimaryWeapon = EquippedWeapons[0];
	bmouseMoveMode = false;


	{

		if (enemyFSM)
		{
			enemyFSM->UpdateWeaponDamage();
		}

		// ���� UI ũ�ν� ��� �ν��Ͻ� ����
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


		// WeaponForEquip���� ȹ���� ���� Ȯ��
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
				// ������ �ý��۶����� ���� ���������
				WeaponActor->SetOwner(this);  // 'this'�� ������ �����ڰ� �� ����

			}
		}
	}


}

void ACharacterBase::ChangeState()
{
	switch (EQCharacterState)
	{
	case ECharacterState::NoWeaponMode:

		// ĳ���Ϳ� ��Ʈ�ѷ��� ȸ�� ����
		bUseControllerRotationYaw = false;
		// �̵� �������� ĳ���͸� ȸ��
		GetCharacterMovement()->bOrientRotationToMovement = true;
		// ���������� �׸�ŭ �������
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

	// �ִϸ��̼� �������Ʈ�� ���� ���� ����
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		UEQAnimInstance* EQAnimInstance = Cast<UEQAnimInstance>(AnimInstance);
		if (EQAnimInstance)
		{
			// �ִϸ��̼� ���� ����
			EQAnimInstance->SetCharacterState(EQCharacterState);
		}
	}


	if (GetVelocity().Size() >= 800.f)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}



	//// ���� �޸��� �ִٸ�
	//if (GetVelocity().Size() >= 800.f) {
	//	GetCharacterMovement()->bOrientRotationToMovement = true;
	//	bUseControllerRotationYaw = false;
	//}
	//// �޸��� ���߸�
	//else
	//{
	//	bUseControllerRotationYaw = true;
	//	/*���� ������*/if (EquippedWeapons.Num() > 0) GetCharacterMovement()->bOrientRotationToMovement = false;
	//	/*���� ������*/else if (EquippedWeapons.Num() == 0) GetCharacterMovement()->bOrientRotationToMovement = true;
	//}

}

void ACharacterBase::MouseClickMove()
{
	// ���� ȹ���ϸ� ����
	if (EquippedWeapons.Num() > 0) return;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{

		// ���� ��ġ, ���� ����
		FVector WorldLocation, WorldDirection;
		//
		const float TraceDistance = 10000.f;

		// 2Dȭ�� ��ǥ �����ͼ� 3D ��ǥ�� ������
		PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

		// ���� Ʈ���̽��� ���� Ŭ���� ��ġ�� ���
		FVector TraceEnd = WorldLocation + (WorldDirection * TraceDistance);

		// �浹 ���� ���� ����ü
		FHitResult HitResult;
		// ���� Ʈ���̽��� ���� 
		// WorldLocation -> TraceEnd ���� 
		// ECC_Visibility �浹ä���� �浹 ������ HitResult�� ����
		GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility);


		if (HitResult.IsValidBlockingHit())
		{
			// �� ��η� ��Ʈ�ѷ��� ���
			// ���ο��� ĳ���� ���� �����ؼ� ������ ��ġ�� ������
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), HitResult.Location);
		}
	}


}

void ACharacterBase::WASDClick(const FInputActionValue& InputValue)
{
	// ���Ⱑ ������ ����
	if (EquippedWeapons.Num() == 0) return;


	// �Էµ� ������ X, Y ���� ����
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	float ValueX = MovementVector.X;
	float ValueY = MovementVector.Y;


	// ���� ��Ʈ�ѷ� ȸ����
	FRotator LocalControlRotation = GetControlRotation();
	// ���溤��
	FVector ForwardVector = FRotationMatrix(LocalControlRotation).GetUnitAxis(EAxis::X);
	// �캤��
	FVector RightVector = FRotationMatrix(LocalControlRotation).GetUnitAxis(EAxis::Y);


	// https://leekangw.github.io/posts/73/
	// Z ���� ȸ���� ����� 2D�� ���� �ʿ��� ����� �ӵ��� ����
	ForwardVector.Z = 0;
	RightVector.Z = 0;

	ForwardVector.Normalize();
	RightVector.Normalize();

	AddMovementInput(RightVector, ValueX);
	AddMovementInput(ForwardVector, ValueY);

}

void ACharacterBase::Rotate(const FInputActionValue& InputValue)
{
	// ���Ⱑ ������ ����
	if (EquippedWeapons.Num() == 0) return;

	APlayerController* EQPlayerController = Cast<APlayerController>(GetController());

	// �Էµ� ������ X, Y ���� ����
	FVector2D MovementVector = InputValue.Get<FVector2D>();
	float ValueX = -MovementVector.X; // ?
	float ValueY = MovementVector.Y;

	EQPlayerController->AddYawInput(ValueX);
	EQPlayerController->AddPitchInput(ValueY);
}

void ACharacterBase::CustomJump()
{
	// ���Ⱑ ������ ����
	if (EquippedWeapons.Num() == 0) return;

	Jump();
}

void ACharacterBase::RunStart()
{
	// ���Ⱑ ������ ����
	if (EquippedWeapons.Num() == 0) return;

	GetCharacterMovement()->MaxWalkSpeed = 1000.f;

	bIsRunning = true;
}

void ACharacterBase::RunStop()
{
	// ���Ⱑ ������ ����
	if (EquippedWeapons.Num() == 0) return;

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;

	bIsRunning = false;
}

// �뽬 ��ų
void ACharacterBase::DashStart()
{
	// ���Ⱑ ������ ����
	if (EquippedWeapons.Num() == 0) return;

	// �뽬�� ��Ÿ�� ���̸� �Լ� ����
	if (!bCanDash) return;


	bIsDashing = true;  // �뽬 ���� �� ���� ����

	// �뽬 ����
	bCanDash = false;

	// ĳ������ �̵� ���� ��������
	FVector CurrentVelocity = GetVelocity();

	// �뽬 �� �ӵ� ����
	beforeDashVelocity = CurrentVelocity;

	// �̵� ������ ���� ���(���� ����), �⺻������ �ٶ󺸴� �������� �뽬
	if (CurrentVelocity.IsNearlyZero())
	{
		CurrentVelocity = GetActorForwardVector();
	}

	// �뽬 ������ ���� �̵� �������� ���� (����ȭ�Ͽ� ũ�⸦ 1�� ����)
	FVector DashDirection = CurrentVelocity.GetSafeNormal();
	DashDirection.Z = 0;

	// �뽬 �Ÿ�
	float DashDistance = 20000.0f;

	// �뽬 �۵�
	GetMovementComponent()->Velocity = DashDirection * DashDistance;

	// �뽬 �۵� ��
	GetWorld()->GetTimerManager().SetTimer
	(DashTimer, this, &ACharacterBase::StopDash, 0.1f, false);

}

void ACharacterBase::StopDash()
{
	// �뽬 �� �ӵ��� ���ư���
	GetMovementComponent()->Velocity = beforeDashVelocity;

	bIsDashing = false;

	// 2�� �Ŀ� �뽬 ���� ���·� ����
	GetWorld()->GetTimerManager().SetTimer
	(DashTimer, this, &ACharacterBase::ResetDash, dashCoolTime, false);
}

void ACharacterBase::ResetDash()
{
	// �뽬 ���� 
	bCanDash = true;

}

void ACharacterBase::WeaponAttack()
{
	// �뽬�� �޸��� ���� ���� �߻����� ����
	// �޺� ������ ���� ���ݰ��� ����
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
		//// ����
		//const char* comboList[] = { "RIghtSlash","LeftSlash"};

		//// UAnimInstance�� �ùٸ��� ������
		//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		//if (AnimInstance && swordComboMontage)
		//{


		//	if (currentCombo == 0)
		//	{
		//		AnimInstance->Montage_Play(swordComboMontage);
		//		AnimInstance->Montage_JumpToSection(FName("RightSlash"));
		//		currentCombo = 1;
		//		// Ÿ�̸� ���� - 0.5�� ���� �� ��° �Է��� ������ �޺� �ʱ�ȭ
		//		GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ACharacterBase::ResetCombo, 0.1f, false);
		//	}


		//	// �� ��° �������� �̵�
		//	else if (currentCombo == 1)
		//	{
		//		//bCanAttack = false; // ���� �Է� ����
		//		
		//		currentCombo = 0;
		//		AnimInstance->Montage_JumpToSection(FName("LeftSlash"));
		//		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
		//		// Ÿ�̸� �ʱ�ȭ (�� ��° ������ ���������� ����Ǿ��� ������ �ʱ�ȭ)
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
			RunStop(); // ���¹̳��� �� ������ �ڵ����� �޸��� ����
		}
	}
	else if (!bIsRunning && stamina < maxStamina) // �޸��� ������ �� ������ ȸ��
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

	// ����
	//const char* comboList[] = { "RIghtSlash","LeftSlash" };

	// UAnimInstance�� �ùٸ��� ������
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && swordComboMontage)
	{
		AnimInstance->Montage_Play(swordComboMontage);
		if (currentCombo == 0)
		{
			AnimInstance->Montage_JumpToSection(FName("RightSlash"));
			currentCombo = 1;
			// Ÿ�̸� ���� - ���� �ð� ���� �� ��° �Է��� ������ �޺� �ʱ�ȭ
			GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ACharacterBase::ResetCombo, 0.5f, false);
		}
		else if (currentCombo == 1)
		{
			AnimInstance->Montage_JumpToSection(FName("LeftSlash"));
			currentCombo = 0;
			// Ÿ�̸� �ʱ�ȭ (�� ��° ������ ���������� ����Ǿ��� ������ �ʱ�ȭ)
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
		MyGameInstance->SavedEquippedWeapons = EquippedWeapons;  // ���� ���� ���� ����

		// PrimaryWeapon ����
		if (!EquippedWeapons.IsEmpty())
		{
			MyGameInstance->SavedPrimaryWeapon = PrimaryWeapon;  // ù ��° ���⸦ �ֹ���� ����
		}

		// ĳ���� ���� ���� �� �ִϸ��̼� ����ȭ
		MyGameInstance->SavedState = EQCharacterState;

		//// ü�� �� ���¹̳� ����
		MyGameInstance->SavedHealth = health;
		MyGameInstance->SavedStamina = stamina;
	}
}

void ACharacterBase::RestoreStateAfterLevelChange()
{
	UEQGameInstance* MyGameInstance = Cast<UEQGameInstance>(GetGameInstance());

	if (MyGameInstance)
	{
		// ����� ���� ���� ����
		EquippedWeapons = MyGameInstance->SavedEquippedWeapons;

		// PrimaryWeapon ����
		if (!EquippedWeapons.IsEmpty())
		{
			PrimaryWeapon = MyGameInstance->SavedPrimaryWeapon;  // ù ��° ���⸦ �ֹ���� ����
		}

		// ���� ���� ���� - CheckEquipWeapon ȣ��� ��Ȱ��
		CheckEquipWeapon();

		// ĳ���� ���� ����
		EQCharacterState = MyGameInstance->SavedState;
		ChangeState();  // ĳ���� ���¿� ���� ���� ������Ʈ

		// ü��, ���¹̳� �� �߰� ���� ���� (�ʿ� ��)
		health = MyGameInstance->SavedHealth;
		stamina = MyGameInstance->SavedStamina;
	}

	if (springArmComp)
	{
		// �������� ���� ����
		springArmComp->bUsePawnControlRotation = true;  // ��Ʈ�ѷ� ȸ���� ����
		//springArmComp->bInheritPitch = true;           // ���� ȸ�� ���
		//springArmComp->bInheritYaw = true;             // �¿� ȸ�� ���
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
	//	// ������ �ý��۶����� ���� ���������
	//	WeaponActor->SetOwner(this);  // 'this'�� ������ �����ڰ� �� ����

	//}

}

void ACharacterBase::ThrowGrenade()
{
	if (bIsDashing || bIsRunning || !bCanThrowGrenade || EquippedWeapons.IsEmpty()) return;

	// ����ź ������ �Ұ��� ���·� ��ȯ
	bCanThrowGrenade = false;
	
	// �޽����� "Grenade" ������ ���� ��ġ�� ȸ���� ��������
	FVector GrenadeSpawnLocation = GetMesh()->GetSocketLocation(FName(TEXT("Grenade")));
	FRotator GrenadeSpawnRotation = GetMesh()->GetSocketRotation(FName(TEXT("Grenade")));

	// ����ź ����
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this; // ����ź�� ������ ����


	// ����ź ����
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

			// ������ ���Ͽ� ������ ���� ��������
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
				// ��Ʈ ������Ʈ�� ������

				if (attachedActor->IsA(ASword::StaticClass()))
				{
					attachedActor->AttachToComponent(
						GetMesh(),
						FAttachmentTransformRules::SnapToTargetNotIncludingScale,
						LeftSwordSocket);
				}
				else
				{
					// �����տ� �پ� �ִ� ���⸦ �޼� �������� �̵�
					attachedActor->AttachToComponent(
						GetMesh(),
						FAttachmentTransformRules::SnapToTargetNotIncludingScale,
						LeftHandSocket
					);
				}
			}
		}
	}

	// ��Ÿ�� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(GrenadeCooldownTimer, this, &ACharacterBase::ResetGrenadeCooldown, 1.0f, false);
}

// ��Ÿ�� ���� �Լ�
void ACharacterBase::ResetGrenadeCooldown()
{
	bCanThrowGrenade = true; // ����ź ���� �� �ִ� ���·� ��ȯ
}

void ACharacterBase::ChangeWeapon()
{
	// �迭�� �� ���� ���⸦ ������ �ִ��� Ȯ��
	if (EquippedWeapons.Num() < 2)
	{
		return; // ���Ⱑ �� �� �̸��̸� �Լ� ����
	}
	
	// �ֹ��Ⱑ ������
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
	// ���� ���⸦ �տ� ����
	if (EquippedWeapons[1] != EWeaponType::None)
	{
		AActor* BackAttachWeapon = nullptr;
		FName HandSocketName;

		if (EquippedWeapons[1] == EWeaponType::Rifle)
		{
			BackAttachWeapon = GetAttachedActorAtSocket(FName(TEXT("RifleBack")));

			HandSocketName = FName(TEXT("Rifle")); // ���� ���Ⱑ Shotgun�̶�� ����
		}
		else if (EquippedWeapons[1] == EWeaponType::Shotgun)
		{
			BackAttachWeapon = GetAttachedActorAtSocket(FName(TEXT("ShotgunBack")));

			HandSocketName = FName(TEXT("Shotgun")); // ���� ���Ⱑ Shotgun�̶�� ����
		}
		else if (EquippedWeapons[1] == EWeaponType::RocketLauncher)
		{
			BackAttachWeapon = GetAttachedActorAtSocket(FName(TEXT("RocketLauncherBack")));

			HandSocketName = FName(TEXT("RocketLauncher")); // ���� ���Ⱑ Shotgun�̶�� ����
		}
		else if (EquippedWeapons[1] == EWeaponType::Sword)
		{
			BackAttachWeapon = GetAttachedActorAtSocket(FName(TEXT("SwordBack")));

			HandSocketName = FName(TEXT("Sword")); // ���� ���Ⱑ Shotgun�̶�� ����
		}

		BackAttachWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			HandSocketName
		);
	}

	// �迭�� 0��°�� 1��° ��� ��ȯ
	Swap(EquippedWeapons[0], EquippedWeapons[1]);

	// �� ���⸦ ������Ʈ
	PrimaryWeapon = EquippedWeapons[0];
}

AActor* ACharacterBase::GetAttachedActorAtSocket(FName SocketName)
{
	// ĳ������ SkeletalMeshComponent�� ��������
	USkeletalMeshComponent* SkeletalMesh = GetMesh();
	
	if (!SkeletalMesh)
	{
		return nullptr;
	}

	// ������ ��� ������Ʈ�� �����ɴϴ�.
	const TArray<USceneComponent*>& AttachedComponents = SkeletalMesh->GetAttachChildren();

	for (USceneComponent* Component : AttachedComponents)
	{
		// ������ ������Ʈ�� ���� �̸��� Ȯ��
		if (Component && Component->GetAttachSocketName() == SocketName)
		{
			// �ش� ������Ʈ�� ������(������ ����)�� ��ȯ
			return Component->GetOwner();
		}
	}

	// ���Ͽ� ������ ���Ͱ� ������ nullptr ��ȯ
	return nullptr;

}

void ACharacterBase::ToggleInventory()
{
	// InventoryUI�� �ʱ�ȭ���� �ʾҴٸ� ����
	if (!InventoryUI)
	{
		InventoryUI = CreateWidget<UInventoryUI>(GetWorld()->GetFirstPlayerController(), InventoryUIClass);

	}

	// �κ��丮 UI ǥ��/���� ó��
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