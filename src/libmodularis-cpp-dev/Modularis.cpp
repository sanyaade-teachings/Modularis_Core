/*
(C) 2025 Серый MLGamer. All freedoms preserved.
Дзен: <https://dzen.ru/seriy_mlgamer>
SoundCloud: <https://soundcloud.com/seriy_mlgamer>
YouTube: <https://www.youtube.com/@Seriy_MLGamer>
GitVerse: <https://gitverse.ru/Seriy_MLGamer>
E-mail: <Seriy-MLGamer@yandex.ru>

This file is part of Modularis Core C++.
Modularis Core C++ is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Modularis Core C++ is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Modularis Core C++. If not, see <https://www.gnu.org/licenses/>.
*/

#include <Modularis_Core_C++/Modularis.hpp>
#include <Modularis_Core_C++/extensions/modules/IModule.hpp>
#include <Modularis_Core_C++/extensions/ports/IPort.hpp>
#include <Modularis_Core_C++/modules/effects/note/Note_chorus.hpp>
#include <Modularis_Core_C++/modules/effects/note/Transpose.hpp>
#include <Modularis_Core_C++/modules/effects/sound/Amplifier.hpp>
#include <Modularis_Core_C++/modules/effects/sound/Delay.hpp>
#include <Modularis_Core_C++/modules/effects/sound/Modulator.hpp>
#include <Modularis_Core_C++/modules/players/Sequencer.hpp>
#include <Modularis_Core_C++/modules/synthesizers/Oscillator.hpp>
#include <Modularis_Core_C++/ports/controllers/ADSR.hpp>
#include <Modularis_Core_C++/ports/controllers/Integer_controller.hpp>
#include <Modularis_Core_C++/ports/controllers/Real_controller.hpp>
#include <Modularis_Core_C++/ports/Note.hpp>
#include <Modularis_Core_C++/ports/Port_group.hpp>
#include <Modularis_Core_C++/ports/Sound.hpp>
#include <Modularis_Core_C++/system/modules/Module.hpp>
#include <Modularis_Core_C++/system/ports/Note/Note_table.hpp>
#include <Modularis_Core_C++/system/ports/Any_port.hpp>
#include <Modularis_Core_C++/system/ports/Port.hpp>
#include <Modularis_Core_C++/user/modules/players/Sequencer/Continuous_key.hpp>
#include <Modularis_Core_C++/user/modules/players/Sequencer/Interpolation.hpp>
#include <cmath>
#include <Modularis_Core_C++/user/modules/players/Sequencer/Note_key.hpp>

