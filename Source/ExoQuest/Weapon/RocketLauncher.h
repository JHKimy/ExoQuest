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
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	//float damage =100.f;


	UFUNCTION(BlueprintCallable)
	void Fire();

	bool bCanFire = true; // �߻� ���� ����
	FTimerHandle FireRateTimerHandle; // Ÿ�̸� �ڵ�

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRateDelay = 0.5f; // �߻� ��� �ð� (�� ����)


	void ResetFire();

	void ShowProjectilePrediction();

public:
	UFUNCTION(BlueprintCallable, Category = "Recoil")
	void ApplyRecoil();

	UFUNCTION(BlueprintCallable, Category = "Recoil")
	void RecoverRecoil();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FTimerHandle RecoilRecoveryTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FRotator recoveryRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilVerticalMin{ 3.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilVerticalMax{ 5.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilHorizontalMin{ -0.55f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilHorizontalMax{ 0.55f };

	class APlayerController* PlayerController;


	// ī�޶� ����ũ ȿ�� �߰�
	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<class UCameraShakeBase> cameraShakeClass;
};
