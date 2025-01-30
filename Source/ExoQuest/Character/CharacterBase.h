#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <InputAction.h> // 인풋 액션

#include "Kismet/KismetSystemLibrary.h"

#include "Item/ItemDataBase.h" // 정확한 경로로 수정


#include "CharacterBase.generated.h"


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

// 수류탄 타입
UENUM(BlueprintType)
enum class EGrenadeType : uint8
{
	BasicGrenade,
	SolidGrenade,
	LandMine
};


// 위치 기록을 위한 구조체
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
// 카메라 관련
//=====================================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	class UCameraComponent* tpsCamComp;

//=====================================================================================
// 미니맵
//=====================================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	class USpringArmComponent* miniMapSpringArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	class USceneCaptureComponent2D* miniMapCam;
	// 미니맵 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
	class UPaperSpriteComponent* characterPositionArrow;
	
//=====================================================================================
// 애니메이션
//=====================================================================================

	// 애니메이션
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Animation")
	UClass* AnimBP;

	// 검 콤보 공격 몽타주
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Animation")
	class UAnimMontage* swordComboMontage;

	// 대검 콤보 공격 몽타주
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Animation")
	class UAnimMontage* ThrowGrenadeMontage;

//=====================================================================================
// 이동 & 입력 관련
//=====================================================================================

	// 입력 제한 함수
	void SetInputRestrictions(bool bRestrict);

	// 마우스 우클릭 이동
	UFUNCTION(BlueprintCallable)
	void MouseClickMove();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bmouseMoveMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bMouseCursorHidden = false; // 마우스 커서 숨김 상태를 추적


	// WASD 입력 함수
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void WASDClick(const FInputActionValue& InputValue);

	// 마우스 회전	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void Rotate(const FInputActionValue& InputValue);

	// 점프
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void CustomJump();

	// 달리기
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RunStart();
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RunStop();
	// 달리기 중인지 상태를 확인하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsRunning = false;


	// 대쉬
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void DashStart();
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopDash();
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ResetDash();
	
	// 대쉬 쿨 타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCanDash = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector beforeDashVelocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FTimerHandle DashTimer;
	
	// 대쉬 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float dashCoolTime{ 1.f };
	
	// 대쉬 중인지 상태를 확인하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsDashing = false;

	// 조준 줌
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ZoomIn();
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ZoomOut();


//=====================================================================================
// 스킬
//=====================================================================================

	// 고스트 모드 활성화
	UFUNCTION(BlueprintCallable,				Category = "SkillGhost")
	void ActivateGhostMode(float Duration);

	// 고스트 모드 비활성화
	UFUNCTION(BlueprintCallable,				Category = "SkillGhost")
	void DeactivateGhostMode();

	// 콤보 리셋 타이머
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillGhost")
	FTimerHandle GhostTimerHandle;

	UFUNCTION(BlueprintCallable,				Category = "SkillGhost")
	void SetCharacterTransparency(float transparency);






	UFUNCTION(BlueprintCallable, Category = "SkillTime")
	void ActivateTimeRewind(float duration);

	UFUNCTION(BlueprintCallable, Category = "SkillTime")
	void RewindStep();

	// 시간 되돌리기 관련 데이터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTime")
	TArray<FTimeRecord> TimeRecords; // 위치 기록 배열
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTime")
	FTimerHandle RewindTimerHandle;  // 시간 되돌리기 타이머
	//
	//float LastSaveTime = 0.0f; // 마지막으로 저장한 시간
	//int CurrentTargetIndex = -1;
	//FVector PreviousPosition;
	//FVector NextPosition;
	//float LerpAlpha = 0.0f;
	float LastSaveTime = 0.f;
	float SaveInterval = 0.1f;
	float MaxRecordTime = 5.f;
	// 되돌리기 시간
	float RewindDuration = 1.f;
	float RewindElapsedTime = 0.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillTime")
	FTimerHandle LocationRecordTimerHandle;



//=====================================================================================
// UI & 아이템
//=====================================================================================

	// 조준 크로스 헤어 UI 위젯
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "CrossHair")
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	// 크로스헤어 인스턴스
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "CrossHair")
	class UUserWidget* crosshairUI;

	// 인벤토리 UI 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<class UInventoryUI> InventoryUIClass;

	// 인벤토리 인스턴스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventoryUI* InventoryUI;

	// 인벤토리 열림 상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	bool bIsInventoryOpen = false;


	// 입력 함수
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ToggleInventory();

	// 아이템 데이터베이스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<class UItemDataBase> ItemDataBase;

	// 얻은 자원 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int StarfluxNum{};

