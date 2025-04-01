#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.h"
#include "Enemy1.generated.h"

UCLASS()
class EXOQUEST_API AEnemy1 : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemy1();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


};
