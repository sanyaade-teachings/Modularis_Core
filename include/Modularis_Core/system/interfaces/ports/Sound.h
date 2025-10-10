#pragma once

#include <Modularis_Core/system/types/Sound_value.h>

struct MDLRS_Sound;

struct MDLRS_Module;

struct MDLRS_Sound *MDLRS_Sound_new(struct MDLRS_Module *module);
void MDLRS_Sound_init(struct MDLRS_Sound *self, struct MDLRS_Module *module);
void MDLRS_Sound_set(struct MDLRS_Sound *self, MDLRS_Sound_value frame);
MDLRS_Sound_value MDLRS_Sound_get(struct MDLRS_Sound *self);
void MDLRS_Sound_deinit(struct MDLRS_Sound *self);
void MDLRS_Sound_remove(struct MDLRS_Sound *self);