#include "WhiplashLoader.h"
#include "IImageWrapper.h"
#include "UnrealWhiplashPlugin.h"
#include "../internal/WhipLib/include/WhipLib.h"
//-------------------------------------------------------------------------------------------------

UTexture2D* UWhiplashLoader::LoadTexture(const FString &TexFile, const FString &PalFile, bool& IsValid)
{
  IsValid = false;
  UTexture2D* pUTexture = NULL;

  //load texture
  uint8 *pBmpBuf = new uint8[1000000];
  int iBmpSize = -1;
  if (g_pModule && g_pModule->m_pfnLoadTexture) {
    iBmpSize = g_pModule->m_pfnLoadTexture(pBmpBuf,
                                           1000000,
                                           TCHAR_TO_ANSI(*TexFile),
                                           TCHAR_TO_ANSI(*PalFile));
  }
  if (iBmpSize <= 0) {
    delete[] pBmpBuf;
    return NULL;
  }

  //create UTexture
  int iWidth = 64;
  int iHeight = iBmpSize / 3 / 64;
  pUTexture = UTexture2D::CreateTransient(iWidth, iHeight, PF_R8G8B8A8);
  if (!pUTexture) {
    delete[] pBmpBuf;
    return NULL;
  }

  //update texture
  void* TextureData = pUTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
  FMemory::Memcpy(TextureData, pBmpBuf, iBmpSize);
  pUTexture->GetPlatformData()->Mips[0].BulkData.Unlock();  
  pUTexture->UpdateResource();

  IsValid = true;
  delete[] pBmpBuf;
  return pUTexture;
}

//-------------------------------------------------------------------------------------------------

FWhipModel UWhiplashLoader::LoadModel(EWhipModel model, const FString &TexFile, const FString &PalFile, bool &IsValid)
{
  FWhipModel outModel;

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
                                                                         (eWhipModel)model,
                                                                         TCHAR_TO_ANSI(*TexFile),
                                                                         TCHAR_TO_ANSI(*PalFile),
                                                                         iBmpSize,
                                                                         iNumVertices,
                                                                         iNumIndices)) {
    delete[] pBmpBuf;
    delete[] pVertexBuf;
    delete[] pIndexBuf;
    return outModel;
  }

  //create UTexture
  int iWidth = 64;
  int iHeight = iBmpSize / 3 / 64;
  outModel.Texture = UTexture2D::CreateTransient(iWidth, iHeight, PF_R8G8B8A8);
  if (!outModel.Texture) {
    delete[] pBmpBuf;
    delete[] pVertexBuf;
    delete[] pIndexBuf;
    return outModel;
  }

  //update texture
  void *TextureData = outModel.Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
  FMemory::Memcpy(TextureData, pBmpBuf, iBmpSize);
  outModel.Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
  outModel.Texture->UpdateResource();

  //add vertices
  for (int i = 0; i < iNumVertices; ++i) {
    FVector vertex = FVector(pVertexBuf[i].fX, pVertexBuf[i].fY, pVertexBuf[i].fZ);
    FVector normal = FVector(pVertexBuf[i].fNormalX, pVertexBuf[i].fNormalY, pVertexBuf[i].fNormalZ);
    FVector2D texCoord = FVector2D(pVertexBuf[i].fTexX, pVertexBuf[i].fTexY);
    outModel.Vertices.Add(vertex);
    outModel.Normals.Add(normal);
    outModel.TexCoords.Add(texCoord);
  }

  //add indices
  for (int i = 0; i < iNumIndices; ++i) {
    outModel.Indices.Add(pIndexBuf[i]);
  }

  IsValid = true;
  delete[] pBmpBuf;
  delete[] pVertexBuf;
  delete[] pIndexBuf;

  return outModel;
}

//-------------------------------------------------------------------------------------------------