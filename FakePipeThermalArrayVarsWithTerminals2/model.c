#include "config.h"
#include "model.h"


void setStartValues(ModelInstance *comp) {
	const double start_mesh_vertices[NUMBER_OF_VERTICES][DIMENSION] = {
    	{0.0, 0.0, 0.0},
    	{0.2, 0.0, 0.0},
        {0.4, 0.0, 0.0},
        {0.6, 0.0, 0.0},
        {0.8, 0.0, 0.0},
        {1.0, 0.0, 0.0}
    };
    
    for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i)
    {
        M(wall_temp_cts_in)[i] = 293.15;
    }

    for (int j=0; j<NUMBER_OF_VERTICES; ++j)
        for (int k=0; k<DIMENSION; ++k)
            M(grid_coordinates_out)[j][k] = start_mesh_vertices[j][k];
    

    const unsigned long long start_elements_type[NUMBER_OF_ELEMENTS] = { 3,3,3,3,3 };
    for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i)
    {
        M(element_type_out)[i] = start_elements_type[i];
    }

    const unsigned long long start_mesh_vertices_per_element[ELEMENT_CONNECTIVITY] = { 0,1,1,2,2,3,3,4,4,5 };
    for (int i = 0; i < ELEMENT_CONNECTIVITY; ++i)
    {
        M(element_connectivity_out)[i] = start_mesh_vertices_per_element[i];
    }

    const unsigned long long start_element_list_z1[NUMBER_OF_ELEMENTS] = { 0,1,2,3,4 };
    for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i)
    {
        M(element_list_z1_out)[i] = start_element_list_z1[i];
    }

    M(contact_area_fixed_param) = 1;
    M(number_of_elements_out) = NUMBER_OF_ELEMENTS;
    M(grid_location_z1_out) = GRID_LOCATION;
    M(element_dimensionality_out) = FLOMASTER_ELEMENT_DIMENSIONALITY;
}

Status calculateValues(ModelInstance* comp) {
    for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i) {
        M(fluid_temp_cts_out)[i] = 293.15 + (M(wall_temp_cts_in)[i] - 293.15) * 0.1;
		M(fluid_htc_cts_out)[i] = 10000 + (M(wall_temp_cts_in)[i] - 293.15) * 20.0;
        M(surface_area_out)[i] = M(contact_area_fixed_param) / NUMBER_OF_ELEMENTS;
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
        case vr_surface_area_out:
            ASSERT_NVALUES(NUMBER_OF_ELEMENTS);
            for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i) {
                values[(*index)++] = M(surface_area_out)[i];
            }
            return OK;
        case vr_fluid_htc_cts_out:
            ASSERT_NVALUES(NUMBER_OF_ELEMENTS);
            for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i) {
                values[(*index)++] = M(fluid_htc_cts_out)[i];
            }
            return OK;
        case vr_grid_coordinates_out:
            ASSERT_NVALUES(NUMBER_OF_VERTICES * DIMENSION);
            for (int i = 0; i < NUMBER_OF_VERTICES; ++i) {
                for (int j = 0; j < DIMENSION; ++j) {
                    values[(*index)++] = M(grid_coordinates_out)[i][j];
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

//maybe should be int64
Status getUInt64(ModelInstance* comp, ValueReference vr, uint64_t values[], size_t nValues, size_t* index) {
    calculateValues(comp);
    switch (vr) {
    case vr_element_dimensionality_out:
        ASSERT_NVALUES(1);
        values[(*index)++] = M(element_dimensionality_out);
        return OK;
    case vr_element_type_out:
        ASSERT_NVALUES(NUMBER_OF_ELEMENTS);
        for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i)
        {
            values[(*index)++] = M(element_type_out)[i];
        }
        return OK;
    case vr_element_connectivity_out:
        ASSERT_NVALUES(ELEMENT_CONNECTIVITY);
        for (int i = 0; i < ELEMENT_CONNECTIVITY; ++i)
        {
            values[(*index)++] = M(element_connectivity_out)[i];
        }
        return OK;

    case vr_element_list_z1_out:
        ASSERT_NVALUES(NUMBER_OF_ELEMENTS);
        for (int i = 0; i < NUMBER_OF_ELEMENTS; ++i)
        {
            values[(*index)++] = M(element_list_z1_out)[i];
        }
        return OK;
    case vr_grid_location_z1_out:
        ASSERT_NVALUES(1);
        values[(*index)++] = M(grid_location_z1_out);
        return OK;
    case vr_number_of_elements_out:
        ASSERT_NVALUES(1);
        values[(*index)++] = M(number_of_elements_out);
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
