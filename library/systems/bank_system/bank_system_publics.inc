
forward LoadPlayerBankCard(playerid);
public LoadPlayerBankCard(playerid){
	SetPlayerBankCardData(playerid, pbc_is_loaded, true);

	new rows;
	cache_get_row_count(rows);
	if(!rows) return false;

	cache_get_value_name_int(0, "id", GetPlayerBankCardData(playerid, pbc_id));
	cache_get_value_name_int(0, "account_id", GetPlayerBankCardData(playerid, pbc_account_id));
	cache_get_value_name_int(0, "balance", GetPlayerBankCardData(playerid, pbc_balance));
	cache_get_value_name_int(0, "create_time", GetPlayerBankCardData(playerid, pbc_create_time));
	cache_get_value_name_int(0, "pin_code", GetPlayerBankCardData(playerid, pbc_pin_code));
	cache_get_value_name_int(0, "status", GetPlayerBankCardData(playerid, pbc_status));
	cache_get_value_name_int(0, "timer", GetPlayerBankCardData(playerid, pbc_timer));

	cache_get_value_name_int(0, "type", GetPlayerBankCardData(playerid, pbc_type));
	cache_get_value_name_int(0, "deposit_balance", GetPlayerBankCardData(playerid, pbc_deposit_balance));
	cache_get_value_name_int(0, "deposit_percent", GetPlayerBankCardData(playerid, pbc_deposit_percent));
	return true;
}

forward LoadBankCardID(playerid);
public LoadBankCardID(playerid){
	SetPlayerBankCardData(playerid, pbc_is_loaded, true);
	SetPlayerBankCardData(playerid, pbc_id, cache_insert_id());
	SetPlayerBankCardData(playerid, pbc_status, BC_STATUS_WAIT);
	SetPlayerBankCardData(playerid, pbc_timer, BANK_CARD_CREATE_TIME * 60);

	SendClientMessage(playerid, COLOR_GREEN, "Ваша банковская карта будет готова в течении "#BANK_CARD_CREATE_TIME"-и минут.");
	return true;
}