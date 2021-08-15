#ifndef CONFIG_19_H
#define CONFIG_19_H

constexpr static float Sim_TimeMultiplier   =   0.3f ;
constexpr static   int Sim_Particles        = 150000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.10f;
constexpr static float Sim_DiffuseRate      =    0.65f;
constexpr static float Sim_TrailWeight      =    .20f;
constexpr static float Sim_SensorAngle      =  20.00f;
constexpr static   int Sim_SensorSize       =    1   ;
constexpr static float Sim_MoveSpeed        =  4.00f;
constexpr static float Sim_SensorOffset     =  35.00f;
constexpr static float Sim_TurnAngle        =  150.00f;

constexpr static float Sim_RandomAutoTurnFactor    = 1.0f;
constexpr static   int Sim_BorderRepulsionMinRange = 140    ;
constexpr static   int Sim_BorderRepulsionStrength = 0.0f;


#endif
