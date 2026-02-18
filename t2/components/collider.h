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

    float GetHeight() const { return circle.h; }
    float GetRadius() const { return circle.r; }
    float GetX() const { return circle.x; }
    float GetY() const { return circle.y; }
    float GetZ() const { return circle.z; }
    CollisionType GetType() const { return type; }

private:
    int entityId;
    Circle circle;
    CollisionType type;
    Callback onCollision;
    bool registered = false;

    friend class CollisionSystem;
};
