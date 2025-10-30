
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook stock SetVehicleEngine(vehicleid, status){
	new playerid = GetVehicleDriver(vehicleid);
	if(playerid == INVALID_PLAYER_ID) return continue(vehicleid, status);

	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][1]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][1], (status) ? (SPEEDOMETR_COLOR_ON) : (SPEEDOMETR_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][1]);

	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][7]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][7], (status) ? (SPEEDOMETR_TEXT_COLOR_ON) : (SPEEDOMETR_TEXT_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][7]);
	
	return continue(vehicleid, status);
}

hook stock SetVehicleDoors(vehicleid, status){
	new playerid = GetVehicleDriver(vehicleid);
	if(playerid == INVALID_PLAYER_ID) return continue(vehicleid, status);

	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][2]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][2], (!status) ? (SPEEDOMETR_COLOR_ON) : (SPEEDOMETR_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][2]);

	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][8]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][8], (!status) ? (SPEEDOMETR_TEXT_COLOR_ON) : (SPEEDOMETR_TEXT_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][8]);

	return continue(vehicleid, status);
}

hook stock SetVehicleLights(vehicleid, status){
	new playerid = GetVehicleDriver(vehicleid);
	if(playerid == INVALID_PLAYER_ID) return continue(vehicleid, status);

	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][3]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][3], (status) ? (SPEEDOMETR_COLOR_ON) : (SPEEDOMETR_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][3]);

	PlayerTextDrawHide(playerid, TDEditor_PTD[playerid][9]);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][9], (status) ? (SPEEDOMETR_TEXT_COLOR_ON) : (SPEEDOMETR_TEXT_COLOR_OFF));
	PlayerTextDrawShow(playerid, TDEditor_PTD[playerid][9]);
	
	return continue(vehicleid, status);
}

hook stock SetVehicleFuel(vehicleid, Float:fuel){
	new result = continue(vehicleid, fuel);

	new playerid = GetVehicleDriver(vehicleid);
	if(playerid == INVALID_PLAYER_ID) return result;

	Speedometr:UpdateFuel(playerid);
	return result;
}

