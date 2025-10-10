/*
(C) 2022, 2025 Серый MLGamer. All freedoms preserved.
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

#include <stddef.h>
#include <Modularis_Core/system/Safe_block.h>
#include <string.h>

struct MDLRS_Modularis;

void *safe_object(struct MDLRS_Modularis *project, const char *name, size_t size);
void *safe_array(struct MDLRS_Modularis *project, const char *name, size_t size, size_t count);
inline void safe_clean(void *data)
{
	struct MDLRS_Safe_block *block=data-sizeof(struct MDLRS_Safe_block);
	memset(data, 0, block->count*block->size);
}
void *safe_resize(struct MDLRS_Modularis *project, void *data, size_t count);
inline const char *safe_name(void *data)
{
	return ((struct MDLRS_Safe_block *)data-1)->name;
}
inline size_t safe_size(void *data)
{
	struct MDLRS_Safe_block *block=data-sizeof(struct MDLRS_Safe_block);
	return block->count*block->size;
}
inline size_t safe_object_size(void *data)
{
	return ((struct MDLRS_Safe_block *)data-1)->size;
}
inline size_t safe_count(void *data)
{
	return ((struct MDLRS_Safe_block *)data-1)->count;
}
void safe_dispose(struct MDLRS_Modularis *project, void *data);