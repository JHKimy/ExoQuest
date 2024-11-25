#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <InputAction.h> // 인풋 액션

#include "CharacterBase.generated.h"
//#include <string>

//class ARifle;
//class Shotgun;
//class RocketLauncher;

// 무기 타입
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	Rifle,
	Shotgun,
	RocketLauncher,
	Sword
};


// 캐릭터 상태
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

	// 미니맵 화살표
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = MiniMap)
	class UPaperSpriteComponent* characterPositionArrow;

	// 일반 조준 크로스 헤어 UI 위젯
	UPROPERTY(EditDefaultsOnly, Category = CrossHair)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	// 크로스헤어 인스턴스
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
	float staminaDrainRate; // 초당 감소량


	// 스태미나 관리 함수 호출
	UFUNCTION()
	void HandleStamina(float DeltaTime);

public:
	// 획득 무기들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EquippedWeapons)
	TArray<EWeaponType> EquippedWeapons;

	// 주 무기
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	EWeaponType PrimaryWeapon;

	// 무기 변경 시에만 호출하여 성능 최적화를 위한 이전 무기 상태 저장 
	// 매 Tick마다 중복 연산을 방지하고, 무기가 변경될 때 한 번만 업데이트
	// 이전 무기 타입 추적용 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerWeapon)
	EWeaponType PreviousWeaponType = EWeaponType::None;


	// 라이플
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EquippedWeapons)
	// ARifle* Rifle;


public:


	// 애니메이션
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Animation)
	UClass* AnimBP;




	// 대검 콤보 공격 몽타주
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
	
	

	// 캐릭터 상태
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = CharacterState)
	ECharacterState EQCharacterState;


	// 회득한 무기 초기화, 소켓에 무기 부착
	UFUNCTION(BlueprintCallable)
	void CheckEquipWeapon();


	// 무기에 따른 캐릭터 상태 변경 
	UFUNCTION(BlueprintCallable)
	void ChangeState();


	// 마우스 우클릭 이동
	UFUNCTION(BlueprintCallable)
	void MouseClickMove();

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = mouseMoveMode)
	bool bmouseMoveMode;

	// WASD 입력 함수
	UFUNCTION(BlueprintCallable)
	void WASDClick(const FInputActionValue& InputValue);

	// 마우스 회전	
	UFUNCTION(BlueprintCallable)
	void Rotate(const FInputActionValue& InputValue);

	// 점프
	UFUNCTION(BlueprintCallable)
	void CustomJump();

	// 달리기
	UFUNCTION(BlueprintCallable)
	void RunStart();
	UFUNCTION(BlueprintCallable)
	void RunStop();

	// 대쉬
	UFUNCTION(BlueprintCallable)
	void DashStart();
	void StopDash();
	void ResetDash();

	// 대쉬 쿨 타임
	bool bCanDash = true;
	FVector beforeDashVelocity;
	FTimerHandle DashTimer;
	// 대쉬 쿨타임
	float dashCoolTime{ 1.f };

	// 대쉬 중인지 상태를 확인하는 변수
	bool bIsDashing = false;
	
	// 달리기 중인지 상태를 확인하는 변수
	bool bIsRunning = false;



	// 총 발사
	UFUNCTION(BlueprintCallable)
	void WeaponAttack();

	// 조준 줌
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

	// 맵 이동 시 인스턴스
	UFUNCTION()
	void SaveStateBeforeLevelChange();

	// 인스턴스 다시 입히기
	UFUNCTION()
	void RestoreStateAfterLevelChange();
};
