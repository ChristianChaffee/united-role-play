
CMD:bpanel(playerid){
	if(!player_logged[playerid]) return false;
	
	Business:ShowPlayerSelectBusiness(playerid);
	return true;
}