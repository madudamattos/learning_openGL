#pragma once
#include <functional>
#include <mutex>

class CollisionSystem;

extern CollisionSystem* gCollisionSystem;
extern int uid;
extern std::mutex uidMutex;

enum class CollisionType { External = 0, Internal = 1 };

class Collider {
public:
    struct Circle {
        float x;
        float y;
        float r;
        float epsilon;
    };

    using Callback = std::function<void(Collider*, Collider*)>;

    Collider();
    ~Collider();

    void SetParameters(float x, float y, float r, float epsilon = 0.0f,
                       CollisionType colType = CollisionType::External,
                       Callback onColisionFunc = Callback());

    void SetCircle(float x, float y, float r, float epsilon = 0.0f) { circle = {x,y,r,epsilon}; }
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
