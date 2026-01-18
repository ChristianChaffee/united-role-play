
CMD:hpanel(playerid){
	if(!player_logged[playerid]) return false;

	new house_index = House:GetPlayerHouseIndex(playerid);

	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в личном доме.");
		return false;
	}

	House:ShowMenu(playerid, house_index);
	return true;
}