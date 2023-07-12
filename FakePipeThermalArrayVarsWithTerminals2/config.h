#ifndef config_h
#define config_h

// define class name and unique id
#define MODEL_IDENTIFIER FakePipeThermalArrayVarsWithTerminals2
#define INSTANTIATION_TOKEN "{E083415C-0907-40D5-A12F-8D45990322C3}"

#define CO_SIMULATION
#define MODEL_EXCHANGE

#define FIXED_SOLVER_STEP 0.1
#define DEFAULT_STOP_TIME 2

// define model size
#define NX 0
#define NZ 0

#define SET_FLOAT64
#define GET_FLOAT64
#define GET_UINT64

#define GRID_LOCATION (0)
#define FLOMASTER_ELEMENT_DIMENSIONALITY (1)
#define NUMBER_OF_ELEMENTS (5)
#define NUMBER_OF_VERTICES (6)
#define ELEMENT_CONNECTIVITY (10)
#define DIMENSION (3)

typedef enum {
    vr_time,
    vr_contact_area_fixed_param,
    vr_wall_temp_cts_in,
    vr_number_of_elements_out,
    vr_element_dimensionality_out,
    vr_grid_coordinates_out,
    vr_element_type_out,
    vr_element_connectivity_out,
    vr_element_list_z1_out,
    vr_grid_location_z1_out,
    vr_surface_area_out,
    vr_fluid_temp_cts_out,
    vr_fluid_htc_cts_out
} ValueReference;

typedef struct {
    double contact_area_fixed_param;
    double wall_temp_cts_in[NUMBER_OF_ELEMENTS];
    unsigned long long number_of_elements_out;
    unsigned long long element_dimensionality_out;
    double grid_coordinates_out[NUMBER_OF_VERTICES][DIMENSION];
    unsigned long long element_type_out[NUMBER_OF_ELEMENTS];
    unsigned long long element_connectivity_out[ELEMENT_CONNECTIVITY];
    unsigned long long element_list_z1_out[NUMBER_OF_ELEMENTS];
    unsigned long long grid_location_z1_out;
    double surface_area_out[NUMBER_OF_ELEMENTS];
    double fluid_temp_cts_out[NUMBER_OF_ELEMENTS];
    double fluid_htc_cts_out[NUMBER_OF_ELEMENTS];
} ModelData;

#endif /* config_h */
