#include <Modularis_Core/user/modules/players/Sequencer/Note_key.h>

#include <Modularis_Core/user/modules/players/Sequencer/Interpolation.h>
#include <math.h>

float MDLRS_Note_key_get_value(struct MDLRS_Note_key *self, float position)
{
	switch (self->p.curve)
	{
		case INTERPOLATION_NONE: return self->p.value;
		case INTERPOLATION_LINEAR: return self->p.value+(self[1].p.value-self->p.value)*position/self->p.duration;
		case INTERPOLATION_FAST: return self->p.value+(self[1].p.value-self->p.value)*(1-(self->p.duration-position)*(self->p.duration-position)/(self->p.duration*self->p.duration));
		case INTERPOLATION_SLOW: return self->p.value+(self[1].p.value-self->p.value)*position*position/(self->p.duration*self->p.duration);
		case INTERPOLATION_SMOOTH: return self->p.value+(self[1].p.value-self->p.value)*(1-cosf((float)M_PI*position/self->p.duration))/2;
	}
}