#include <system/ports/Any_port.h>

#include <system/safe memory.h>
#include <system/modules/Module.h>

void MDLRS_Any_port_dispose(struct MDLRS_Any_port *self)
{
	safe_dispose(self->module->project, self);
}