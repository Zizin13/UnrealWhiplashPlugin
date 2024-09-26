#pragma once
//-------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "../internal/WhipLib/include/WhipLib.h"
//-------------------------------------------------------------------------------------------------
DECLARE_LOG_CATEGORY_EXTERN(LogWhiplash, Log, All);
//-------------------------------------------------------------------------------------------------

class FUnrealWhiplashPluginModule : public IModuleInterface
{
public:
  /** IModuleInterface implementation */
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;

  // DLL functions
  wlLoadTextureFunc m_pfnLoadTexture;

private:
  void* m_pDllHandle;
};

//-------------------------------------------------------------------------------------------------

extern FUnrealWhiplashPluginModule* g_pModule;

//-------------------------------------------------------------------------------------------------