extern "C"
{
	using namespace MDLRS;

	Modularis *MDLRS_Modularis_allocate();
	void MDLRS_Modularis_init(Modularis *self, unsigned sample_rate, unsigned channels);
	void MDLRS_Modularis_update(Modularis *self);
	Sound_value MDLRS_Modularis_get(Modularis *self, unsigned channel);
	void MDLRS_Modularis_deinit(Modularis *self);
	void MDLRS_Modularis_dispose(void *self);

	void MDLRS_IModule_init(IModule *self, Modularis *project);
	void MDLRS_IModule_deinit(IModule *self);

	void MDLRS_IPort_init(IPort *self, const char *type, Module *module);
	void MDLRS_IPort_deinit(IPort *self);

	Note_chorus *MDLRS_Note_chorus_allocate(Modularis *project);
	void MDLRS_Note_chorus_init(Note_chorus *self, Modularis *project, float spread, unsigned voices);
	void
		MDLRS_Note_chorus_set_spread(Note_chorus *self, float spread),
		MDLRS_Note_chorus_set_voices(Note_chorus *self, unsigned voices),
		MDLRS_Note_chorus_set_random_phases(Note_chorus *self, bool random_phases);
	Note *MDLRS_Note_chorus_get_input(Note_chorus *self);
	Real_controller *MDLRS_Note_chorus_get_spread(Note_chorus *self);
	Integer_controller *MDLRS_Note_chorus_get_voices(Note_chorus *self);
	Integer_controller *MDLRS_Note_chorus_get_random_phases(Note_chorus *self);
	Note *MDLRS_Note_chorus_get_output(Note_chorus *self);
	void MDLRS_Note_chorus_deinit(Note_chorus *self);

	Transpose *MDLRS_Transpose_allocate(Modularis *project);
	void MDLRS_Transpose_init(Transpose *self, Modularis *project, float transposition);
	void MDLRS_Transpose_set_transposition(Transpose *self, float transposition);
	void MDLRS_Transpose_set_velocity(Transpose *self, float velocity);
	Note *MDLRS_Transpose_get_input(Transpose *self);
	Real_controller *MDLRS_Transpose_get_transposition(Transpose *self);
	Real_controller *MDLRS_Transpose_get_velocity(Transpose *self);
	Note *MDLRS_Transpose_get_output(Transpose *self);
	void MDLRS_Transpose_deinit(Transpose *self);

	Amplifier *MDLRS_Amplifier_allocate(Modularis *project);
	void MDLRS_Amplifier_init(Amplifier *self, Modularis *project, float volume);
	void MDLRS_Amplifier_set_volume(Amplifier *self, float volume);
	Sound *MDLRS_Amplifier_get_input(Amplifier *self);
	Real_controller *MDLRS_Amplifier_get_volume(Amplifier *self);
	Sound *MDLRS_Amplifier_get_output(Amplifier *self);
	void MDLRS_Amplifier_deinit(Amplifier *self);

	Delay *MDLRS_Delay_allocate(Modularis *project);
	void MDLRS_Delay_init(Delay *self, Modularis *project, float delay);
	void MDLRS_Delay_set_delay(Delay *self, float delay);
	Sound *MDLRS_Delay_get_input(Delay *self);
	Real_controller *MDLRS_Delay_get_delay(Delay *self);
	Sound *MDLRS_Delay_get_output(Delay *self);
	void MDLRS_Delay_deinit(Delay *self);

	Modulator *MDLRS_Modulator_allocate(Modularis *project);
	void MDLRS_Modulator_init(Modulator *self, Modularis *project);
	Sound *MDLRS_Modulator_get_carrier(Modulator *self);
	Sound *MDLRS_Modulator_get_modulator(Modulator *self);
	Sound *MDLRS_Modulator_get_output(Modulator *self);
	void MDLRS_Modulator_deinit(Modulator *self);

	Sequencer *MDLRS_Sequencer_allocate(Modularis *project);
	void MDLRS_Sequencer_init(Sequencer *self, Modularis *project);
	void
		MDLRS_Sequencer_set_BPM(Sequencer *self, float BPM),
		MDLRS_Sequencer_set_LPB(Sequencer *self, float LPB),
		MDLRS_Sequencer_set_position(Sequencer *self, float cursor_position),
		MDLRS_Sequencer_set_loop(Sequencer *self, bool loop),
		MDLRS_Sequencer_set_play(Sequencer *self, bool play);
	Real_controller *MDLRS_Sequencer_get_BPM(Sequencer *self);
	Integer_controller *MDLRS_Sequencer_get_LPB(Sequencer *self);
	Real_controller *MDLRS_Sequencer_get_position(Sequencer *self);
	Integer_controller *MDLRS_Sequencer_get_loop(Sequencer *self);
	Integer_controller *MDLRS_Sequencer_get_play(Sequencer *self);
	Note *MDLRS_Sequencer_get_output(Sequencer *self);
	void MDLRS_Sequencer_add(Sequencer *self, Pattern ***tracks, unsigned track_count);
	void MDLRS_Sequencer_deinit(Sequencer *self);

	Oscillator *MDLRS_Oscillator_allocate(Modularis *project);
	void MDLRS_Oscillator_init(Oscillator *self, Modularis *project);
	void MDLRS_Oscillator_set_volume(Oscillator *self, float volume);
	void MDLRS_Oscillator_set_waveform(Oscillator *self, unsigned waveform);
	Note *MDLRS_Oscillator_get_input(Oscillator *self);
	Real_controller *MDLRS_Oscillator_get_volume(Oscillator *self);
	Integer_controller *MDLRS_Oscillator_get_waveform(Oscillator *self);
	ADSR *MDLRS_Oscillator_get_envelope(Oscillator *self);
	Sound *MDLRS_Oscillator_get_output(Oscillator *self);
	void MDLRS_Oscillator_deinit(Oscillator *self);

	ADSR *MDLRS_ADSR_allocate(Module *module);
	void MDLRS_ADSR_init_body(ADSR *self, Module *module, float attack, float decay, float sustain, float release);
	void
		MDLRS_ADSR_set_attack(ADSR *self, float attack),
		MDLRS_ADSR_set_decay(ADSR *self, float decay),
		MDLRS_ADSR_set_sustain(ADSR *self, float sustain),
		MDLRS_ADSR_set_release(ADSR *self, float release);
	Real_controller
		*MDLRS_ADSR_get_attack(ADSR *self),
		*MDLRS_ADSR_get_decay(ADSR *self),
		*MDLRS_ADSR_get_sustain(ADSR *self),
		*MDLRS_ADSR_get_release(ADSR *self);
	float MDLRS_ADSR_envelope(ADSR *self, ADSR_state state, float time);
	void MDLRS_ADSR_deinit(ADSR *self);

	Integer_controller *MDLRS_Integer_controller_allocate(Module *module);
	void MDLRS_Integer_controller_init(Integer_controller *self, Module *module, int32_t value);
	void MDLRS_Integer_controller_set(Integer_controller *self, int32_t value);
	int32_t MDLRS_Integer_controller_get(Integer_controller *self);
	void MDLRS_Integer_controller_deinit(Integer_controller *self);

	Real_controller *MDLRS_Real_controller_allocate(Module *module);
	void MDLRS_Real_controller_init(Real_controller *self, Module *module, float value);
	void MDLRS_Real_controller_set(Real_controller *self, float value);
	float MDLRS_Real_controller_get(Real_controller *self);
	void MDLRS_Real_controller_deinit(Real_controller *self);

	Note *MDLRS_Note_allocate(Module *module);
	void MDLRS_Note_init(Note *self, Module *module);
	Note_event *MDLRS_Note_get_events(Note *self);
	unsigned MDLRS_Note_get_event_count(Note *self);
	unsigned MDLRS_Note_add_start(Note *self, float pitch, float velocity, float phase);
	void MDLRS_Note_add_change(Note *self, unsigned scancode, float pitch, float velocity);
	void MDLRS_Note_add_stop(Note *self, unsigned scancode);
	void MDLRS_Note_clean(Note *self);
	void MDLRS_Note_deinit(Note *self);

	Port_group *MDLRS_Port_group_allocate(Module *module);
	void MDLRS_Port_group_init(Port_group *self, Module *module);
	int
		MDLRS_Port_group_connect(Port_group *self, Any_port *port),
		MDLRS_Port_group_connect_port(Port_group *self, Port *port),
		MDLRS_Port_group_connect_group(Port_group *self, Port_group *group),
		MDLRS_Port_group_disconnect(Port_group *self),
		MDLRS_Port_group_disconnect_from_port(Port_group *self, Any_port *port),
		MDLRS_Port_group_disconnect_port(Port_group *self, Port *port),
		MDLRS_Port_group_disconnect_group(Port_group *self, Port_group *group),
		MDLRS_Port_group_disconnect_input(Port_group *self);
	void MDLRS_Port_group_update(Port_group *self);
	void MDLRS_Port_group_get_ready(Port_group *self);
	void MDLRS_Port_group_add(Port_group *self, Any_port *port);
	Any_port *MDLRS_Port_group_port(Port_group *self, unsigned port);
	unsigned MDLRS_Port_group_count(Port_group *self);
	void MDLRS_Port_group_deinit(Port_group *self);

	Sound *MDLRS_Sound_allocate(Module *module);
	void MDLRS_Sound_init(Sound *self, Module *module);
	void MDLRS_Sound_set(Sound *self, Sound_value frame);
	Sound_value MDLRS_Sound_get(Sound *self);
	void MDLRS_Sound_deinit(Sound *self);

	Port_group *MDLRS_Module_get_inputs(Module *self);
	Port_group *MDLRS_Module_get_outputs(Module *self);
	Modularis *MDLRS_Module_get_project(Module *self);
	void MDLRS_Module_disconnect(Module *self);
	void MDLRS_Module_dispose(void *self);

	Note_table *MDLRS_Note_table_allocate(Modularis *project);
	void MDLRS_Note_table_init(Note_table *self, Modularis *project);
	void
		MDLRS_Note_table_set(Note_table *self, unsigned scancode, void *data),
		*MDLRS_Note_table_get(Note_table *self, unsigned scancode),
		MDLRS_Note_table_unset(Note_table *self, unsigned scancode);
	void MDLRS_Note_table_deinit(Note_table *self);
	void MDLRS_Note_table_dispose(void *self);

	void MDLRS_Any_port_dispose(void *self);

	int
		MDLRS_Port_connect(Port *self, Any_port *port),
		MDLRS_Port_connect_port(Port *self, Port *port),
		MDLRS_Port_connect_group(Port *self, Port_group *group),
		MDLRS_Port_disconnect(Port *self),
		MDLRS_Port_disconnect_from_port(Port *self, Any_port *port),
		MDLRS_Port_disconnect_port(Port *self, Port *port),
		MDLRS_Port_disconnect_group(Port *self, Port_group *group),
		MDLRS_Port_disconnect_input(Port *self);
	void MDLRS_Port_update(Port *self);
	void MDLRS_Port_get_ready(Port *self);
	Port *MDLRS_Port_connection(Port *self, unsigned connection);
	unsigned MDLRS_Port_count(Port *self);
	unsigned MDLRS_Port_find_connection(Port *self, Port *port);
}

