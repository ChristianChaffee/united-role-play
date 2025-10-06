
DialogResponse:D_BANK_CARD_ACCEPT_CREATE(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	if(GetPlayerData(playerid, p_money) < BANK_CARD_CREATE_PRICE){
		SendErrorMessage(playerid, "У Вас недостаточно наличных денежных средств.");
		return false;
	}

	GiveMoney(playerid, -BANK_CARD_CREATE_PRICE);

	static const fmt_query[] = "INSERT INTO bank_cards (account_id, status, timer) VALUES (%d, %d, %d)";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_id), BC_STATUS_WAIT, BANK_CARD_CREATE_TIME * 60);
	mysql_tquery(mysql, query, "LoadBankCardID", "i", playerid);
	return true;
}