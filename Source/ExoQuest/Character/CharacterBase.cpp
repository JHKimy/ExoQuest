#include "Character/CharacterBase.h"
#include <GameFramework/SpringArmComponent.h>	//������ ��
#include <Camera/CameraComponent.h>				// ī�޶�
#include "GameFramework/CharacterMovementComponent.h" // ĳ���� ������

#include "NavigationSystem.h"		// ���콺 ������ �׺�
#include "Blueprint/AIBlueprintHelperLibrary.h" // AI ���� �������Ʈ �Լ����� C++ �ڵ忡�� ���

#include "Player/EQPlayerController.h"		// �ִϸ��̼� ���� ���
#include "Weapon/WeaponForEquip.h"

#include "Weapon/Rifle.h"

#include "Character/Animation/EQAnimInstance.h"

//#include "GameFramework/PlayerController.h"	// ��Ʈ�ѷ�



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

}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	//EQPlayerController = Cast<AEQPlayerController>(GetController());

	EQCharacterState = ECharacterState::NoWeaponMode;
	ChangeState();


	USkeletalMeshComponent* localMesh = GetMesh();
	UClass* AnimBP;
	// StaticLoadClass�� ����Ͽ� �ִϸ��̼� �������Ʈ Ŭ���� �ε�
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
	// ���� Ȯ��
	CheckEquipWeapon();
	// ���⿡ ���� ���� ����
	ChangeState();


}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacterBase::CheckEquipWeapon()
{
	// ���� �ϳ��� ������ 
	if (EquippedWeapons.Num() > 0) 
	{
		PrimaryWeapon = EquippedWeapons[0];
		
	}
	else {
		// ������ ����
		return;
	}

	// WeaponForEquip���� �ֹ��� ���Ͽ� ���̱�
	switch (PrimaryWeapon)
	{
	//case EWeaponType::None:

	//	break;

	
	case EWeaponType::Rifle:
	{
		FName RifleSocket(TEXT("Rifle"));
		auto RifleforEquip = GetWorld()->SpawnActor<ARifle>(FVector::ZeroVector, FRotator::ZeroRotator);

		// ���Ͽ� ����
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

	// �ִϸ��̼� �������Ʈ�� ���� ���� ����
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

		// ĳ���Ϳ� ��Ʈ�ѷ��� ȸ�� ����
		bUseControllerRotationYaw = false;
		// �̵� �������� ĳ���͸� ȸ��
		GetCharacterMovement()->bOrientRotationToMovement = true;
		// ���������� �׸�ŭ �������
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
}

void ACharacterBase::RunStop()
{
	// ���Ⱑ ������ ����
	if (EquippedWeapons.Num() == 0) return;

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

// �뽬 ��ų
void ACharacterBase::DashStart()
{
	// ���Ⱑ ������ ����
	if (EquippedWeapons.Num() == 0) return;

	// �뽬�� ��Ÿ�� ���̸� �Լ� ����
	if (!bCanDash) return; 
	



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

	// 2�� �Ŀ� �뽬 ���� ���·� ����
	GetWorld()->GetTimerManager().SetTimer
	(DashTimer, this, &ACharacterBase::ResetDash, dashCoolTime, false);
}

void ACharacterBase::ResetDash()
{
	// �뽬 ���� 
	bCanDash = true;

}