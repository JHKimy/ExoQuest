// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Starflux.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/CharacterBase.h"





AStarflux::AStarflux()
{
	PrimaryActorTick.bCanEverTick = true;

	capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	RootComponent = capsuleComp;


	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Item/Starflux/SM_Starflux.SM_Starflux'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}
	meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);




	// 충돌 연동
	capsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AStarflux::OnBeginOverlap);



	Amplitude = 15.f;
	RotationSpeed = 1.f;


}

void AStarflux::BeginPlay()
{
	Super::BeginPlay();
	
	StartPos = GetActorLocation();

	
	// 팝업 애니메이션 시작
	ElapsedTime = 0.f; // 애니메이션 진행 시간 초기화
	bIsPopping = true; // 팝업 상태 활성화

}

void AStarflux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPopping)
	{
		Pop(DeltaTime); // 팝업 애니메이션
	}
	else
	{
		Floating(DeltaTime); // 부유 효과
	}
}


void AStarflux::Pop(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	// 팝업 애니메이션 지속 시간 (초)
	const float PopDuration = 2.f;

	// 애니메이션 진행 비율 (0 ~ 1)
	float Alpha = FMath::Clamp(ElapsedTime / PopDuration, 0.f, 1.f);

	// Ease-In-Out 보간 적용
	float EasedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 3.f); // 2.f는 곡률 조정값


	StartBelowLocation = StartPos;

	StartBelowLocation.Z -= 150.f; // 아래로 100 유닛
	SetActorLocation(StartBelowLocation);


	// 위치 계산
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = FMath::Lerp(StartBelowLocation.Z, StartPos.Z, EasedAlpha);
	SetActorLocation(CurrentLocation);

	// 팝업 애니메이션 종료 조건
	if (ElapsedTime >= PopDuration)
	{
		bIsPopping = false; // 팝업 종료
		ElapsedTime = 0.f;  // 시간 초기화
		// 부유 애니메이션의 시작점을 현재 위치로 설정
		StartPos = CurrentLocation - 10.f; // 부유 효과와 매끄럽게 연결
	}
}

void AStarflux::Floating(float DeltaTime)
{
	FVector NewLocation = StartPos;
	float Time = GetWorld()->GetTimeSeconds();

	NewLocation.Z += FMath::Sin(Time) * Amplitude;

	SetActorLocation(NewLocation);
	AddActorLocalRotation(FRotator(0, RotationSpeed, 0));
}

void AStarflux::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFormSweep, const FHitResult& SweepResult)
{
	ACharacterBase* EquippedCharacter = Cast<ACharacterBase>(OtherActor);

	if (EquippedCharacter) 
	{
		Destroy();
	}

	UE_LOG(LogTemp, Warning, TEXT("dlfsjkfjlei jlsfnk0"));

	// 여기에 캐릭터 인벤토리 연동
}
