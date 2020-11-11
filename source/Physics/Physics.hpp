#pragma once
#include "PxPhysicsAPI.h"
#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxDefaultAllocator.h"
#include "pvd/PxPvd.h"
#include "PxConfig.h"

class Physics {
    physx::PxFoundation*    foundation;
    physx::PxPvd*           pvd;
    physx::PxPhysics*       physics;
    physx::PxCooking*       cooking;
    physx::PxScene*         scene;
    physx::PxCpuDispatcher* dispatcher;

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