

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

	{"/cars", "открыть список личных транспортных средств", COMMAND_TYPE_VEHICLE},
	{"/park", "припарковать личное транспортное средство", COMMAND_TYPE_VEHICLE},
	{"/lock", "открыть/закрыть транспортное средство", COMMAND_TYPE_VEHICLE}
};