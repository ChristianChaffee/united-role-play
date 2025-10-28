
DialogResponse:D_JOB_GF_INVITE(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	SetPlayerData(playerid, p_job, JOB_GUN_FACTORY);

	DisablePlayerCheckpoint(playerid);
	GPSDisable(playerid);

	SetPlayerSkin(playerid, (!GetPlayerData(playerid, p_sex)) ? (8) : (27));
	Job:GenerateMaterials(playerid);
	return true;
}

DialogResponse:D_JOB_GF_UNINVITE(playerid, response, listitem, inputtext[]){
	if(response) Job:GunFactoryUninvite(playerid);
}