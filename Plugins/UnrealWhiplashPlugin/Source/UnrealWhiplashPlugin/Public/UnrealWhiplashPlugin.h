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
  wlLoadTextureFunc     m_pfnLoadTexture;
  wlGetModelFunc        m_pfnGetModel;
  wlLoadTrackFunc       m_pfnLoadTrack;
  wlUnloadTrackFunc     m_pfnUnloadTrack;
  wlShutdownModuleFunc  m_pfnShutdownModule;
  wlGetTrackTexFunc     m_pfnGetTrackTex;
  wlGetTrackBldFunc     m_pfnGetTrackBld;
  wlGetNumSignsFunc     m_pfnGetNumSigns;
  wlGetTrackModelFunc   m_pfnGetTrackModel;
  wlGetSignModelFunc    m_pfnGetSignModel;

private:
  void* m_pDllHandle;
};

//-------------------------------------------------------------------------------------------------

extern FUnrealWhiplashPluginModule* g_pModule;

//-------------------------------------------------------------------------------------------------