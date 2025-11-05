
CMD:needs(playerid){
	if(!player_logged[playerid]) return false;
	PlayerNeeds:ShowNeedMenu(playerid);
	return true;
}

CMD:piss(playerid){
	if(!player_logged[playerid]) return false;
	if(GetPlayerNeedValue(playerid, PLAYER_NEED_URINE) >= 80) return SendErrorMessage(playerid, "Вы не хотите в туалет.");

	new pissing_time = floatround(float((100 - GetPlayerNeedValue(playerid, PLAYER_NEED_URINE))) / 6.0);
	if(pissing_time < 5) pissing_time = 5;

	TogglePlayerControllable(playerid, false);
	SetPVarInt(playerid, "PlayerNeeds_PissingTime", pissing_time);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_PISSING);
	return true;
}