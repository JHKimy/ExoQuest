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

	// �̴ϸ� ǥ�� ������
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = MiniMap)
	class UPaperSpriteComponent* enemyPosition;

	// �ִϸ��̼�
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Animation)
	UClass* AnimBP;



public:
	// ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float health;

	// ���ݷ�
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