
#ifndef GAME_H
#define GAME_H

#define MAX_PLAYERS 8

/* TODO: Give name these defines */
#define NUM_UNKNOWN_TYPES_1 100
#define NUM_UNKNOWN_TYPES_2 20

struct RESDATA
{
	char language;
	char controls;
	char field_2;
	char _pad3;
	short music;
	short sound_effect;
	short field_8;
	short field_A[NUM_UNKNOWN_TYPES_1];
	short field_D2;
	short field_D4;
	short field_D6;
	short field_D8;
	char _pad218;
	char _pad219;
	int field_DC[MAX_PLAYERS];
	short field_FC[MAX_PLAYERS];
	short field_10C[MAX_PLAYERS];
	short field_11C;
	short field_11E;
	short field_120[NUM_UNKNOWN_TYPES_2];
	short field_148;
	short difficulty;
	short screen_resolution;
	short field_14E;
	short draw_distance;
	short object_detail;
	short reflections;
	short transparencies;
	short field_158;
	short field_15A;
	short field_15C;
	char _pad350;
	char _pad351;
};
typedef struct RESDATA RESDATA;

extern HWND game_window;
extern float fog_distance_1;
extern float fog_distance_2;
extern int reflections;
extern int transparencies;
extern int object_detail;

extern RESDATA *res_data;




#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

void
ResetSettings(void);

RwBool
SaveSettings(void);

RwBool
LoadSettings(void);




#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* GAME_H */
