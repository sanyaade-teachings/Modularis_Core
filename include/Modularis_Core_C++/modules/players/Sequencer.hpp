/*
(C) 2023-2025 Серый MLGamer. All freedoms preserved.
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

#pragma once

#include <Modularis_Core_C++/system/modules/Module.hpp>

#include <cstddef>

namespace MDLRS
{
	struct Modularis;
	struct Real_controller;
	struct Integer_controller;
	struct Note;
	struct Pattern;

	struct Sequencer: Module
	{
		void *operator new(size_t size, Modularis *project);
		Sequencer(Modularis *project);
		void
			set_BPM(float BPM),
			set_LPB(float LPB),
			set_position(float cursor_position),
			set_loop(bool loop),
			set_play(bool play);
		Real_controller *get_BPM();
		Integer_controller *get_LPB();
		Real_controller *get_position();
		Integer_controller *get_loop();
		Integer_controller *get_play();
		Note *get_output();
		void add(Pattern ***tracks, unsigned track_count);
		~Sequencer();
	};
}