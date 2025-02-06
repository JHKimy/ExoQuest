// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Grenade/Barrier.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABarrier::ABarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Weapon/Grenade/SolidGrenade/shield/SM_Barrier.SM_Barrier'"));
	if (tempMesh.Succeeded())
	{
		meshComponent->SetStaticMesh(tempMesh.Object);
	}
	meshComponent->SetRelativeScale3D(FVector(70.f, 30.f, 70.f));
	meshComponent->AddRelativeRotation(FRotator(0.f, 90.f, 0.f));
	RootComponent = meshComponent;
	



	// 충돌체 생성 및 설정
	collisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	collisionComponent->SetRelativeScale3D(FVector(0.060041f, 0.023242f, 0.039915f));
	
	collisionComponent->SetupAttachment(RootComponent);
	////collisionComponent->SetCollisionProfileName(TEXT("Projectile"));

	//collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
	//collisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

	//collisionComponent->SetLinearDamping(0.5f);  // 선형 감속 증가 (마찰력 비슷한 효과)
	//collisionComponent->SetAngularDamping(1.0f); // 회전 감속 증가 (바운스 시 회전 멈춤)
	////collisionComponent->SetSimulatePhysics(true);

	//collisionComponent->SetNotifyRigidBodyCollision(true);
	//collisionComponent->SetGenerateOverlapEvents(true);
	////collisionComponent->BodyInstance.SetMassOverride(0.f);

	//RootComponent = collisionComponent;

	//// 충돌 이벤트 연결
	////collisionComponent->OnComponentHit.AddDynamic(this, &ABasicGrenade::OnHit);

	////meshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
	////meshComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 비활성화

}

// Called when the game starts or when spawned
void ABarrier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

