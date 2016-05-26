#ifndef G3D_H
#define G3D_H

#include "stdint.h"
#include <iostream>
#include <vector>

class AbstractReader;
class AbstractWriter;

class G3D {
public:
    enum AttributeSemantic { Position = 0, Normal = 1, Tangent = 2, Color = 3, Tex = 4, Float = 5 };

    enum PrimitiveType { Point, Line, Triangle, TriangleAdj };

    enum VertexType { SoA, AoS };

    struct GeometryInfo {
        GeometryInfo()
            : vertexType(AoS)
            , numberPrimitives(0)
            , primitiveType(Triangle)
            , numberIndices(0)
            , numberVertices(0)
            , vertexSize(0)
            , indexSize(0)
            , isOpaque(true) {}
        uint32_t vertexType;
        uint32_t numberPrimitives;
        uint32_t primitiveType;
        uint32_t numberIndices;
        uint32_t numberVertices;
        uint32_t vertexSize;
        uint32_t indexSize;
        bool isOpaque;

        std::vector<uint32_t> attributeSemantics;
    };

    struct Geometry {
        Geometry()
            : indices(nullptr) {}
        GeometryInfo info;

        uint32_t* indices;
    };

    struct GeometryAoS : Geometry {
        GeometryAoS()
            : Geometry()
            , vertices(nullptr) {
            info.vertexType = AoS;
        }

        float* vertices;
    };

    struct GeometrySoA : Geometry {
        GeometrySoA()
            : Geometry() {
            info.vertexType = SoA;
        }

        std::vector<float*> vertexAttributes;
    };

    static uint32_t floats(uint32_t semantic) {
        switch (semantic) {
        case Position:
            return 3;
        case Normal:
            return 3;
        case Tangent:
            return 3;
        case Color:
            return 4;
        case Tex:
            return 2;
        case Float:
            return 1;
        }
        return 0;
    }

    static std::unique_ptr<GeometrySoA> createLineGeometry(const std::vector<uint16_t>& indices, const std::vector<float>& positions,
                                                           const std::vector<float>& colors);

    static void write(AbstractWriter& writer, const GeometryAoS* const geometry, uint32_t vertexType = AoS);
    static void write(AbstractWriter& writer, const GeometrySoA* const geometry, uint32_t vertexType = SoA);
    static void read(AbstractReader& reader, GeometryAoS* const geometry);
    static void read(AbstractReader& reader, GeometrySoA* const geometry);
    static std::string printPrimitiveType(const Geometry* const geometry);
    static std::string printVertexType(const Geometry* const geometry);
    static std::string printAttributeSemantics(const Geometry* const geometry);
    static void print(const Geometry* const geometry, std::ostream& output = std::cout);
    static void clean(GeometryAoS* geometry);
    static void clean(GeometrySoA* geometry);
    static bool merge(GeometrySoA* a, const GeometrySoA* const b); // merge a and b into a

private:
    static void writeHeader(AbstractWriter& writer, const GeometryInfo& info, const uint32_t* const vertexType = nullptr);
    static void writeIndices(AbstractWriter& writer, const uint32_t* const indices, const GeometryInfo& info);
    static void writeVertices(AbstractWriter& writer, const float* const vertices, const GeometryInfo& info);
    static void writeVertices(AbstractWriter& writer, const std::vector<float*>& vertexAttributes, const GeometryInfo& info);
    static void writeContent(AbstractWriter& writer, const GeometryAoS& geometry);
    static void writeContent(AbstractWriter& writer, const GeometrySoA& geometry);

    static void readHeader(AbstractReader& reader, GeometryInfo& info);
    static void readIndices(AbstractReader& reader, uint32_t*& indices, const GeometryInfo& info);
    static void readVertices(AbstractReader& reader, float*& vertices, const GeometryInfo& info);
    static void readVertices(AbstractReader& reader, std::vector<float*>& vertexAttributes, const GeometryInfo& info);
    static void readContent(AbstractReader& reader, GeometryAoS& geometry);
    static void readContent(AbstractReader& reader, GeometrySoA& geometry);

    static void convertVertices(const std::vector<float*>& vertexAttributes, float*& vertices, const GeometryInfo& info);
    static void convertVertices(const float* const vertices, std::vector<float*>& vertexAttributes, const GeometryInfo& info);

    static void cleanIndices(uint32_t* indices);
    static void cleanVertices(float* vertices);
    static void cleanVertices(std::vector<float*>& vertexAttributes);
};

#endif // G3D_H

/*
 The MIT License

 Copyright (c) 2012 Interactive Visualization and Data Analysis Group
 Saarland University, DFKI, Intel VCI, MMCI

 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */
