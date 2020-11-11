#include "Physics.hpp"

#include <exception>
#include <cassert>
#include <thread>

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


        pvd = PxCreatePvd(*foundation);
        assert(pvd);

        auto* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 3232, 10);
        assert(transport);

        pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
        assert(pvd);
    }

    {
        auto scale = PxTolerancesScale();
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

        if (auto* pvd = scene->getScenePvdClient(); pvd) {
            pvd->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS,  true);
            pvd->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS,     true);
            pvd->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }

        scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
        scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);
    }
}

void Physics::ShutDown() {
    using namespace physx;

    physics->release();
    cooking->release();
    foundation->release();

}
