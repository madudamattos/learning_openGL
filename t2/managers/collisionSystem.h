#pragma once
#include <vector>
#include "collider.h"

class CollisionSystem {
public:
    void Register(Collider* c);
    void Unregister(Collider* c);
    void Update(); 

    bool TestCollision(const Collider& collider, const Collider* ignore = nullptr);
    size_t GetCount() const { return colliders.size(); }

private:
    std::vector<Collider*> colliders;

    void CheckPair(Collider* a, Collider* b);
    bool CheckCollision(const Collider& A, const Collider& B);
};
