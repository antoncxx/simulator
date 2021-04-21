#include "Physics.hpp"

#include <exception>
#include <cassert>
#include <thread>

#define SCALE_FACTOR 0.01f
#define GRAVITY      9.81f

namespace {
    physx::PxDefaultErrorCallback gDefaultErrorCallback;
    physx::PxDefaultAllocator     gDefaultAllocator;
}


Physics& Physics::Instance() {
    static Physics instance;
    return instance;
}

void Physics::StartUp() {
    using namespace physx;
    {
        foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocator, gDefaultErrorCallback);
        assert(foundation);
    }

    {
        auto scale = PxTolerancesScale();

        scale.length = SCALE_FACTOR; //TODO: WTF????
        scale.speed = 1 / SCALE_FACTOR;

        physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, scale, true, nullptr);
        assert(physics);

        PxCookingParams cookingParams(physics->getTolerancesScale());
        cookingParams.midphaseDesc.setToDefault(PxMeshMidPhase::eBVH34);
        cookingParams.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
        cookingParams.meshPreprocessParams |= PxMeshPreprocessingFlag::eFORCE_32BIT_INDICES;

        cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, cookingParams);
        assert(cooking);

        dispatcher = PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
        assert(dispatcher);

        PxSceneDesc desc(physics->getTolerancesScale());
        desc.cpuDispatcher = dispatcher;
        desc.gravity = { 0.f, -GRAVITY, 0.f };
        desc.filterShader = PxDefaultSimulationFilterShader;

        scene = physics->createScene(desc);
    }
}

void Physics::ShutDown() {
    using namespace physx;

    physics->release();
    cooking->release();
    foundation->release();

}
