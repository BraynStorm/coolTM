#ifndef CONFIG_10_H
#define CONFIG_10_H

constexpr static float Sim_TimeMultiplier   =   0.20f ;
constexpr static   int Sim_Particles        = 150000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.35f;
constexpr static float Sim_DiffuseRate      =    5.00f;
constexpr static float Sim_TrailWeight      =    .80f;
constexpr static float Sim_SensorAngle      =  130.00f;
constexpr static   int Sim_SensorSize       =    1   ;
constexpr static float Sim_MoveSpeed        =   12.00f;
constexpr static float Sim_SensorOffset     =  6.00f;
constexpr static float Sim_TurnAngle        =   -30.00f;

constexpr static float Sim_RandomAutoTurnFactor    = 0.20f;
constexpr static   int Sim_BorderRepulsionMinRange = 300    ;
constexpr static   int Sim_BorderRepulsionStrength = 2.00f;


#endif
