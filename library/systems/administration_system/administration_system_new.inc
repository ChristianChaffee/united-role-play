
/*
Используемые PVar`ы:
	Admin_GiveItemID

*/

#define Admin:%0(%1)  					Admn_%0(%1)
//==============================================================================

#define GetPlayerAdminData(%0,%1) 		player_admin_info[%0][%1]
#define SetPlayerAdminData(%0,%1,%2)  	player_admin_info[%0][%1] = %2
#define AddPlayerAdminData(%0,%1,%2)  	player_admin_info[%0][%1] += %2

//==============================================================================
enum player_admin_info_struct
{	
	pa_id,
	pa_account_id,
	pa_level,
	pa_logged,
	pa_password[32],
}
new player_admin_info[MAX_PLAYERS][player_admin_info_struct];
new const player_admin_info_default[player_admin_info_struct] = {
	0,
	0,
	0,
	false,
	""
};

//==============================================================================
enum admin_command_info_struct
{
	ac_level,
	ac_command[32],
	ac_description[128]
}
new const admin_command_info[][admin_command_info_struct] = {
	{1, "a", "Отправить сообщение в чат администрации."},
	{1, "hp", "Восстановить здоровье и нужды."},
	
	{2, "kick", "Отключить игрока от сервера."},
	{2, "gotobiz", "Телепортироваться к бизнесу по ID."},

	{3, "tpcoord", "Телепортироваться по координатам."},
	{3, "setvw", "Установить интерьер игроку."},
	{3, "setint", "Установить виртуальный мир игроку."},
	
	{4, "ooc", "Отправить сообщение в общий чат."},
	{4, "tveh", "Создать временный транспорт."},
	{4, "delveh", "Удалить транспорт."},
	{4, "nrg", "Создать временный транспорт NRG-500."},

	{6, "agivemoney", "Выдать игроку игровую валюту ($)."},
	{6, "giveinv", "Выдать предмет в инвентарь игроку."},
	{6, "payday", "Внеплановый PayDay."},
	{6, "paydayres", "Внеплановый PayDay с рестартом сервера."},
	{6, "gmx", "Произвести рестарт сервера через 5 секунд."}
};