hook OnPlayerStateChange(playerid, newstate, oldstate){
	if(newstate == PLAYER_STATE_DRIVER){
		Speedometr:ShowPlayerSpeedometr(playerid, true);
	}
	if(oldstate == PLAYER_STATE_DRIVER){
		Speedometr:ShowPlayerSpeedometr(playerid, false);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit(){
	SetTimer("SpeedometrTimer", SPEEDOMETR_TIMER, true);

	TDEditor_TD[0] = TextDrawCreate(537.0017, 396.0075, "LD_SPAC:white"); // fon - km/h
	TextDrawTextSize(TDEditor_TD[0], 33.0000, 38.0000);
	TextDrawAlignment(TDEditor_TD[0], 1);
	TextDrawColor(TDEditor_TD[0], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[0], 255);
	TextDrawFont(TDEditor_TD[0], 4);
	TextDrawSetProportional(TDEditor_TD[0], 0);
	TextDrawSetShadow(TDEditor_TD[0], 0);

	TDEditor_TD[1] = TextDrawCreate(534.6350, 399.3258, "LD_SPAC:white"); // fon - km/h
	TextDrawTextSize(TDEditor_TD[1], 37.5097, 31.0000);
	TextDrawAlignment(TDEditor_TD[1], 1);
	TextDrawColor(TDEditor_TD[1], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[1], 255);
	TextDrawFont(TDEditor_TD[1], 4);
	TextDrawSetProportional(TDEditor_TD[1], 0);
	TextDrawSetShadow(TDEditor_TD[1], 0);

	TDEditor_TD[2] = TextDrawCreate(533.2681, 394.5332, "LD_BEAT:chit"); // fon - km/h
	TextDrawTextSize(TDEditor_TD[2], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[2], 1);
	TextDrawColor(TDEditor_TD[2], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[2], 255);
	TextDrawFont(TDEditor_TD[2], 4);
	TextDrawSetProportional(TDEditor_TD[2], 0);
	TextDrawSetShadow(TDEditor_TD[2], 0);

	TDEditor_TD[3] = TextDrawCreate(565.4680, 394.6036, "LD_BEAT:chit"); // fon - km/h
	TextDrawTextSize(TDEditor_TD[3], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[3], 1);
	TextDrawColor(TDEditor_TD[3], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[3], 255);
	TextDrawFont(TDEditor_TD[3], 4);
	TextDrawSetProportional(TDEditor_TD[3], 0);
	TextDrawSetShadow(TDEditor_TD[3], 0);

	TDEditor_TD[4] = TextDrawCreate(480.2665, 399.0963, "LD_BEAT:chit"); // fon - engine
	TextDrawTextSize(TDEditor_TD[4], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[4], 1);
	TextDrawColor(TDEditor_TD[4], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[4], 255);
	TextDrawFont(TDEditor_TD[4], 4);
	TextDrawSetProportional(TDEditor_TD[4], 0);
	TextDrawSetShadow(TDEditor_TD[4], 0);

	TDEditor_TD[5] = TextDrawCreate(480.1661, 407.4776, "LD_BEAT:chit"); // fon - engine
	TextDrawTextSize(TDEditor_TD[5], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[5], 1);
	TextDrawColor(TDEditor_TD[5], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[5], 255);
	TextDrawFont(TDEditor_TD[5], 4);
	TextDrawSetProportional(TDEditor_TD[5], 0);
	TextDrawSetShadow(TDEditor_TD[5], 0);

	TDEditor_TD[6] = TextDrawCreate(512.4663, 399.1665, "LD_BEAT:chit"); // fon - engine
	TextDrawTextSize(TDEditor_TD[6], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[6], 1);
	TextDrawColor(TDEditor_TD[6], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[6], 255);
	TextDrawFont(TDEditor_TD[6], 4);
	TextDrawSetProportional(TDEditor_TD[6], 0);
	TextDrawSetShadow(TDEditor_TD[6], 0);

	TDEditor_TD[7] = TextDrawCreate(512.4332, 407.7072, "LD_BEAT:chit"); // fon - engine
	TextDrawTextSize(TDEditor_TD[7], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[7], 1);
	TextDrawColor(TDEditor_TD[7], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[7], 255);
	TextDrawFont(TDEditor_TD[7], 4);
	TextDrawSetProportional(TDEditor_TD[7], 0);
	TextDrawSetShadow(TDEditor_TD[7], 0);

	TDEditor_TD[8] = TextDrawCreate(484.3333, 400.5704, "LD_SPAC:white"); // fon - engine
	TextDrawTextSize(TDEditor_TD[8], 32.0000, 14.5099);
	TextDrawAlignment(TDEditor_TD[8], 1);
	TextDrawColor(TDEditor_TD[8], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[8], 255);
	TextDrawFont(TDEditor_TD[8], 4);
	TextDrawSetProportional(TDEditor_TD[8], 0);
	TextDrawSetShadow(TDEditor_TD[8], 0);

	TDEditor_TD[9] = TextDrawCreate(481.6665, 403.0592, "LD_SPAC:white"); // fon - engine
	TextDrawTextSize(TDEditor_TD[9], 37.5298, 9.4699);
	TextDrawAlignment(TDEditor_TD[9], 1);
	TextDrawColor(TDEditor_TD[9], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[9], 255);
	TextDrawFont(TDEditor_TD[9], 4);
	TextDrawSetProportional(TDEditor_TD[9], 0);
	TextDrawSetShadow(TDEditor_TD[9], 0);

	TDEditor_TD[10] = TextDrawCreate(485.2662, 414.8591, "LD_BEAT:chit"); // fon - lock
	TextDrawTextSize(TDEditor_TD[10], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[10], 1);
	TextDrawColor(TDEditor_TD[10], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[10], 255);
	TextDrawFont(TDEditor_TD[10], 4);
	TextDrawSetProportional(TDEditor_TD[10], 0);
	TextDrawSetShadow(TDEditor_TD[10], 0);

	TDEditor_TD[11] = TextDrawCreate(485.1661, 423.2405, "LD_BEAT:chit"); // fon - lock
	TextDrawTextSize(TDEditor_TD[11], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[11], 1);
	TextDrawColor(TDEditor_TD[11], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[11], 255);
	TextDrawFont(TDEditor_TD[11], 4);
	TextDrawSetProportional(TDEditor_TD[11], 0);
	TextDrawSetShadow(TDEditor_TD[11], 0);

	TDEditor_TD[12] = TextDrawCreate(517.4666, 414.9295, "LD_BEAT:chit"); // fon - lock
	TextDrawTextSize(TDEditor_TD[12], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[12], 1);
	TextDrawColor(TDEditor_TD[12], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[12], 255);
	TextDrawFont(TDEditor_TD[12], 4);
	TextDrawSetProportional(TDEditor_TD[12], 0);
	TextDrawSetShadow(TDEditor_TD[12], 0);

	TDEditor_TD[13] = TextDrawCreate(517.4334, 423.4700, "LD_BEAT:chit"); // fon - lock
	TextDrawTextSize(TDEditor_TD[13], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[13], 1);
	TextDrawColor(TDEditor_TD[13], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[13], 255);
	TextDrawFont(TDEditor_TD[13], 4);
	TextDrawSetProportional(TDEditor_TD[13], 0);
	TextDrawSetShadow(TDEditor_TD[13], 0);

	TDEditor_TD[14] = TextDrawCreate(489.3330, 416.3330, "LD_SPAC:white"); // fon - lock
	TextDrawTextSize(TDEditor_TD[14], 32.0000, 14.5099);
	TextDrawAlignment(TDEditor_TD[14], 1);
	TextDrawColor(TDEditor_TD[14], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[14], 255);
	TextDrawFont(TDEditor_TD[14], 4);
	TextDrawSetProportional(TDEditor_TD[14], 0);
	TextDrawSetShadow(TDEditor_TD[14], 0);

	TDEditor_TD[15] = TextDrawCreate(486.6665, 418.8222, "LD_SPAC:white"); // fon - lock
	TextDrawTextSize(TDEditor_TD[15], 37.5298, 9.4699);
	TextDrawAlignment(TDEditor_TD[15], 1);
	TextDrawColor(TDEditor_TD[15], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[15], 255);
	TextDrawFont(TDEditor_TD[15], 4);
	TextDrawSetProportional(TDEditor_TD[15], 0);
	TextDrawSetShadow(TDEditor_TD[15], 0);

	TDEditor_TD[16] = TextDrawCreate(584.6157, 399.0960, "LD_BEAT:chit"); // fon - lights
	TextDrawTextSize(TDEditor_TD[16], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[16], 1);
	TextDrawColor(TDEditor_TD[16], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[16], 255);
	TextDrawFont(TDEditor_TD[16], 4);
	TextDrawSetProportional(TDEditor_TD[16], 0);
	TextDrawSetShadow(TDEditor_TD[16], 0);

	TDEditor_TD[17] = TextDrawCreate(584.7155, 407.4775, "LD_BEAT:chit"); // fon - lights
	TextDrawTextSize(TDEditor_TD[17], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[17], 1);
	TextDrawColor(TDEditor_TD[17], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[17], 255);
	TextDrawFont(TDEditor_TD[17], 4);
	TextDrawSetProportional(TDEditor_TD[17], 0);
	TextDrawSetShadow(TDEditor_TD[17], 0);

	TDEditor_TD[18] = TextDrawCreate(616.7160, 399.1665, "LD_BEAT:chit"); // fon - lights
	TextDrawTextSize(TDEditor_TD[18], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[18], 1);
	TextDrawColor(TDEditor_TD[18], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[18], 255);
	TextDrawFont(TDEditor_TD[18], 4);
	TextDrawSetProportional(TDEditor_TD[18], 0);
	TextDrawSetShadow(TDEditor_TD[18], 0);

	TDEditor_TD[19] = TextDrawCreate(616.6828, 407.7070, "LD_BEAT:chit"); // fon - lights
	TextDrawTextSize(TDEditor_TD[19], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[19], 1);
	TextDrawColor(TDEditor_TD[19], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[19], 255);
	TextDrawFont(TDEditor_TD[19], 4);
	TextDrawSetProportional(TDEditor_TD[19], 0);
	TextDrawSetShadow(TDEditor_TD[19], 0);

	TDEditor_TD[20] = TextDrawCreate(588.5825, 400.5700, "LD_SPAC:white"); // fon - lights
	TextDrawTextSize(TDEditor_TD[20], 32.0000, 14.5099);
	TextDrawAlignment(TDEditor_TD[20], 1);
	TextDrawColor(TDEditor_TD[20], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[20], 255);
	TextDrawFont(TDEditor_TD[20], 4);
	TextDrawSetProportional(TDEditor_TD[20], 0);
	TextDrawSetShadow(TDEditor_TD[20], 0);

	TDEditor_TD[21] = TextDrawCreate(585.9157, 403.0592, "LD_SPAC:white"); // fon - lights
	TextDrawTextSize(TDEditor_TD[21], 37.5298, 9.4699);
	TextDrawAlignment(TDEditor_TD[21], 1);
	TextDrawColor(TDEditor_TD[21], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[21], 255);
	TextDrawFont(TDEditor_TD[21], 4);
	TextDrawSetProportional(TDEditor_TD[21], 0);
	TextDrawSetShadow(TDEditor_TD[21], 0);

	TDEditor_TD[22] = TextDrawCreate(579.3828, 414.8741, "LD_BEAT:chit"); // fon - health
	TextDrawTextSize(TDEditor_TD[22], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[22], 1);
	TextDrawColor(TDEditor_TD[22], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[22], 255);
	TextDrawFont(TDEditor_TD[22], 4);
	TextDrawSetProportional(TDEditor_TD[22], 0);
	TextDrawSetShadow(TDEditor_TD[22], 0);

	TDEditor_TD[23] = TextDrawCreate(579.5828, 423.2554, "LD_BEAT:chit"); // fon - health
	TextDrawTextSize(TDEditor_TD[23], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[23], 1);
	TextDrawColor(TDEditor_TD[23], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[23], 255);
	TextDrawFont(TDEditor_TD[23], 4);
	TextDrawSetProportional(TDEditor_TD[23], 0);
	TextDrawSetShadow(TDEditor_TD[23], 0);

	TDEditor_TD[24] = TextDrawCreate(611.5831, 414.9443, "LD_BEAT:chit"); // fon - health
	TextDrawTextSize(TDEditor_TD[24], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[24], 1);
	TextDrawColor(TDEditor_TD[24], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[24], 255);
	TextDrawFont(TDEditor_TD[24], 4);
	TextDrawSetProportional(TDEditor_TD[24], 0);
	TextDrawSetShadow(TDEditor_TD[24], 0);

	TDEditor_TD[25] = TextDrawCreate(611.5499, 423.4848, "LD_BEAT:chit"); // fon - health
	TextDrawTextSize(TDEditor_TD[25], 8.0000, 9.0000);
	TextDrawAlignment(TDEditor_TD[25], 1);
	TextDrawColor(TDEditor_TD[25], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[25], 255);
	TextDrawFont(TDEditor_TD[25], 4);
	TextDrawSetProportional(TDEditor_TD[25], 0);
	TextDrawSetShadow(TDEditor_TD[25], 0);

	TDEditor_TD[26] = TextDrawCreate(583.4497, 416.3481, "LD_SPAC:white"); // fon - health
	TextDrawTextSize(TDEditor_TD[26], 32.0000, 14.5099);
	TextDrawAlignment(TDEditor_TD[26], 1);
	TextDrawColor(TDEditor_TD[26], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[26], 255);
	TextDrawFont(TDEditor_TD[26], 4);
	TextDrawSetProportional(TDEditor_TD[26], 0);
	TextDrawSetShadow(TDEditor_TD[26], 0);

	TDEditor_TD[27] = TextDrawCreate(580.7830, 418.8370, "LD_SPAC:white"); // fon - health
	TextDrawTextSize(TDEditor_TD[27], 37.5298, 9.4699);
	TextDrawAlignment(TDEditor_TD[27], 1);
	TextDrawColor(TDEditor_TD[27], 623191551);
	TextDrawBackgroundColor(TDEditor_TD[27], 255);
	TextDrawFont(TDEditor_TD[27], 4);
	TextDrawSetProportional(TDEditor_TD[27], 0);
	TextDrawSetShadow(TDEditor_TD[27], 0);

	TDEditor_TD[28] = TextDrawCreate(534.9995, 427.9482, "LD_SPAC:white"); // fon - fuel
	TextDrawTextSize(TDEditor_TD[28], 36.0000, 9.9799);
	TextDrawAlignment(TDEditor_TD[28], 1);
	TextDrawColor(TDEditor_TD[28], 1027423743);
	TextDrawBackgroundColor(TDEditor_TD[28], 255);
	TextDrawFont(TDEditor_TD[28], 4);
	TextDrawSetProportional(TDEditor_TD[28], 0);
	TextDrawSetShadow(TDEditor_TD[28], 0);

	TDEditor_TD[29] = TextDrawCreate(529.3331, 425.4591, "LD_BEAT:chit"); // fon - fuel
	TextDrawTextSize(TDEditor_TD[29], 13.0000, 15.0000);
	TextDrawAlignment(TDEditor_TD[29], 1);
	TextDrawColor(TDEditor_TD[29], 1027423743);
	TextDrawBackgroundColor(TDEditor_TD[29], 255);
	TextDrawFont(TDEditor_TD[29], 4);
	TextDrawSetProportional(TDEditor_TD[29], 0);
	TextDrawSetShadow(TDEditor_TD[29], 0);

	TDEditor_TD[30] = TextDrawCreate(564.9995, 425.4591, "LD_BEAT:chit"); // fon - fuel
	TextDrawTextSize(TDEditor_TD[30], 13.0000, 15.0000);
	TextDrawAlignment(TDEditor_TD[30], 1);
	TextDrawColor(TDEditor_TD[30], 1027423743);
	TextDrawBackgroundColor(TDEditor_TD[30], 255);
	TextDrawFont(TDEditor_TD[30], 4);
	TextDrawSetProportional(TDEditor_TD[30], 0);
	TextDrawSetShadow(TDEditor_TD[30], 0);

	TDEditor_TD[31] = TextDrawCreate(542.3333, 431.2666, "LD_SPAC:white"); // пусто
	TextDrawTextSize(TDEditor_TD[31], 28.0000, 4.0000);
	TextDrawAlignment(TDEditor_TD[31], 1);
	TextDrawColor(TDEditor_TD[31], 538976511);
	TextDrawBackgroundColor(TDEditor_TD[31], 255);
	TextDrawFont(TDEditor_TD[31], 4);
	TextDrawSetProportional(TDEditor_TD[31], 0);
	TextDrawSetShadow(TDEditor_TD[31], 0);

	TDEditor_TD[32] = TextDrawCreate(536.3330, 428.1036, "F"); // пусто
	TextDrawLetterSize(TDEditor_TD[32], 0.1545, 0.9487);
	TextDrawAlignment(TDEditor_TD[32], 1);
	TextDrawColor(TDEditor_TD[32], -2139062017);
	TextDrawBackgroundColor(TDEditor_TD[32], 255);
	TextDrawFont(TDEditor_TD[32], 1);
	TextDrawSetProportional(TDEditor_TD[32], 1);
	TextDrawSetShadow(TDEditor_TD[32], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid){
	TDEditor_PTD[playerid][0] = CreatePlayerTextDraw(playerid, 535.9998, 412.1849, "particle:lamp_shad_64"); // km/h
	PlayerTextDrawTextSize(playerid, TDEditor_PTD[playerid][0], 35.0000, 22.0000);
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][0], 1);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][0], -238863772);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][0], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][0], 4);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][0], 0);

	TDEditor_PTD[playerid][1] = CreatePlayerTextDraw(playerid, 484.3333, 407.2073, "particle:lamp_shad_64"); // engine
	PlayerTextDrawTextSize(playerid, TDEditor_PTD[playerid][1], 33.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][1], 1);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][1], -238863772);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][1], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][1], 4);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][1], 0);

	TDEditor_PTD[playerid][2] = CreatePlayerTextDraw(playerid, 488.9999, 422.9703, "particle:lamp_shad_64"); // lock
	PlayerTextDrawTextSize(playerid, TDEditor_PTD[playerid][2], 33.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][2], 1);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][2], -238863772);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][2], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][2], 4);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][2], 0);

	TDEditor_PTD[playerid][3] = CreatePlayerTextDraw(playerid, 587.9995, 407.2073, "particle:lamp_shad_64"); // lights
	PlayerTextDrawTextSize(playerid, TDEditor_PTD[playerid][3], 33.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][3], 1);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][3], -1061109660);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][3], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][3], 4);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][3], 0);

	TDEditor_PTD[playerid][4] = CreatePlayerTextDraw(playerid, 583.3330, 422.9703, "particle:lamp_shad_64"); // health
	PlayerTextDrawTextSize(playerid, TDEditor_PTD[playerid][4], 33.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][4], 1);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][4], -238863772);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][4], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][4], 4);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][4], 0);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][4], 0);

	TDEditor_PTD[playerid][5] = CreatePlayerTextDraw(playerid, 543.3331, 432.5111, "LD_SPAC:white"); // пусто
	PlayerTextDrawTextSize(playerid, TDEditor_PTD[playerid][5], 26.0000, 1.6599); // 26.0, 1.6599
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][5], 1);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][5], -238863617);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][5], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][5], 4);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][5], 0);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][5], 0);

	TDEditor_PTD[playerid][6] = CreatePlayerTextDraw(playerid, 552.9998, 403.6296, "100"); // пусто
	PlayerTextDrawLetterSize(playerid, TDEditor_PTD[playerid][6], 0.3569, 1.9400);
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][6], 2);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][6], -1);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][6], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][6], 3);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][6], 1);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][6], 0);

	TDEditor_PTD[playerid][7] = CreatePlayerTextDraw(playerid, 500.6665, 403.2149, "ENGINE"); // пусто
	PlayerTextDrawLetterSize(playerid, TDEditor_PTD[playerid][7], 0.1555, 0.8988);
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][7], 2);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][7], -238863617);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][7], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][7], 2);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][7], 1);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][7], 0);

	TDEditor_PTD[playerid][8] = CreatePlayerTextDraw(playerid, 506.0000, 418.5629, "lock"); // пусто
	PlayerTextDrawLetterSize(playerid, TDEditor_PTD[playerid][8], 0.1555, 0.8988);
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][8], 2);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][8], -238863617);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][8], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][8], 2);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][8], 1);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][8], 0);

	TDEditor_PTD[playerid][9] = CreatePlayerTextDraw(playerid, 604.6668, 403.2149, "lights"); // пусто
	PlayerTextDrawLetterSize(playerid, TDEditor_PTD[playerid][9], 0.1555, 0.8988);
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][9], 2);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][9], -2139062017);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][9], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][9], 2);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][9], 1);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][9], 0);

	TDEditor_PTD[playerid][10] = CreatePlayerTextDraw(playerid, 599.3336, 419.3926, "health:_1000"); // пусто
	PlayerTextDrawLetterSize(playerid, TDEditor_PTD[playerid][10], 0.1129, 0.7868);
	PlayerTextDrawAlignment(playerid, TDEditor_PTD[playerid][10], 2);
	PlayerTextDrawColor(playerid, TDEditor_PTD[playerid][10], -238863617);
	PlayerTextDrawBackgroundColor(playerid, TDEditor_PTD[playerid][10], 255);
	PlayerTextDrawFont(playerid, TDEditor_PTD[playerid][10], 2);
	PlayerTextDrawSetProportional(playerid, TDEditor_PTD[playerid][10], 1);
	PlayerTextDrawSetShadow(playerid, TDEditor_PTD[playerid][10], 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}