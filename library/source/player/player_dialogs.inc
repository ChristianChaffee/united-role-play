
DialogResponse:D_MAIN_MENU(playerid, response, listitem, inputtext[]){
	if(response){
		switch(listitem){
			case 0: ShowStats(playerid, playerid);
			case 1: ShowHelpMenu(playerid);
			case 2: callcmd::rep(playerid);
		}
	}
	return true;
}

DialogResponse:D_HELP_MENU(playerid, response, listitem, inputtext[]){
	if(!response) return ShowMainMenu(playerid);

	switch(listitem){
		case 0: ShowHelp(playerid, COMMAND_TYPE_GENERAL);
		case 1: ShowHelp(playerid, COMMAND_TYPE_VEHICLE);
	}
	return true;
}

DialogResponse:D_HELP(playerid, response, listitem, inputtext[]){
	ShowHelpMenu(playerid);
	return true;
}