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

    // casos com Internal/External originais (mantidos)
    if (B.type == CollisionType::Internal && A.type == CollisionType::External)
    {
        dx = A.circle.x - B.circle.x;
        dz = A.circle.z - B.circle.z;
        rsum = A.circle.r + B.circle.r;
        dist2 = dx * dx + dz * dz;

        if (B.circle.r - sqrt(dist2) <= A.circle.r)
        {
            float A_bottom = A.circle.y;
            float A_top    = A.circle.y + A.circle.h;
            float B_bottom = B.circle.y;
            float B_top    = B.circle.y + B.circle.h;
            if (A_bottom <= B_top && A_top >= B_bottom) return true;
        }
        return false;
    }
    else if (A.type == CollisionType::Internal && B.type == CollisionType::External)
    {
        dx = A.circle.x - B.circle.x;
        dz = A.circle.z - B.circle.z;
        rsum = A.circle.r + B.circle.r;
        dist2 = dx * dx + dz * dz;

        if (A.circle.r - sqrt(dist2) <= B.circle.r)
        {
            float A_bottom = A.circle.y;
            float A_top    = A.circle.y + A.circle.h;
            float B_bottom = B.circle.y;
            float B_top    = B.circle.y + B.circle.h;
            if (A_bottom <= B_top && A_top >= B_bottom) return true;
        }
        return false;
    }

    // calcular separação horizontal 
    dx = A.circle.x - B.circle.x;
    dz = A.circle.z - B.circle.z;
    rsum = A.circle.r + B.circle.r;
    dist2 = dx * dx + dz * dz;

    // região de colisão em XZ; só se isto for verdade checamos altura (Y)
    if (dist2 <= rsum * rsum)
    {
        // A = cilindro, B = esfera
        if (A.circle.h > 0 && B.circle.h == 0)
        {
            float A_bottom = A.circle.y;
            float A_top    = A.circle.y + A.circle.h;
            float B_bottom = B.circle.y - B.circle.r;
            float B_top    = B.circle.y + B.circle.r;
            if (A_bottom <= B_top && A_top >= B_bottom) return true;
        }

        // A = esfera, B = cilindro
        if (A.circle.h == 0 && B.circle.h > 0)
        {
            float A_bottom = A.circle.y - A.circle.r;
            float A_top    = A.circle.y + A.circle.r;
            float B_bottom = B.circle.y;
            float B_top    = B.circle.y + B.circle.h;
            if (A_bottom <= B_top && A_top >= B_bottom) return true;
        }

        // ambos cilindros
        if (A.circle.h > 0 && B.circle.h > 0)
        {
            float A_bottom = A.circle.y;
            float A_top    = A.circle.y + A.circle.h;
            float B_bottom = B.circle.y;
            float B_top    = B.circle.y + B.circle.h;
            if (A_bottom < B_top && A_top > B_bottom) return true;
        }

        // ambos esferas (vertical overlap já garantida porque h==0 -> sphere extends +/- r)
        if (A.circle.h == 0 && B.circle.h == 0)
        {
            float dy = A.circle.y - B.circle.y;
            // já sabemos separação horizontal <= rsum; verificar 3D
            if (dy*dy + dist2 <= rsum * rsum) return true;
        }
    }

    return false;
}

bool CollisionSystem::TestCollision(const Collider& collider, const Collider* ignore) {
    for (auto *c : colliders) {
        if (!c) continue;
        if (ignore && c == ignore) continue;
        if (c->GetEntityId() == collider.GetEntityId()) continue;
        if (CheckCollision(collider, *c)) {
            return true;
        }
    }

    return false;
}
