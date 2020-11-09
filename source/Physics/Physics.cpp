#include "Physics.hpp"

#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxDefaultAllocator.h"
#include "pvd/PxPvd.h"
#include "PxConfig.h"

#include <exception>
#include <cassert>

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

    foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocator, gDefaultErrorCallback);
    assert(foundation);

    pvd = PxCreatePvd(*foundation);
    assert(pvd);

    auto* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 3232, 10);
    assert(transport);

    pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
    assert(pvd);

    auto scale = PxTolerancesScale();
    physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, scale, true, pvd);
    assert(physics);

    cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, scale);
    assert(cooking);
}

void Physics::ShutDown() {
    using namespace physx;

    physics->release();
    cooking->release();
    foundation->release();

}
