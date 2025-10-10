#pragma once

struct MDLRS_Module;

struct MDLRS_Port_group;
struct MDLRS_Modularis;

struct MDLRS_Port_group *MDLRS_Module_get_inputs(struct MDLRS_Module *self);
struct MDLRS_Port_group *MDLRS_Module_get_outputs(struct MDLRS_Module *self);
struct MDLRS_Modularis *MDLRS_Module_get_project(struct MDLRS_Module *self);
void MDLRS_Module_disconnect(struct MDLRS_Module *self);