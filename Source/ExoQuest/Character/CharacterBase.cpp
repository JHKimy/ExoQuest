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

//#include "Components/SkeletalMeshComponent.h"

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

#include "Item/ItemDataBase.h"

#include "Weapon/WeaponForEquip.h"
#include "Components/SceneComponent.h"


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
	springArmComp->TargetArmLength = 600;
	springArmComp->SocketOffset.Z = 400;

	// 3-2. ī�޶� ������Ʈ ���̱�
	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->SetRelativeRotation(FRotator(-25.f, 0.f, 0.f));

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
	miniMapCam->SetRelativeRotation(FRotator(-25.f, 0.f, 0.f));
	miniMapCam->ProjectionType = ECameraProjectionMode::Orthographic;
	miniMapCam->OrthoWidth = 1024.f;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	GrenadeLaunchPosition = CreateDefaultSubobject<UArrowComponent>(TEXT("Grenade"));
	GrenadeLaunchPosition->SetupAttachment(GetCapsuleComponent());
	GrenadeLaunchPosition->SetRelativeLocation(FVector(200.f, 0.f, 100.f));




	LaunchPosition = CreateDefaultSubobject<USceneComponent>(TEXT("LaunchPos"));
	LaunchPosition->SetupAttachment(GetCapsuleComponent());
	LaunchPosition->SetRelativeLocation(FVector(200.f, 0.f, 150.f));




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












	// �����ͺ��̽� ����
	ItemDataBase = CreateDefaultSubobject<UItemDataBase>(TEXT("ItemDataBase"));
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


	springArmComp->TargetArmLength = 600;
	springArmComp->SocketOffset.Z = 400;
	springArmComp->SocketOffset.Y = 10;

	tpsCamComp->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	// ĳ���Ϳ� ��Ʈ�ѷ��� ȸ�� ����
	bUseControllerRotationYaw = false;
	// �̵� �������� ĳ���͸� ȸ��
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// ���������� �׸�ŭ �������
	springArmComp->bUsePawnControlRotation = true;
	isFirstSpawn = true;



	// ItemDataBase�� nullptr�̸� �ʱ�ȭ
	if (!ItemDataBase)
	{
		ItemDataBase = NewObject<UItemDataBase>(this);
	}








	// InventoryUI�� �ʱ�ȭ���� �ʾҴٸ� ����
	if (!InventoryUI)
	{
		InventoryUI = CreateWidget<UInventoryUI>(GetWorld()->GetFirstPlayerController(), InventoryUIClass);

	}



	//// 1�� �������� ��ġ ���
	//GetWorldTimerManager().SetTimer(LocationRecordTimerHandle, this, &ACharacterBase::RecordLocation, 1.0f, true);


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


	// ���� ���� ���¸� ���
	// PrintEquippedWeapons();



	// FString WeaponsList = "Equipped Weapons : ";

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



	//PrintCharacterState();





	//PrintInventory();


	//// ���� ��ġ�� ȸ�� ���
	//FTimeRecord NewRecord(GetActorLocation(), GetActorRotation(), GetWorld()->GetTimeSeconds());
	//TimeRecords.Add(NewRecord);

	//// ������ ��� ����
	//while (TimeRecords.Num() > 0 && GetWorld()->GetTimeSeconds() - TimeRecords[0].Timestamp > MaxRecordTime)
	//{
	//	TimeRecords.RemoveAt(0);
	//}




