#pragma once
//-------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DynamicMeshComponent.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "AWhipActor.generated.h"
//-------------------------------------------------------------------------------------------------

UCLASS()
class UNREALWHIPLASHPLUGIN_API AAWhipActor : public AActor
{
  GENERATED_BODY()

public:
        // Sets default values for this actor's properties
  AAWhipActor();

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
  UDynamicMeshComponent *MeshComponent;

protected:
        // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
        // Called every frame
  virtual void Tick(float DeltaTime) override;

private:
  void BuildMesh();

  UE::Geometry::FDynamicMesh3 m_mesh;
  UTexture2D *m_pTex;
};

//-------------------------------------------------------------------------------------------------