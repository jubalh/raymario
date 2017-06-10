#ifndef PHYSAC
#define PHYSAC

#define PHYSAC_MAX_ELEMENTS 1024

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum { RectangleCollider, CircleCollider } ColliderType;

typedef struct Physics {
    bool enabled;
    bool debug;
    Vector2 gravity;
} Physics;

typedef struct Transform {
    Vector2 position;
    float rotation;
    Vector2 scale;
} Transform;
    
typedef struct Collider {
    bool enabled;
    ColliderType type;
    Rectangle bounds;
    int radius;
} Collider;

typedef struct Rigidbody {
    bool enabled;
    float mass;
    Vector2 acceleration;
    Vector2 velocity;
    bool isGrounded;
    bool isContact;
    bool applyGravity;
    float friction;
    float bounciness;
} Rigidbody;

void InitPhysics();
void SetPhysics(Physics settings);
void SetGravity(Vector2 force);
void AddCollider(int index, Collider collider);
void AddRigidbody(int index, Rigidbody rigidbody);
void ApplyPhysics(int index, Vector2 *position);
void SetRigidbodyEnabled(int index, bool state);
void SetRigidbodyVelocity(int index, Vector2 velocity);
void AddRigidbodyForce(int index, Vector2 force);
void SetColliderEnabled(int index, bool state);
Collider GetCollider(int index);
Rigidbody GetRigidbody(int index);

#endif
