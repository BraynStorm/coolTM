#ifndef CONFIG_07_H
#define CONFIG_07_H

constexpr static float Sim_TimeMultiplier   =   0.30f ;
constexpr static   int Sim_Particles        = 150000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    1.00f;
constexpr static float Sim_DiffuseRate      =    10.00f;
constexpr static float Sim_TrailWeight      =    2.00f;
constexpr static float Sim_SensorAngle      =  52.00f;
constexpr static   int Sim_SensorSize       =    3    ;
constexpr static float Sim_MoveSpeed        =   10.00f;
constexpr static float Sim_SensorOffset     =   60.00f;
constexpr static float Sim_TurnAngle        =   10.00f;

constexpr static float Sim_RandomAutoTurnFactor    = 0.20f;
constexpr static   int Sim_BorderRepulsionMinRange = 0    ;
constexpr static   int Sim_BorderRepulsionStrength = 1.00f;

#endif
