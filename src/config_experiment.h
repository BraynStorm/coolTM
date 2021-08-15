#ifndef CONFIG_EXPERIMENT_H
#define CONFIG_EXPERIMENT_H
constexpr static float Sim_TimeMultiplier   =   0.27f ;
constexpr static   int Sim_Particles        = 140000  ;
constexpr static   int Sim_ImageWidth       = 1280    ;
constexpr static   int Sim_ImageHeight      =  720    ;
constexpr static   int Sim_DiffuseFilterHW  =    1    ;
constexpr static float Sim_DecayRate        =    0.10f;
constexpr static float Sim_DiffuseRate      =    1.3f;
constexpr static float Sim_TrailWeight      =    1.0f;
constexpr static float Sim_SensorAngle      = 290.00f;
constexpr static   int Sim_SensorSize       =    3   ;
constexpr static float Sim_MoveSpeed        =  3.0f;
constexpr static float Sim_SensorOffset     = 20.00f;
constexpr static float Sim_TurnAngle        =   140; //;120.10f;

constexpr static float Sim_RandomAutoTurnFactor    = PI/.7;
constexpr static   int Sim_BorderRepulsionMinRange = 140    ;
constexpr static   int Sim_BorderRepulsionStrength = 0.0f;

#endif
