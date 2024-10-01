#include "AWhipActor.h"
#include "UnrealWhiplashPlugin.h"
#include "DynamicMesh/MeshAttributeUtil.h"
//-------------------------------------------------------------------------------------------------

// Sets default values
AAWhipActor::AAWhipActor()
  : m_mesh(true, false, true, false)
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  MeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>(
      TEXT("MeshComponent"), false);
  SetRootComponent(MeshComponent);
  
  UMaterial *pParentMaterial = LoadObject<UMaterial>(NULL,
    TEXT("/UnrealWhiplashPlugin/Content/WhiplashBaseMaterial.WhiplashBaseMaterial"));
  MaterialInstance = UMaterialInstanceDynamic::Create(pParentMaterial, MeshComponent, TEXT("MaterialInstance"));
  MeshComponent->SetMaterial(0, MaterialInstance);
}

//-------------------------------------------------------------------------------------------------

// Called when the game starts or when spawned
void AAWhipActor::BeginPlay()
{
  Super::BeginPlay();
}

//-------------------------------------------------------------------------------------------------

void AAWhipActor::PostLoad()
{
  Super::PostLoad();
  
  BuildMesh();
  *(MeshComponent->GetMesh()) = m_mesh;
  MaterialInstance->SetTextureParameterValue("TextureParam", m_pTex);
  MeshComponent->NotifyMeshUpdated();
}

//-------------------------------------------------------------------------------------------------

// Called every frame
void AAWhipActor::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

}

//-------------------------------------------------------------------------------------------------

void AAWhipActor::BuildMesh()
{
  FString TexFile = "C:\\WHIP\\WHIPLASH\\FATDATA\\YZIZIN.BM";
  FString PalFile = "C:\\WHIP\\WHIPLASH\\FATDATA\\PALETTE.PAL";

  //load model
  uint8 *pBmpBuf = new uint8[1000000];
  tWhiplashVertex *pVertexBuf = new tWhiplashVertex[1000]; //cars have 400
  uint32 *pIndexBuf = new uint32[1000]; //cars have 600
  int iBmpSize = -1;
  int iNumVertices = -1;
  int iNumIndices = -1;
  if (g_pModule && g_pModule->m_pfnGetModel && !g_pModule->m_pfnGetModel(pBmpBuf,
                                                                         1000000,
                                                                         pVertexBuf,
                                                                         1000 * sizeof(tWhiplashVertex),
                                                                         pIndexBuf,
                                                                         1000 * sizeof(uint32),
                                                                         eWhipModel::CAR_YZIZIN,
                                                                         TCHAR_TO_ANSI(*TexFile),
                                                                         TCHAR_TO_ANSI(*PalFile),
                                                                         iBmpSize,
                                                                         iNumVertices,
                                                                         iNumIndices)) {
    delete[] pBmpBuf;
    delete[] pVertexBuf;
    delete[] pIndexBuf;
    return;
  }

  //create UTexture
  int iWidth = 64;
  int iHeight = iBmpSize / 4 / 64;
  m_pTex = UTexture2D::CreateTransient(iWidth, iHeight, PF_R8G8B8A8);
  if (!m_pTex) {
    delete[] pBmpBuf;
    delete[] pVertexBuf;
    delete[] pIndexBuf;
    return;
  }
  m_pTex->Filter = TextureFilter::TF_Nearest;

  //update texture
  void *TextureData = m_pTex->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
  FMemory::Memcpy(TextureData, pBmpBuf, iBmpSize);
  m_pTex->GetPlatformData()->Mips[0].BulkData.Unlock();
  m_pTex->UpdateResource();

  //add vertices
  m_mesh.EnableVertexNormals(FVector3f(1.0f, 0.0f, 0.0f));
  m_mesh.EnableVertexUVs(FVector2f(0.0f, 0.0f));
  for (int i = 0; i < iNumVertices; ++i) {
    FVector3d vertex = FVector3d(pVertexBuf[i].fX, pVertexBuf[i].fY * -1, pVertexBuf[i].fZ);
    FVector3f normal = FVector3f(pVertexBuf[i].fNormalX, pVertexBuf[i].fNormalY, pVertexBuf[i].fNormalZ);
    FVector2f texCoord = FVector2f(pVertexBuf[i].fTexX, pVertexBuf[i].fTexY);
    m_mesh.AppendVertex(vertex);
    m_mesh.SetVertexNormal(i, normal);
    m_mesh.SetVertexUV(i, texCoord);
  }

  //add triangles
  for (int i = 0; i < iNumIndices; i += 3) {
    m_mesh.AppendTriangle(pIndexBuf[i], pIndexBuf[i + 1], pIndexBuf[i + 2]);
  }

  m_mesh.EnableAttributes();
  UE::Geometry::CopyVertexUVsToOverlay(m_mesh, *(m_mesh.Attributes()->PrimaryUV()));
  UE::Geometry::CopyVertexNormalsToOverlay(m_mesh, *(m_mesh.Attributes()->PrimaryNormals()));

  delete[] pBmpBuf;
  delete[] pVertexBuf;
  delete[] pIndexBuf;

  return;
}

//-------------------------------------------------------------------------------------------------