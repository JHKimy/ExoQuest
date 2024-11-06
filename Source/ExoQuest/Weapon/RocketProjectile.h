#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketProjectile.generated.h"

UCLASS()
class EXOQUEST_API ARocketProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ARocketProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// �浹 ó�� �� ���� ȿ��
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// �ӵ�, ������ �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float maxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float gravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float damage;

public:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* meshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* collisionComponent;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* movementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* explosionEffect;


	FTimerHandle destroyTimerHandle; // Ÿ�̸� �ڵ�

};