// =======================================================
	// ���� �ð�
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// ���� ��
	if (CurrentTime - LastSaveTime >= 0.1f)
	{
		// ��ġ ���� �迭�� ����
		FTimeRecord newRecord(GetActorLocation(), GetActorRotation(), CurrentTime);
		TimeRecords.Add(newRecord);

		// ������ ���� �ð� ������Ʈ
		LastSaveTime = CurrentTime;
	}
	if (TimeRecords.Num() > 0)
	{
		// 5�� �̻� ���� ��� ����
		while ((CurrentTime - TimeRecords[0].Timestamp) > 5.0f)
		{
			TimeRecords.RemoveAt(0);
		}
	}














	if ((GetVelocity().SizeSquared()) > 0)
	{
		bIsMoving = true;
	}
	else 
	{
		bIsMoving = false;
	}



	if (bIsMoving)
	{
		rootYawOffset = 0;
	}

	else if (!bIsMoving)
	{
		prevCharacterYaw = characterYaw;

		characterYaw = GetActorRotation().Yaw;

		rootYawOffset = UKismetMathLibrary::NormalizeAxis
		(rootYawOffset - (characterYaw - prevCharacterYaw));

		rootYawOffset = FMath::Clamp(rootYawOffset, -90.0f, 90.0f);
	}

	//if (rootYawOffset >= 80)
	//{
	//	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("rootYawOffset: %f"), rootYawOffset), true, true, FColor::Red, 2.0f);
	//	bUseControllerRotationYaw = false;

	//	TurnInPlace();
	//}















	if (bIsThrowingGrenade)
	{
		ShowProjectilePrediction();
		//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		//UEQAnimInstance* anim = Cast<UEQAnimInstance>(AnimInstance);

		// anim->AnimNotify_Aim();
	}


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

	if (EquippedWeapons.Num() <= 2) {


		// WeaponForEquip���� ȹ���� ���� Ȯ��
		for (EWeaponType WeaponType : EquippedWeapons)
		{
			FName SocketName;
			AActor* WeaponActor = nullptr;

			switch (WeaponType)
			{
			case EWeaponType::Rifle:
				if (playerRifle) continue;
				playerRifle = GetWorld()->SpawnActor<ARifle>(rifleBlueprint);
				//playerRifle = GetWorld()->SpawnActor<ARifle>();
				WeaponActor = playerRifle;
				SocketName = (WeaponType == PrimaryWeapon) ? FName(TEXT("Rifle")) : FName(TEXT("RifleBack"));
				EQCharacterState = (PrimaryWeapon == EWeaponType::Rifle) ? ECharacterState::RifleMode : EQCharacterState;
				break;



			case EWeaponType::Shotgun:
				if (playerShotgun) continue;
				playerShotgun = GetWorld()->SpawnActor<AShotgun>(shotgunBlueprint);

				//playerShotgun = GetWorld()->SpawnActor<AShotgun>();
				WeaponActor = playerShotgun;
				SocketName = (WeaponType == PrimaryWeapon) ? FName(TEXT("Shotgun")) : FName(TEXT("ShotgunBack"));
				EQCharacterState = (PrimaryWeapon == EWeaponType::Shotgun) ? ECharacterState::ShotgunMode : EQCharacterState;
				break;


			case EWeaponType::RocketLauncher:
				if (playerRocketLauncher) continue;
				playerRocketLauncher = GetWorld()->SpawnActor<ARocketLauncher>(rocketLauncherBlueprint);

				//playerRocketLauncher = GetWorld()->SpawnActor<ARocketLauncher>();
				WeaponActor = playerRocketLauncher;
				SocketName = (WeaponType == PrimaryWeapon) ? FName(TEXT("RocketLauncher")) : FName(TEXT("RocketLauncherBack"));
				EQCharacterState = (PrimaryWeapon == EWeaponType::RocketLauncher) ? ECharacterState::RocketLauncherMode : EQCharacterState;
				break;

			case EWeaponType::Sword:
				if (playerSword) continue;
				playerSword = GetWorld()->SpawnActor<ASword>(swordBlueprint);

				//	playerSword = GetWorld()->SpawnActor<ASword>();
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

	APlayerController* PlayerController = Cast<APlayerController>(GetController());


	if (EquippedWeapons.Num() == 1 && !bMouseCursorHidden && PlayerController)
	{
		PlayerController->bShowMouseCursor = false;          // ���콺 Ŀ�� ����
		// PlayerController->SetInputMode(FInputModeGameOnly()); // ���� ���� ��ȯ
		bMouseCursorHidden = true;                           // ���콺 Ŀ�� ���� ���� ����
	}
}

void ACharacterBase::ChangeState()
{
	switch (EQCharacterState)
	{
	case ECharacterState::NoWeaponMode:
		if (isFirstSpawn) {

			springArmComp->TargetArmLength = 600;
			springArmComp->SocketOffset.Z = 400;
			// ĳ���Ϳ� ��Ʈ�ѷ��� ȸ�� ����
			bUseControllerRotationYaw = false;
			// �̵� �������� ĳ���͸� ȸ��
			GetCharacterMovement()->bOrientRotationToMovement = true;
			// ���������� �׸�ŭ �������
			springArmComp->bUsePawnControlRotation = true;
		}
		else if (!isFirstSpawn) {
			springArmComp->TargetArmLength = 200;
			springArmComp->SocketOffset.Z = 200;
			springArmComp->SocketOffset.Y = 70;
			tpsCamComp->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
			GetCharacterMovement()->bOrientRotationToMovement = true;
			springArmComp->bUsePawnControlRotation = true;

		

			bUseControllerRotationYaw = true;
		










	}
		break;

	case ECharacterState::RifleMode:
	case ECharacterState::ShotgunMode:
	case ECharacterState::RocketLauncherMode:
	case ECharacterState::SwordMode:
		springArmComp->TargetArmLength = 200;
		springArmComp->SocketOffset.Z = 200;
		springArmComp->SocketOffset.Y = 70;
		tpsCamComp->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
		GetCharacterMovement()->bOrientRotationToMovement = false;
		
		
		
		
		bUseControllerRotationYaw = true;
		

		// �̰�    
		// bUseControllerRotationYaw = true;
		
		isFirstSpawn = false;
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
	if (EquippedWeapons.Num() > 0 && !isFirstSpawn) return;

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
	if (EquippedWeapons.Num() == 0 && isFirstSpawn) return;


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
	if (EquippedWeapons.Num() == 0 && isFirstSpawn) return;
	if (bIsInventoryOpen) return;


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
	if (bIsDashing || bIsRunning || bIsInventoryOpen) return;

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
	if (currentCombo == 3 || bIsInventoryOpen || bIsDashing || bIsRunning || PrimaryWeapon != EWeaponType::Sword) return;
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
			currentCombo = 2;
			// Ÿ�̸� �ʱ�ȭ (�� ��° ������ ���������� ����Ǿ��� ������ �ʱ�ȭ)
			GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ACharacterBase::ResetCombo, 0.15f, false);
			
		}
		else if (currentCombo == 2)
		{
			AnimInstance->Montage_JumpToSection(FName("UpSlash"));
			currentCombo = 3;
			// Ÿ�̸� �ʱ�ȭ (�� ��° ������ ���������� ����Ǿ��� ������ �ʱ�ȭ)
			//GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ACharacterBase::ResetCombo, 0.5f, false);

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

void ACharacterBase::ActivateGhostMode(float Duration)
{
	// �浹 ä�� ����
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// ���� �浹�� Ȱ��ȭ (ECC_WorldStatic�� ���� ä�η� ����)
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Block);

	// ���� ĳ���� �޽ÿ� ������ ����
	SetCharacterTransparency(0.7f); // ���� 30% (0.0 = ���� ����, 1.0 = ���� ������)
	//// ���� ������ ���� ó��
	//if (CurrentWeapon)
	//{
	//	CurrentWeapon->SetActorEnableCollision(false);
	//}

	// Ÿ�̸ӷ� ���� �ð� �� ��Ʈ ���� ����
	GetWorldTimerManager().SetTimer(GhostTimerHandle, this, &ACharacterBase::DeactivateGhostMode, Duration, false);
}

void ACharacterBase::DeactivateGhostMode()
{
	// �浹 ä�� ������� ����
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	SetCharacterTransparency(0.f);
	//// ���� ������ ���� ó��
	//if (CurrentWeapon)
	//{
	//	CurrentWeapon->SetActorEnableCollision(true);
	//}
}

void ACharacterBase::SetCharacterTransparency(float transparency)
{
	// ĳ������ �޽ÿ� ����
	USkeletalMeshComponent* tempMesh = GetMesh();
	if (!tempMesh) return;

	// �޽ÿ� �Ҵ�� ��Ƽ���� �������� (Ȥ �������� �� �� ����)
	int32 MaterialCount = tempMesh->GetNumMaterials();
	for (int32 i = 0; i < MaterialCount; i++)
	{
		UMaterialInstanceDynamic* DynamicMaterial = tempMesh->CreateDynamicMaterialInstance(i);
		if (DynamicMaterial)
		{
			// ��Ƽ������ Opacity �� ���� (��Ƽ���� Opacity �Ķ���Ͱ� �־�� ��)
			DynamicMaterial->SetScalarParameterValue(TEXT("Transparency"), transparency);
		}
	}
}

void ACharacterBase::ActivateTimeRewind(float duration)
{
	if (TimeRecords.Num() == 0 )
	{
		return; // �迭�� ��� �ְų� �̹� �ǵ����� ���̶�� ����
	}

	RewindElapsedTime = 0.0f;

	// �ǵ����� �� �浹 ����
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 0.01�� �������� 50�� �����Ͽ� 1�� ���� �ǵ�����
	GetWorldTimerManager().SetTimer(RewindTimerHandle, this, &ACharacterBase::RewindStep, 0.01f, true);
}

void ACharacterBase::RewindStep()
{
	if (TimeRecords.Num() == 0 || RewindElapsedTime >= RewindDuration)
	{
		// �ǵ����� ����
		GetWorldTimerManager().ClearTimer(RewindTimerHandle);
		TimeRecords.Empty();

		// �浹 �ٽ� Ȱ��ȭ
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		return;
	}

	// ���൵ ��� (1 ~ 0)
	float Progress = 1.f - RewindElapsedTime / RewindDuration;
	// 
	int32 TotalFrames = TimeRecords.Num();
	
	// ����(1) -> ����(0) 
	int32 TargetIndex = FMath::Clamp(FMath::Lerp(0, TotalFrames - 1, Progress), 0, TotalFrames - 1);

	if (TimeRecords.IsValidIndex(TargetIndex))
	{
		SetActorLocation(TimeRecords[TargetIndex].Location);
		//SetActorRotation(TimeRecords[TargetIndex].Rotation);
	}

	RewindElapsedTime += 0.01f; // Ÿ�̸� ���ݰ� �����ϰ� ����
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


		// ������ ������ ����
		MyGameInstance->SavedItems = ItemDataBase->Items;
		//MyGameInstance->SavedSlots.Add(SlotData);
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


		// ������ ������ ����
		if (ItemDataBase)
		{
			ItemDataBase->Items = MyGameInstance->SavedItems;
		}

		// UI ������Ʈ
		if (InventoryUI)
		{
			InventoryUI->UpdateInventory();
		}
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
	//if (bIsDashing || bIsRunning || !bCanThrowGrenade || EquippedWeapons.IsEmpty()) return;

	//// ����ź ������ �Ұ��� ���·� ��ȯ
	//bCanThrowGrenade = false;

	//// �޽����� "Grenade" ������ ���� ��ġ�� ȸ���� ��������
	//FVector GrenadeSpawnLocation = GetMesh()->GetSocketLocation(FName(TEXT("Grenade")));
	//FRotator GrenadeSpawnRotation = GetMesh()->GetSocketRotation(FName(TEXT("Grenade")));

	//// ����ź ����
	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this; // ����ź�� ������ ����


	//// ����ź ����
	//playerBasicGrenade = GetWorld()->SpawnActor<ABasicGrenade>(
	//	GrenadeSpawnLocation,
	//	GrenadeSpawnRotation,
	//	SpawnParams
	//);

	////********************
	//playerBasicGrenade->GetMesh()->SetSimulatePhysics(false);
	//playerBasicGrenade->collisionComponent->SetSimulatePhysics(false);
	//playerBasicGrenade->collisionComponent->SetGenerateOverlapEvents(true);







	//playerBasicGrenade->AttachToComponent(GetMesh(),
	//	FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	//	FName(TEXT("Grenade")));

	//playerBasicGrenade->SetOwner(this);



	//{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	//	if (AnimInstance && ThrowGrenadeMontage)
	//	{
	//		//AnimInstance->Montage_Play(ThrowGrenadeMontage);
			AnimInstance->Montage_Resume(ThrowGrenadeMontage);
	//		// ������ ���Ͽ� ������ ���� ��������
	//		AActor* attachedActor = nullptr;

	//		for (USceneComponent* ChildComp : GetMesh()->GetAttachChildren())
	//		{
	//			if (ChildComp)
	//			{
	//				auto cg = ChildComp->GetAttachSocketName();

	//				if (cg == RifleSocket || cg == ShotgunSocket || cg == RocketLauncherSocket || cg == SwordSocket)
	//				{
	//					attachedActor = Cast<AActor>(ChildComp->GetOwner());
	//					break;
	//				}
	//			}
	//		}

	//		if (attachedActor)
	//		{
	//			// ��Ʈ ������Ʈ�� ������

	//			if (attachedActor->IsA(ASword::StaticClass()))
	//			{
	//				attachedActor->AttachToComponent(
	//					GetMesh(),
	//					FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	//					LeftSwordSocket);
	//			}
	//			else
	//			{
	//				// �����տ� �پ� �ִ� ���⸦ �޼� �������� �̵�
	//				attachedActor->AttachToComponent(
	//					GetMesh(),
	//					FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	//					LeftHandSocket
	//				);
	//			}
	//		}
	//	}
	//}


	bIsThrowingGrenade = false;

	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//UEQAnimInstance* anim = Cast<UEQAnimInstance>(AnimInstance);
	//anim->Montage_Resume(nullptr);


	// ��Ÿ�� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(GrenadeCooldownTimer, this, &ACharacterBase::ResetGrenadeCooldown, 1.0f, false);
}

void ACharacterBase::ReadyAimGrenade()
{
	if (bIsThrowingGrenade) return; // �̹� ���� ���̸� �������� ����
	if (bIsDashing || bIsRunning  || EquippedWeapons.IsEmpty()) return;


	UKismetSystemLibrary::PrintString(this, TEXT("Aim"), true, true, FColor::Green, 5.0f);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	UEQAnimInstance* anim = Cast<UEQAnimInstance>(AnimInstance);
	anim->Montage_Play(ThrowGrenadeMontage);

	AimGrenade();

	bIsThrowingGrenade = true; // ���� ���� ����
	//GetWorldTimerManager().SetTimer(TimerHandle2, this, &ACharacterBase::AimGrenade, 1.0f, false);
}


void ACharacterBase::AimGrenade()
{

	//if (playerBasicGrenade) return;
	//anim->AnimNotify_Aim();



	//bIsThrowingGrenade = false; // ���� ���� ����

	 // �̹� ����ź�� �����Ǿ����� �ٽ� �������� ����
	if (GrenadeMeshComponent)
	{
		return;
	}



	// ����ź ������ �Ұ��� ���·� ��ȯ
	//bCanThrowGrenade = false;


	if (!GrenadeMeshComponent)
	{

		GrenadeMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());

		// StaticLoadObject�� StaticMesh �ε� (FObjectFinder ��� X)
		UStaticMesh* GrenadeMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/Asset/Weapon/Grenade/BasicGrenade/SM_Grenade.SM_Grenade")));

		if (GrenadeMesh)
		{
			GrenadeMeshComponent->SetStaticMesh(GrenadeMesh);
		}
		else
		{
			UKismetSystemLibrary::PrintString(this, TEXT("Grenade Mesh Not Found!"), true, true, FColor::Red, 3.0f);
		}

		GrenadeMeshComponent->RegisterComponent();
		GrenadeMeshComponent->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName(TEXT("Grenade")));

		GrenadeMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GrenadeMeshComponent->SetSimulatePhysics(false);
	}
	//// �޽����� "Grenade" ������ ���� ��ġ�� ȸ���� ��������
	//FVector GrenadeSpawnLocation = GetMesh()->GetSocketLocation(FName(TEXT("Grenade")));
	//FRotator GrenadeSpawnRotation = GetMesh()->GetSocketRotation(FName(TEXT("Grenade")));

	//// ����ź ����
	//FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this; // ����ź�� ������ ����


	//// ����ź ����
	//playerBasicGrenade = GetWorld()->SpawnActor<ABasicGrenade>(
	//	GrenadeSpawnLocation,
	//	GrenadeSpawnRotation,
	//	SpawnParams
	//);

	//********************
	//playerBasicGrenade->GetMesh()->SetSimulatePhysics(false);
	//playerBasicGrenade->collisionComponent->SetSimulatePhysics(false);
	//playerBasicGrenade->collisionComponent->SetGenerateOverlapEvents(false);
	//playerBasicGrenade->Speed = 0.f; // �ʱ� �ӵ��� ����
	//playerBasicGrenade->maxSpeed = 0.f;
	//playerBasicGrenade->gravity = 0.f;
	//

	//playerBasicGrenade->collisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//playerBasicGrenade->collisionComponent->SetGenerateOverlapEvents(false);
	//playerBasicGrenade->collisionComponent->SetSimulatePhysics(false);





	//playerBasicGrenade->AttachToComponent(GetMesh(),
	//	FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	//	FName(TEXT("Grenade")));

	//playerBasicGrenade->SetOwner(this);


	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && ThrowGrenadeMontage)
	{
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



	// ��Ÿ�� Ÿ�̸� ����
	//GetWorld()->GetTimerManager().SetTimer(GrenadeCooldownTimer, this, &ACharacterBase::ResetGrenadeCooldown, 1.0f, false);





	// anim -> Montage_Play(ThrowGrenadeMontage);

}




