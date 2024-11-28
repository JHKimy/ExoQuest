// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicGrenade.generated.h"

UCLASS()
class EXOQUEST_API ABasicGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* meshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* collisionComponent;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* movementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* explosionEffect;


	// �浹 ó�� �� ���� ȿ��
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// �޽� ���� �Լ�
	UFUNCTION()
	UStaticMeshComponent* GetMesh() const { return meshComponent; }

	// ���� ��� �Լ�
	UFUNCTION()
	void PredictGrenadePath(FVector LaunchVelocity, float MaxSimTime = 5.f);





	// ����ź�� ������ �ݰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = "true"))
	float ProjectileRadius = 10.0f;

	// �߷� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = "true"))
	float GravityZ = -9.8f;

	// �ùķ��̼� �ֱ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = "true"))
	float SimFrequency = 15.0f;

	// ���� ��� �浹 ä��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grenade", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

//protected:
//	FVector LaunchVelocity;
};
