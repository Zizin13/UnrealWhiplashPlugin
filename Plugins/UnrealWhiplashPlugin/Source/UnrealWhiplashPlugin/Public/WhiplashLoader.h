#pragma once
//-------------------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WhiplashLoader.generated.h"
//-------------------------------------------------------------------------------------------------

/**
 *
 */
UCLASS()
class UNREALWHIPLASHPLUGIN_API UWhiplashLoader : public UBlueprintFunctionLibrary
{
  GENERATED_BODY()

  UFUNCTION(BlueprintCallable, Category = "WhiplashLoader")
  static UTexture2D* LoadTexture(const FString& TexFile, const FString &PalFile, bool& IsValid);

};

//-------------------------------------------------------------------------------------------------