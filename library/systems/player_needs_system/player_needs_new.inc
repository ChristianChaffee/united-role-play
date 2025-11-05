
#define PlayerNeeds:%0(%1) 					PlrNds_%0(%1)

//==============================================================================
#define GetPlayerNeedValue(%0,%1) 			player_need_info[%0][%1]
#define SetPlayerNeedValue(%0,%1,%2) 		player_need_info[%0][%1] = %2
#define AddPlayerNeedValue(%0,%1,%2) 		player_need_info[%0][%1] += %2

#define GetNeedData(%0,%1)  				need_info[%0][%1]

//==============================================================================
#define PLAYER_NEED_MAX_VALUE 				100

//==============================================================================
enum
{
	PLAYER_NEED_HUNGRY,
	PLAYER_NEED_THIRST,
	PLAYER_NEED_URINE,
	PLAYER_NEED_CLEANLINESS,

	PLAYER_NEEDS_COUNT
}

//==============================================================================
new player_need_info[MAX_PLAYERS][PLAYER_NEEDS_COUNT];

//==============================================================================
enum need_info_struct
{
	n_name[32],
	n_message_name[32],
	n_minus_per_minute,
	n_critical_value,
	n_advice[128],
}
new const need_info[PLAYER_NEEDS_COUNT][need_info_struct] = {
	{"Сытость", "сытости", 2, 20, "Сходите в закусочную или купите еды в магазине"},
	{"Жажда", "жажды", 3, 20, "Сходите в закусочную или купите попить в магазине"},
	{"Нужда", "нужды", 3, 30, "Сходите в туалет ('/piss')"},
	{"Чистоплотность", "чистоплотности", 2, 30, "Помойтесь в общественном душе или у себя дома"}
};