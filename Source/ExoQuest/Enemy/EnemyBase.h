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

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 미니맵 표시 아이콘
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = MiniMap)
	class UPaperSpriteComponent* enemyPosition;

	// 애니메이션
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Animation)
	UClass* AnimBP;



public:
	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float health;

	// 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float attackPower;

	// AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FSM")
	class UEnemyFSM* fsm;


	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual float TakeDamage
	(float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser) override;

};