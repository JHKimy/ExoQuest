#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shotgun.generated.h"

UCLASS()
class EXOQUEST_API AShotgun : public AActor
{
	GENERATED_BODY()
	
public:	
	AShotgun();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
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
	float damage = 5;

	UFUNCTION(BlueprintCallable)
	void Fire();


private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	int32 Pellets = 10;  // ������ �Ѿ� ����

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float SpreadAngle = 10.f;  // �Ѿ��� ������ ����

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float FireRange = 1000.0f;  // �Ѿ��� ������ �Ÿ�





	bool bCanFire = true; // �߻� ���� ����
	FTimerHandle FireRateTimerHandle; // Ÿ�̸� �ڵ�

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRateDelay = 2.0f; // �߻� ��� �ð� (�� ����)

	void ResetFire(); // �߻� ���� ���·� �����ϴ� �Լ�

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
	float recoilHorizontalMin{ -0.3f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float recoilHorizontalMax{ 0.3f };

	class APlayerController* PlayerController;


	// ī�޶� ����ũ ȿ�� �߰�
	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<class UCameraShakeBase> cameraShakeClass;
};
