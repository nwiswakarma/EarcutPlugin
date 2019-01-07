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

#include "CoreTypes.h"
#include "UnrealMemory.h"
#include "mapbox/earcut.hpp"
#include <vector>

namespace mapbox {
namespace util {

template <>
struct nth<0, FVector2D> {
    inline static auto get(const FVector2D &t) {
        return t.X;
    };
};
template <>
struct nth<1, FVector2D> {
    inline static auto get(const FVector2D &t) {
        return t.Y;
    };
};

} // namespace util
} // namespace mapbox

struct FECVertexContainer
{
    typedef FVector2D          value_type;
    typedef TArray<value_type> container_type;

    const container_type& Vertices;

    FECVertexContainer(const container_type& InVertices)
        : Vertices(InVertices)
    {
    }

    SIZE_T size() const
    {
        return Vertices.Num();
    }

    bool empty() const
    {
        return Vertices.Num() == 0;
    }

    const FVector2D& operator[](SIZE_T Index) const
    {
        return Vertices[Index];
    }
};

struct FECPolygon
{
    typedef FECVertexContainer value_type;
    typedef TArray<value_type> container_type;

    container_type Data;

    FECPolygon() = default;

    FECPolygon(const container_type& InPolygon)
        : Data(InPolygon)
    {
    }

    SIZE_T size() const
    {
        return Data.Num();
    }

    bool empty() const
    {
        return Data.Num() == 0;
    }

    const FECVertexContainer& operator[](SIZE_T i) const
    {
        return Data[i];
    }
};

class FECUtils
{
public:

    FORCEINLINE static void Earcut(const TArray<FVector2D>& Points, TArray<int32>& OutIndices, bool bInversed = false)
    {
        Earcut(FECPolygon({ { Points } }), OutIndices, bInversed);
    }

    static void Earcut(const FECPolygon& Polygon, TArray<int32>& OutIndices, bool bInversed = false)
    {
        std::vector<int32> ECIndices( mapbox::earcut<int32>(Polygon) );
        const int32 IndexCount = ECIndices.size();
        const int32 TriCount = IndexCount / 3;

        // Copy result
        OutIndices.SetNumUninitialized(IndexCount);

        if (bInversed)
        {
            FMemory::Memcpy(OutIndices.GetData(), ECIndices.data(), ECIndices.size() * sizeof(int32));
        }
        else
        {
            for (int32 ti=0; ti<TriCount; ++ti)
            {
                const int32 i = ti*3;
                OutIndices[i  ] = ECIndices[i+2];
                OutIndices[i+1] = ECIndices[i+1];
                OutIndices[i+2] = ECIndices[i  ];
            }
        }
    }
};
