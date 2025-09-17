
#define CHARACTER_MIN_AGE 				18
#define CHARACTER_MAX_AGE 				75

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
}
new player_info[MAX_PLAYERS][player_info_struct];
new player_logged[MAX_PLAYERS] = { false, ... };

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