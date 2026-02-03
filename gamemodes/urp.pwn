#pragma warning disable 239

#include <a_samp>
#include <crashdetect>
#include <a_mysql>
#include <mdialog>
#include <Pawn.Regex>
#include <Pawn.CMD>
#include <streamer>
#include <sscanf2>
#include <cef>
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

native IsValidVehicle(vehicleid);

#include "..\library\source\colors_new.inc"
#include "..\library\source\global_new.inc"
#include "..\library\source\dialogs_new.inc"
#include "..\library\source\database\database_new.inc"
#include "..\library\source\player\player_new.inc"
#include "..\library\source\vehicle\vehicle_new.inc"
#include "..\library\source\payday\payday_new.inc"
#include "..\library\source\cef\cef_new.inc"

#include "..\library\source\player\player_functions.inc"
#include "..\library\source\vehicle\vehicle_functions.inc"
#include "..\library\source\payday\payday_functions.inc"

#include "..\library\source\vehicle\vehicle_publics.inc"
#include "..\library\source\player\player_publics.inc"

#include "..\library\source\database\database_hooks.inc"
#include "..\library\source\player\player_hooks.inc"
#include "..\library\source\vehicle\vehicle_hooks.inc"
#include "..\library\source\payday\payday_hooks.inc"
#include "..\library\source\cef\cef_hooks.inc"

#include "..\library\source\cef\cef_publics.inc"

#include "..\library\source\player\player_dialogs.inc"
#include "..\library\source\vehicle\vehicle_dialogs.inc"

#include "..\library\source\vehicle\vehicle_commands.inc"
#include "..\library\source\player\player_commands.inc"
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
#include "..\library\systems\job_system\job_lumberjack\job_lumberjack_new.inc"
#include "..\library\systems\job_system\job_garbage_sorter\job_garbage_sorter_new.inc"
#include "..\library\systems\job_system\job_dealler\job_dealler_new.inc"
#include "..\library\systems\job_system\job_gun_factory\job_gun_factory_new.inc"
#include "..\library\systems\job_system\job_products_delivery\job_products_delivery_new.inc"
#include "..\library\systems\speedometr_system\speedometr_new.inc"
#include "..\library\systems\licensing_center\licensing_center_new.inc"
#include "..\library\systems\administration_system\administration_system_new.inc"
#include "..\library\systems\player_needs_system\player_needs_new.inc"
#include "..\library\systems\car_showroom_system\car_showroom_new.inc"
#include "..\library\systems\parking_fine_system\parking_fine_new.inc"
#include "..\library\systems\report_system\report_system_new.inc"
#include "..\library\systems\business_system\business_new.inc"
#include "..\library\systems\business_system\business_skin_shop_new.inc"
#include "..\library\systems\logo_system\logo_system_new.inc"
#include "..\library\systems\phone_system\phone_system_new.inc"
#include "..\library\systems\house_system\house_system_new.inc"
#include "..\library\systems\house_system\house_garage_system\house_garage_new.inc"
#include "..\library\systems\spawn_select_system\spawn_select_new.inc"
#include "..\library\systems\donate_shop_system\donate_shop_new.inc"

public OnGameModeInit(){
	EnableStuntBonusForAll(false);
	DisableInteriorEnterExits();

	Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, 1000);
	Streamer_SetMaxItems(STREAMER_TYPE_OBJECT, 1000);

	SetTimer("SecondTimer", 1000, true);
	SetTimer("MinuteTimer", 60000, true);


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
	if(!IsDataBaseLoaded()){
		SendClientMessage(playerid, COLOR_DARK_RED, "Сервер не загружен. Попробуйте позже.");
		KickPlayer(playerid);
		return 0;
	}

	ApplyAnimation(playerid, "DEALER", "_", 4.1, 1, 1, 1, 1, 1);
	ApplyAnimation(playerid, "PED", "_", 4.1, 1, 1, 1, 1, 1);
	ApplyAnimation(playerid, "SWORD", "_", 4.1, 1, 1, 1, 1, 1);
	ApplyAnimation(playerid, "BOMBER", "_", 4.1, 1, 1, 1, 1, 1);
	ApplyAnimation(playerid, "DEALER", "_", 4.1, 1, 1, 1, 1, 1);
	ApplyAnimation(playerid, "OTB", "_", 4.1, 1, 1, 1, 1, 1);
	ApplyAnimation(playerid, "VENDING", "_", 4.1, 1, 1, 1, 1, 1);
	ApplyAnimation(playerid, "shop", "_", 4.1, 1, 1, 1, 1, 1);
	ApplyAnimation(playerid, "COP_AMBIENT", "_", 4.1, 1, 1, 1, 1, 1);
	return 1;
}

