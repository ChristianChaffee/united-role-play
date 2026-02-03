
CMD:donate(playerid){
	if(!player_logged[playerid]) return false;

	Donate:ShowUI(playerid);
	return true;
}