namespace MDLRS
{
	void *Modularis::operator new(size_t size)
	{
		return MDLRS_Modularis_allocate();
	}
	Modularis::Modularis(unsigned sample_rate, unsigned channels)
	{
		MDLRS_Modularis_init(this, sample_rate, channels);
	}
	void Modularis::update()
	{
		MDLRS_Modularis_update(this);
	}
	Sound_value Modularis::get(unsigned channel)
	{
		return MDLRS_Modularis_get(this, channel);
	}
	Modularis::~Modularis()
	{
		MDLRS_Modularis_deinit(this);
	}
	void Modularis::operator delete(void *data)
	{
		MDLRS_Modularis_dispose(data);
	}

	IModule::IModule(Modularis *project)
	{
		MDLRS_IModule_init(this, project);
	}
	IModule::~IModule()
	{
		MDLRS_IModule_deinit(this);
	}

	IPort::IPort(const char *type, Module *module)
	{
		MDLRS_IPort_init(this, type, module);
	}
	IPort::~IPort()
	{
		MDLRS_IPort_deinit(this);
	}

	void *Note_chorus::operator new(size_t size, Modularis *project)
	{
		return MDLRS_Note_chorus_allocate(project);
	}
	Note_chorus::Note_chorus(Modularis *project, float spread, unsigned voices)
	{
		MDLRS_Note_chorus_init(this, project, spread, voices);
	}
	void Note_chorus::set_spread(float spread)
	{
		MDLRS_Note_chorus_set_spread(this, spread);
	}
	void Note_chorus::set_voices(unsigned voices)
	{
		MDLRS_Note_chorus_set_voices(this, voices);
	}
	void Note_chorus::set_random_phases(bool random_phases)
	{
		MDLRS_Note_chorus_set_random_phases(this, random_phases);
	}
	Note *Note_chorus::get_input()
	{
		return MDLRS_Note_chorus_get_input(this);
	}
	Real_controller *Note_chorus::get_spread()
	{
		return MDLRS_Note_chorus_get_spread(this);
	}
	Integer_controller *Note_chorus::get_voices()
	{
		return MDLRS_Note_chorus_get_voices(this);
	}
	Integer_controller *Note_chorus::get_random_phases()
	{
		return MDLRS_Note_chorus_get_random_phases(this);
	}
	Note *Note_chorus::get_output()
	{
		return MDLRS_Note_chorus_get_output(this);
	}
	Note_chorus::~Note_chorus()
	{
		MDLRS_Note_chorus_deinit(this);
	}

