#ifndef CONFIG_18_H
#define CONFIG_18_H

constexpr static float Sim_TimeMultiplier   =   0.20f ;
constexpr static   int Sim_Particles        = 150000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.6f;
constexpr static float Sim_DiffuseRate      =    0.64f;
constexpr static float Sim_TrailWeight      =    .60f;
constexpr static float Sim_SensorAngle      =  90.00f;
constexpr static   int Sim_SensorSize       =    1   ;
constexpr static float Sim_MoveSpeed        =  8.00f;
constexpr static float Sim_SensorOffset     =  16.00f;
constexpr static float Sim_TurnAngle        =  100.00f;

constexpr static float Sim_RandomAutoTurnFactor    = 6.0f;
constexpr static   int Sim_BorderRepulsionMinRange = 140    ;
constexpr static   int Sim_BorderRepulsionStrength = 0.0f;


#endif