//=====================================================================================
// 상태 & 스텟
//=====================================================================================

	// 캐릭터 상태
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "CharacterState")
	ECharacterState EQCharacterState;

	// 무기에 따른 캐릭터 상태 변경 
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
	float staminaDrainRate; // 초당 감소량


	// 스태미나 관리 함수 호출
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void HandleStamina(float DeltaTime);

//=====================================================================================
// 무기 & 공격
//=====================================================================================

	// 소켓
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

	// 회득한 무기 초기화, 소켓에 무기 부착
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void CheckEquipWeapon();

	// 특정 소켓에 부착된 액터를 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	AActor* GetAttachedActorAtSocket(FName SocketName);





	// 무기 객체
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

	// 획득 무기들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<EWeaponType> EquippedWeapons;

	// 주 무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType PrimaryWeapon;

	// 무기 변경 시에만 호출하여 성능 최적화를 위한 이전 무기 상태 저장 
	// 매 Tick마다 중복 연산을 방지하고, 무기가 변경될 때 한 번만 업데이트
	// 이전 무기 타입 추적용 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType PreviousWeaponType = EWeaponType::None;





	// 무기 바꾸기
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ChangeWeapon();

	// 무기 버리기
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DropWeapon();

	// 총 발사
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void WeaponAttack();





// 검 공격
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SwordAttack();

	// 콤보 계산 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	int currentCombo;

	// 콤보 리셋 타이머
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FTimerHandle ComboResetTimerHandle;

	// 콤보 리셋
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ResetCombo() { currentCombo = 0; }
	




// 수류탄
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ThrowGrenade();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FTimerHandle GrenadeCooldownTimer; // 수류탄 쿨타임 타이머

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bCanThrowGrenade = true;     // 수류탄 던질 수 있는 상태

	// 쿨타임 해제 함수
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ResetGrenadeCooldown();

	// 수류탄 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FVector GrenadeLaunchVelocity;

	// 수류탄 파워
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float GrenadeLaunchPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	class UArrowComponent* GrenadeLaunchPosition;
	
//=====================================================================================
// 적 관련
//=====================================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	class UEnemyFSM* enemyFSM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	class AEnemyBase* enemy;


//=====================================================================================
// 맵 이동
//=====================================================================================
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	bool isFirstSpawn = true;

	// 맵 이동 시 인스턴스
	UFUNCTION(BlueprintCallable, Category = "Map")
	void SaveStateBeforeLevelChange();

	// 인스턴스 다시 입히기
	UFUNCTION(BlueprintCallable, Category = "Map")
	void RestoreStateAfterLevelChange();

	UFUNCTION(BlueprintCallable, Category = "Map")
	void TeleportRoom();

//=====================================================================================
// 디버그 용
//=====================================================================================
	
	// 획득 무기 출력
	void PrintEquippedWeapons()
	{
		// 현재 장착된 무기가 없으면 안내 메시지 출력
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
		
			
		// 무기 목록을 출력하기 위한 문자열 생성
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
	
		// 화면에 출력
		UKismetSystemLibrary::PrintString(this, WeaponsList, true, true, FColor::Green, 5.0f);
	}

	// 인벤토리를 화면에 출력하는 함수
	void PrintInventory()
	{
		if (!ItemDataBase) return; // 데이터베이스가 없으면 함수 종료

		FString InventoryString = "get ITEM : :\n";

		// 아이템 데이터베이스를 순회하며 정보 추가
		for (const FItem& Item : ItemDataBase->Items)
		{
			InventoryString += FString::Printf(TEXT("%s: %d\n"), *Item.Name, Item.Num);
		}

		// 화면에 출력
		UKismetSystemLibrary::PrintString(this, InventoryString, true, true, FColor::Green, 5.0f);
	}


	void PrintCharacterState()
	{
		FString CharacterStateString;

		// 현재 상태를 문자열로 변환
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

		// 상태를 화면에 출력
		UKismetSystemLibrary::PrintString(this, CharacterStateString, true, true, FColor::Blue, 5.0f);
	}

	//// 애니메이션 몽타주 노티파이
	//UFUNCTION()
	//void ThrowEnd();
	//// Grenade Launch Position
	//UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Grenade)
	//class UChildActorComponent* grenadePos;
	//// 던질 수류탄 클래스
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grenade)
	//TSubclassOf<class ABasicGrenade> GrenadeClass; // Spawn Grenade
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	//bool bFullWepaonEquipped;
};