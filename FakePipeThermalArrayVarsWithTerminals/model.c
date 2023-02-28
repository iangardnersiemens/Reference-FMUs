#include "config.h"
#include "model.h"


void setStartValues(ModelInstance *comp) {
	const double start_mesh_vertices[NUMBER_OF_VERTICES][DIMENSION] = {
    	{0.0, 0.0, 0.0},
    	{0.0, 0.0, 0.2},
        {0.0, 0.0, 0.4},
        {0.0, 0.0, 0.6},
        {0.0, 0.0, 0.8},
        {0.0, 0.0, 1.0}
    };
    M(contact_area_fixed_param) = 1;
    for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i)
    {
        M(wall_temp_cts_in)[i] = 293.15;
    }
    for (int j=0; j<NUMBER_OF_VERTICES; ++j)
        for (int k=0; k<DIMENSION; ++k)
            M(mesh_vertices_out)[j][k] = start_mesh_vertices[j][k];
    M(number_of_elements) = NUMBER_OF_ELEMENTS;
}

Status calculateValues(ModelInstance* comp) {
    for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i) {
        M(fluid_temp_cts_out)[i] = 293.15 + (M(wall_temp_cts_in)[i] - 293.15) * 0.1;
		M(fluid_htc_cts_out)[i] = 10000 + (M(wall_temp_cts_in)[i] - 293.15) * 20.0;
	}
    return OK;
}

Status getFloat64(ModelInstance* comp, ValueReference vr, double values[], size_t nValues, size_t* index) {
    calculateValues(comp);
    switch (vr) {
        case vr_time:
            ASSERT_NVALUES(1);
            values[(*index)++] = comp->time;
            return OK;
        case vr_contact_area_fixed_param:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(contact_area_fixed_param);
            return OK;
        case vr_wall_temp_cts_in:
            ASSERT_NVALUES(NUMBER_OF_ELEMENTS);
            for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i) {
                values[(*index)++] = M(wall_temp_cts_in)[i];
            }
            return OK;
        case vr_fluid_temp_cts_out:
            ASSERT_NVALUES(NUMBER_OF_ELEMENTS);
            for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i) {
                values[(*index)++] = M(fluid_temp_cts_out)[i];
            }
            return OK;
        case vr_fluid_htc_cts_out:
            ASSERT_NVALUES(NUMBER_OF_ELEMENTS);
            for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i) {
                values[(*index)++] = M(fluid_htc_cts_out)[i];
            }
            return OK;
        case vr_mesh_vertices_out:
            ASSERT_NVALUES(NUMBER_OF_VERTICES * DIMENSION);
            for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i) {
                for (int j = 0; j < DIMENSION; ++j) {
                    values[(*index)++] = M(mesh_vertices_out)[i][j];
                }
            }
            return OK;
        default:
            logError(comp, "Get Float64 is not allowed for value reference %u.", vr);
            return Error;
    }
}

Status setFloat64(ModelInstance* comp, ValueReference vr, const double values[], size_t nValues, size_t* index) {
    switch (vr) {
        case vr_contact_area_fixed_param:
            ASSERT_NVALUES(1);
#if FMI_VERSION > 1
            if (comp->state != Instantiated &&
                comp->state != InitializationMode) {
                logError(comp, "Variable contact_area_fixed_param can only be set after instantiation, in initialization mode or event mode.");
                return Error;
            }
#endif
            M(contact_area_fixed_param) = values[(*index)++];
            return OK;
        case vr_wall_temp_cts_in:
            ASSERT_NVALUES(NUMBER_OF_ELEMENTS);
            for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i) {
                M(wall_temp_cts_in)[i] = values[(*index)++];
            }
            return OK;
        default:
            logError(comp, "Set Float64 is not allowed for value reference %u.", vr);
            return Error;
    }
}

Status getUInt64(ModelInstance* comp, ValueReference vr, uint64_t values[], size_t nValues, size_t* index) {
    calculateValues(comp);
    switch (vr) {
	    case vr_number_of_elements_out:
            ASSERT_NVALUES(1);
            values[(*index)++] = M(number_of_elements);
	        return OK;
	    default:
	        logError(comp, "Get UInt64 is not allowed for value reference %u.", vr);
	        return Error;
    }
}


void eventUpdate(ModelInstance *comp) {
    comp->valuesOfContinuousStatesChanged   = false;
    comp->nominalsOfContinuousStatesChanged = false;
    comp->terminateSimulation               = false;
    comp->nextEventTimeDefined              = false;
}
