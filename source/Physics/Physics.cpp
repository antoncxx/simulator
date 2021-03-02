#include "Physics.hpp"

#include <exception>
#include <cassert>
#include <thread>

#define SCALE_FACTOR 0.1f

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

        #ifdef DEBUG
            pvd = PxCreatePvd(*foundation);
            assert(pvd);

            auto* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 3232, 10);
            assert(transport);

            pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
            assert(pvd);
        #endif // DEBUG
    }

    {
        auto scale = PxTolerancesScale();
        scale.length = SCALE_FACTOR;
        physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, scale, true, pvd);
        assert(physics);

        PxCookingParams cookingParams(physics->getTolerancesScale());
        cookingParams.midphaseDesc.setToDefault(PxMeshMidPhase::eBVH33);
        cookingParams.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
        cookingParams.meshPreprocessParams |= PxMeshPreprocessingFlag::eFORCE_32BIT_INDICES;

        cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, cookingParams);
        assert(cooking);

        dispatcher = PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());
        assert(dispatcher);

        PxSceneDesc desc(scale);
        desc.cpuDispatcher = dispatcher;
        desc.gravity = { 0.f, -9.81f, 0.f };
        desc.filterShader = PxDefaultSimulationFilterShader;

        scene = physics->createScene(desc);

        #ifdef DEBUG
            if (auto* pvd = scene->getScenePvdClient(); pvd) {
                pvd->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS,  true);
                pvd->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS,     true);
                pvd->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
            }
        #endif // DEBUG
    }
}

void Physics::ShutDown() {
    using namespace physx;

    physics->release();
    cooking->release();
    foundation->release();

}
