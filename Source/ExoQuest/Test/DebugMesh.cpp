#include "Test/DebugMesh.h"
#include "Engine/StaticMesh.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

ADebugMesh::ADebugMesh()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = Mesh;

	Utility = CreateDefaultSubobject<UProceduralMeshUtility>(TEXT("ProceduralMeshUtility"));
}

// Called when the game starts or when spawned
void ADebugMesh::BeginPlay()
{
	Super::BeginPlay();
	LogMeshData();
}

// Called every frame
void ADebugMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADebugMesh::LogMeshData()
{
	FMeshDatas Data;
	Utility->GetMeshDataFromStaticMesh(Mesh->GetStaticMesh(), Data, 0);

	UE_LOG(LogTemp, Warning, TEXT("========== Mesh Info =========="));
	UE_LOG(LogTemp, Warning, TEXT("Verts: %d | Tris: %d | Normals: %d | UVs: %d | Colors: %d"),
		Data.Verts.Num(), Data.Tris.Num(), Data.Normals.Num(), Data.UVs.Num(), Data.Colors.Num());

	for (int i = 0; i < FMath::Min(30, Data.Verts.Num()); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("Vert[%d]: %s"), i, *Data.Verts[i].ToString());
	}
	for (int i = 0; i < FMath::Min(30, Data.Tris.Num()); i += 3)
	{
		UE_LOG(LogTemp, Warning, TEXT("Triangle[%d]: %d, %d, %d"), i / 3, Data.Tris[i], Data.Tris[i + 1], Data.Tris[i + 2]);
	}
	for (int i = 0; i < FMath::Min(30, Data.UVs.Num()); ++i)
	{
		UE_LOG(LogTemp, Warning, TEXT("UV[%d]: %s"), i, *Data.UVs[i].ToString());
	}
}

