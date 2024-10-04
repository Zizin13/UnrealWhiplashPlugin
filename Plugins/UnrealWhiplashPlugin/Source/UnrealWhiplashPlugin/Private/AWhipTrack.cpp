#include "AWhipTrack.h"
#include "UnrealWhiplashPlugin.h"
#include "DynamicMesh/MeshAttributeUtil.h"
//-------------------------------------------------------------------------------------------------

// Sets default values
AAWhipTrack::AAWhipTrack()
  : m_mesh(true, false, true, false)
  , m_iTrackId(-1)
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

AAWhipTrack::~AAWhipTrack()
{
  if (g_pModule->m_pfnUnloadTrack)
    g_pModule->m_pfnUnloadTrack(m_iTrackId);
}

//-------------------------------------------------------------------------------------------------

// Called when the game starts or when spawned
void AAWhipTrack::BeginPlay()
{
  Super::BeginPlay();
}

//-------------------------------------------------------------------------------------------------

void AAWhipTrack::PostLoad()
{
  Super::PostLoad();
  
  BuildMesh();
  *(MeshComponent->GetMesh()) = m_mesh;
  MaterialInstance->SetTextureParameterValue("TextureParam", m_pTex);
  MeshComponent->NotifyMeshUpdated();
}

//-------------------------------------------------------------------------------------------------

// Called every frame
void AAWhipTrack::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

//-------------------------------------------------------------------------------------------------

void AAWhipTrack::BuildMesh()
{
  if (!g_pModule
      || !g_pModule->m_pfnLoadTrack
      || !g_pModule->m_pfnGetTrackModel
      || !g_pModule->m_pfnGetTrackTex)
    return;
    
  FString TrackFile = "C:\\WHIP\\WHIPLASH\\FATDATA\\TRACK3.TRK";
  m_iTrackId = g_pModule->m_pfnLoadTrack(TCHAR_TO_ANSI(*TrackFile));
  if (m_iTrackId < 0)
    return;

  //load model
  uint8 *pBmpBuf = new uint8[1000000];
  tWhiplashVertex *pVertexBuf = new tWhiplashVertex[10000]; //cars have 400
  uint32 *pIndexBuf = new uint32[100000]; //cars have 600
  int iBmpSize = -1;
  int iNumVertices = -1;
  int iNumIndices = -1;
  if (!g_pModule->m_pfnGetTrackModel(m_iTrackId,
                                     pVertexBuf,
                                     10000 * sizeof(tWhiplashVertex),
                                     pIndexBuf,
                                     100000 * sizeof(uint32),
                                     iNumVertices,
                                     iNumIndices)) {
    delete[] pBmpBuf;
    delete[] pVertexBuf;
    delete[] pIndexBuf;
    return;
  }
  iBmpSize = g_pModule->m_pfnGetTrackTex(m_iTrackId, pBmpBuf, 1000000);
  if (iBmpSize <= 0) {
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