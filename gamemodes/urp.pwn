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

public OnGameModeInit()
{
	Streamer_SetVisibleItems(STREAMER_TYPE_OBJECT, 1000);
	Streamer_SetMaxItems(STREAMER_TYPE_OBJECT, 1000);
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
	SetPlayerPosFindZ(playerid, fX, fY, fZ);
	SetPlayerInterior(playerid, 0);
	SetPlayerVirtualWorld(playerid, 0);
	return 1;
}

#include "..\library\systems\registration_system\registration_publics.inc"
#include "..\library\systems\inventory_system\inventory_items\inventory_items_publics.inc"
#include "..\library\systems\inventory_system\accessories_items\accessories_items_publics.inc"
#include "..\library\systems\enter_exits_system\enter_exits_publics.inc"
#include "..\library\systems\bank_system\bank_system_publics.inc"
#include "..\library\systems\actor_system\actor_system_publics.inc"

#include "..\library\systems\registration_system\registration_dialogs.inc"
#include "..\library\systems\bank_system\bank_system_dialogs.inc"

#include "..\library\systems\skin_select_system\skin_select_functions.inc"
#include "..\library\systems\registration_system\registration_functions.inc"
#include "..\library\systems\inventory_system\inventory_functions.inc"
#include "..\library\systems\inventory_system\inventory_items\inventory_items_functions.inc"
#include "..\library\systems\inventory_system\drop_items\drop_items_functions.inc"
#include "..\library\systems\inventory_system\accessories_items\accessories_items_functions.inc"
#include "..\library\systems\enter_exits_system\enter_exits_functions.inc"
#include "..\library\systems\bank_system\bank_system_functions.inc"

#include "..\library\systems\skin_select_system\skin_select_hooks.inc"
#include "..\library\systems\registration_system\registration_hooks.inc"
#include "..\library\systems\inventory_system\inventory_hooks.inc"
#include "..\library\systems\inventory_system\accessories_items\accessories_items_hooks.inc"
#include "..\library\systems\enter_exits_system\enter_exits_hooks.inc"
#include "..\library\systems\bank_system\bank_system_hooks.inc"
#include "..\library\systems\actor_system\actor_system_hooks.inc"

#include "..\library\systems\inventory_system\inventory_commands.inc"

//==============================================================================
#include "..\library\source\mapping\spawn.inc"
#include "..\library\source\mapping\bank_int.inc"

#warning TEST_CMD
CMD:tpt(playerid){
	SetPlayerVirtualWorld(playerid, 5);
	SetPlayerInterior(playerid, 5);
	SetPlayerPos(playerid, 1319.4584, 11.6674, 2068.5129);
}