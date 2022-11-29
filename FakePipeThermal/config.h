#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER FakePipeThermal
#define INSTANTIATION_TOKEN "{D083415C-0907-40D5-A12F-8D45990322C3}"

#define CO_SIMULATION
#define MODEL_EXCHANGE

#define FIXED_SOLVER_STEP 0.1
#define DEFAULT_STOP_TIME 2

// define model size
#define NX 0
#define NZ 0

#define SET_FLOAT64
#define GET_FLOAT64

typedef enum {
    vr_time,
	vr_contact_area_fixed_param,
	vr_wall_temp_cts_in,
	vr_fluid_temp_cts_out,
	vr_fluid_htc_cts_out
} ValueReference;

typedef struct {

    double contact_area_fixed_param;
    double wall_temp_cts_in;
    double fluid_temp_cts_out;
    double fluid_htc_cts_out;

} ModelData;

#endif /* config_h */
