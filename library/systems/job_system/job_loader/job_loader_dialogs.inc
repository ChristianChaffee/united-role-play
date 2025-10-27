
DialogResponse:D_JOB_LOADER_INVITE(playerid, response, listitem, inputtext[]){
	if(response){
		SendClientMessage(playerid, COLOR_YELLOW, "Вы устроились на работу грузчика.");

		SetPlayerData(playerid, p_job, JOB_LOADER);

		if(!GetPlayerData(playerid, p_sex)){
			SetPlayerSkin(playerid, (!random(2)) ? (260) : (27));
		}
		else{
			SetPlayerSkin(playerid, 41);
		}

		DisablePlayerCheckpoint(playerid);
		GPSDisable(playerid);

		Job:GenerateLoaderLoad(playerid);
	}
}

DialogResponse:D_JOB_LOADER_UNINVITE(playerid, response, listitem, inputtext[]){
	if(response) Job:LoaderUninvite(playerid);
}