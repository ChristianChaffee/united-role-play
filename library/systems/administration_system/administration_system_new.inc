
/*
Используемые PVar`ы:
	Admin_GiveItemID

	Admin_BusinessCreateType
	Admin_BusinessCreatePrice
	Admin_BusinessCreateName

	Admin_HouseCreateType
	Admin_HouseCreatePrice

	Admin_FracCreateName
	Admin_FracCreateType
	Admin_FracCreateColor
	Admin_FractionIndex
	Admin_FractionRank

	Admin_MakeleaderID

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
	{2, "gotohouse", "Телепортироваться к дому по ID."},

	{3, "tpcoord", "Телепортироваться по координатам."},
	{3, "setvw", "Установить интерьер игроку."},
	{3, "setint", "Установить виртуальный мир игроку."},
	{3, "settime", "Изменить время на сервере."},
	{3, "setweather", "Изменить погоду на сервере."},
	{3, "setskin", "Сменить скин."},
	
	{4, "ooc", "Отправить сообщение в общий чат."},
	{4, "tveh", "Создать временный транспорт."},
	{4, "delveh", "Удалить транспорт."},
	{4, "nrg", "Создать временный транспорт NRG-500."},
	{4, "makeleader", "Назначить игрока лидером фракции."},

	{6, "agivemoney", "Выдать игроку игровую валюту ($)."},
	{6, "giveinv", "Выдать предмет в инвентарь игроку."},
	{6, "payday", "Внеплановый PayDay."},
	{6, "paydayres", "Внеплановый PayDay с рестартом сервера."},
	{6, "addbiz", "Создать бизнес."},
	{6, "addhouse", "Создать дом."},
	{6, "fracmenu", "Управление фракциями."},
	{6, "gmx", "Произвести рестарт сервера через 5 секунд."}
};