public OnPlayerDisconnect(playerid, reason){
	player_info[playerid] = player_info_default;
	player_logged[playerid] = false;
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

public OnPlayerExitVehicle(playerid, vehicleid){
	if(GetPVarType(playerid, "InTempVehicle") && GetPVarInt(playerid, "InTempVehicle") == vehicleid){
		DestroyVehicle(vehicleid);
		DeletePVar(playerid, "InTempVehicle");
	}
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate){
	if(oldstate == PLAYER_STATE_DRIVER && GetPVarType(playerid, "InTempVehicle") && GetPVarInt(playerid, "InTempVehicle")
		&& IsValidVehicle(GetPVarInt(playerid, "InTempVehicle"))){

		DestroyVehicle(GetPVarInt(playerid, "InTempVehicle"));
		DeletePVar(playerid, "InTempVehicle");
	}
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
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ){
	if(!IsPlayerInAnyVehicle(playerid)){
		SetPlayerPos(playerid, fX, fY, fZ);
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);
	}
	else{
		SetVehiclePos(GetPlayerVehicleID(playerid), fX, fY, fZ);
		LinkVehicleToInterior(GetPlayerVehicleID(playerid), 0);
		SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), 0);

		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);
	}	
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

public OnPlayerCommandReceived(playerid, cmd[], params[], flags){
	return 1;
}

forward SecondTimer();
public SecondTimer(){
	SecondTimerStock();
	
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		OnPlayerSecondUpdate(i);
	}
	return 1;
}

forward MinuteTimer();
public MinuteTimer(){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		OnPlayerMinuteUpdate(i);
	}
	return 1;
}

forward KickPlayerPublic(playerid);
public KickPlayerPublic(playerid) { Kick(playerid); }

forward ServerRestartTimer();
public ServerRestartTimer() { SendRconCommand("gmx"); }

stock OnPlayerSecondUpdate(playerid){
	#pragma unused playerid
	return 1;
}

stock OnPlayerMinuteUpdate(playerid){
	#pragma unused playerid
	return 1;
}

stock SecondTimerStock(){
	return 1;
}

