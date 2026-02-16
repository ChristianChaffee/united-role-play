
#define Fraction:%0(%1) 				Frctn_%0(%1)

//==============================================================================
#define GetFractionData(%0,%1) 			fraction_info[%0][%1]
#define SetFractionData(%0,%1,%2) 		fraction_info[%0][%1] = %2
#define AddFractionData(%0,%1,%2)  		fraction_info[%0][%1] += %2

#define GetFractionSkinData(%0,%1)  	fraction_skin_info[%0][%1]

//==============================================================================
#define MAX_FRACTIONS 					30
#define MAX_FRACTION_SKINS  			200
#define MAX_FRACTION_RANKS 				20
#define FRACTIONS_TYPES_COUNT 			5

#define FRAC_DRESSROOM_PICKUP_MODEL 	1275

//==============================================================================
new const fraction_type_names[FRACTIONS_TYPES_COUNT][64] = {
	"Правительство",
	"Правоохранительные органы",
	"Организация здравоохранения",
	"Средство массовой информации",
	"Криминальная структура"
};

enum
{
	FRAC_TYPE_GOVERMENT,
	FRAC_TYPE_POLICE,
	FRAC_TYPE_MEDIC,
	FRAC_TYPE_NEWS,
	FRAC_TYPE_CRIMINAL
}

//==============================================================================
enum fraction_skin_info_struct
{
	fs_id,
	fs_fraction_id,
	fs_skin_id,
	fs_rank,
	fs_sex
}
new fraction_skin_info[MAX_FRACTION_SKINS][fraction_skin_info_struct];

//==============================================================================
enum fraction_info_struct
{
	f_id,
	f_name[64],
	f_tag[16],
	f_type,
	f_leader_account_id,
	f_color_str[16],
	f_color_value,
	f_ranks_count,
	Float:f_spawn_x,
	Float:f_spawn_y,
	Float:f_spawn_z,
	Float:f_spawn_angle,
	f_spawn_vw,
	f_spawn_int,
	f_icon_url[128],
	Float:f_dressroom_x,
	Float:f_dressroom_y,
	Float:f_dressroom_z,
	f_dressroom_vw,
	f_dressroom_int,

	f_dressroom_pic,
	Text3D:f_dressroom_text
}
new fraction_info[MAX_FRACTIONS][fraction_info_struct];
new fraction_ranks[MAX_FRACTIONS][MAX_FRACTION_RANKS][32];
