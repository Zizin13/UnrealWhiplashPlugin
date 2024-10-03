#include "UnrealWhiplashPlugin.h"
#include "Interfaces/IPluginManager.h"
//-------------------------------------------------------------------------------------------------
#define LOCTEXT_NAMESPACE "FUnrealWhiplashPluginModule"
DEFINE_LOG_CATEGORY(LogWhiplash);
//-------------------------------------------------------------------------------------------------
FUnrealWhiplashPluginModule* g_pModule = NULL;
//-------------------------------------------------------------------------------------------------

static void LogMessageCbStatic(const char *szMsg, int iLen)
{
  FString sLog = szMsg;
  UE_LOG(LogWhiplash, Log, TEXT("%s"), *sLog);
}

//-------------------------------------------------------------------------------------------------

void FUnrealWhiplashPluginModule::StartupModule()
{
  // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
  g_pModule = this;

  //load dll
  FString s = IPluginManager::Get().FindPlugin("UnrealWhiplashPlugin")->GetBaseDir();
  UE_LOG(LogWhiplash, Log, TEXT("%s"), *s);
  m_pDllHandle = FPlatformProcess::GetDllHandle(*(IPluginManager::Get().FindPlugin("UnrealWhiplashPlugin")->GetBaseDir() + 
                                                  "/internal/WhipLib/bin/x64/WhipLib_static.dll"));

  //setup logging
  FString funcName = "wlSetLoggingCallback";
  wlSetLoggingCallbackFunc pfnSetLoggingCallback = (wlSetLoggingCallbackFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
  if (pfnSetLoggingCallback)
    pfnSetLoggingCallback(LogMessageCbStatic);

  //get functions
  funcName = "wlLoadTexture";
  m_pfnLoadTexture = (wlLoadTextureFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
  funcName = "wlGetModel";
  m_pfnGetModel = (wlGetModelFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
  funcName = "wlLoadTrack";
  m_pfnLoadTrack = (wlLoadTrackFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
  funcName = "wlUnloadTrack";
  m_pfnUnloadTrack = (wlUnloadTrackFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
  funcName = "wlShutdownModule";
  m_pfnShutdownModule = (wlShutdownModuleFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
  funcName = "wlGetTrackTex";
  m_pfnGetTrackTex = (wlGetTrackTexFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
  funcName = "wlGetTrackBld";
  m_pfnGetTrackBld = (wlGetTrackBldFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
  funcName = "wlGetNumSigns";
  m_pfnGetNumSigns = (wlGetNumSignsFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
  funcName = "wlGetTrackModel";
  m_pfnGetTrackModel = (wlGetTrackModelFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
  funcName = "wlGetSignModel";
  m_pfnGetSignModel = (wlGetSignModelFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
}

//-------------------------------------------------------------------------------------------------

void FUnrealWhiplashPluginModule::ShutdownModule()
{
  // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
  // we call this function before unloading the module.
  if (m_pfnShutdownModule)
    m_pfnShutdownModule();
  FPlatformProcess::FreeDllHandle(m_pDllHandle);
}

//-------------------------------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE	
IMPLEMENT_MODULE(FUnrealWhiplashPluginModule, UnrealWhiplashPlugin)