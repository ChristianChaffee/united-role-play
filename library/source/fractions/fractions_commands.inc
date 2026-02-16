
CMD:fmenu(playerid){
	if(!player_logged[playerid]) return false;
	Fraction:ShowMenuUI(playerid);
	return true;
}