	void *Transpose::operator new(size_t size, Modularis *project)
	{
		return MDLRS_Transpose_allocate(project);
	}
	Transpose::Transpose(Modularis *project, float transposition)
	{
		MDLRS_Transpose_init(this, project, transposition);
	}
	void Transpose::set_transposition(float transposition)
	{
		MDLRS_Transpose_set_transposition(this, transposition);
	}
	void Transpose::set_velocity(float velocity)
	{
		MDLRS_Transpose_set_velocity(this, velocity);
	}
	Note *Transpose::get_input()
	{
		return MDLRS_Transpose_get_input(this);
	}
	Real_controller *Transpose::get_transposition()
	{
		return MDLRS_Transpose_get_transposition(this);
	}
	Real_controller *Transpose::get_velocity()
	{
		return MDLRS_Transpose_get_velocity(this);
	}
	Note *Transpose::get_output()
	{
		return MDLRS_Transpose_get_output(this);
	}
	Transpose::~Transpose()
	{
		MDLRS_Transpose_deinit(this);
	}

	void *Amplifier::operator new(size_t size, Modularis *project)
	{
		return MDLRS_Amplifier_allocate(project);
	}
	Amplifier::Amplifier(Modularis *project, float volume)
	{
		MDLRS_Amplifier_init(this, project, volume);
	}
	void Amplifier::set_volume(float volume)
	{
		MDLRS_Amplifier_set_volume(this, volume);
	}
	Sound *Amplifier::get_input()
	{
		return MDLRS_Amplifier_get_input(this);
	}
	Real_controller *Amplifier::get_volume()
	{
		return MDLRS_Amplifier_get_volume(this);
	}
	Sound *Amplifier::get_output()
	{
		return MDLRS_Amplifier_get_output(this);
	}
	Amplifier::~Amplifier()
	{
		MDLRS_Amplifier_deinit(this);
	}

