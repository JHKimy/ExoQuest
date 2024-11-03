#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

UCLASS()
class EXOQUEST_API ARifle : public AActor
{
	GENERATED_BODY()
	
public:	
	ARifle();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;



public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = MeshComp)
	UStaticMeshComponent* meshComp;
};
