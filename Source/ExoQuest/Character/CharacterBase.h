#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <InputAction.h> // ��ǲ �׼�

#include "Kismet/KismetSystemLibrary.h"

#include "Item/ItemDataBase.h" // ��Ȯ�� ��η� ����


#include "CharacterBase.generated.h"


//class ARifle;
//class Shotgun;
//class RocketLauncher;

// ���� Ÿ��
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	Rifle,
	Shotgun,
	RocketLauncher,
	Sword
};

// ĳ���� ����
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	NoWeaponMode,
	RifleMode,
	ShotgunMode,
	RocketLauncherMode,
	SwordMode
};

// ����ź Ÿ��
UENUM(BlueprintType)
enum class EGrenadeType : uint8
{
	BasicGrenade,
	SolidGrenade,
	LandMine
};


// ��ġ ����� ���� ����ü
USTRUCT(BlueprintType)
struct FTimeRecord
{
	GENERATED_BODY()

	FVector Location;
	FRotator Rotation;
	float Timestamp;

	FTimeRecord() 
		: Location(FVector::ZeroVector), 
		Rotation(FRotator::ZeroRotator), 
		Timestamp(0.0f) {}

	FTimeRecord
	(const FVector& InLocation, 
		const FRotator& InRotation, 
		float InTimestamp)
		: Location(InLocation), 
		Rotation(InRotation), 
		Timestamp(InTimestamp) {}
};

class UItemDataBase;



UCLASS()
class EXOQUEST_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:
//=====================================================================================
// ī�޶� ����
//=====================================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* tpsCamComp;

//=====================================================================================
// �̴ϸ�
//=====================================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	class USpringArmComponent* miniMapSpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	class USceneCaptureComponent2D* miniMapCam;
	// �̴ϸ� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	class UPaperSpriteComponent* characterPositionArrow;
	
//=====================================================================================
// �ִϸ��̼�
//=====================================================================================

	// �ִϸ��̼�
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Animation")
	UClass* AnimBP;

	// �� �޺� ���� ��Ÿ��
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Animation")
	class UAnimMontage* swordComboMontage;

	// ��� �޺� ���� ��Ÿ��
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Animation")
	class UAnimMontage* ThrowGrenadeMontage;

//=====================================================================================
// �̵� & �Է� ����
//=====================================================================================

	// �Է� ���� �Լ�
	void SetInputRestrictions(bool bRestrict);

	// ���콺 ��Ŭ�� �̵�
	UFUNCTION(BlueprintCallable)
	void MouseClickMove();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bmouseMoveMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bMouseCursorHidden = false; // ���콺 Ŀ�� ���� ���¸� ����


	// WASD �Է� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void WASDClick(const FInputActionValue& InputValue);

	// ���콺 ȸ��	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Rotate(const FInputActionValue& InputValue);

	// ����
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void CustomJump();

	// �޸���
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RunStart();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RunStop();
	// �޸��� ������ ���¸� Ȯ���ϴ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsRunning = false;


	// �뽬
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void DashStart();
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopDash();
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetDash();
	
	// �뽬 �� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCanDash = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector beforeDashVelocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FTimerHandle DashTimer;
	
	// �뽬 ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float dashCoolTime{ 1.f };
	
	// �뽬 ������ ���¸� Ȯ���ϴ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsDashing = false;

	// ���� ��
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ZoomIn();
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ZoomOut();


//=====================================================================================
// ��ų
//=====================================================================================

	// ��Ʈ ��� Ȱ��ȭ
	UFUNCTION(BlueprintCallable,				Category = "SkillGhost")
	void ActivateGhostMode(float Duration);

	// ��Ʈ ��� ��Ȱ��ȭ
	UFUNCTION(BlueprintCallable,				Category = "SkillGhost")
	void DeactivateGhostMode();

	// �޺� ���� Ÿ�̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillGhost")
	FTimerHandle GhostTimerHandle;

	UFUNCTION(BlueprintCallable,				Category = "SkillGhost")
	void SetCharacterTransparency(float transparency);






	UFUNCTION(BlueprintCallable, Category = "SkillTime")
	void ActivateTimeRewind(float duration);

	UFUNCTION(BlueprintCallable, Category = "SkillTime")
	void RewindStep();

	// �ð� �ǵ����� ���� ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTime")
	TArray<FTimeRecord> TimeRecords; // ��ġ ��� �迭
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTime")
	FTimerHandle RewindTimerHandle;  // �ð� �ǵ����� Ÿ�̸�
	//
	//float LastSaveTime = 0.0f; // ���������� ������ �ð�
	//int CurrentTargetIndex = -1;
	//FVector PreviousPosition;
	//FVector NextPosition;
	//float LerpAlpha = 0.0f;
	float LastSaveTime = 0.f;
	float SaveInterval = 0.1f;
	float MaxRecordTime = 5.f;
	// �ǵ����� �ð�
	float RewindDuration = 1.f;
	float RewindElapsedTime = 0.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTime")
	FTimerHandle LocationRecordTimerHandle;



