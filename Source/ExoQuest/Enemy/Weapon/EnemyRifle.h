// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyRifle.generated.h"

UCLASS()
class EXOQUEST_API AEnemyRifle : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemyRifle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    TObjectPtr<class USkeletalMeshComponent> rifleMesh;


    FTransform GetLeftHandSocketTransform() const;

    // Enemy 참조
    UPROPERTY(BlueprintReadWrite, Category = "Owner")
    TObjectPtr<class AEnemy2> OwnerEnemy;

    // 발사 위치
    UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Weapon")
    TObjectPtr<USceneComponent> MuzzleLocation;

    // 사정거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float TraceDistance = 5000.f;

    // 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float Damage = 10.f;

    // 디버그용
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bDrawDebugLine = true;


    // 총 발사
    UFUNCTION(BlueprintCallable)
    void Fire();

};