void ACharacterBase::ShowProjectilePrediction()
{
	APlayerController* playerController2 = GetWorld()->GetFirstPlayerController();

	FVector StartLocation = LaunchPosition->GetComponentLocation();
	FRotator LaunchRotation;


	FVector CameraLocation;
	FRotator CameraRotation;
	playerController2->GetPlayerViewPoint(CameraLocation, CameraRotation);
	LaunchRotation = CameraRotation;

	// ī�޶� ���� �Ǵ� �ѱ� ������ ������� �� �ʱ� �ӵ� ����
	FVector LaunchVelocity = LaunchRotation.Vector() * 3500.f; // ���� �߻� �ӵ��� ��ġ�ϴ� �ʱ� �ӵ� ����

	// FPredictProjectilePathParams ����
	FPredictProjectilePathParams PredictParams;
	PredictParams.StartLocation = StartLocation;
	PredictParams.LaunchVelocity = LaunchVelocity;
	PredictParams.bTraceWithCollision = true;
	PredictParams.ProjectileRadius = 10.f; // �߻�ü ũ��� ��ġ�ϵ��� ����
	PredictParams.MaxSimTime = 5.0f; // �ùķ��̼� �ð� (��)

	PredictParams.OverrideGravityZ = GetWorld()->GetGravityZ() * 2;


	PredictParams.SimFrequency = 15.f;
	PredictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
	PredictParams.DrawDebugTime = 0.1f;

	FPredictProjectilePathResult PredictResult;
	UGameplayStatics::PredictProjectilePath(this, PredictParams, PredictResult);
	
}






