
#define CHARACTER_MIN_AGE 				18
#define CHARACTER_MAX_AGE 				75

#define NATIONALITY_LEVEL 				3

#define LICENSES_COUNT 					7

//==============================================================================
enum
{
	ATTACH_INDEX_ACC_1,
	ATTACH_INDEX_ACC_2,
	ATTACH_INDEX_ACC_3,
	ATTACH_INDEX_ACC_4,
	ATTACH_INDEX_ACC_5,
	ATTACH_INDEX_ACC_6,
	ATTACH_INDEX_JOB,
	ATTACH_INDEX_INV
}

//==============================================================================
#define GetPlayerData(%0,%1)			player_info[%0][%1]
#define SetPlayerData(%0,%1,%2) 		player_info[%0][%1] = %2
#define AddPlayerData(%0,%1,%2)  		player_info[%0][%1] += %2
#define ResetPlayerStringData(%0,%1) 	player_info[%0][%1][0] = EOS

//==============================================================================
enum player_info_struct
{
	p_id,
	p_name[MAX_PLAYER_NAME],
	p_password[32],
	p_email[32],
	p_nationality,
	p_age,
	p_skin,
	p_sex,
	p_money,
	p_job,

	p_licenses[LICENSES_COUNT],
	p_licenses_times[LICENSES_COUNT],

	p_level,
	p_exp,

	p_start_quests_loaded,

	Float:p_health,
}
new player_info[MAX_PLAYERS][player_info_struct];
new player_logged[MAX_PLAYERS] = { false, ... };

new const player_info_default[player_info_struct] = {
	0,
	"",
	"",
	"",
	0,
	0,
	0,
	0,
	0,
	0,

	{0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0},

	0,
	0,
	false,

	100.0
};

//==============================================================================
new const character_nationality[15][32] = {
	"Не выбрано",
	"США",
	"Канада",
	"Мексика",
	"Россия",
	"Германия",
	"Франция",
	"Испания",
	"Португалия",
	"Польша",
	"Бельгия",
	"Нидерланды",
	"Китай",
	"Япония",
	"Южная Корея"
};

//==============================================================================
new const licensing_names[LICENSES_COUNT][128] = {
	"Класс D (легковоый транспорт)",
	"Класс M (мототранспорт)",
	"Коммерчиские водительские права - CDL (грузовой транспорт/автобусы)",
	"Лицензия на водный транспорт",
	"Свидетельство пилота",
	"Лицензия на владение оружием",
	"Лицензия на рыбную ловлю"
};

//==============================================================================
enum spawn_type_struct
{
	SPAWN_TYPE_STANDART,
}

new const Float:standart_spawn_pos[5][4] = {
	{1159.9659, -1770.3, 16.5938, 360.0},
	{1157.1459, -1770.3,16.5938, 360.0},
	{1154.0668, -1770.3,16.5938, 360.0},
	{1150.7965, -1770.3,16.5992, 360.0},
	{1147.6335, -1770.3,16.5992, 360.0}
};

new player_listitems[MAX_PLAYERS][100];