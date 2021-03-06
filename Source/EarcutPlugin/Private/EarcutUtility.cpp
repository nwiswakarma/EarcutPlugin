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

#include "EarcutTypes.h"
#include "mapbox/earcut.hpp"

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

void FECUtils::Earcut(const FECPolygon& Polygon, TArray<int32>& OutIndices, bool bInversed)
{
    mapbox::detail::Earcut<int32> ECImpl;
    ECImpl(Polygon);

    const std::vector<int32>& ECIndices(ECImpl.indices);
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
