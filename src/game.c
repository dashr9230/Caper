
/* temporary including from rwsdk. TODO: remove this */
#include <ostypes.h> /* rwplcore.h */
#include <batypes.h> /* rwplcore.h */

#include <windows.h>

#include "game.h"

HWND game_window;
float fog_distance_1 = 200.0f;
float fog_distance_2 = 200.0f;
int reflections = 1;
int transparencies = 1;
int object_detail = 0;

RESDATA _res_data;
RESDATA *res_data = &_res_data;

void
ResetSettings(void)
{
	int i;

	res_data->language = 0;
	res_data->controls = 0;
	res_data->field_2 = 0;
	res_data->music = 127;
	res_data->sound_effect = 127;
	res_data->field_8 = 1;

	for (i = 0; i < NUM_UNKNOWN_TYPES_1; i++)
	{
		res_data->field_A[i] = 0;
	}

	res_data->field_D2 = 0;
	res_data->field_D4 = 0;
	res_data->field_D6 = 0;
	res_data->field_D8 = 0;

	for (i = 0; i < MAX_PLAYERS; i++)
	{
		res_data->field_DC[i] = 0;
		res_data->field_FC[i] = 0;
		res_data->field_10C[i] = 0;
	}

	res_data->field_11C = 1;
	res_data->field_11E = 0;

	for (i = 0; i < NUM_UNKNOWN_TYPES_2; i++)
	{
		res_data->field_120[i] = 0;
	}

	res_data->field_148 = 0;
	res_data->difficulty = 0;
	res_data->screen_resolution = 0;
	res_data->draw_distance = 2;
	res_data->object_detail = 0;
	res_data->reflections = 1;
	res_data->transparencies = 1;
	res_data->field_158 = 0;
	res_data->field_15A = 0;
	res_data->field_15C = 0;
}

RwBool
SaveSettings(void)
{
	DWORD numRead;
	HANDLE file;
	DWORD lastError;

	numRead = 0;

	file = CreateFile(
					"..\\..\\res_data.dat",
					GENERIC_WRITE,
					0,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);

	lastError = GetLastError();

	if (lastError != ERROR_ALREADY_EXISTS && lastError != ERROR_SUCCESS)
	{
		CloseHandle(file);

		return FALSE;
	}

	WriteFile(file, res_data, sizeof(RESDATA), &numRead, NULL);

	CloseHandle(file);

	return TRUE;
}

RwBool
LoadSettings(void)
{
	HANDLE file;
	DWORD lastError, numRead;

	numRead = 0;

	file = CreateFile(
					"..\\..\\res_data.dat",
					GENERIC_READ,
					0,
					NULL,
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL);

	lastError = GetLastError();

	if (lastError != ERROR_ALREADY_EXISTS && lastError != ERROR_SUCCESS)
	{
		CloseHandle(file);
		ResetSettings();
		return FALSE;
	}

	ReadFile(file, res_data, sizeof(RESDATA), &numRead, NULL);

	if (numRead == 0)
	{
		ResetSettings();
		CloseHandle(file);
		return FALSE;
	}

	CloseHandle(file);

	return TRUE;
}
