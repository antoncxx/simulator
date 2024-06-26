#pragma once

#include "Graphics/ResourceManager.hpp"
#include "Physics/Physics.hpp"

class PXConverter {
    PXConverter();

public:
    static physx::PxTriangleMesh* ConvertMesh(const std::shared_ptr<Mesh>& mesh, physx::PxPhysics* physics, physx::PxCooking* cooking);
    static physx::PxVec3 ConvertVector3(const glm::vec3& vec);
    static glm::vec3 ConvertVector3(const physx::PxVec3& vec);

};