	void *Delay::operator new(size_t size, Modularis *project)
	{
		return MDLRS_Delay_allocate(project);
	}
	Delay::Delay(Modularis *project, float delay)
	{
		MDLRS_Delay_init(this, project, delay);
	}
	void Delay::set_delay(float delay)
	{
		MDLRS_Delay_set_delay(this, delay);
	}
	Sound *Delay::get_input()
	{
		return MDLRS_Delay_get_input(this);
	}
	Real_controller *Delay::get_delay()
	{
		return MDLRS_Delay_get_delay(this);
	}
	Sound *Delay::get_output()
	{
		return MDLRS_Delay_get_output(this);
	}
	Delay::~Delay()
	{
		MDLRS_Delay_deinit(this);
	}

	void *Modulator::operator new(size_t size, Modularis *project)
	{
		return MDLRS_Modulator_allocate(project);
	}
	Modulator::Modulator(Modularis *project)
	{
		MDLRS_Modulator_init(this, project);
	}
	Sound *Modulator::get_carrier()
	{
		return MDLRS_Modulator_get_carrier(this);
	}
	Sound *Modulator::get_modulator()
	{
		return MDLRS_Modulator_get_modulator(this);
	}
	Sound *Modulator::get_output()
	{
		return MDLRS_Modulator_get_output(this);
	}
	Modulator::~Modulator()
	{
		MDLRS_Modulator_deinit(this);
	}

