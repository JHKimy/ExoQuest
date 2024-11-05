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

	// 충돌 처리 및 폭발 효과
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// 속도, 데미지 등 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float Damage;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* meshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* collisionComponent;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* movementComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* explosionEffect;

};