//=====================================================================================
// UI & ������
//=====================================================================================

	// ���� ũ�ν� ��� UI ����
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "CrossHair")
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	// ũ�ν���� �ν��Ͻ�
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "CrossHair")
	class UUserWidget* crosshairUI;

	// �κ��丮 UI Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<class UInventoryUI> InventoryUIClass;

	// �κ��丮 �ν��Ͻ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventoryUI* InventoryUI;

	// �κ��丮 ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bIsInventoryOpen = false;


	// �Է� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	// ������ �����ͺ��̽�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<class UItemDataBase> ItemDataBase;

	// ���� �ڿ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int StarfluxNum{};

//=====================================================================================
// ���� & ����
//=====================================================================================

	// ĳ���� ����
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "CharacterState")
	ECharacterState EQCharacterState;

	// ���⿡ ���� ĳ���� ���� ���� 
	UFUNCTION(BlueprintCallable, Category = "CharacterState")
	void ChangeState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FString healthString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float maxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	FString staminaString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float staminaDrainRate; // �ʴ� ���ҷ�


	// ���¹̳� ���� �Լ� ȣ��
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void HandleStamina(float DeltaTime);

//=====================================================================================
// ���� & ����
//=====================================================================================

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName RifleSocket			= FName(TEXT("Rifle"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName ShotgunSocket			= FName(TEXT("Shotgun"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName RocketLauncherSocket	= FName(TEXT("RocketLauncher"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName SwordSocket			= FName(TEXT("Sword"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName LeftHandSocket		= FName(TEXT("WeaponLeft"));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName LeftSwordSocket		= FName(TEXT("SwordLeft"));

	// ȸ���� ���� �ʱ�ȭ, ���Ͽ� ���� ����
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void CheckEquipWeapon();

	// Ư�� ���Ͽ� ������ ���͸� �������� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AActor* GetAttachedActorAtSocket(FName SocketName);





	// ���� ��ü
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ARifle> rifleBlueprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class ARifle* playerRifle;
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AShotgun> shotgunBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AShotgun* playerShotgun;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ARocketLauncher> rocketLauncherBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class ARocketLauncher* playerRocketLauncher;
	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf <class ASword> swordBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class ASword* playerSword;






	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class ABasicGrenade* playerBasicGrenade;

	// ȹ�� �����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<EWeaponType> EquippedWeapons;

	// �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType PrimaryWeapon;

	// ���� ���� �ÿ��� ȣ���Ͽ� ���� ����ȭ�� ���� ���� ���� ���� ���� 
	// �� Tick���� �ߺ� ������ �����ϰ�, ���Ⱑ ����� �� �� ���� ������Ʈ
	// ���� ���� Ÿ�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType PreviousWeaponType = EWeaponType::None;





	// ���� �ٲٱ�
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ChangeWeapon();

	// ���� ������
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DropWeapon();

	// �� �߻�
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void WeaponAttack();





// �� ����
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SwordAttack();

	// �޺� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int currentCombo;

	// �޺� ���� Ÿ�̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FTimerHandle ComboResetTimerHandle;

	// �޺� ����
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ResetCombo() { currentCombo = 0; }
	




// ����ź
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ThrowGrenade();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FTimerHandle GrenadeCooldownTimer; // ����ź ��Ÿ�� Ÿ�̸�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bCanThrowGrenade = true;     // ����ź ���� �� �ִ� ����

	// ��Ÿ�� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ResetGrenadeCooldown();

	// ����ź �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FVector GrenadeLaunchVelocity;

	// ����ź �Ŀ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float GrenadeLaunchPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	class UArrowComponent* GrenadeLaunchPosition;
	
//=====================================================================================
// �� ����
//=====================================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	class UEnemyFSM* enemyFSM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	class AEnemyBase* enemy;


//=====================================================================================
// �� �̵�
//=====================================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	bool isFirstSpawn = true;

	// �� �̵� �� �ν��Ͻ�
	UFUNCTION(BlueprintCallable, Category = "Map")
	void SaveStateBeforeLevelChange();

	// �ν��Ͻ� �ٽ� ������
	UFUNCTION(BlueprintCallable, Category = "Map")
	void RestoreStateAfterLevelChange();

	UFUNCTION(BlueprintCallable, Category = "Map")
	void TeleportRoom();

//=====================================================================================
// ����� ��
//=====================================================================================
	
	// ȹ�� ���� ���
	void PrintEquippedWeapons()
	{
		// ���� ������ ���Ⱑ ������ �ȳ� �޽��� ���
		if (EquippedWeapons.IsEmpty())
		{
			UKismetSystemLibrary::PrintString(this, TEXT("No weapons equipped."), true, true, FColor::Red, 5.0f);
			return;
		}

		switch (PrimaryWeapon)
		{
		case EWeaponType::Rifle:
			UKismetSystemLibrary::PrintString(this, "P : Rifle", true, true, FColor::Blue, 5.0f);
			break;
		case EWeaponType::Shotgun:
			UKismetSystemLibrary::PrintString(this, "P : Shotgun", true, true, FColor::Blue, 5.0f);
			break;
		case EWeaponType::RocketLauncher:
			UKismetSystemLibrary::PrintString(this, "P : RocketLauncher", true, true, FColor::Blue, 5.0f);
			break;
		case EWeaponType::Sword:
			UKismetSystemLibrary::PrintString(this, "P : Sword", true, true, FColor::Blue, 5.0f);
			break;
		}
		
			
		// ���� ����� ����ϱ� ���� ���ڿ� ����
		FString WeaponsList = TEXT("Equipped Weapons: ");
	
		for (EWeaponType WeaponType : EquippedWeapons)
		{
			switch (WeaponType)
			{
			case EWeaponType::Rifle:
				WeaponsList += TEXT("Rifle ");
				break;
			case EWeaponType::Shotgun:
				WeaponsList += TEXT("Shotgun ");
				break;
			case EWeaponType::RocketLauncher:
				WeaponsList += TEXT("RocketLauncher ");
				break;
			case EWeaponType::Sword:
				WeaponsList += TEXT("Sword ");
				break;
			default:
				WeaponsList += TEXT("Unknown ");
				break;
			}
		}
	
		// ȭ�鿡 ���
		UKismetSystemLibrary::PrintString(this, WeaponsList, true, true, FColor::Green, 5.0f);
	}

	// �κ��丮�� ȭ�鿡 ����ϴ� �Լ�
	void PrintInventory()
	{
		if (!ItemDataBase) return; // �����ͺ��̽��� ������ �Լ� ����

		FString InventoryString = "get ITEM : :\n";

		// ������ �����ͺ��̽��� ��ȸ�ϸ� ���� �߰�
		for (const FItem& Item : ItemDataBase->Items)
		{
			InventoryString += FString::Printf(TEXT("%s: %d\n"), *Item.Name, Item.Num);
		}

		// ȭ�鿡 ���
		UKismetSystemLibrary::PrintString(this, InventoryString, true, true, FColor::Green, 5.0f);
	}


	void PrintCharacterState()
	{
		FString CharacterStateString;

		// ���� ���¸� ���ڿ��� ��ȯ
		switch (EQCharacterState)
		{
		case ECharacterState::NoWeaponMode:
			CharacterStateString = TEXT("No Weapon Mode");
			break;
		case ECharacterState::RifleMode:
			CharacterStateString = TEXT("Rifle Mode");
			break;
		case ECharacterState::ShotgunMode:
			CharacterStateString = TEXT("Shotgun Mode");
			break;
		case ECharacterState::RocketLauncherMode:
			CharacterStateString = TEXT("Rocket Launcher Mode");
			break;
		case ECharacterState::SwordMode:
			CharacterStateString = TEXT("Sword Mode");
			break;
		default:
			CharacterStateString = TEXT("Unknown State");
			break;
		}

		// ���¸� ȭ�鿡 ���
		UKismetSystemLibrary::PrintString(this, CharacterStateString, true, true, FColor::Blue, 5.0f);
	}

	//// �ִϸ��̼� ��Ÿ�� ��Ƽ����
	//UFUNCTION()
	//void ThrowEnd();
	//// Grenade Launch Position
	//UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Grenade)
	//class UChildActorComponent* grenadePos;
	//// ���� ����ź Ŭ����
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grenade)
	//TSubclassOf<class ABasicGrenade> GrenadeClass; // Spawn Grenade
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//bool bFullWepaonEquipped;
};