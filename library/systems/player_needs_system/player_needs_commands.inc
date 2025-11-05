
CMD:needs(playerid){
	if(!player_logged[playerid]) return false;
	PlayerNeeds:ShowNeedMenu(playerid);
	return true;
}