#ifndef COOL_BASE_H
#define COOL_BASE_H

#define internal static
#define global static
#define local_persist static

#define pure internal constexpr

using i8 = signed char;
using i16 = short;
using i32 = int;
using i64 = long long;

using bool16 = i16;
using bool32 = i32;
using bool64 = i64;

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using ubool16 = u16;
using ubool32 = u32;
using ubool64 = u64;

#define PI       3.14159265358979323846
#define EULER    2.71828182845904523536


pure float MinF32(float a, float b) { return a < b ? a : b; }
pure int   MinI32(int   a, int   b) { return a < b ? a : b; }
pure float MaxF32(float a, float b) { return a > b ? a : b; }
pure int   MaxI32(int   a, int   b) { return a > b ? a : b; }

pure float ClampF32(float v, float low, float high) { return MinF32(MaxF32(v, low), high); }
pure i32   ClampI32(i32   v, i32   low, i32   high) { return MinI32(MaxI32(v, low), high); }


union Vec2
{
    float c[2];
    struct
    {
        float x;
        float y;
    };

    inline float& operator[](int index)       { return c[index]; }
    inline float  operator[](int index) const { return c[index]; }
};
union Vec2i
{
    i32 c[2];
    struct
    {
        i32 x;
        i32 y;
    };

    inline i32& operator[](int index)       { return c[index]; }
    inline i32  operator[](int index) const { return c[index]; }
};
#define HvDefineOperator_Plus_Vec2(Type) \
    internal Type  operator+ (Type const&         left, Type const&        right) { return {{ left.x + right.x, left.y + right.y }}; }   \
    internal Type  operator+ (Type const&         left, decltype(Type::x) scalar) { return {{ left.x +  scalar, left.y +  scalar }}; }   \
    internal Type  operator+ (decltype(Type::x) scalar, Type const&        right) { return right + scalar; }                             \
    internal Type& operator+=(Type&               left, Type const&        right) { left.x += right.x; left.y += right.y; return left; } \
    internal Type& operator+=(Type&               left, decltype(Type::x) scalar) { left.x +=  scalar; left.y +=  scalar; return left; } \
    /**/
#define HvDefineOperator_Minus_Vec2(Type) \
    internal Type  operator- (Type const& self)                           { return {{ -self.x, -self.y }}; }                     \
    internal Type  operator- (Type const& left, Type const&        right) { return {{ left.x - right.x, left.y - right.y }}; }   \
    internal Type  operator- (Type const& left, decltype(Type::x) scalar) { return {{ left.x -  scalar, left.y -  scalar }}; }   \
    internal Type& operator-=(Type&       left, Type const&        right) { left.x -= right.x; left.y -= right.y; return left; } \
    internal Type& operator-=(Type&       left, decltype(Type::x) scalar) { left.x -=  scalar; left.y -=  scalar; return left; } \
    /**/
#define HvDefineOperator_Mul_Vec2(Type) \
    internal Type  operator* (Type const&         left, decltype(Type::x)   scalar) { return {{ left.x * scalar, left.y * scalar }}; }   \
    internal Type  operator* (decltype(Type::x) scalar, Type const&          right) { return right * scalar; }                           \
    internal Type& operator*=(Type&               left, decltype(Type::x)   scalar) { left.x *= scalar; left.y *= scalar; return left; } \
    /**/
HvDefineOperator_Plus_Vec2  (Vec2);
HvDefineOperator_Minus_Vec2 (Vec2);
HvDefineOperator_Mul_Vec2   (Vec2);

HvDefineOperator_Plus_Vec2  (Vec2i);
HvDefineOperator_Minus_Vec2 (Vec2i);
HvDefineOperator_Mul_Vec2   (Vec2i);
internal Vec2 operator+ (Vec2  const& left, Vec2i const& right) { return {{ left.x + right.x, left.y + right.y }}; }   \
internal Vec2 operator+ (Vec2i const& left, Vec2  const& right) { return {{ left.x + right.x, left.y + right.y }}; }

pure
Vec2i
ToVec2i(Vec2 v)
{
    Vec2i r = {};
    r.x = (int)v.x;
    r.y = (int)v.y;
    return r;
}

struct Image
{
    i32* px;
};

struct ImageF
{
    float* px;
};

struct Particle
{
    Vec2 pos;
    Vec2 dir;
};

#endif