	void *Sequencer::operator new(size_t size, Modularis *project)
	{
		return MDLRS_Sequencer_allocate(project);
	}
	Sequencer::Sequencer(Modularis *project)
	{
		MDLRS_Sequencer_init(this, project);
	}
	void Sequencer::set_BPM(float BPM)
	{
		MDLRS_Sequencer_set_BPM(this, BPM);
	}
	void Sequencer::set_LPB(float LPB)
	{
		MDLRS_Sequencer_set_LPB(this, LPB);
	}
	void Sequencer::set_position(float cursor_position)
	{
		MDLRS_Sequencer_set_position(this, cursor_position);
	}
	void Sequencer::set_loop(bool loop)
	{
		MDLRS_Sequencer_set_loop(this, loop);
	}
	void Sequencer::set_play(bool play)
	{
		MDLRS_Sequencer_set_play(this, play);
	}
	Real_controller *Sequencer::get_BPM()
	{
		return MDLRS_Sequencer_get_BPM(this);
	}
	Integer_controller *Sequencer::get_LPB()
	{
		return MDLRS_Sequencer_get_LPB(this);
	}
	Real_controller *Sequencer::get_position()
	{
		return MDLRS_Sequencer_get_position(this);
	}
	Integer_controller *Sequencer::get_loop()
	{
		return MDLRS_Sequencer_get_loop(this);
	}
	Integer_controller *Sequencer::get_play()
	{
		return MDLRS_Sequencer_get_play(this);
	}
	Note *Sequencer::get_output()
	{
		return MDLRS_Sequencer_get_output(this);
	}
	void Sequencer::add(Pattern ***tracks, unsigned track_count)
	{
		MDLRS_Sequencer_add(this, tracks, track_count);
	}
	Sequencer::~Sequencer()
	{
		MDLRS_Sequencer_deinit(this);
	}

	void *Oscillator::operator new(size_t size, Modularis *project)
	{
		return MDLRS_Oscillator_allocate(project);
	}
	Oscillator::Oscillator(Modularis *project)
	{
		MDLRS_Oscillator_init(this, project);
	}
	void Oscillator::set_volume(float volume)
	{
		MDLRS_Oscillator_set_volume(this, volume);
	}
	void Oscillator::set_waveform(unsigned waveform)
	{
		MDLRS_Oscillator_set_waveform(this, waveform);
	}
	Note *Oscillator::get_input()
	{
		return MDLRS_Oscillator_get_input(this);
	}
	Real_controller *Oscillator::get_volume()
	{
		return MDLRS_Oscillator_get_volume(this);
	}
	Integer_controller *Oscillator::get_waveform()
	{
		return MDLRS_Oscillator_get_waveform(this);
	}
	ADSR *Oscillator::get_envelope()
	{
		return MDLRS_Oscillator_get_envelope(this);
	}
	Sound *Oscillator::get_output()
	{
		return MDLRS_Oscillator_get_output(this);
	}
	Oscillator::~Oscillator()
	{
		MDLRS_Oscillator_deinit(this);
	}

	void *ADSR::operator new(size_t size, Module *module)
	{
		return MDLRS_ADSR_allocate(module);
	}
	ADSR::ADSR(Module *module, float attack, float decay, float sustain, float release): Port_group(module)
	{
		MDLRS_ADSR_init_body(this, module, attack, decay, sustain, release);
	}
	void ADSR::set_attack(float attack)
	{
		MDLRS_ADSR_set_attack(this, attack);
	}
	void ADSR::set_decay(float decay)
	{
		MDLRS_ADSR_set_decay(this, decay);
	}
	void ADSR::set_sustain(float sustain)
	{
		MDLRS_ADSR_set_sustain(this, sustain);
	}
	void ADSR::set_release(float release)
	{
		MDLRS_ADSR_set_release(this, release);
	}
	Real_controller *ADSR::get_attack()
	{
		return MDLRS_ADSR_get_attack(this);
	}
	Real_controller *ADSR::get_decay()
	{
		return MDLRS_ADSR_get_decay(this);
	}
	Real_controller *ADSR::get_sustain()
	{
		return MDLRS_ADSR_get_sustain(this);
	}
	Real_controller *ADSR::get_release()
	{
		return MDLRS_ADSR_get_release(this);
	}
	float ADSR::envelope(ADSR_state state, float time)
	{
		return MDLRS_ADSR_envelope(this, state, time);
	}
	ADSR::~ADSR()
	{
		MDLRS_ADSR_deinit(this);
	}

