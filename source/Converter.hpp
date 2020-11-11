#pragma once

#include "Graphics/ResourceManager.hpp"
#include "Physics/Physics.hpp"

class PXConverter {
    PXConverter();

public:
    static physx::PxTriangleMesh* ConvertMesh(const Mesh& mesh, physx::PxPhysics* physics, physx::PxCooking* cooking);
    static physx::PxVec3 ConvertVector3(const glm::vec3& vec);
};