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

#include "EarcutBlueprintUtility.h"
#include "EarcutTypes.h"

void UEarcutBlueprintUtility::Earcut(TArray<int32>& OutIndices, const TArray<FVector2D>& InPoints, bool bInversed)
{
    FECUtils::Earcut(InPoints, OutIndices, bInversed);
}

void UEarcutBlueprintUtility::EarcutWithHole(TArray<int32>& OutIndices, const TArray<FVector2D>& InPoints, const TArray<FVector2D>& InHolePoints, bool bInversed)
{
    FECUtils::Earcut(FECPolygon({ { InPoints }, { InHolePoints } }), OutIndices, bInversed);
}

void UEarcutBlueprintUtility::EarcutPolygons(TArray<int32>& OutIndices, const TArray<FVector2D>& InPoints, const TArray<FEarcutPolyIndexPair>& InPolyIndexRanges, bool bInversed)
{
    const int32 PointCount = InPoints.Num();
    const int32 PolyCount = InPolyIndexRanges.Num();

    if (InPoints.Num() < 3 || PolyCount < 1)
    {
        return;
    }

    FECPolygon Polygon;
    Polygon.Data.SetNum(PolyCount);
    
    const int32 PointLastIndex = PointCount-1;

    for (int32 gi=0; gi<PolyCount; ++gi)
    {
        const FEarcutPolyIndexPair& IndexPair(InPolyIndexRanges[gi]);

        int32 IndexStart = IndexPair.IndexStart;
        int32 IndexEnd = IndexPair.IndexEnd;
        IndexStart = FMath::Clamp(IndexStart, 0, PointLastIndex);
        IndexEnd = FMath::Clamp(IndexEnd, IndexStart, PointLastIndex);

        int32 IndexCount = (IndexEnd-IndexStart) + 1;

        if (IndexCount < 3)
        {
            continue;
        }

        FECPointContainer& PointContainer(Polygon.Data[gi]);
        PointContainer.Data.SetNumUninitialized(IndexCount);

        const FVector2D* SrcPointData = InPoints.GetData();
        FVector2D* DstPointData = PointContainer.Data.GetData();

        FMemory::Memcpy(DstPointData, SrcPointData+IndexStart, IndexCount*InPoints.GetTypeSize());
    }

    FECUtils::Earcut(Polygon, OutIndices, bInversed);
}

void UEarcutBlueprintUtility::EarcutPolygonsByIndexOffset(TArray<int32>& OutIndices, const TArray<FVector2D>& InPoints, const TArray<int32>& InPolyIndexOffsets, bool bInversed)
{
    const int32 PointCount = InPoints.Num();
    const int32 PolyOffsetCount = InPolyIndexOffsets.Num();
    const int32 PolyCount = PolyOffsetCount+1;
    const int32 PointLastIndex = PointCount-1;

    if (PointCount < 3)
    {
        return;
    }

    TArray<FEarcutPolyIndexPair> IndexRanges;

    // Earcut with offset ranges
    if (PolyOffsetCount > 0)
    {
        int32 IndexStart = 0;
        int32 IndexEnd = 0;

        // Add poly by offset ranges
        for (int32 i=0; i<PolyOffsetCount; ++i)
        {
            const int32 IndexOffset = InPolyIndexOffsets[i];

            IndexEnd = IndexOffset-1;

            if (IndexEnd > IndexStart)
            {
                FEarcutPolyIndexPair IndexPair = { IndexStart, IndexEnd };
                IndexRanges.Emplace(IndexPair);

                IndexStart = IndexOffset;
            }
        }
        // Add last remaining points as the last poly
        if ((IndexEnd+1) < PointLastIndex)
        {
            FEarcutPolyIndexPair IndexPair = { IndexEnd+1, PointLastIndex };
            IndexRanges.Emplace(IndexPair);
        }

        EarcutPolygons(OutIndices, InPoints, IndexRanges, bInversed);
    }
    // No offset specified, simple earcut with single poly
    else
    {
        Earcut(OutIndices, InPoints, bInversed);
    }
}

void UEarcutBlueprintUtility::EarcutIndexedPolyGroups(TArray<FGULIntGroup>& OutPolyIndices, const TArray<FGULIndexedPolyGroup>& InIndexGroups, const TArray<FGULVector2DGroup>& InPolyGroups, bool bInversed)
{
    // Set number of output poly indices equal to input index group,
    // invalid index group would have empty indices
    OutPolyIndices.SetNum(InIndexGroups.Num());

    for (int32 i=0; i<InIndexGroups.Num(); ++i)
    {
        const FGULIndexedPolyGroup& IndexGroup(InIndexGroups[i]);

        // Skip invalid index group
        if (! IndexGroup.IsValidIndexGroup(InPolyGroups))
        {
            continue;
        }

        int32 OuterPolyIndex = IndexGroup.OuterPolyIndex;
        const TArray<int32>& InnerPolyIndices(IndexGroup.InnerPolyIndices);

        FECPolygon Polygon({ { InPolyGroups[OuterPolyIndex].Points } });

        for (int32 pi : InnerPolyIndices)
        {
            Polygon.Data.Emplace(InPolyGroups[pi].Points);
        }

        FECUtils::Earcut(Polygon, OutPolyIndices[i].Values, bInversed);
    }
}
