
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	CreateDynamicMapIcon(BANK_MAP_ICON_POS, BANK_MAP_ICON_TYPE, -1, 0, 0);

	for(new i; i < sizeof(bank_cashboxes_pos); i++){
		CreateDynamicPickup(BANK_CASHBOX_PICKUP_MODEL_ID, 23, 
			bank_cashboxes_pos[i][0], bank_cashboxes_pos[i][1], bank_cashboxes_pos[i][2], 
			BANK_VW, BANK_INT);

		small_string[0] = EOS;
		format(small_string, sizeof(small_string), ""cl_white"Касса №%d\n"cl_grey"'Left ALT'", i + 1);
		CreateDynamic3DTextLabel(small_string, -1, 
			bank_cashboxes_pos[i][0], bank_cashboxes_pos[i][1], bank_cashboxes_pos[i][2], 15.0, 
			_, _, 1, 
			BANK_VW, BANK_INT);
		small_string[0] = EOS;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerSpawn(playerid){
	if(!GetPlayerBankCardData(playerid, pbc_is_loaded) && player_logged[playerid]){
		static const fmt_query[] = "SELECT * FROM bank_cards WHERE account_id = %d LIMIT 1";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id));
		mysql_tquery(mysql, query, "LoadPlayerBankCard", "i", playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK){
		for(new i; i < sizeof(bank_cashboxes_pos); i++){
			if(IsPlayerInRangeOfPoint(playerid, 1.0, bank_cashboxes_pos[i][0], bank_cashboxes_pos[i][1], bank_cashboxes_pos[i][2])){
				Bank:ShowMainMenu(playerid);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	if(GetPlayerBankCardData(playerid, pbc_id)){
		static const fmt_query[] = "UPDATE bank_cards SET timer = %d WHERE id = %d LIMIT 1";
		new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetPlayerBankCardData(playerid, pbc_timer), GetPlayerBankCardData(playerid, pbc_id));
		mysql_tquery(mysql, query);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook stock OnPlayerSecondUpdate(playerid){
	if(GetPlayerBankCardData(playerid, pbc_is_loaded) && GetPlayerBankCardData(playerid, pbc_id)
		&& GetPlayerBankCardData(playerid, pbc_status) == BC_STATUS_WAIT && GetPlayerBankCardData(playerid, pbc_timer) > 0){

		SetPlayerBankCardData(playerid, pbc_timer, GetPlayerBankCardData(playerid, pbc_timer) - 1);
		if(GetPlayerBankCardData(playerid, pbc_timer) <= 0){
			SendClientMessage(playerid, COLOR_GREEN, "Ваша банковская карта готова! Для получения обратитесь в отделение банка.");
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}