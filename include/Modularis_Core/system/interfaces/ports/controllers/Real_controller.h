#pragma once

struct MDLRS_Real_controller;

struct MDLRS_Module;

struct MDLRS_Real_controller *MDLRS_Real_controller_new(struct MDLRS_Module *module, float value);
void MDLRS_Real_controller_init(struct MDLRS_Real_controller *self, struct MDLRS_Module *module, float value);
void MDLRS_Real_controller_set(struct MDLRS_Real_controller *self, float value);
float MDLRS_Real_controller_get(struct MDLRS_Real_controller *self);
void MDLRS_Real_controller_deinit(struct MDLRS_Real_controller *self);
void MDLRS_Real_controller_remove(struct MDLRS_Real_controller *self);