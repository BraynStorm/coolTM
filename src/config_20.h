#ifndef CONFIG_20_H
#define CONFIG_20_H
constexpr static float Sim_TimeMultiplier   =   0.2f ;
constexpr static   int Sim_Particles        = 150000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.75f;
constexpr static float Sim_DiffuseRate      =    4.96f;
constexpr static float Sim_TrailWeight      =    2.0f;
constexpr static float Sim_SensorAngle      =  112.00f;
constexpr static   int Sim_SensorSize       =    1   ;
constexpr static float Sim_MoveSpeed        =  30.0f;
constexpr static float Sim_SensorOffset     = 20.00f;
constexpr static float Sim_TurnAngle        =   -3 * 360; //;120.10f;

constexpr static float Sim_RandomAutoTurnFactor    = 1.0f;
constexpr static   int Sim_BorderRepulsionMinRange = 140    ;
constexpr static   int Sim_BorderRepulsionStrength = 0.0f;


#endif
