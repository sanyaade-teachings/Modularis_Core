#pragma once

#include <stdint.h>

struct MDLRS_Integer_controller;

struct MDLRS_Module;

struct MDLRS_Integer_controller *MDLRS_Integer_controller_new(struct MDLRS_Module *module, int32_t value);
void MDLRS_Integer_controller_init(struct MDLRS_Integer_controller *self, struct MDLRS_Module *module, int32_t value);
void MDLRS_Integer_controller_set(struct MDLRS_Integer_controller *self, int32_t value);
int32_t MDLRS_Integer_controller_get(struct MDLRS_Integer_controller *self);
void MDLRS_Integer_controller_deinit(struct MDLRS_Integer_controller *self);
void MDLRS_Integer_controller_remove(struct MDLRS_Integer_controller *self);