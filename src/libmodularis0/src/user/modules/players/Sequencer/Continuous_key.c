#include <Modularis_Core/user/modules/players/Sequencer/Continuous_key.h>

#include <Modularis_Core/user/modules/players/Sequencer/Interpolation.h>
#include <math.h>

float MDLRS_Continuous_key_get_value(struct MDLRS_Continuous_key *self, float position)
{
	switch (self->curve)
	{
		case INTERPOLATION_NONE: return self->value;
		case INTERPOLATION_LINEAR: return self->value+(self[1].value-self->value)*position/self->duration;
		case INTERPOLATION_FAST: return self->value+(self[1].value-self->value)*(1-(self->duration-position)*(self->duration-position)/(self->duration*self->duration));
		case INTERPOLATION_SLOW: return self->value+(self[1].value-self->value)*position*position/(self->duration*self->duration);
		case INTERPOLATION_SMOOTH: return self->value+(self[1].value-self->value)*(1-cosf((float)M_PI*position/self->duration))/2;
	}
}