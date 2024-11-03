#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponForEquip.generated.h"

UENUM(BlueprintType)
enum class EWeaponTypeForEquip : uint8
{
	Rifle,
	Shotgun,
	RocketLauncher,
	Sword
};

UCLASS()
class EXOQUEST_API AWeaponForEquip : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponForEquip();



protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Motion)
	FVector StartPos;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Motion)
	float Amplitude;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Motion)
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = WeaponType)
	EWeaponTypeForEquip WeaponType;




	// 무기 담아줄 포인터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<class UStaticMeshComponent> Weapon;

	// 충돌체 
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "BoxCollision")
	class UBoxComponent* BoxComponent;

	// 메쉬
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = StaticMesh)
	class UStaticMeshComponent* MeshComponent;



	// 무기 아이템 움직임
	void Floating();


	// 충돌 함수
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFormSweep,
		const FHitResult& SweepResult);

};
