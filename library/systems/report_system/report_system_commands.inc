
CMD:rep(playerid){
	if(!player_logged[playerid]) return false;
	if(GetPlayerAdminData(playerid, pa_id) > 0 && GetPlayerAdminData(playerid, pa_logged)){
		Report:ShowAdminList(playerid);
	}
	else Report:ShowSelectType(playerid);
	return true;
}