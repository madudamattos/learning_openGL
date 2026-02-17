#include "collisionSystem.h"
#include <algorithm>
#include <cmath>
#include <iostream>


void CollisionSystem::Register(Collider* c) {
    colliders.push_back(c);}

void CollisionSystem::Unregister(Collider* c) {
    colliders.erase(std::remove(colliders.begin(), colliders.end(), c), colliders.end());
}

void CollisionSystem::Update() {
    size_t n = colliders.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            CheckPair(colliders[i], colliders[j]);
        }
    }
}

void CollisionSystem::CheckPair(Collider* A, Collider* B) {
    if (!A || !B) return;

    if (CheckCollision(*A, *B)) {
        if (A->onCollision) A->onCollision(A, B);
        if (B->onCollision) B->onCollision(B, A);
    }
}

bool CollisionSystem::CheckCollision(const Collider& A, const Collider& B) {
    
    float dx, dz, rsum, dist2;

    if(B.type == CollisionType::Internal && A.type == CollisionType::External)
    {
        dx = A.circle.x - B.circle.x;
        dz = A.circle.z - B.circle.z;
        rsum = A.circle.r + B.circle.r;
        dist2 = dx*dx + dz*dz;
        return B.circle.r - sqrt(dist2) <= A.circle.r;
    } 
    else if(A.type == CollisionType::Internal && B.type == CollisionType::External)
    {
        dx = A.circle.x - B.circle.x;
        dz = A.circle.z - B.circle.z;
        rsum = A.circle.r + B.circle.r;
        dist2 = dx*dx + dz*dz;
        return A.circle.r - sqrt(dist2) <= B.circle.r;
    }
    
    dx = A.circle.x - B.circle.x;
    dz = A.circle.z - B.circle.z;
    rsum = A.circle.r + B.circle.r;
    dist2 = dx*dx + dz*dz;
    return dist2 <= rsum * rsum;
}

bool CollisionSystem::TestCollision(const Collider& collider, const Collider* ignore) {
    for (auto *c : colliders) {
        if (!c) continue;
        if (ignore && c == ignore) continue;
        if (c->GetEntityId() == collider.GetEntityId()) continue;
        if (CheckCollision(collider, *c)) {
            // DEBUG
            // std::cout << "   -> COLLISION with id="<<c->GetEntityId()<<"\n";
            return true;
        }
    }

    // DEBUG
    // std::cout << "   -> no collision\n";
    return false;
}
