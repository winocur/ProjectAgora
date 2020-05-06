#ifndef H_PHYSICS
#define H_PHYSICS

struct Vector3 {
    f32 x, y, z;
};

struct Vector2 {
    f32 x, y;
};

struct BoundingBox {
    int x;
    int y;
    int width;
    int height;
};

float Lerp (float from, float to, float value) {
    if(value > 1) value = 1;

    return from + (to - from) * value;
}

Vector2 Lerp (Vector2 from, Vector2 to, float value) {
    return {
        Lerp(from.x, to.x, value),
        Lerp(from.y, to.y, value),
    };
}

float Dot (Vector2 vector1, Vector2 vector2) {
    return vector1.x * vector2.x + vector1.y * vector2.y;
};

Vector2 operator/ (Vector2 vector, float scalar) {
    return {
        vector.x / scalar,
        vector.y / scalar,
    };
}

Vector2 operator* (Vector2 vector, float scalar) {
    return {
        vector.x * scalar,
        vector.y * scalar,
    };
}

// -- adding and substracting vectors
Vector2 operator+ (const Vector2& first, const Vector2& second) {
    return {
        first.x + second.x,
        first.y + second.y, 
    };
}

Vector2 operator- (const Vector2& first, const Vector2& second) {
    return {
        first.x - second.x,
        first.y - second.y, 
    };
}

Vector3 operator+ (const Vector3& first, const Vector3& second) {
    return {
        first.x + second.x,
        first.y + second.y, 
        first.z + second.z,
    };
}

Vector3 operator- (const Vector3& first, const Vector3& second) {
    return {
        first.x - second.x,
        first.y - second.y, 
        first.z - second.z,
    };
}

// Magnitude functions
f32 Square (f32 number) {
    return number * number;
}

f32 SQRMagnitude (const Vector3* vector) {
    return Square(vector->x) + Square(vector->y) + Square(vector->z);
};

f32 SQRMagnitude (const Vector2* vector) {
    return Square(vector->x) + Square(vector->y);
};

f32 Magnitude (const Vector3* vector) {
    return sqrt(SQRMagnitude(vector));
}

f32 Magnitude (const Vector2* vector) {
    return sqrt(SQRMagnitude(vector));
}
// -- -- 

bool CheckCollision (Vector2 point, BoundingBox box) {

    if(point.x < box.x) return false;
    if(point.x > box.x + box.width) return false;
    if(point.y < box.y) return false;
    if(point.y > box.y + box.height) return false;

    return true;
}

// @UNUSED
bool CheckCollisionRhombus (Vector2 Point, Vector2 center, float width, float height) {

    float a = 0.5*width;       // half-width (in the x-direction)
    float b = 0.5*height;      // half-height (y-direction)

    Vector2 C = center + Vector2 { a, 0 };
    Vector2 A = center + Vector2 { -a, 0 };

    Vector2 D = center + Vector2 { 0, b };
    Vector2 B = center + Vector2 { 0, -b };

    Vector2 U = (C - A)/(2*a); // unit vector in x-direction
    Vector2 V = (D - B)/(2*b); // unit vector in y-direction

    Vector2  W = Point - center;
    float xabs = abs(Dot(W,U));    // here W*U is the dot product of W and U
    float yabs = abs(Dot(W,V));    // here W*V is the dot product of W and V
    if (xabs/a + yabs/b <= 1)  {
        return true;
    }

    return false;


}

#endif