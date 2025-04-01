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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	// EnemyBase.h
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPBarWidget;




	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	bool bIsAlive = true;
	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float maxHealth = 100.f;

public:
	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float health = 100.f;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float attackPower = 10.f;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float healthBarRange = 500.f;


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

	virtual void Attack(); // FSM이 호출할 함수


public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealthBar();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void MovementHealthBar();


};