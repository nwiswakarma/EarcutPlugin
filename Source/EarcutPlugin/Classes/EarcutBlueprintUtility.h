////////////////////////////////////////////////////////////////////////////////
//
// MIT License
// 
// Copyright (c) 2018-2019 Nuraga Wiswakarma
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////
// 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GULTypes.h"
#include "Poly/GULPolyTypes.h"
#include "EarcutBlueprintUtility.generated.h"

USTRUCT(BlueprintType)
struct FEarcutPolyIndexPair
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 IndexStart;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 IndexEnd;
};

UCLASS()
class EARCUTPLUGIN_API UEarcutBlueprintUtility : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintCallable, Category="Earcut")
    static void Earcut(TArray<int32>& OutIndices, const TArray<FVector2D>& InPoints, bool bInversed = false);

    UFUNCTION(BlueprintCallable, Category="Earcut")
    static void EarcutWithHole(TArray<int32>& OutIndices, const TArray<FVector2D>& InPoints, const TArray<FVector2D>& InHolePoints, bool bInversed = false);

    UFUNCTION(BlueprintCallable, Category="Earcut")
    static void EarcutPolygons(TArray<int32>& OutIndices, const TArray<FVector2D>& InPoints, const TArray<FEarcutPolyIndexPair>& InPolyIndexRanges, bool bInversed = false);

    UFUNCTION(BlueprintCallable, Category="Earcut")
    static void EarcutPolygonsByIndexOffset(TArray<int32>& OutIndices, const TArray<FVector2D>& InPoints, const TArray<int32>& InPolyIndexOffsets, bool bInversed = false);

    UFUNCTION(BlueprintCallable, Category="Earcut")
    static void EarcutIndexedPolyGroups(TArray<FGULIntGroup>& OutPolyIndices, const TArray<FGULIndexedPolyGroup>& InIndexGroups, const TArray<FGULVector2DGroup>& InPolyGroups, bool bInversed = false);
};
