/*
(C) 2024-2025 Серый MLGamer. All freedoms preserved.
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
	struct Note;
	struct Real_controller;
	struct Integer_controller;

	struct Note_chorus: Module
	{
		void *operator new(size_t size, Modularis *project);
		Note_chorus(Modularis *project, float spread, unsigned voices);
		void
			set_spread(float spread),
			set_voices(unsigned voices),
			set_random_phases(bool random_phases);
		Note *get_input();
		Real_controller *get_spread();
		Integer_controller *get_voices();
		Integer_controller *get_random_phases();
		Note *get_output();
		~Note_chorus();
	};
}