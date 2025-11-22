
CMD:mm(playerid){
	if(!player_logged[playerid]) return false;
	ShowMainMenu(playerid);
	return true;
}

CMD:help(playerid){
	if(!player_logged[playerid]) return false;
	ShowHelpMenu(playerid);
	return true;
}