#include <Windows.h>

#include <cmath>
#include <time.h>

#include <chrono>
#include <random>

#define internal static
#define global static
#define local_persist static

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

global int WindowWidth;
global int WindowHeight;

using Color = i32;

#define PI       3.14159265358979323846
#define EULER    2.71828182845904523536

constexpr
internal
float
ISQRT(float number)
{
    union Reinterpret
    {
        float f;
        int i;
    };

    float threehalfs = 1.5F;

    float half = number * 0.5F;
    Reinterpret r{};
    r.f = number;
    r.i = 0x5f3759df - (r.i >> 1);
    float y = r.f;
    y = y * (threehalfs - (half * y * y));
    return number;
}

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

#define pure internal constexpr

pure float MinF32(float a, float b) { return a < b ? a : b; }
pure int   MinI32(int   a, int   b) { return a < b ? a : b; }
pure float MaxF32(float a, float b) { return a > b ? a : b; }
pure int   MaxI32(int   a, int   b) { return a > b ? a : b; }
pure float ClampF32(float v, float low, float high) { return MinF32(MaxF32(v, low), high); }
pure i32   ClampI32(i32   v, i32   low, i32   high) { return MinI32(MaxI32(v, low), high); }

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

struct Store
{
    Particle* particles;
    ImageF trail;
    ImageF extra;
    int counter;
    std::mt19937 mt;
};

// #include "config_01.h"
// #include "config_02.h"
// #include "config_03.h"
// #include "config_04.h"
// #include "config_05.h"
// #include "config_06.h"
// #include "config_07.h"
// #include "config_08.h"
// #include "config_09.h"
#include "config_experiment.h"


#ifdef each
#undef each
#endif

#define for_every_particle(name) for (i32 name = 0; name < Sim_Particles; ++name)


internal
void
MakeImage(Image& image)
{
    i32 totalPixels = Sim_ImageWidth * Sim_ImageHeight;
    i32 totalSize = totalPixels * sizeof(i32);
    image.px = (Color*)VirtualAlloc(0, totalSize, MEM_COMMIT, PAGE_READWRITE);
}

internal
void
MakeImage(ImageF& image)
{
    i32 totalPixels = Sim_ImageWidth * Sim_ImageHeight;
    i32 totalSize = totalPixels * sizeof(float);
    image.px = (float*)VirtualAlloc(0, totalSize, MEM_COMMIT, PAGE_READWRITE);
}

internal
Vec2
ClampPos(Vec2 v)
{
    Vec2 r;
    r.x = ClampF32(v.x, (float)0, (float)Sim_ImageWidth  - 1);
    r.y = ClampF32(v.y, (float)0, (float)Sim_ImageHeight - 1);
    return r;
}
internal
Vec2i
ClampPos(Vec2i v)
{
    Vec2i r;
    r.x = ClampI32(v.x, 0, Sim_ImageWidth - 1);
    r.y = ClampI32(v.y, 0, Sim_ImageHeight - 1);
    return r;
}


internal
void
Normalize(float& x, float& y)
{
    float len = ISQRT(x * x + y * y);
    x *= len;
    y *= len;
}

internal
Vec2
Normalize(Vec2 v)
{
    Vec2 r = {};
    float one_over_len = ISQRT(v.x * v.x + v.y * v.y);
    r.x = v.x * one_over_len;
    r.y = v.y * one_over_len;
    return r;
}

internal
Vec2
Rotate(Vec2 v, float rads)
{
    Vec2 r = {};
    float rotSin = sinf(rads);
    float rotCos = cosf(rads);
    r.x = v.x * rotCos - v.y * rotSin;
    r.y = v.x * rotSin + v.y * rotCos;
    return r;
}
internal
Vec2 
RandomOrientation(std::mt19937& rng)
{
    std::uniform_real_distribution<float> dirDist(0, PI*2);

    float rads = dirDist(rng);
    return Vec2{ cosf(rads), sinf(rads) };
}

internal
void
InitSim(Store& store)
{
    store.particles = (Particle*) VirtualAlloc(
        0, 
        Sim_Particles * sizeof(Particle), 
        MEM_COMMIT, 
        PAGE_READWRITE
    );

    MakeImage(store.trail);
    MakeImage(store.extra);

    // std::uniform_real_distribution<float> posDistX((float)0, (float)(Sim_ImageWidth  - 1));
    // std::uniform_real_distribution<float> posDistY((float)0, (float)(Sim_ImageHeight - 1));

    std::uniform_real_distribution<float> posDistX(0, 1);
    std::uniform_real_distribution<float> posDistY(0, 1);

    for (int i = 0; i < Sim_Particles; ++i) {
        auto& p = store.particles[i];
        Vec2 pos = {};
        pos.x = Sim_ImageWidth  / 2 + (2 * posDistX(store.mt) - 1) * 32;
        pos.y = Sim_ImageHeight / 2 + (2 * posDistY(store.mt) - 1) * 32;
        p.pos = ClampPos(pos);
        p.dir = RandomOrientation(store.mt);
    }
}

