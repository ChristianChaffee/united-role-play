

#define SPD 							ShowPlayerDialog
#define SendErrorMessage(%0,%1) 		SendClientMessage(%0,COLOR_GREY,%1)
#define RandomEx(%0,%1) 				random(%1-%0)+%0

#define GAME_VW 						0
#define GAME_INT 						0

//==============================================================================
new small_string[144], big_string[4096];

//==============================================================================
enum
{
	COMMAND_TYPE_GENERAL,
	COMMAND_TYPE_VEHICLE,
	COMMAND_TYPE_CHAT,
	COMMAND_TYPE_MAX
}

//==============================================================================
enum command_info_struct
{
	ci_command[32],
	ci_description[256],
	ci_type
}
new command_info[][command_info_struct] = {
	{"/mm", "открыть главное меню", COMMAND_TYPE_GENERAL},
	{"/inv", "открыть меню инвентаря", COMMAND_TYPE_GENERAL},
	{"/takedrop", "подобрать вещь себе в инвентарь", COMMAND_TYPE_GENERAL},
	{"/needs", "открыть меню потребностей", COMMAND_TYPE_GENERAL},
	{"/piss", "справить нужду", COMMAND_TYPE_GENERAL},
	{"/maskoff", "снять маску", COMMAND_TYPE_GENERAL},
	{"/time", "взглянуть на часы", COMMAND_TYPE_GENERAL},

	{"/park", "припарковать личное транспортное средство", COMMAND_TYPE_VEHICLE},
	{"/lock", "открыть/закрыть транспортное средство", COMMAND_TYPE_VEHICLE},

	{"/me", "действие от 1-го лица", COMMAND_TYPE_CHAT},
	{"/do", "действие/описание от 3-го лица", COMMAND_TYPE_CHAT},
	{"/try", "действие-попытка (удачное/неудачное)", COMMAND_TYPE_CHAT}
};

//==============================================================================
enum load_info_struct
{
	loaded_vehicles,
	loaded_businesses,
	loaded_inventory_items,
	loaded_start_quests,
	loaded_houses,
	loaded_garages
}

new load_info[load_info_struct] = {
	false,
	false,
	false,
	false,
	false,
	false
};