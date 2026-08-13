#pragma once
#include <vector>
#include <string>
#include <cstring>
#include "../../Renderer/Mesh.h"

struct RwHeader {
    uint32_t type;
    uint32_t size;
    uint32_t libVersion;
};

class DFFParser {
public:
    static bool ParseFromMemory(const std::vector<char>& data, Mesh& outMesh) {
        if (data.size() < sizeof(RwHeader)) return false;
        return ParseChunks(data.data(), data.data() + data.size(), outMesh);
    }

private:
    static bool ParseChunks(const char* ptr, const char* end, Mesh& outMesh) {
        while (ptr + sizeof(RwHeader) <= end) {
            RwHeader header;
            std::memcpy(&header, ptr, sizeof(RwHeader));
            const char* chunkData = ptr + sizeof(RwHeader);
            const char* nextChunk = chunkData + header.size;

            if (nextChunk > end) break;

            // 0x0F - Чанк геометрии RenderWare (Geometry)
            if (header.type == 0x0F) {
                if (ParseGeometry(chunkData, header.size, outMesh)) {
                    return true;
                }
            }
            // Контейнерные чанки (Clump, GeometryList, Extension) — заходим внутрь
            else if (header.type == 0x10 || header.type == 0x1A || header.type == 0x03) {
                if (ParseChunks(chunkData, nextChunk, outMesh)) {
                    return true;
                }
            }
            ptr = nextChunk;
        }
        return false;
    }

    static bool ParseGeometry(const char* data, uint32_t size, Mesh& outMesh) {
        const char* ptr = data;
        const char* end = data + size;

        if (ptr + sizeof(RwHeader) > end) return false;
        RwHeader structHeader;
        std::memcpy(&structHeader, ptr, sizeof(RwHeader));
        ptr += sizeof(RwHeader);

        if (structHeader.type != 0x01) return false; // Ожидается STRUCT чанк
        const char* sPtr = ptr;

        // Читаем основные параметры геометрии GTA SA
        uint32_t format;
        std::memcpy(&format, sPtr, 4);
        int32_t numTriangles;
        std::memcpy(&numTriangles, sPtr + 4, 4);
        int32_t numVertices;
        std::memcpy(&numVertices, sPtr + 8, 4);
        int32_t numMorphTargets;
        std::memcpy(&numMorphTargets, sPtr + 12, 4);

        sPtr += 16;

        // Проверяем адекватность размеров во избежание мусора
        if (numVertices <= 0 || numVertices > 150000 || numTriangles <= 0 || numTriangles > 300000) {
            return false;
        }

        // В RenderWare для GTA SA после заголовка идут свойства материала (если версия стандартная)
        // Пропускаем 12 байт surface properties (ambient, diffuse, specular)
        sPtr += 12;

        // Если цвета вершин присутствуют в формате (bit 0x04)
        bool hasColors = (format & 0x00000004) != 0;
        // Если текстурные координаты присутствуют (bit 0x01 или 0x100)
        bool hasTexCoords = (format & 0x00000008) != 0 || (format & 0x00000100) != 0;

        if (hasColors) sPtr += numVertices * 4;

        // Текстурные координаты (U, V)
        std::vector<float> uvs(numVertices * 2, 0.0f);
        if (hasTexCoords) {
            std::memcpy(uvs.data(), sPtr, numVertices * 2 * sizeof(float));
            sPtr += numVertices * 2 * sizeof(float);
        }

        // Чтение треугольников (RwTriangle: v1, v2, materialIndex, v3)
        struct RwTriangle {
            uint16_t v2;
            uint16_t v1;
            uint16_t materialIndex;
            uint16_t v0;
        };

        std::vector<RwTriangle> triangles(numTriangles);
        std::memcpy(triangles.data(), sPtr, numTriangles * sizeof(RwTriangle));
        sPtr += numTriangles * sizeof(RwTriangle);

        // Морф-таргеты (сфера охвата + сами вершины и нормали)
        // Пропускаем сфера охвата (16 байт) + флаги (8 байт)
        sPtr += 24;

        // Вершины (positions)
        std::vector<float> positions(numVertices * 3);
        std::memcpy(positions.data(), sPtr, numVertices * 3 * sizeof(float));
        sPtr += numVertices * 3 * sizeof(float);

        // Нормали
        std::vector<float> normals(numVertices * 3, 0.0f);
        bool hasNormals = (format & 0x00000010) != 0;
        if (hasNormals) {
            std::memcpy(normals.data(), sPtr, numVertices * 3 * sizeof(float));
        }

        // Заполняем наш универсальный меш
        outMesh.vertices.resize(numVertices);
        for (int i = 0; i < numVertices; ++i) {
            outMesh.vertices[i] = {
                positions[i * 3 + 0], positions[i * 3 + 1], positions[i * 3 + 2],
                normals[i * 3 + 0],   normals[i * 3 + 1],   normals[i * 3 + 2],
                uvs[i * 2 + 0],       uvs[i * 2 + 1]
            };
        }

        outMesh.indices.resize(numTriangles * 3);
        for (int i = 0; i < numTriangles; ++i) {
            outMesh.indices[i * 3 + 0] = triangles[i].v0;
            outMesh.indices[i * 3 + 1] = triangles[i].v1;
            outMesh.indices[i * 3 + 2] = triangles[i].v2;
        }

        outMesh.UploadToGPU();
        return true;
    }
};