// ��Ÿ�� ���� �Լ�
void ACharacterBase::ResetGrenadeCooldown()
{
	//bCanThrowGrenade = true; // ����ź ���� �� �ִ� ���·� ��ȯ
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

	//CheckEquipWeapon();

	//ChangeState();


	if (PrimaryWeapon == EWeaponType::Rifle)
	{
		EQCharacterState = ECharacterState::RifleMode;
	}
	if (PrimaryWeapon == EWeaponType::Shotgun)
	{
		EQCharacterState = ECharacterState::ShotgunMode;
	}
	if (PrimaryWeapon == EWeaponType::RocketLauncher)
	{
		EQCharacterState = ECharacterState::RocketLauncherMode;
	}
	if (PrimaryWeapon == EWeaponType::Sword)
	{
		EQCharacterState = ECharacterState::SwordMode;
	}



	// �ִϸ��̼� ���� ����

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

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();



	// �κ��丮 UI ǥ��/���� ó��
	if (bIsInventoryOpen)
	{
		InventoryUI->RemoveFromViewport();
		playerController->bShowMouseCursor = false;
	}
	else
	{
		InventoryUI->AddToViewport();
		playerController->bShowMouseCursor = true;
	}

	bIsInventoryOpen = !bIsInventoryOpen;
}

void ACharacterBase::TeleportRoom()
{
	// �� ��ȯ �� ĳ���� ���� ����
	SaveStateBeforeLevelChange();

	// �� ��ȯ (���� ������)
	if (GetWorld())
	{
		GetWorld()->ServerTravel(TEXT("/Game/Maps/GameRoom?listen"), true);
	}
}



void ACharacterBase::SetInputRestrictions(bool bRestrict)
{
	// ���� �߻� ����
	bIsInventoryOpen = !bRestrict;

	// �ʿ��� ��� �߰� �Է� ���� ó�� ����
	if (bRestrict)
	{
		UE_LOG(LogTemp, Log, TEXT("Character input restricted."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Character input restored."));
	}
}

void ACharacterBase::DropWeapon()
{

	// ���� ������ ���Ⱑ ������ �Լ� ����
	if (EquippedWeapons.IsEmpty())
	{
		return;
	}

	// �� ����(PrimaryWeapon)�� ����
	EWeaponType WeaponToDrop = PrimaryWeapon;

	// �� ���⿡ �ش��ϴ� ���� ã��
	AActor* WeaponActor = nullptr;

	switch (WeaponToDrop)
	{
	case EWeaponType::Rifle:
		WeaponActor = playerRifle;
		playerRifle = nullptr; // ���� �� ���� ����
		break;
	case EWeaponType::Shotgun:
		WeaponActor = playerShotgun;
		playerShotgun = nullptr; // ���� �� ���� ����
		break;
	case EWeaponType::RocketLauncher:
		WeaponActor = playerRocketLauncher;
		playerRocketLauncher = nullptr; // ���� �� ���� ����
		break;
	case EWeaponType::Sword:
		WeaponActor = playerSword;
		playerSword = nullptr; // ���� �� ���� ����
		break;
	default:
		break;
	}

	// ���� ���Ͱ� �����ϸ� ó��
	if (WeaponActor)
	{
		// ���忡 ���� ��� (Socket���� �и�)
		WeaponActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//WeaponActor->SetActorEnableCollision(true); // �浹 Ȱ��ȭ
		WeaponActor->SetOwner(nullptr); // ������ ����
		WeaponActor->Destroy(); // ���� ����
	}

	// EquippedWeapons �迭���� ����
	EquippedWeapons.Remove(WeaponToDrop);

	// ���� ���⸦ �� ����� ������Ʈ
	if (!EquippedWeapons.IsEmpty())
	{
		PrimaryWeapon = EquippedWeapons[0];

		// ���� ���⸦ �տ� ����
		FName HandSocketName;
		AActor* SubWeaponActor = nullptr;

		switch (PrimaryWeapon)
		{
		case EWeaponType::Rifle:
			SubWeaponActor = playerRifle;
			HandSocketName = FName(TEXT("Rifle"));
			break;
		case EWeaponType::Shotgun:
			SubWeaponActor = playerShotgun;
			HandSocketName = FName(TEXT("Shotgun"));
			break;
		case EWeaponType::RocketLauncher:
			SubWeaponActor = playerRocketLauncher;
			HandSocketName = FName(TEXT("RocketLauncher"));
			break;
		case EWeaponType::Sword:
			SubWeaponActor = playerSword;
			HandSocketName = FName(TEXT("Sword"));
			break;
		default:
			break;
		}

		if (SubWeaponActor)
		{
			SubWeaponActor->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				HandSocketName);
		}
	}
	else
	{
		// ��� ���⸦ ������ ��� ���¸� ������Ʈ
		PrimaryWeapon = EWeaponType::None;
		EQCharacterState = ECharacterState::NoWeaponMode;
	}

	// ���� ���¿� ���� ���� ����

	ChangeState();

	if (PrimaryWeapon == EWeaponType::Rifle)
	{
		EQCharacterState = ECharacterState::RifleMode;
	}
	if (PrimaryWeapon == EWeaponType::Shotgun)
	{
		EQCharacterState = ECharacterState::ShotgunMode;
	}
	if (PrimaryWeapon == EWeaponType::RocketLauncher)
	{
		EQCharacterState = ECharacterState::RocketLauncherMode;
	}
	if (PrimaryWeapon == EWeaponType::Sword)
	{
		EQCharacterState = ECharacterState::SwordMode;
	}



	TArray<AActor*> FoundWeapons;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWeaponForEquip::StaticClass(), FoundWeapons);

	for (AActor* Actor : FoundWeapons)
	{
		AWeaponForEquip* Weapon = Cast<AWeaponForEquip>(Actor);

		if (Weapon && Weapon->IsHidden()) // Hidden ������ ���⸸ ��������
		{
			Weapon->ShowWeapon();
			break; // ù ��° Hidden ���⿡�� ���� ����
		}
	}
}







void ACharacterBase::TurnInPlace()
{
	if (bIsRotating)
	{
		return;
	}

	bIsRotating = true;

	// ��Ʈ�ѷ� ȸ�� ��Ȱ��ȭ
	//bUseControllerRotationYaw = false;

	yawStart = GetActorRotation().Yaw;
	yawTarget = yawStart + rotationAmount;

	SetActorRotation(FRotator(0, yawTarget, 0));
	bIsRotating = false;
	// ���� �ð��� ���� �� ȸ���� �����ٰ� �Ǵ��ϰ� bUseControllerRotationYaw�� �ٽ� true�� ����
	//GetWorldTimerManager().SetTimer(RotationTimerHandle, this, &ACharacterBase::ResetRotation, 1.f, false);
}

void ACharacterBase::ResetRotation()
{
	//bIsRotating = false;
	//bUseControllerRotationYaw = true;  // ȸ���� ���� �� �ٽ� ��Ʈ�ѷ� ȸ���� Ȱ��ȭ
}