stock ConvertTime(unixtime, &year = 0, &month = 0, &day = 0, &hour = 0, &minute = 0, &second = 0)
{
	unixtime += 3600 * 3;

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

stock Float:frandom(Float:max)
{
    return floatdiv(float(random(0)), floatdiv(float(cellmax), max));
}

stock OnPlayerGiveInvItem(playerid, slot_index){
	#pragma unused playerid
	#pragma unused slot_index
	return 1;
}

stock OnPlayerLevelChanged(playerid, old_level, new_level){
	#pragma unused playerid
	#pragma unused old_level
	#pragma unused new_level
	return 1;
}

stock OnMySQLConnect(){
	return 1;
}

#include "..\library\systems\registration_system\registration_publics.inc"
#include "..\library\systems\inventory_system\inventory_publics.inc"
#include "..\library\systems\inventory_system\inventory_items\inventory_items_publics.inc"
#include "..\library\systems\inventory_system\accessories_items\accessories_items_publics.inc"
#include "..\library\systems\enter_exits_system\enter_exits_publics.inc"
#include "..\library\systems\bank_system\bank_system_publics.inc"
#include "..\library\systems\actor_system\actor_system_publics.inc"
#include "..\library\systems\quest_system\start_quest_system\start_quest_publics.inc"
#include "..\library\systems\speedometr_system\speedometr_publics.inc"
#include "..\library\systems\administration_system\administration_system_publics.inc"
#include "..\library\systems\car_showroom_system\car_showroom_publics.inc"
#include "..\library\systems\parking_fine_system\parking_fine_publics.inc"
#include "..\library\systems\report_system\report_system_publics.inc"
#include "..\library\systems\business_system\business_publics.inc"
#include "..\library\systems\logo_system\logo_system_publics.inc"
#include "..\library\systems\phone_system\phone_system_publics.inc"
#include "..\library\systems\house_system\house_system_publics.inc"
#include "..\library\systems\house_system\house_garage_system\house_garage_publics.inc"
#include "..\library\systems\spawn_select_system\spawn_select_publics.inc"
#include "..\library\systems\donate_shop_system\donate_shop_publics.inc"

#include "..\library\systems\registration_system\registration_dialogs.inc"
#include "..\library\systems\bank_system\bank_system_dialogs.inc"
#include "..\library\systems\quest_system\start_quest_system\start_quest_dialogs.inc"
#include "..\library\systems\job_system\job_loader\job_loader_dialogs.inc"
#include "..\library\systems\job_system\job_lumberjack\job_lumberjack_dialogs.inc"
#include "..\library\systems\job_system\job_garbage_sorter\job_garbage_sorter_dialogs.inc"
#include "..\library\systems\job_system\job_dealler\job_dealler_dialogs.inc"
#include "..\library\systems\job_system\job_gun_factory\job_gun_factory_dialogs.inc"
#include "..\library\systems\job_system\job_products_delivery\job_products_delivery_dialogs.inc"
#include "..\library\systems\licensing_center\licensing_center_dialogs.inc"
#include "..\library\systems\administration_system\administration_system_dialogs.inc"
#include "..\library\systems\player_needs_system\player_needs_dialogs.inc"
#include "..\library\systems\car_showroom_system\car_showroom_dialogs.inc"
#include "..\library\systems\parking_fine_system\parking_fine_dialogs.inc"
#include "..\library\systems\report_system\report_system_dialogs.inc"
#include "..\library\systems\business_system\business_dialogs.inc"
#include "..\library\systems\inventory_system\inventory_items\inventory_items_dialogs.inc"

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
#include "..\library\systems\job_system\job_lumberjack\job_lumberjack_functions.inc"
#include "..\library\systems\job_system\job_garbage_sorter\job_garbage_sorter_functions.inc"
#include "..\library\systems\job_system\job_dealler\job_dealler_functions.inc"
#include "..\library\systems\job_system\job_gun_factory\job_gun_factory_functions.inc"
#include "..\library\systems\job_system\job_products_delivery\job_products_delivery_functions.inc"
#include "..\library\systems\speedometr_system\speedometr_functions.inc"
#include "..\library\systems\licensing_center\licensing_center_functions.inc"
#include "..\library\systems\administration_system\administration_system_functions.inc"
#include "..\library\systems\player_needs_system\player_needs_functions.inc"
#include "..\library\systems\car_showroom_system\car_showroom_functions.inc"
#include "..\library\systems\report_system\report_system_functions.inc"
#include "..\library\systems\business_system\business_functions.inc"
#include "..\library\systems\logo_system\logo_system_functions.inc"
#include "..\library\systems\phone_system\phone_system_functions.inc"
#include "..\library\systems\house_system\house_system_functions.inc"
#include "..\library\systems\house_system\house_garage_system\house_garage_functions.inc"
#include "..\library\systems\spawn_select_system\spawn_select_functions.inc"
#include "..\library\systems\donate_shop_system\donate_shop_functions.inc"

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
#include "..\library\systems\job_system\job_lumberjack\job_lumberjack_hooks.inc"
#include "..\library\systems\job_system\job_garbage_sorter\job_garbage_sorter_hooks.inc"
#include "..\library\systems\job_system\job_dealler\job_dealler_hooks.inc"
#include "..\library\systems\job_system\job_gun_factory\job_gun_factory_hooks.inc"
#include "..\library\systems\job_system\job_products_delivery\job_products_delivery_hooks.inc"
#include "..\library\systems\speedometr_system\speedometr_hooks.inc"
#include "..\library\systems\licensing_center\licensing_center_hooks.inc"
#include "..\library\systems\administration_system\administration_system_hooks.inc"
#include "..\library\systems\player_needs_system\player_needs_hooks.inc"
#include "..\library\systems\car_showroom_system\car_showroom_hooks.inc"
#include "..\library\systems\parking_fine_system\parking_fine_hooks.inc"
#include "..\library\systems\report_system\report_system_hooks.inc"
#include "..\library\systems\business_system\business_hooks.inc"
#include "..\library\systems\logo_system\logo_system_hooks.inc"
#include "..\library\systems\phone_system\phone_system_hooks.inc"
#include "..\library\systems\inventory_system\inventory_items\inventory_items_hooks.inc"
#include "..\library\systems\house_system\house_system_hooks.inc"
#include "..\library\systems\house_system\house_garage_system\house_garage_hooks.inc"
#include "..\library\systems\spawn_select_system\spawn_select_hooks.inc"
#include "..\library\systems\donate_shop_system\donate_shop_hooks.inc"

#include "..\library\systems\inventory_system\inventory_commands.inc"
#include "..\library\systems\administration_system\administration_system_commands.inc"
#include "..\library\systems\player_needs_system\player_needs_commands.inc"
#include "..\library\systems\report_system\report_system_commands.inc"
#include "..\library\systems\business_system\business_commands.inc"
#include "..\library\systems\job_system\job_products_delivery\job_products_delivery_commands.inc"
#include "..\library\systems\phone_system\phone_system_commands.inc"
#include "..\library\systems\house_system\house_system_commands.inc"
#include "..\library\systems\house_system\house_garage_system\house_garage_commands.inc"
#include "..\library\systems\donate_shop_system\donate_shop_commands.inc"

//==============================================================================
#include "..\library\source\mapping\spawn.inc"
#include "..\library\source\mapping\bank_int.inc"
#include "..\library\source\mapping\loader_job_ext.inc"
#include "..\library\source\mapping\gun_factory.inc"
#include "..\library\source\mapping\licensing_center_ext.inc"
#include "..\library\source\mapping\licensing_center_int.inc"
#include "..\library\source\mapping\car_showroom_sf.inc"
#include "..\library\source\mapping\garage_int.inc"

//=============================================================== ДРУГИЕ ФУНКЦИИ
stock ShowStats(playerid, forplayerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"ID аккаунта: "cl_white"%d\n\
		"cl_blue"Ник-нейм: "cl_white"%s "cl_grey"[%d]\n\
		"cl_blue"Игровой уровень: "cl_white"%d "cl_grey"[%d/%d exp]\n\n\
		"cl_blue"Наличиные денежные средства: "cl_white"%d$\n\
		"cl_blue"Денежные средства на счету в банке: "cl_white"%d$",
		GetPlayerData(playerid, p_id), GetPlayerData(playerid, p_name), playerid,
		GetPlayerData(playerid, p_level), GetPlayerData(playerid, p_exp),
		GetExpToNextLevel(GetPlayerData(playerid, p_level) + 1),
		GetPlayerData(playerid, p_money), GetPlayerBankCardData(playerid, pbc_balance));

	Dialog_Open(forplayerid, Dialog:D_NULL, DIALOG_STYLE_MSGBOX,
		"Статистика персонажа", big_string,
		"Закрыть", "");
	big_string[0] = EOS;
	return true;
}

