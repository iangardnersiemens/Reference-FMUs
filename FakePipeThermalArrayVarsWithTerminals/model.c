#include "config.h"
#include "model.h"


void setStartValues(ModelInstance *comp) {
    M(contact_area_fixed_param) = 1;
    for (int i = 0; i < NUMBER_OF_SEGMENTS; ++i)
    {
        M(wall_temp_cts_in)[i] = 293.15;
    }
    M(number_of_segments) = NUMBER_OF_SEGMENTS;
}

Status calculateValues(ModelInstance* comp) {
    for (int i = 0; i < NUMBER_OF_SEGMENTS; ++i) {
        M(fluid_temp_cts_out)[i] = 293.15 + (M(wall_temp_cts_in)[i] - 293.15) * 0.1;
		M(fluid_htc_cts_out)[i] = 10000 + (M(wall_temp_cts_in)[i] - 293.15) * 20.0;
	}
    return OK;
}

Status getFloat64(ModelInstance* comp, ValueReference vr, double *value, size_t *index) {
    calculateValues(comp);
    switch (vr) {
        case vr_time:
            value[(*index)++] = comp->time;
            return OK;
        case vr_contact_area_fixed_param:
            value[(*index)++] = M(contact_area_fixed_param);
            return OK;
        case vr_wall_temp_cts_in:
            for (int i = 0; i < NUMBER_OF_SEGMENTS; ++i) {
                value[(*index)++] = M(wall_temp_cts_in)[i];
            }
            return OK;
        case vr_fluid_temp_cts_out:
            for (int i = 0; i < NUMBER_OF_SEGMENTS; ++i) {
                value[(*index)++] = M(fluid_temp_cts_out)[i];
            }
            return OK;
        case vr_fluid_htc_cts_out:
            for (int i = 0; i < NUMBER_OF_SEGMENTS; ++i) {
                value[(*index)++] = M(fluid_htc_cts_out)[i];
            }
            return OK;
        default:
            logError(comp, "Get Float64 is not allowed for value reference %u.", vr);
            return Error;
    }
}

Status setFloat64(ModelInstance* comp, ValueReference vr, const double *value, size_t *index) {
    switch (vr) {
        case vr_contact_area_fixed_param:
#if FMI_VERSION > 1
            if (comp->state != Instantiated &&
                comp->state != InitializationMode) {
                logError(comp, "Variable contact_area_fixed_param can only be set after instantiation, in initialization mode or event mode.");
                return Error;
            }
#endif
            M(contact_area_fixed_param) = value[(*index)++];
            return OK;
        case vr_wall_temp_cts_in:
            for (int i = 0; i < NUMBER_OF_SEGMENTS; ++i) {
                M(wall_temp_cts_in)[i] = value[(*index)++];
            }
            return OK;
        default:
            logError(comp, "Set Float64 is not allowed for value reference %u.", vr);
            return Error;
    }
}

Status getUInt64(ModelInstance* comp, ValueReference vr, uint64_t* value, size_t* index) {
    calculateValues(comp);
    switch (vr) {
	    case vr_number_of_segments_out:
            value[(*index)++] = M(number_of_segments);
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
