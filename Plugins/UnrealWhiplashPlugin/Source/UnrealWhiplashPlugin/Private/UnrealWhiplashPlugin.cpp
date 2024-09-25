#include "UnrealWhiplashPlugin.h"
#include "Interfaces/IPluginManager.h"
//-------------------------------------------------------------------------------------------------
#define LOCTEXT_NAMESPACE "FUnrealWhiplashPluginModule"
//-------------------------------------------------------------------------------------------------
FUnrealWhiplashPluginModule* g_pModule = NULL;
//-------------------------------------------------------------------------------------------------

void FUnrealWhiplashPluginModule::StartupModule()
{
  // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
  g_pModule = this;

  FString s = IPluginManager::Get().FindPlugin("UnrealWhiplashPlugin")->GetBaseDir();
  UE_LOG(LogTemp, Log, TEXT("%s"), *s);
  m_pDllHandle = FPlatformProcess::GetDllHandle(*(IPluginManager::Get().FindPlugin("UnrealWhiplashPlugin")->GetBaseDir() + "\\internal\\WhipLib\\bin\\x64\\WhipLib-static.dll"));
  FString funcName = "wlLoadTexture";
  m_pfnLoadTexture = (wlLoadTextureFunc)FPlatformProcess::GetDllExport(m_pDllHandle, *funcName);
}

//-------------------------------------------------------------------------------------------------

void FUnrealWhiplashPluginModule::ShutdownModule()
{
  // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
  // we call this function before unloading the module.
  FPlatformProcess::FreeDllHandle(m_pDllHandle);
}

//-------------------------------------------------------------------------------------------------
#undef LOCTEXT_NAMESPACE	
IMPLEMENT_MODULE(FUnrealWhiplashPluginModule, UnrealWhiplashPlugin)