
DialogResponse:D_JOB_LUMBERJACK_INVITE(playerid, response, listitem, inputtext[]){
	if(response){
		SendClientMessage(playerid, COLOR_YELLOW, "Вы устроились на работу лесоруба.");

		SetPlayerData(playerid, p_job, JOB_LUMBERJACK);
		SetPlayerSkin(playerid, (!GetPlayerData(playerid, p_sex)) ? (16) : (41));

		DisablePlayerCheckpoint(playerid);
		GPSDisable(playerid);

		Job:SetLumberjackTreesCP(playerid);
	}
}

DialogResponse:D_JOB_LUMBERJACK_UNINVITE(playerid, response, listitem, inputtext[]){
	if(response) Job:LumberjackUninvite(playerid);
}