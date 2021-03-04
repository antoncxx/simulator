#pragma once
#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxDefaultAllocator.h"
#include "pvd/PxPvd.h"
#include "PxConfig.h"

class Physics {
    physx::PxFoundation*    foundation{ nullptr };
    physx::PxPhysics*       physics{ nullptr };
    physx::PxCooking*       cooking{ nullptr };
    physx::PxScene*         scene{ nullptr };
    physx::PxCpuDispatcher* dispatcher{ nullptr };

    Physics() = default;
public:
    ~Physics() = default;
  
    static Physics& Instance();

    inline auto* GetScene() noexcept { return scene; }
    inline auto* GetCooking() noexcept { return cooking; }
    inline auto* GetPhysics() noexcept { return physics; }

    void StartUp();
    void ShutDown();
};