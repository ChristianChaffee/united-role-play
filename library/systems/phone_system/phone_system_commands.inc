
CMD:phone(playerid){
	if(!player_logged[playerid]) return false;
	if(!GetPlayerData(playerid, p_phone)) return SendErrorMessage(playerid, "У Вас нет телефона.");
	
	Phone:ShowPhone(playerid);
	return true;
}