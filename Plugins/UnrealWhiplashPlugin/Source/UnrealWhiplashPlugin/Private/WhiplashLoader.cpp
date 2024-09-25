#include "WhiplashLoader.h"
#include "IImageWrapper.h"
#include "UnrealWhiplashPlugin.h"
#include "../internal/WhipLib/include/WhipLib.h"
//-------------------------------------------------------------------------------------------------

UTexture2D* UWhiplashLoader::LoadTexture(const FString &TexFile, const FString &PalFile, bool& IsValid, int32& Width, int32& Height)
{
  IsValid = false;
  UTexture2D* pUTexture = NULL;
  Width = 0;
  Height = 0;

  //load texture
  uint8 *pBmpBuf = new uint8[900000];
  int iBmpSize = -1;
  if (g_pModule && g_pModule->m_pfnLoadTexture) {
    iBmpSize = g_pModule->m_pfnLoadTexture(pBmpBuf,
                                           900000,
                                           TCHAR_TO_ANSI(*TexFile),
                                           TCHAR_TO_ANSI(*PalFile));
  }
  if (iBmpSize <= 0)
    return NULL;

  //update size output
  Width = 64;
  Height = iBmpSize / 3 / 64;

  //create UTexture
  pUTexture = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);
  if (!pUTexture)
    return NULL;

  //update texture
  void* TextureData = pUTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
  FMemory::Memcpy(TextureData, pBmpBuf, iBmpSize);
  pUTexture->GetPlatformData()->Mips[0].BulkData.Unlock();  
  pUTexture->UpdateResource();

  IsValid = true;
  return pUTexture;
}

//-------------------------------------------------------------------------------------------------