internal
void
Deposit(float* trail, Vec2 pos, float dt)
{
    Vec2i ipos = ToVec2i(pos);
    i32 index = ipos.x + ipos.y * Sim_ImageWidth;
    trail[index] = MinF32(1, trail[index] + Sim_TrailWeight * dt);
}

internal
void
Diffuse(float* __restrict px, float* __restrict tmp, float dt)
{
    if (px == tmp)
        return;
    
    auto f = Sim_DiffuseFilterHW;
    auto W = Sim_ImageWidth;
    auto H = Sim_ImageHeight;
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            float sum = 0;
            for (int fy = -f; fy <= f; ++fy) {
                for (int fx = -f; fx <= f; ++fx) {
                    int ex = fx + x;
                    int ey = fy + y;

                    ex = ClampI32(ex, 0, W - 1);
                    ey = ClampI32(ey, 0, H - 1);

                    sum += px[ex + ey * W];
                }
            }
            tmp[x + y * W] = sum / 9.0f;
        }
    }

    float diffuseWeight = ClampF32(Sim_DiffuseRate * dt, 0, 1);
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            int i = x + y * W;
            float blurred = px[i] * (1 - diffuseWeight) + tmp[i] * diffuseWeight;
            float burredAndDecayed = MaxF32(0, blurred * ClampF32(1 - Sim_DecayRate * dt, 0, 1));
            px[i] = burredAndDecayed;
            // px[i] = MaxF32(0, blurred - Sim_DecayRate * dt);
        }
    }
}

internal
float
Simulation_SensorSample(float* px, Vec2 center)
{
    constexpr float totalSamples = float((Sim_SensorSize * 2) | 1);
   
    float sum = 0;

    for (int i = -Sim_SensorSize; i <= Sim_SensorSize; ++i) {
        for (int j = -Sim_SensorSize; j <= Sim_SensorSize; ++j) {
            Vec2i p;
            p.x = j + (int)center.x;
            p.y = i + (int)center.y;
            p = ClampPos(p);
            sum += 3 * px[p.x + p.y * Sim_ImageWidth];
        }
    }

    return  sum / totalSamples;
}


internal
void
Simulation_Monitor(Store& store, float dt)
{
    float* trail = store.trail.px;

    // TODO: try to vectorize/paralelize
    for_every_particle(i)
    {
        Particle p = store.particles[i];
        Vec2 newPos = p.pos + p.dir * (Sim_MoveSpeed * dt);

        i32 outOfBounds = {};
        outOfBounds |= newPos.x < 0;
        outOfBounds |= newPos.y < 0;
        outOfBounds |= newPos.x >= Sim_ImageWidth;
        outOfBounds |= newPos.y >= Sim_ImageHeight;

        if (outOfBounds) {
            newPos = ClampPos(newPos);
            store.particles[i].dir = RandomOrientation(store.mt);
        } else {
            Deposit(trail, newPos, dt);
            store.particles[i].pos = newPos;
        }
    }
}

internal void
Simulation_Sensor(Store& store, float dt)
{
    float radsSensor = Sim_SensorAngle * PI / 180.f;
    std::uniform_real_distribution<float> steerDist(0.f, 1.f);

    float* trail = store.trail.px;
  
    // TODO: try to vectorize/paralelize
    for_every_particle(i)
    {
        Particle p = store.particles[i];

        Vec2 sensorPosL = p.pos + Rotate(p.dir, +radsSensor) * Sim_SensorOffset;
        Vec2 sensorPosC = p.pos +        p.dir               * Sim_SensorOffset;
        Vec2 sensorPosR = p.pos + Rotate(p.dir, -radsSensor) * Sim_SensorOffset;

        float sampleL = Simulation_SensorSample(trail, sensorPosL);
        float sampleC = Simulation_SensorSample(trail, sensorPosC);
        float sampleR = Simulation_SensorSample(trail, sensorPosR);

        float randomStrength = steerDist(store.mt);
        float turnSpeed = (Sim_TurnAngle / 360.0f) * 2 * PI * dt;

        if (p.pos.y > Sim_ImageHeight - Sim_BorderRepulsionMinRange ||
            p.pos.y < Sim_BorderRepulsionMinRange ||
            p.pos.x > Sim_ImageWidth - Sim_BorderRepulsionMinRange ||
            p.pos.x < Sim_BorderRepulsionMinRange) {
            randomStrength *= 1.0f + Sim_BorderRepulsionStrength;
        }

        if (sampleC > sampleL && sampleC > sampleR) {
        } else if (sampleC < sampleL && sampleC < sampleR) {
            store.particles[i].dir = Rotate(p.dir, (randomStrength - 0.5f) * 2.0f * Sim_RandomAutoTurnFactor);
        } else if (sampleR > sampleL) {
            store.particles[i].dir = Rotate(p.dir, -randomStrength * turnSpeed);
        } else if (sampleL > sampleR) {
            store.particles[i].dir = Rotate(p.dir, +randomStrength * turnSpeed);
        }
    }
}
internal
void
Simulate(Store& store, float dt)
{
    float* trail = store.trail.px;
    Simulation_Sensor(store, dt);
    Simulation_Monitor(store, dt);
    Diffuse(trail, store.extra.px, dt);
}

