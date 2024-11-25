#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <InputAction.h> // ��ǲ �׼�

#include "CharacterBase.generated.h"
//#include <string>

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


	//=====================================================================================
public:
	// class APlayerController* EQPlayerController;

public:
	// SpringArm
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;
	
	// Camera
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* tpsCamComp;

	// MiniMap
	UPROPERTY(VisibleAnywhere, Category = MiniMap)
	class USpringArmComponent* miniMapSpringArmComp;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = MiniMap)
	class USceneCaptureComponent2D* miniMapCam;

	// �̴ϸ� ȭ��ǥ
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = MiniMap)
	class UPaperSpriteComponent* characterPositionArrow;

	// �Ϲ� ���� ũ�ν� ��� UI ����
	UPROPERTY(EditDefaultsOnly, Category = CrossHair)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	// ũ�ν���� �ν��Ͻ�
	class UUserWidget* crosshairUI;

	//=======================================
	// STATS
	//=======================================

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = Stats)
	float maxHealth;

	UPROPERTY(EditDefaultsOnly,BluePrintReadWrite, Category = Stats)
	float health;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = Stats)
	FString healthString;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = Stats)
	float maxStamina;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = Stats)
	float stamina;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = Stats)
	FString staminaString;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = Stats)
	float staminaDrainRate; // �ʴ� ���ҷ�


	// ���¹̳� ���� �Լ� ȣ��
	UFUNCTION()
	void HandleStamina(float DeltaTime);

public:
	// ȹ�� �����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EquippedWeapons)
	TArray<EWeaponType> EquippedWeapons;

	// �� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	EWeaponType PrimaryWeapon;

	// ���� ���� �ÿ��� ȣ���Ͽ� ���� ����ȭ�� ���� ���� ���� ���� ���� 
	// �� Tick���� �ߺ� ������ �����ϰ�, ���Ⱑ ����� �� �� ���� ������Ʈ
	// ���� ���� Ÿ�� ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerWeapon)
	EWeaponType PreviousWeaponType = EWeaponType::None;


	// ������
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EquippedWeapons)
	// ARifle* Rifle;


public:


	// �ִϸ��̼�
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Animation)
	UClass* AnimBP;




	// ��� �޺� ���� ��Ÿ��
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Combo)
	class UAnimMontage* swordComboMontage;


	UFUNCTION()
	void SwordAttack();


	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Combo)
	int currentCombo;


	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Combo)
	FTimerHandle ComboResetTimerHandle;

	UFUNCTION()
	void ResetCombo() { currentCombo = 0; }
	
	

	// ĳ���� ����
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = CharacterState)
	ECharacterState EQCharacterState;


	// ȸ���� ���� �ʱ�ȭ, ���Ͽ� ���� ����
	UFUNCTION(BlueprintCallable)
	void CheckEquipWeapon();


	// ���⿡ ���� ĳ���� ���� ���� 
	UFUNCTION(BlueprintCallable)
	void ChangeState();


	// ���콺 ��Ŭ�� �̵�
	UFUNCTION(BlueprintCallable)
	void MouseClickMove();

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = mouseMoveMode)
	bool bmouseMoveMode;

	// WASD �Է� �Լ�
	UFUNCTION(BlueprintCallable)
	void WASDClick(const FInputActionValue& InputValue);

	// ���콺 ȸ��	
	UFUNCTION(BlueprintCallable)
	void Rotate(const FInputActionValue& InputValue);

	// ����
	UFUNCTION(BlueprintCallable)
	void CustomJump();

	// �޸���
	UFUNCTION(BlueprintCallable)
	void RunStart();
	UFUNCTION(BlueprintCallable)
	void RunStop();

	// �뽬
	UFUNCTION(BlueprintCallable)
	void DashStart();
	void StopDash();
	void ResetDash();

	// �뽬 �� Ÿ��
	bool bCanDash = true;
	FVector beforeDashVelocity;
	FTimerHandle DashTimer;
	// �뽬 ��Ÿ��
	float dashCoolTime{ 1.f };

	// �뽬 ������ ���¸� Ȯ���ϴ� ����
	bool bIsDashing = false;
	
	// �޸��� ������ ���¸� Ȯ���ϴ� ����
	bool bIsRunning = false;



	// �� �߻�
	UFUNCTION(BlueprintCallable)
	void WeaponAttack();

	// ���� ��
	UFUNCTION(BlueprintCallable)
	void ZoomIn();
	UFUNCTION(BlueprintCallable)
	void ZoomOut();


	UPROPERTY(EditAnywhere, Category = Damage)
	class UEnemyFSM* enemyFSM;

	UPROPERTY(EditAnywhere, Category = Damage)
	class AEnemyBase* enemy;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Weapon)
	class ARifle* playerRifle;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Weapon)
	class AShotgun* playerShotgun;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Weapon)
	class ARocketLauncher* playerRocketLauncher;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Weapon)
	class ASword* playerSword;

	// �� �̵� �� �ν��Ͻ�
	UFUNCTION()
	void SaveStateBeforeLevelChange();

	// �ν��Ͻ� �ٽ� ������
	UFUNCTION()
	void RestoreStateAfterLevelChange();
};
