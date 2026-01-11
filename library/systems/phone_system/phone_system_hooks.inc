
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	cef_subscribe("Phone_Call", "Phone_Call");
	cef_subscribe("Phone_CallHangup", "Phone_CallHangup");
	cef_subscribe("Phone_BankTransfer", "Phone_BankTransfer");
	cef_subscribe("Phone_BusinessDeposit", "Phone_BusinessDeposit");
	cef_subscribe("Phone_BusinessWithdraw", "Phone_BusinessWithdraw");
	cef_subscribe("Phone_Close", "Phone_Close");

	cef_subscribe("Phone_VehicleLoad", "Phone_VehicleLoad");
	cef_subscribe("Phone_VehicleUnload", "Phone_VehicleUnload");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	new scdplayerid = GetPlayerCallerId(playerid);
	if(scdplayerid != INVALID_PLAYER_ID){
		SendClientMessage(scdplayerid, COLOR_GREY, "Абонент вышел из сети.");
		Phone:StopCall(scdplayerid);
	}

	Phone:StopCall(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerSpawn(playerid){
	new scdplayerid = GetPlayerCallerId(playerid);
	if(scdplayerid != INVALID_PLAYER_ID){
		SendClientMessage(scdplayerid, COLOR_GREY,  "Абонент вышел из сети.");
		Phone:StopCall(scdplayerid);
	}

	Phone:StopCall(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDeath(playerid, killerid, reason){
	new scdplayerid = GetPlayerCallerId(playerid);
	if(scdplayerid != INVALID_PLAYER_ID){
		SendClientMessage(scdplayerid, COLOR_GREY,  "Абонент вышел из сети.");
		Phone:StopCall(scdplayerid);
	}

	Phone:StopCall(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}