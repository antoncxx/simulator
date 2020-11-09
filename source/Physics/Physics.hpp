#pragma once
#include "PxPhysicsAPI.h"


class Physics {
    physx::PxFoundation* foundation;
    physx::PxPvd*        pvd;
    physx::PxPhysics*    physics;
    physx::PxCooking*    cooking;

    Physics() = default;
public:
    ~Physics() = default;
  
    static Physics& Instance();

    void StartUp();
    void ShutDown();
};