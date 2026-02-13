#include "collider.h"
#include "collisionSystem.h"
#include <utility>
#include <iostream>

extern CollisionSystem* gCollisionSystem;

Collider::Collider()
{
    // std::lock_guard<std::mutex> lk(uidMutex);
    // entityId = uid++;
    circle = {0.f,0.f,0.f,0.f};
    type = CollisionType::External;
    registered = false;
    onCollision = Callback();
}

Collider::~Collider()
{
    if (registered && gCollisionSystem) gCollisionSystem->Unregister(this);
}

void Collider::SetParameters(float x, float y, float r, float epsilon, CollisionType colType, Callback cbfunc)
{
    circle = { x, y, r, epsilon };
    type = colType;
    onCollision = std::move(cbfunc);
    if (!registered && gCollisionSystem) {
        gCollisionSystem->Register(this);
        registered = true;
    }
}