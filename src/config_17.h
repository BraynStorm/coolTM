#ifndef CONFIG_17_H
#define CONFIG_17_H


constexpr static float Sim_TimeMultiplier   =   0.50f ;
constexpr static   int Sim_Particles        = 150000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.1126f;
constexpr static float Sim_DiffuseRate      =    0.20f;
constexpr static float Sim_TrailWeight      =    .10f;
constexpr static float Sim_SensorAngle      =  60.00f;
constexpr static   int Sim_SensorSize       =    1   ;
constexpr static float Sim_MoveSpeed        =  6.00f;
constexpr static float Sim_SensorOffset     =  5.00f;
constexpr static float Sim_TurnAngle        =  161.00f;

constexpr static float Sim_RandomAutoTurnFactor    = 1.0f;
constexpr static   int Sim_BorderRepulsionMinRange = 140    ;
constexpr static   int Sim_BorderRepulsionStrength = 00.0f;


#endif
