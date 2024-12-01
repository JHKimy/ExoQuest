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




	// �浹 ����
	capsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AStarflux::OnBeginOverlap);



	Amplitude = 15.f;
	RotationSpeed = 1.f;


}

void AStarflux::BeginPlay()
{
	Super::BeginPlay();
	
	StartPos = GetActorLocation();

	
	// �˾� �ִϸ��̼� ����
	ElapsedTime = 0.f; // �ִϸ��̼� ���� �ð� �ʱ�ȭ
	bIsPopping = true; // �˾� ���� Ȱ��ȭ

}

void AStarflux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPopping)
	{
		Pop(DeltaTime); // �˾� �ִϸ��̼�
	}
	else
	{
		Floating(DeltaTime); // ���� ȿ��
	}
}


void AStarflux::Pop(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	// �˾� �ִϸ��̼� ���� �ð� (��)
	const float PopDuration = 2.f;

	// �ִϸ��̼� ���� ���� (0 ~ 1)
	float Alpha = FMath::Clamp(ElapsedTime / PopDuration, 0.f, 1.f);

	// Ease-In-Out ���� ����
	float EasedAlpha = FMath::InterpEaseInOut(0.f, 1.f, Alpha, 3.f); // 2.f�� ��� ������


	StartBelowLocation = StartPos;

	StartBelowLocation.Z -= 150.f; // �Ʒ��� 100 ����
	SetActorLocation(StartBelowLocation);


	// ��ġ ���
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = FMath::Lerp(StartBelowLocation.Z, StartPos.Z, EasedAlpha);
	SetActorLocation(CurrentLocation);

	// �˾� �ִϸ��̼� ���� ����
	if (ElapsedTime >= PopDuration)
	{
		bIsPopping = false; // �˾� ����
		ElapsedTime = 0.f;  // �ð� �ʱ�ȭ
		// ���� �ִϸ��̼��� �������� ���� ��ġ�� ����
		StartPos = CurrentLocation - 10.f; // ���� ȿ���� �Ų����� ����
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

	// ���⿡ ĳ���� �κ��丮 ����
}
