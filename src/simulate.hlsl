struct Particle {
    float2 pos;
    float2 dir;
};

struct Simulation {
    float  DT;
    float  MoveSpeed;
    float  SensorOffset;
    float  TurnAngle;

    float  RandomAutoTurnFactor;
    float  DecayRate;
    float  DiffuseRate;
    float  TrailWeight;

    float  SensorAngle;
    // float wasted
    // float wasted
    // float wasted

    int   Particles;
    int   W;
    int   H;
    int   DiffuseFilterHW;

    int   SensorSize;
    int   BorderRepulsionMinRange;
    int   BorderRepulsionStrength;
    // int wasted
};

StructuredBuffer<Simulation>          inSIM;
Texture2D<float1>                   inTRAIL;

RWStructuredBuffer<Particle> inoutPARTICLES;
RWTexture2D<float1>              inoutTRAIL;

[numthreads(320,1,1)]
void
Simulate_Monitor(
    uint3 id : SV_DISPATCHTHREADID
) {
    Simulation S = inSIM[0];
    uint idx = id.x;
    
    float2 pos = inoutPARTICLES[idx].pos;
    float2 dir = inoutPARTICLES[idx].dir;

    float2 newPos = pos + dir * S.DT * S.MoveSpeed;

    float2 clampMin = float2(0,       0      ) + 5;
    float2 clampMax = float2(S.W - 1, S.H - 1) - 5;

    if (newPos.x < clampMin.x || newPos.x > clampMax.x) {
        dir = reflect(dir, float2(1, 0));
    } else if (newPos.y > clampMax.y || newPos.y < clampMin.y) {
        dir = reflect(dir, float2(0, 1));
    }

    newPos = clamp(newPos, clampMin, clampMax);
    pos = newPos;

    int2 ipos = int2(newPos);

    inoutPARTICLES[idx].pos = pos;
    inoutPARTICLES[idx].dir = dir;
    inoutTRAIL[ipos] = inoutTRAIL[ipos] + S.TrailWeight * S.DT;
}

// [numthreads(32,1,1)]
// void
// Simulate_Sense(
//     uint3 groupXYZ : SV_GROUPID,
//     uint  inGroupI : SV_GROUPINDEX
// ) {
//     Simulation S = inSIM[0];
//     uint idx = groupXYZ.x * 32 + inGroupI;
    
//     float2 pos = inoutPARTICLES[idx].pos;
//     float2 dir = inoutPARTICLES[idx].dir;
// }

// [numthreads(4,4,4)]
// void
// Simulate_Diffuse(
//     uint3 center_ : SV_DISPATCHTHREADID
// ) {
//     int2 center = int2(center_.xy);

//     float1 sum = 0;
//     for(int y = -1; y <= 1; ++y) {
//         for(int x = -1; x <= 1; ++x) {
//             int2 pos = int2(x, y) + center;
//             sum += inTRAIL[pos];
//         }
//     }

//     // TODO: fix the formula
//     float1 color = inTRAIL[center] * (1 - S.DiffuseRate) + (sum * S.DiffuseRate) / 9.0f;
//     outTRAIL[center] = inTRAIL[center] + sum / 9.0f;
// }