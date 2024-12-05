#include "Player/EQPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"	// 입력 시스템

#include "Character/CharacterBase.h"
//#include "Kismet/GameplayStatics.h"


AEQPlayerController::AEQPlayerController()
{
}

void AEQPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 마우스 커서 보이게
	bShowMouseCursor = true;

	// IMC 연동
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
		(GetLocalPlayer()))
	{
		// 블루프린트에서 할당된 IMC_Control을 사용
		if (IMC_Control)
		{
			Subsystem->AddMappingContext(IMC_Control, 0);
		}
	}

	// 현재 컨트롤 중인 캐릭터
	MyControlledCharacter = Cast<ACharacterBase>(GetCharacter());
	// 바인딩 BeginPlay 뒤에 호출
	SetupInputBindings(MyControlledCharacter);

}

void AEQPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//// 무기 있으면 마우스 커서 지우기
	//if (MyControlledCharacter->EquippedWeapons.Num() > 0) {
	//	bShowMouseCursor = false;
	//}
	//else {
	//	bShowMouseCursor = true;
	//}

}

void AEQPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void AEQPlayerController::SetupInputBindings(ACharacterBase* myCharacter)
{

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_MouseRightClickMove)
		{
			EnhancedInputComponent->BindAction(IA_MouseRightClickMove, ETriggerEvent::Triggered, myCharacter, &ACharacterBase::MouseClickMove);
		}
		if (IA_WASD)
		{
			EnhancedInputComponent->BindAction(IA_WASD, ETriggerEvent::Triggered, myCharacter, &ACharacterBase::WASDClick);
		}
		if (IA_Rotate)
		{
			EnhancedInputComponent->BindAction(IA_Rotate, ETriggerEvent::Triggered, myCharacter, &ACharacterBase::Rotate);
		}
		if (IA_Jump)
		{
			EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, myCharacter, &ACharacterBase::CustomJump);
		}
		if (IA_Run)
		{
			// Run 시작: Shift 키가 눌렸을 때
			EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Started, myCharacter, &ACharacterBase::RunStart);

			// Run 중지: Shift 키에서 손을 뗄 때
			EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, myCharacter, &ACharacterBase::RunStop);
		}
		if (IA_Dash)
		{
			EnhancedInputComponent->BindAction(IA_Dash, ETriggerEvent::Triggered, myCharacter, &ACharacterBase::DashStart);
		}
		if (IA_Attack)
		{				
			EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Triggered, myCharacter, &ACharacterBase::WeaponAttack);
		}

		if (IA_SwordAttack)
		{
			EnhancedInputComponent->BindAction(IA_Attack, ETriggerEvent::Started, myCharacter, &ACharacterBase::SwordAttack);

		}

		if (IA_Zoom)
		{
			EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Started, myCharacter, &ACharacterBase::ZoomIn);
			EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Completed, myCharacter, &ACharacterBase::ZoomOut);
		}

		if (IA_ThrowGrenade)
		{
			EnhancedInputComponent->BindAction(IA_ThrowGrenade, ETriggerEvent::Triggered, myCharacter, &ACharacterBase::ThrowGrenade);
		}

		if (IA_1)
		{
			EnhancedInputComponent->BindAction(IA_1, ETriggerEvent::Completed, myCharacter, &ACharacterBase::ChangeWeapon);
		}

		if (IA_2)
		{
			EnhancedInputComponent->BindAction(IA_2, ETriggerEvent::Triggered, myCharacter, &ACharacterBase::ChangeWeapon);
		}

		if (IA_Inventory)
		{
			EnhancedInputComponent->BindAction(IA_Inventory, ETriggerEvent::Completed, myCharacter, &ACharacterBase::ToggleInventory);
		}
	}
}

