#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class EXOQUEST_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//======================================================
	// 컴포넌트
	//======================================================
protected:
	// 미니맵에 표시되는 아이콘
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MiniMap")
	TObjectPtr<class UPaperSpriteComponent> EnemyPosition = nullptr;

	// 애니메이션
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimBPClass;

	//FSM 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FSM")
	TObjectPtr<UActorComponent> FSM = nullptr;

	// HP Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	class UWidgetComponent* HPBarWidget;
	// HP Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "UI")
	bool bisHPBarShow;
	// 플레이어 타겟
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSM)
	class ACharacterBase* target;

	//// 애니메이션
	//UFUNCTION()
	//virtual void SetAnimState(EEnemyState NewState);
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsAlive = true;
	//// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float maxHealth = 100.f;
	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float health = 100.f;

	// 아래로 사라짐
	UPROPERTY(EditAnywhere, Category = FSM)
	float dieSpeed = 50.0f;

	UPROPERTY(EditAnywhere, Category = Animation)
	bool bSpawnStarflux = false;

	//// 공격력
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	//float attackPower = 10.f;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float healthBarRange = 1000.f;


public:
	// Blueprint에서만 읽기 가능, C++에선 바로 접근 가능
	UFUNCTION(BlueprintCallable, Category = "Stats")
	FORCEINLINE bool IsAlive() const { return bIsAlive; }



// 함수
protected:
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float TakeDamage
	(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;




public:
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void Death();

public:
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual void MoveToTarget();

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void UpdateHealthBar();

	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void MovementHealthBar();


};