#include "GameFramework/ObjMeshLoader.h"

#include "Core/Assert.h"
#include "Core/IO/Decorators.h"
#include "Core/Hash.h"

#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <tracy/Tracy.hpp>

struct Vertex {
    Float3 position{};
    Float3 color{};
    Float3 normal{};
    Float2 uv{};

    bool operator==(const Vertex& other) const noexcept {
        return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
    }
};

template<>
struct std::hash<Vertex> {
    size_t operator()(const Vertex& vertex) const {
        size_t seed = 0;
        HashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
        return seed;
    }
};

Scope<Resource> ObjMeshLoader::Load(const ResourceId& id, IO::Reader& reader)
{
    ZoneScoped;

    tinyobj::ObjReader objReader;

    String objText = IO::TextReader::CreateNonOwned(reader).ReadAll();

    {
        ZoneScopedN("tinyobjloader ParseFromString method - ObjMeshLoader");
        if(!objReader.ParseFromString(objText, "")) {
            EX_ASSERT(objReader.Error().empty(), "Failed to read OBJ model. Error message: {}", objReader.Error());
        }
    }

    if(!objReader.Warning().empty()) {
        EX_LOG(Warning, LogCategory::Resource, "OBJ Loader Warning: {}", objReader.Warning());
    }

    auto& attrib = objReader.GetAttrib();
    auto& shapes = objReader.GetShapes();
    auto& materials = objReader.GetMaterials();

    std::vector<float> data;
    std::vector<Uint32> indices;

    std::unordered_map<Vertex, Uint32> uniqueVertices;
    {
        ZoneScopedN("Processing tinyobjloader shapes");

        for(const auto& shape : shapes) {
            for(const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                if(index.vertex_index >= 0) {
                    // Position
                    vertex.position =  {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                    };

                    // Color
                    vertex.color = {
                        attrib.colors[3 * index.vertex_index + 0],
                        attrib.colors[3 * index.vertex_index + 1],
                        attrib.colors[3 * index.vertex_index + 2]
                    };
                }

                if(index.normal_index >= 0) {
                    // Normals
                    vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                }

                if(index.texcoord_index >= 0) {
                    // Texture Coordinates
                    vertex.uv = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }

                if(uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<Uint32>(data.size() / (3 + 3 + 3 + 2));
                    data.push_back(vertex.position.x);
                    data.push_back(vertex.position.y);
                    data.push_back(vertex.position.z);
                    data.push_back(vertex.color.r);
                    data.push_back(vertex.color.g);
                    data.push_back(vertex.color.b);
                    data.push_back(vertex.normal.x);
                    data.push_back(vertex.normal.y);
                    data.push_back(vertex.normal.z);
                    data.push_back(vertex.uv.x);
                    data.push_back(vertex.uv.y);
                }
                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    auto vertexArray = m_rendererContext->CreateVertexArray(id);

    auto vertexBuffer = Ref<Renderer::VertexBuffer>(m_rendererContext->CreateVertexBuffer(data.data(), sizeof(float) * data.size()));

    Renderer::BufferLayout layout = {
        { "a_position", Renderer::ShaderDataType::Float3 },
        { "a_color", Renderer::ShaderDataType::Float3 },
        { "a_normal", Renderer::ShaderDataType::Float3 },
        { "a_uv", Renderer::ShaderDataType::Float2 },
    };

    vertexBuffer->SetLayout(layout);

    auto indexBuffer = Ref<Renderer::IndexBuffer>(m_rendererContext->CreateIndexBuffer(indices.data(), sizeof(Uint32) * indices.size()));

    vertexArray->AddVertexBuffer(vertexBuffer);
    vertexArray->SetIndexBuffer(indexBuffer);
    
    return Scope<Renderer::VertexArray>(vertexArray);
}