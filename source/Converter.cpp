#include "Converter.hpp"

physx::PxTriangleMesh* PXConverter::ConvertMesh(const std::shared_ptr<Mesh>& mesh, physx::PxPhysics* physics, physx::PxCooking* cooking) {
    using namespace physx;

    auto vertices = mesh->GetVertices();
    auto indicies = mesh->GetIndices();

    PxTriangleMeshDesc description;


    description.points.data   = vertices.data();
    description.points.count  = static_cast<PxU32>(vertices.size());
    description.points.stride = sizeof(Vertex);

    description.triangles.count  = static_cast<PxU32>(indicies.size() / 3);
    description.triangles.data   = indicies.data();
    description.triangles.stride = 3 * sizeof(uint32_t);

    if (!description.isValid()) {
        throw std::runtime_error("Mesh description isnt valid");
    }

    PxDefaultMemoryOutputStream       writeBuffer{};
    PxTriangleMeshCookingResult::Enum result{};

    if (!cooking->cookTriangleMesh(description, writeBuffer, &result)) {
        throw std::runtime_error("Mesh cooking failed");
    }

    PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
    return physics->createTriangleMesh(readBuffer);
}

physx::PxVec3 PXConverter::ConvertVector3(const glm::vec3& vec) {
    return { vec.x, vec.y, vec.z };
}