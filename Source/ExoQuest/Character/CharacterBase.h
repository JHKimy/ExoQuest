#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <InputAction.h> // ��ǲ �׼�

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

protected:
	// SpringArm
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	// Camera
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* tpsCamComp;


	// �Ϲ� ���� ũ�ν� ��� UI ����
	UPROPERTY(EditDefaultsOnly, Category = CrossHair)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	// ũ�ν���� �ν��Ͻ�
	class UUserWidget* crosshairUI;


public:
	// ȹ�� �����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EquippedWeapons)
	TArray<EWeaponType> EquippedWeapons;

	// �� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	EWeaponType PrimaryWeapon;

	// ������
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EquippedWeapons)
	// ARifle* Rifle;


public:

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


	// �� �߻�
	UFUNCTION(BlueprintCallable)
	void WeaponAttack();


	UPROPERTY(EditAnywhere, Category = Damage)
	class UEnemyFSM* enemyFSM;

};
