#ifndef CONFIG_13_H
#define CONFIG_13_H


constexpr static float Sim_TimeMultiplier   =   0.50f ;
constexpr static   int Sim_Particles        = 150000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.13f;
constexpr static float Sim_DiffuseRate      =    0.17f;
constexpr static float Sim_TrailWeight      =    .10f;
constexpr static float Sim_SensorAngle      =  24.00f;
constexpr static   int Sim_SensorSize       =    1   ;
constexpr static float Sim_MoveSpeed        =   6.00f;
constexpr static float Sim_SensorOffset     =  42.00f;
constexpr static float Sim_TurnAngle        =  116.00f;

constexpr static float Sim_RandomAutoTurnFactor    = 0.20f;
constexpr static   int Sim_BorderRepulsionMinRange = 10    ;
constexpr static   int Sim_BorderRepulsionStrength = 2.00f;



#endif