stock GetNameBySqlID(id){
	static const fmt_query[] = "SELECT name FROM accounts WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, id);
	mysql_query(mysql, query);

	new owner_name[MAX_PLAYER_NAME];
	cache_get_value_name(0, "name", owner_name);
	return owner_name;
}

stock IsDataBaseLoaded(){
	if(!load_info[loaded_vehicles]) return false;
	if(!load_info[loaded_businesses]) return false;
	if(!load_info[loaded_inventory_items]) return false;
	if(!load_info[loaded_start_quests]) return false;
	if(!load_info[loaded_houses]) return false;
	if(!load_info[loaded_garages]) return false;
	return true;
}

stock GenerateSimCardNumber(){
	new random_number = RandomEx(100000, 999999);

	static const fmt_query[] = "SELECT id FROM player_inventory_items WHERE item_id = %d AND value_one = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		ITEM_SIM_CARD_ID, random_number);
	mysql_query(mysql, query);

	new rows;
	cache_get_row_count(rows);
	
	while(rows){
		random_number = RandomEx(100000, 999999);
		mysql_format(mysql, query, sizeof(query), fmt_query,
			ITEM_SIM_CARD_ID, random_number);
		mysql_query(mysql, query);

		cache_get_row_count(rows);
	}

	return random_number;
}