internal
LRESULT
CALLBACK
Win32_WindowTick(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
        case WM_SIZE: {
            if (wParam != SIZE_MINIMIZED) {
                return 0;
            }
        } break;
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(window, message, wParam, lParam);
}

int CALLBACK
WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    LARGE_INTEGER liFreq;
    QueryPerformanceFrequency(&liFreq);
    u64 freq = liFreq.QuadPart;

    WNDCLASS windowClass = {};
    windowClass.hInstance = instance;
    windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    windowClass.lpszClassName = "CoolTM";
    windowClass.lpfnWndProc = &Win32_WindowTick;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&windowClass);

    WindowWidth = 1280;
    WindowHeight = 720;

    int mouseX = 0;
    int mouseY = 0;

    HWND window = CreateWindowExA(
        0,
        windowClass.lpszClassName,
        "CoolTM",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WindowWidth,
        WindowHeight,
        0,
        0,
        instance,
        0);

    {
        RECT r;
        GetWindowRect(window, &r);
        r.right = r.left + WindowWidth;
        r.bottom = r.top + WindowHeight;

        AdjustWindowRect(&r, GetWindowLong(window, GWL_STYLE), false);
        SetWindowPos(
            window,
            0,
            r.left,
            r.top,
            r.right - r.left,
            r.bottom - r.top,
            SWP_NOZORDER | SWP_NOACTIVATE);

        UpdateWindow(window);
    }

    ShowWindow(window, SW_SHOWNORMAL);

    HDC dc = GetDC(window);

    BITMAPINFOHEADER bmiHeader = {};
    bmiHeader.biSize = sizeof(BITMAPINFO);
    bmiHeader.biWidth = Sim_ImageWidth;
    bmiHeader.biHeight = -Sim_ImageHeight;
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 32;
    bmiHeader.biCompression = BI_RGB;
    bmiHeader.biSizeImage = 0;
    bmiHeader.biXPelsPerMeter = 0;
    bmiHeader.biYPelsPerMeter = 0;
    bmiHeader.biClrUsed = 0;
    bmiHeader.biClrImportant = 0;

    Store store = {};
    InitSim(store);
    Image draw;
    MakeImage(draw);
    ImageF& trail = store.trail;

    constexpr auto Now = [] {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        return li.QuadPart;
    };

    freq /= 1000;
    i64 start = Now() / freq;
    i64 elapsed = 0;

    void* mb128 = &store;

    bool left = false;
    bool middle = false;
    bool right = false;

    float dt = Sim_TimeMultiplier;
    while (true) {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT || msg.message == WM_DESTROY ||
                msg.message == WM_CLOSE)
                return 0;

            if (msg.message == WM_LBUTTONDOWN)
                left = true;
            if (msg.message == WM_LBUTTONUP)
                left = false;
            if (msg.message == WM_RBUTTONDOWN)
                right = true;
            if (msg.message == WM_RBUTTONUP)
                right = false;
            if (msg.message == WM_MBUTTONDOWN)
                middle = true;
            if (msg.message == WM_MBUTTONUP)
                middle = false;
            if (msg.message == WM_MOUSEMOVE || msg.message == WM_MOUSEHOVER) {
                mouseX = (int)(short)LOWORD(msg.lParam);
                mouseY = (int)(short)HIWORD(msg.lParam);
            }
        }
        
        if (!left) {
            Simulate(store, dt);
        } else {
            Sleep(100);
        }

        i64 now = Now() / freq;
        i64 took = now - start;
        elapsed += took;
        start = now;

        if (middle) {
            // TODO: re-implement this
        }
        if (elapsed > 16) {
            elapsed -= 16;

            for (int y = 0; y < Sim_ImageHeight; ++y)
                for (int x = 0; x < Sim_ImageWidth; ++x) {
                    int i = x + y * Sim_ImageWidth;
                    float grayFPx = MinF32(trail.px[i], 1.0f);
                    int grayPx = int(grayFPx * 255);
                    int rgbPx = grayPx | (grayPx << 8) | (grayPx << 16);
                    draw.px[i] = rgbPx;
                }

            if (left) {
                // NOTE: Sample the Trail map.
                int i = mouseX + mouseY * Sim_ImageWidth;
                char buf[10] = {};
                float value = trail.px[i];
                snprintf(buf, sizeof(buf), "%02.2f\n", value);
                OutputDebugStringA(buf);
            }

            SetDIBitsToDevice(
                dc,
                0,
                0,
                WindowWidth,
                WindowHeight,
                0,
                0,
                0,
                Sim_ImageHeight,
                draw.px,
                (BITMAPINFO*)&bmiHeader,
                DIB_RGB_COLORS);
    
            // char buf [128];
            // sprintf(buf, "%04dms\n", (int)took);
            // OutputDebugStringA(buf);
        }
    }
    ReleaseDC(window, dc);
    return 0;
}