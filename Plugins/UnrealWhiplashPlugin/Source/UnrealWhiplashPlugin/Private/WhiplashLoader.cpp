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