	void *Integer_controller::operator new(size_t size, Module *module)
	{
		return MDLRS_Integer_controller_allocate(module);
	}
	Integer_controller::Integer_controller(Module *module, int32_t value)
	{
		MDLRS_Integer_controller_init(this, module, value);
	}
	void Integer_controller::set(int32_t value)
	{
		MDLRS_Integer_controller_set(this, value);
	}
	int32_t Integer_controller::get()
	{
		return MDLRS_Integer_controller_get(this);
	}
	Integer_controller::~Integer_controller()
	{
		MDLRS_Integer_controller_deinit(this);
	}

	void *Real_controller::operator new(size_t size, Module *module)
	{
		return MDLRS_Real_controller_allocate(module);
	}
	Real_controller::Real_controller(Module *module, float value)
	{
		MDLRS_Real_controller_init(this, module, value);
	}
	void Real_controller::set(float value)
	{
		MDLRS_Real_controller_set(this, value);
	}
	float Real_controller::get()
	{
		return MDLRS_Real_controller_get(this);
	}
	Real_controller::~Real_controller()
	{
		MDLRS_Real_controller_deinit(this);
	}

	void *Note::operator new(size_t size, Module *module)
	{
		return MDLRS_Note_allocate(module);
	}
	Note::Note(Module *module)
	{
		MDLRS_Note_init(this, module);
	}
	Note_event *Note::get_events()
	{
		return MDLRS_Note_get_events(this);
	}
	unsigned Note::get_event_count()
	{
		return MDLRS_Note_get_event_count(this);
	}
	unsigned Note::add_start(float pitch, float velocity, float phase)
	{
		return MDLRS_Note_add_start(this, pitch, velocity, phase);
	}
	void Note::add_change(unsigned scancode, float pitch, float velocity)
	{
		MDLRS_Note_add_change(this, scancode, pitch, velocity);
	}
	void Note::add_stop(unsigned scancode)
	{
		MDLRS_Note_add_stop(this, scancode);
	}
	void Note::clean()
	{
		MDLRS_Note_clean(this);
	}
	Note::~Note()
	{
		MDLRS_Note_deinit(this);
	}

	void *Port_group::operator new(size_t size, Module *module)
	{
		return MDLRS_Port_group_allocate(module);
	}
	Port_group::Port_group(Module *module)
	{
		MDLRS_Port_group_init(this, module);
	}
	int Port_group::connect(Any_port *port)
	{
		return MDLRS_Port_group_connect(this, port);
	}
	int Port_group::connect_port(Port *port)
	{
		return MDLRS_Port_group_connect_port(this, port);
	}
	int Port_group::connect_group(Port_group *group)
	{
		return MDLRS_Port_group_connect_group(this, group);
	}
	int Port_group::disconnect()
	{
		return MDLRS_Port_group_disconnect(this);
	}
	int Port_group::disconnect(Any_port *port)
	{
		return MDLRS_Port_group_disconnect_from_port(this, port);
	}
	int Port_group::disconnect_port(Port *port)
	{
		return MDLRS_Port_group_disconnect_port(this, port);
	}
	int Port_group::disconnect_group(Port_group *group)
	{
		return MDLRS_Port_group_disconnect_group(this, group);
	}
	int Port_group::disconnect_input()
	{
		return MDLRS_Port_group_disconnect_input(this);
	}
	void Port_group::update()
	{
		MDLRS_Port_group_update(this);
	}
	void Port_group::get_ready()
	{
		MDLRS_Port_group_get_ready(this);
	}
	void Port_group::add(Any_port *port)
	{
		MDLRS_Port_group_add(this, port);
	}
	Any_port *Port_group::port(unsigned port)
	{
		return MDLRS_Port_group_port(this, port);
	}
	unsigned Port_group::count()
	{
		return MDLRS_Port_group_count(this);
	}
	Port_group::~Port_group()
	{
		MDLRS_Port_group_deinit(this);
	}

	void *Sound::operator new(size_t size, Module *module)
	{
		return MDLRS_Sound_allocate(module);
	}
	Sound::Sound(Module *module)
	{
		MDLRS_Sound_init(this, module);
	}
	void Sound::set(Sound_value frame)
	{
		MDLRS_Sound_set(this, frame);
	}
	Sound_value Sound::get()
	{
		return MDLRS_Sound_get(this);
	}
	Sound::~Sound()
	{
		MDLRS_Sound_deinit(this);
	}

