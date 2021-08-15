#ifndef CONFIG_09_H
#define CONFIG_09_H

constexpr static float Sim_TimeMultiplier   =   0.10f ;
constexpr static   int Sim_Particles        = 150000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.95f;
constexpr static float Sim_DiffuseRate      =    5.00f;
constexpr static float Sim_TrailWeight      =    .10f;
constexpr static float Sim_SensorAngle      =  50.00f;
constexpr static   int Sim_SensorSize       =    1    ;
constexpr static float Sim_MoveSpeed        =   30.00f;
constexpr static float Sim_SensorOffset     =   20.00f;
constexpr static float Sim_TurnAngle        =   150.00f;

constexpr static float Sim_RandomAutoTurnFactor    = 0.20f;
constexpr static   int Sim_BorderRepulsionMinRange = 0    ;
constexpr static   int Sim_BorderRepulsionStrength = 1.00f;


#endif
