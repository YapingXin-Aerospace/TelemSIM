#include "TM_PreLoad.h"

static StepLogSet G_StepLogSet = { 0 };


StepLogSet* get_global_StepLogSet()
{
    return &G_StepLogSet;
}
