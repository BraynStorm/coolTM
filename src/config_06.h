#ifndef CONFIG_06_H
#define CONFIG_06_H

constexpr static float Sim_TimeMultiplier   =   0.30f ;
constexpr static   int Sim_Particles        = 150000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.15f;
constexpr static float Sim_DiffuseRate      =    5.00f;
constexpr static float Sim_TrailWeight      =    2.00f;
constexpr static float Sim_SensorAngle      =  52.00f;
constexpr static   int Sim_SensorSize       =    3    ;
constexpr static float Sim_MoveSpeed        =   30.00f;
constexpr static float Sim_SensorOffset     =   20.00f;
constexpr static float Sim_TurnAngle        =   50.00f;

constexpr static float Sim_RandomAutoTurnFactor    =  0.20f;
constexpr static   int Sim_BorderRepulsionMinRange = 100    ;
constexpr static   int Sim_BorderRepulsionStrength = 2.00f;

#endif
