// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostGear.generated.h"

UCLASS()
class EXOQUEST_API AGhostGear : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGhostGear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



    // ======================================
   // 컴포넌트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* meshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    class UCapsuleComponent* capsuleCollision;

    // ======================================
    // 애니메이션 관련 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float RiseSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float RiseHeight = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float MoveToCharacterSpeed = 5.0f;

    // ======================================
    // 아이템 관련 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
    int32 ItemNum;

    // ======================================
    // 캐릭터 관련 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
    bool IsCharacterNearby = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
    class ACharacterBase* character;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
    float CheckInterval = 1.0f;

    // ======================================
    // 타이머 및 위치 변수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", meta = (AllowPrivateAccess = "true"))
    FVector InitialLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", meta = (AllowPrivateAccess = "true"))
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", meta = (AllowPrivateAccess = "true"))
    bool HasReachedTarget = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (AllowPrivateAccess = "true"))
    FTimerHandle DistanceCheckTimerHandle;

    // ======================================
    UFUNCTION()
    void CheckForNearbyCharacters();

    UFUNCTION()
    void AbsorbToCharacter(float DeltaTime);

    UFUNCTION()
    void OnBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFormSweep,
        const FHitResult& SweepResult);

};
