#ifndef CONFIG_12_H
#define CONFIG_12_H

constexpr static float Sim_TimeMultiplier   =   0.20f ;
constexpr static   int Sim_Particles        = 150000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.27f;
constexpr static float Sim_DiffuseRate      =    25.00f;
constexpr static float Sim_TrailWeight      =    .10f;
constexpr static float Sim_SensorAngle      =  24.00f;
constexpr static   int Sim_SensorSize       =    1   ;
constexpr static float Sim_MoveSpeed        =   02.00f;
constexpr static float Sim_SensorOffset     =  12.00f;
constexpr static float Sim_TurnAngle        =   58.00f;

constexpr static float Sim_RandomAutoTurnFactor    = 0.20f;
constexpr static   int Sim_BorderRepulsionMinRange = 10    ;
constexpr static   int Sim_BorderRepulsionStrength = 2.00f;

#endif
