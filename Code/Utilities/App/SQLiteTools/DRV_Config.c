#include "DRV_Config.h"

static DRVConfig G_DRVConfig = { 0 };


DRVConfig* get_G_DRVConfig()
{
    return &G_DRVConfig;
}
