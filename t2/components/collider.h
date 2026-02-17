#pragma once
#include <functional>
#include <mutex>

class CollisionSystem;

extern CollisionSystem* gCollisionSystem;

enum class CollisionType { External = 0, Internal = 1 };

class Collider {
public:
    struct Circle {
        float x;
        float y;
        float z;
        float r;
        float h; // altura do cilindro
        float epsilon;
    };

    using Callback = std::function<void(Collider*, Collider*)>;

    Collider();
    ~Collider();

    void SetParameters(float x, float y, float z, float r, float h, float epsilon = 0.0f,
                       CollisionType colType = CollisionType::External,
                       Callback onColisionFunc = Callback());

    void SetCircle(float x, float y, float z, float r, float h, float epsilon = 0.0f) { 
        circle = { x, y, z, r, h, epsilon }; 
    }
    const Circle& GetCircle() const { return circle; }
    int GetEntityId() const { return entityId; }
    void SetOnCollision(Callback cb) { onCollision = std::move(cb); }

private:
    int entityId;
    Circle circle;
    CollisionType type;
    Callback onCollision;
    bool registered = false;

    friend class CollisionSystem;
};
