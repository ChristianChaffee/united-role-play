
DialogResponse:D_JOB_GS_INVITE(playerid, response, listitem, inputtext[]){
	if(response){
		SendClientMessage(playerid, COLOR_YELLOW, "Вы устроились на работу сортировщика мусора.");

		SetPlayerData(playerid, p_job, JOB_GARBAGE_SORTER);

		if(!GetPlayerData(playerid, p_sex)){
			SetPlayerSkin(playerid, (!random(2)) ? (260) : (27));
		}
		else{
			SetPlayerSkin(playerid, 41);
		}

		DisablePlayerCheckpoint(playerid);
		GPSDisable(playerid);

		SetPlayerAttachedObject(playerid, JOB_ATTACH_INDEX, 1574, 14, 0.3470, -0.0569, 0.0579, -78.1999, -173.0001, -21.9999, 0.3009, 0.3039, 0.2930, 0, 0);
		SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отправляйтесь собирать мусор.");
	}
}

DialogResponse:D_JOB_GS_UNINVITE(playerid, response, listitem, inputtext[]){
	if(response) Job:GSUninvite(playerid);
}