	Port_group *Module::get_inputs()
	{
		return MDLRS_Module_get_inputs(this);
	}
	Port_group *Module::get_outputs()
	{
		return MDLRS_Module_get_outputs(this);
	}
	Modularis *Module::get_project()
	{
		return MDLRS_Module_get_project(this);
	}
	void Module::disconnect()
	{
		MDLRS_Module_disconnect(this);
	}
	void Module::operator delete(void *data)
	{
		MDLRS_Module_dispose(data);
	}

	void *Note_table::operator new(size_t size, Modularis *project)
	{
		return MDLRS_Note_table_allocate(project);
	}
	Note_table::Note_table(Modularis *project)
	{
		MDLRS_Note_table_init(this, project);
	}
	void Note_table::set(unsigned scancode, void *data)
	{
		MDLRS_Note_table_set(this, scancode, data);
	}
	void *Note_table::get(unsigned scancode)
	{
		return MDLRS_Note_table_get(this, scancode);
	}
	void Note_table::unset(unsigned scancode)
	{
		MDLRS_Note_table_unset(this, scancode);
	}
	Note_table::~Note_table()
	{
		MDLRS_Note_table_deinit(this);
	}
	void Note_table::operator delete(void *data)
	{
		MDLRS_Note_table_dispose(data);
	}

	void Any_port::operator delete(void *data)
	{
		MDLRS_Any_port_dispose(data);
	}

	int Port::connect(Any_port *port)
	{
		return MDLRS_Port_connect(this, port);
	}
	int Port::connect_port(Port *port)
	{
		return MDLRS_Port_connect_port(this, port);
	}
	int Port::connect_group(Port_group *group)
	{
		return MDLRS_Port_connect_group(this, group);
	}
	int Port::disconnect()
	{
		return MDLRS_Port_disconnect(this);
	}
	int Port::disconnect(Any_port *port)
	{
		return MDLRS_Port_disconnect_from_port(this, port);
	}
	int Port::disconnect_port(Port *port)
	{
		return MDLRS_Port_connect_port(this, port);
	}
	int Port::disconnect_group(Port_group *group)
	{
		return MDLRS_Port_disconnect_group(this, group);
	}
	int Port::disconnect_input()
	{
		return MDLRS_Port_disconnect_input(this);
	}
	void Port::update()
	{
		MDLRS_Port_update(this);
	}
	void Port::get_ready()
	{
		MDLRS_Port_get_ready(this);
	}
	Port *Port::connection(unsigned connection)
	{
		return MDLRS_Port_connection(this, connection);
	}
	unsigned Port::count()
	{
		return MDLRS_Port_count(this);
	}
	unsigned Port::find_connection(Port *port)
	{
		return MDLRS_Port_find_connection(this, port);
	}

	float Continuous_key::get_value(float position)
	{
		switch (curve)
		{
			case INTERPOLATION_NONE: return value;
			case INTERPOLATION_LINEAR: return value+(this[1].value-value)*position/duration;
			case INTERPOLATION_FAST: return value+(this[1].value-value)*(1-(duration-position)*(duration-position)/(duration*duration));
			case INTERPOLATION_SLOW: return value+(this[1].value-value)*position*position/(duration*duration);
			case INTERPOLATION_SMOOTH: return value+(this[1].value-value)*(1-cosf((float)M_PI*position/duration))/2;
		}
	}

	float Note_key::get_value(float position)
	{
		switch (curve)
		{
			case INTERPOLATION_NONE: return value;
			case INTERPOLATION_LINEAR: return value+(this[1].value-value)*position/duration;
			case INTERPOLATION_FAST: return value+(this[1].value-value)*(1-(duration-position)*(duration-position)/(duration*duration));
			case INTERPOLATION_SLOW: return value+(this[1].value-value)*position*position/(duration*duration);
			case INTERPOLATION_SMOOTH: return value+(this[1].value-value)*(1-cosf((float)M_PI*position/duration))/2;
		}
	}
}