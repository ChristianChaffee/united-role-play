
stock Inventory:Show(playerid){
	cef_emit_event(playerid, "showInventory");

	cef_emit_event(playerid, "setInventoryData",
		CEFSTR(GetPlayerData(playerid, p_name)),
		CEFINT(GetPlayerData(playerid, p_level)),
		CEFINT(GetPlayerData(playerid, p_exp)),
		CEFINT(GetExpToNextLevel(GetPlayerData(playerid, p_level) + 1)),
		CEFINT(GetPlayerData(playerid, p_money)),
		CEFINT(GetPlayerBankCardData(playerid, pbc_balance)));

	if(GetPlayerData(playerid, p_phone)){
		cef_emit_event(playerid, "setPhoneSlot",
			CEFSTR(" "),
			CEFSTR(GetInventoryItemData(ITEM_PHONE_ID, item_img_url)));
	}
	if(GetPlayerData(playerid, p_sim_card) != -1){
		new sim_card_name[16];
		format(sim_card_name, sizeof(sim_card_name), "%d", GetPlayerData(playerid, p_sim_card));

		cef_emit_event(playerid, "setSimCardSlot",
			CEFSTR(sim_card_name),
			CEFSTR(GetInventoryItemData(ITEM_SIM_CARD_ID, item_img_url)));
	}

	for(new i; i < MAX_INVENTORY_SLOTS; i++){
		if(!GetPlayerInventoryItemData(playerid, i, inv_player_id)) continue;

		cef_emit_event(playerid, "setInventoryItem",
			CEFINT(GetPlayerInventoryItemData(playerid, i, inv_player_item_slot)),
			CEFSTR(GetInventoryItemData(inventory_player[playerid][i][inv_player_item_id], item_name)),
			CEFINT(GetPlayerInventoryItemData(playerid, i, inv_player_item_count)),
			CEFSTR(GetInventoryItemData(inventory_player[playerid][i][inv_player_item_id], item_img_url)),
			CEFSTR(GetInventoryItemData(inventory_player[playerid][i][inv_player_item_id], item_description)));
	}

	for(new i; i < PLAYER_ACCESSORIES_SLOTS_COUNT; i++){
		if(!GetPlayerAccessoryData(playerid, i, player_acc_id)) continue;

		cef_emit_event(playerid, "setAccessorySlot",
			CEFINT(GetPlayerAccessoryData(playerid, i, player_acc_slot_index)),
			CEFSTR(GetInventoryItemData(GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_item_id), item_name)),
			CEFSTR(GetInventoryItemData(GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_item_id), item_img_url)));
	}
	return true;
}

stock Inventory:Hide(playerid){
	cef_emit_event(playerid, "hideInventory");
	return true;
}

stock Inventory:ShowError(playerid, text[]){
	cef_emit_event(playerid, "showInventoryError",
		CEFSTR(text));
	return true;
}