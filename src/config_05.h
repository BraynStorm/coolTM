#ifndef CONFIG_05_H
#define CONFIG_05_H

constexpr static float Sim_TimeMultiplier   =   0.10f ;
constexpr static   int Sim_Particles        = 40000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.75f;
constexpr static float Sim_DiffuseRate      =    5.00f;
constexpr static float Sim_TrailWeight      =    2.00f;
constexpr static float Sim_SensorAngle      =  112.00f;
constexpr static   int Sim_SensorSize       =    3    ;
constexpr static float Sim_MoveSpeed        =   30.00f;
constexpr static float Sim_SensorOffset     =   20.00f;
constexpr static float Sim_TurnAngle        =   50.00f;

constexpr static float Sim_RandomAutoTurnFactor    =  0.90f;
constexpr static   int Sim_BorderRepulsionMinRange = 300    ;
constexpr static   int Sim_BorderRepulsionStrength = 5.00f;

#endif
