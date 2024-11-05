#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketLauncher.generated.h"

UCLASS()
class EXOQUEST_API ARocketLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	ARocketLauncher();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;





	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ARocketProjectile> RocketProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FVector MuzzleOffset;





	// �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = MeshComp)
	class UStaticMeshComponent* meshComp;

	// �ѱ� ��ġ�� ��Ÿ���� Scene Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Muzzle")
	class USceneComponent* muzzleLocation;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	class ACharacterBase* ownerCharacter;

	// �Ѿ� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	// ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	float damage =100.f;


	UFUNCTION(BlueprintCallable)
	void Fire();

	bool bCanFire = true; // �߻� ���� ����
	FTimerHandle FireRateTimerHandle; // Ÿ�̸� �ڵ�

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRateDelay = 2.0f; // �߻� ��� �ð� (�� ����)

	void ResetFire(); // �߻� ���� ���·� �����ϴ� �Լ�

};
