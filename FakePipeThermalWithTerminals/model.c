#include "config.h"
#include "model.h"


void setStartValues(ModelInstance *comp) {
    M(contact_area_fixed_param) = 1;
    M(wall_temp_cts_in) = 293.15;
}

Status calculateValues(ModelInstance *comp) {
    M(fluid_temp_cts_out) = 293.15 + (M(wall_temp_cts_in) - 293.15) * 0.1;
    M(fluid_htc_cts_out) = 10000 + (M(wall_temp_cts_in) - 293.15) * 20.0;
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
            value[(*index)++] = M(wall_temp_cts_in);
            return OK;
        case vr_fluid_temp_cts_out:
            value[(*index)++] = M(fluid_temp_cts_out);
            return OK;
        case vr_fluid_htc_cts_out:
            value[(*index)++] = M(fluid_htc_cts_out);
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
            M(wall_temp_cts_in) = value[(*index)++];
            return OK;
        default:
            logError(comp, "Set Float64 is not allowed for value reference %u.", vr);
            return Error;
    }
}

void eventUpdate(ModelInstance *comp) {
    comp->valuesOfContinuousStatesChanged   = false;
    comp->nominalsOfContinuousStatesChanged = false;
    comp->terminateSimulation               = false;
    comp->nextEventTimeDefined              = false;
}
