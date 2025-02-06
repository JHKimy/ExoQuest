#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SolidGrenade.generated.h"

UCLASS()
class EXOQUEST_API ASolidGrenade : public AActor
{
	GENERATED_BODY()

public:
	ASolidGrenade();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* meshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USphereComponent* collisionComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* explosionEffect;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	class UProjectileMovementComponent* movementComponent;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float maxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float gravity;

	// 충돌 처리 및 폭발 효과
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Explosion")
	TSubclassOf<AActor> barrierClass;
};
