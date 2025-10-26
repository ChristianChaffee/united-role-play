#pragma warning disable 239

#include <a_samp>
#include <a_mysql>
#include <mdialog>
#include <Pawn.Regex>
#include <Pawn.CMD>
#include <streamer>
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

#include "..\library\source\colors_new.inc"
#include "..\library\source\global_new.inc"
#include "..\library\source\dialogs_new.inc"
#include "..\library\source\database\database_new.inc"
#include "..\library\source\player\player_new.inc"

#include "..\library\source\player\player_functions.inc"

#include "..\library\source\database\database_hooks.inc"
//==============================================================================

#include "..\library\systems\skin_select_system\skin_select_new.inc"
#include "..\library\systems\registration_system\registration_new.inc"
#include "..\library\systems\inventory_system\inventory_new.inc"
#include "..\library\systems\inventory_system\inventory_items\inventory_items_new.inc"
#include "..\library\systems\inventory_system\accessories_items\accessories_items_new.inc"
#include "..\library\systems\inventory_system\drop_items\drop_items_new.inc"
#include "..\library\systems\enter_exits_system\enter_exits_new.inc"
#include "..\library\systems\bank_system\bank_system_new.inc"
#include "..\library\systems\actor_system\actor_system_new.inc"
#include "..\library\systems\quest_system\start_quest_system\start_quest_new.inc"
#include "..\library\systems\quest_system\start_quest_system\start_quest_stages.inc"
#include "..\library\systems\job_system\job_system_new.inc"
#include "..\library\systems\job_system\job_loader\job_loader_new.inc"

public OnGameModeInit(){
	Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, 1000);
	Streamer_SetMaxItems(STREAMER_TYPE_OBJECT, 1000);

	SetTimer("SecondTimer", 1000, true);
	return 1;
}

public OnGameModeExit()
{
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
	return 1;
}

public OnPlayerConnect(playerid)
{
	ApplyAnimation(playerid, "DEALER", "_", 4.1, 1, 1, 1, 1, 1);
	ApplyAnimation(playerid, "PED", "_", 4.1, 1, 1, 1, 1, 1);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	return 1;
}

public OnPlayerSpawn(playerid)
{
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
	if (strcmp("/mycommand", cmdtext, true, 10) == 0)
	{
		// Do something here
		return 1;
	}
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	if(GetPVarInt(playerid, "GPS_Enabled") == 1){
		DisablePlayerRaceCheckpoint(playerid);
		DeletePVar(playerid, "GPS_Enabled");
		SendClientMessage(playerid, COLOR_YELLOW, "Вы прибыли к месту назначения.");
	}
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, source)
{
	return 1;
}

stock OnPlayerAccountLoaded(playerid)
{
	#pragma unused playerid
	return 0;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ){
	SetPlayerPos(playerid, fX, fY, fZ);
	SetPlayerInterior(playerid, 0);
	SetPlayerVirtualWorld(playerid, 0);
	return 1;
}

public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle){
	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Ошибка #%d, обратитесь к разработчикам!", errorid);
	SendClientMessageToAll(COLOR_DARK_RED, small_string);
	small_string[0] = EOS;

	printf("[MySQL Error] %d - %s (query: %s)",
		errorid, error, query);

	for(new i, g = GetPlayerPoolSize(); i <= g; i++){
		if(player_logged[i] && !strcmp(GetPlayerData(i, p_name), "William_Tucker")){
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				""cl_dark_red"[Error]: "cl_white"%s", query);
			SendClientMessage(i, -1, small_string);
			small_string[0] = EOS;
			break;
		}
	}
	return 1;
}

forward SecondTimer();
public SecondTimer(){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		OnPlayerSecondUpdate(i);
	}
	return 1;
}

stock OnPlayerSecondUpdate(playerid){
	return 1;
}

