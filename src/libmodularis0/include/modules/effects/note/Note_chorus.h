/*
(C) 2024-2025 Серый MLGamer. All freedoms preserved.
Дзен: <https://dzen.ru/seriy_mlgamer>
SoundCloud: <https://soundcloud.com/seriy_mlgamer>
YouTube: <https://www.youtube.com/@Seriy_MLGamer>
GitVerse: <https://gitverse.ru/Seriy_MLGamer>
E-mail: <Seriy-MLGamer@yandex.ru>

This file is part of Modularis Core.
Modularis Core is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
Modularis Core is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Modularis Core. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <Modularis_Core/system/interfaces/modules/effects/note/Note_chorus.h>
#include <system/modules/Module.h>

#include <ports/Note.h>
#include <ports/controllers/Real_controller.h>
#include <ports/controllers/Integer_controller.h>
#include <system/ports/Note/Note_table.h>
#include <stdint.h>

struct Chorus_stack
{
	struct Chorus_stack *previous;
	int32_t voices;
};
struct MDLRS_Note_chorus
{
	struct MDLRS_Module p;

	struct MDLRS_Note input;
	struct MDLRS_Real_controller spread;
	struct MDLRS_Integer_controller voices;
	struct MDLRS_Integer_controller random_phases;
	struct MDLRS_Note output;
	struct MDLRS_Note_table notes;
	struct Chorus_stack *last;
};