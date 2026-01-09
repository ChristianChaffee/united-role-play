
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

CMD:maskoff(playerid){
	if(!player_logged[playerid]) return false;
	if(!GetPVarType(playerid, "Mask")) return SendErrorMessage(playerid, "На Вас нет маски.");

	SendMeAction(playerid, "снял(-а) маску.");
	DeletePVar(playerid, "Mask");
	DeletePVar(playerid, "Mask_Timer");

	SetPlayerColor(playerid, GetPlayerColorEx(playerid));
	GameTextForPlayer(playerid, "~b~Invisible ~r~OFF", 5000, 3);
	return true;
}