stock ConvertTime(unixtime, &year = 0, &month = 0, &day = 0, &hour = 0, &minute = 0, &second = 0)
{
    // Константы для вычислений
    const SECONDS_PER_DAY = 86400;
    const SECONDS_PER_HOUR = 3600;
    const SECONDS_PER_MINUTE = 60;
    
    // Начальная дата Unix epoch: 1 января 1970
    new days_since_epoch = unixtime / SECONDS_PER_DAY;
    new seconds_in_day = unixtime % SECONDS_PER_DAY;
    
    // Вычисляем время (часы, минуты, секунды)
    hour = seconds_in_day / SECONDS_PER_HOUR;
    minute = (seconds_in_day % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
    second = seconds_in_day % SECONDS_PER_MINUTE;
    
    // Вычисляем дату
    year = 1970;
    day = 1;
    
    // Массив с количеством дней в месяцах (не високосный год)
    new days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Добавляем дни с учетом високосных лет
    while(days_since_epoch >= 365)
    {
        if(IsLeapYear(year))
        {
            if(days_since_epoch >= 366)
            {
                days_since_epoch -= 366;
                year++;
            }
            else
            {
                break;
            }
        }
        else
        {
            days_since_epoch -= 365;
            year++;
        }
    }
    
    // Корректируем массив дней для високосного года
    if(IsLeapYear(year))
    {
        days_in_month[1] = 29; // Февраль в високосный год
    }
    else
    {
        days_in_month[1] = 28; // Февраль в обычный год
    }
    
    // Определяем месяц и день
    month = 0;
    while(days_since_epoch >= days_in_month[month])
    {
        days_since_epoch -= days_in_month[month];
        month++;
    }
    
    day += days_since_epoch;
    month++; // Месяцы от 1 до 12
}

// Вспомогательная функция для определения високосного года
stock IsLeapYear(year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

#include "..\library\systems\registration_system\registration_publics.inc"
#include "..\library\systems\inventory_system\inventory_items\inventory_items_publics.inc"
#include "..\library\systems\inventory_system\accessories_items\accessories_items_publics.inc"
#include "..\library\systems\enter_exits_system\enter_exits_publics.inc"
#include "..\library\systems\bank_system\bank_system_publics.inc"
#include "..\library\systems\actor_system\actor_system_publics.inc"
#include "..\library\systems\quest_system\start_quest_system\start_quest_publics.inc"

#include "..\library\systems\registration_system\registration_dialogs.inc"
#include "..\library\systems\bank_system\bank_system_dialogs.inc"
#include "..\library\systems\quest_system\start_quest_system\start_quest_dialogs.inc"
#include "..\library\systems\job_system\job_loader\job_loader_dialogs.inc"

#include "..\library\systems\skin_select_system\skin_select_functions.inc"
#include "..\library\systems\registration_system\registration_functions.inc"
#include "..\library\systems\inventory_system\inventory_functions.inc"
#include "..\library\systems\inventory_system\inventory_items\inventory_items_functions.inc"
#include "..\library\systems\inventory_system\drop_items\drop_items_functions.inc"
#include "..\library\systems\inventory_system\accessories_items\accessories_items_functions.inc"
#include "..\library\systems\enter_exits_system\enter_exits_functions.inc"
#include "..\library\systems\bank_system\bank_system_functions.inc"
#include "..\library\systems\quest_system\start_quest_system\start_quest_functions.inc"
#include "..\library\systems\job_system\job_system_functions.inc"
#include "..\library\systems\job_system\job_loader\job_loader_functions.inc"

#include "..\library\systems\skin_select_system\skin_select_hooks.inc"
#include "..\library\systems\registration_system\registration_hooks.inc"
#include "..\library\systems\inventory_system\inventory_hooks.inc"
#include "..\library\systems\inventory_system\accessories_items\accessories_items_hooks.inc"
#include "..\library\systems\enter_exits_system\enter_exits_hooks.inc"
#include "..\library\systems\bank_system\bank_system_hooks.inc"
#include "..\library\systems\actor_system\actor_system_hooks.inc"
#include "..\library\systems\quest_system\start_quest_system\start_quest_hooks.inc"
#include "..\library\systems\job_system\job_system_hooks.inc"
#include "..\library\systems\job_system\job_loader\job_loader_hooks.inc"

#include "..\library\systems\inventory_system\inventory_commands.inc"

//==============================================================================
#include "..\library\source\mapping\spawn.inc"
#include "..\library\source\mapping\bank_int.inc"
#include "..\library\source\mapping\loader_job_ext.inc"

#warning TEST_CMD
CMD:tpt(playerid){
	SetPlayerVirtualWorld(playerid, 5);
	SetPlayerInterior(playerid, 5);
	SetPlayerPos(playerid, 1319.4584, 11.6674, 2068.5129);
}