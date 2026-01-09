
stock Inventory:IsAccessoryItem(id){
	for(new i = 1; i < MAX_ACCESSORIES_ITEMS + 1; i++){
		if(GetAccessoryItemData(i, acc_item_id) == id) return true;
	}
	return false;
}

stock Inventory:FindAccItemWithSlot(playerid, slot_index){
	for(new i; i < PLAYER_ACCESSORIES_SLOTS_COUNT; i++){
		if(GetPlayerAccessoryData(playerid, i, player_acc_slot_index) == slot_index && GetPlayerAccessoryData(playerid, i, player_acc_id) > 0) return i;
	}
	return -1;
}

stock Inventory:GetItemIdByAccSlot(playerid, acc_slot_index){
	if(acc_slot_index < 0 || acc_slot_index >= PLAYER_ACCESSORIES_SLOTS_COUNT) return 0;
	return GetInventoryItemData(GetAccessoryItemData(GetPlayerAccessoryData(playerid, acc_slot_index, player_acc_item_id), acc_item_id), item_id);
}

stock Inventory:UpdatePlayersAccessories(playerid){
	for(new i; i < PLAYER_ACCESSORIES_SLOTS_COUNT; i++){
		RemovePlayerAttachedObject(playerid, i);
		if(!GetPlayerAccessoryData(playerid, i, player_acc_id)) continue;

		SetPlayerAttachedObject(playerid, i, 
			GetInventoryItemData(GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_item_id), item_model_id), 
			GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_bone), 
			GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_offset_x), 
			GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_offset_y), 
			GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_offset_z), 
			GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_offset_rot_x), 
			GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_offset_rot_y), 
			GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_offset_rot_z), 
			GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_scalse_x), 
			GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_scalse_y), 
			GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_scalse_z));
	}
}

stock Inventory:SetAccSlotNull(playerid, slot_index){
	if(slot_index < 0 || slot_index >= PLAYER_ACCESSORIES_SLOTS_COUNT) return false;

	static const fmt_query[] = "DELETE from player_accessoires WHERE id = %d LIMIT 1";
	new query[sizeof(fmt_query) - 2 + 11];
	mysql_format(mysql, query, sizeof(query), fmt_query, player_accessories_items_info[playerid][slot_index][player_acc_id]);
	mysql_tquery(mysql, query);

	SetPlayerAccessoryData(playerid, slot_index, player_acc_id, 0);
	SetPlayerAccessoryData(playerid, slot_index, player_acc_item_id, 0);
	SetPlayerAccessoryData(playerid, slot_index, player_acc_slot_index, 0);

	Inventory:UpdatePlayersAccessories(playerid);
	return true;
}

stock Inventory:GetFreeAccSlotIndex(playerid){
	for(new i; i < PLAYER_ACCESSORIES_SLOTS_COUNT; i++){
		if(Inventory:FindAccItemWithSlot(playerid, i) == -1) return i;
	}
	return -1;
}

stock Inventory:GivePlayerAccessory(playerid, slot_index, id){
	new index = -1;
	for(new i; i < PLAYER_ACCESSORIES_SLOTS_COUNT; i++){
		if(!GetPlayerAccessoryData(playerid, i, player_acc_id)){
			index = i;
			break;
		}
	}

	if(index == -1) return false;

	SetPlayerAccessoryData(playerid, index, player_acc_item_id, GetAccessoryItemData(Inventory:GetAccIndexByItemID(id), acc_id));
	SetPlayerAccessoryData(playerid, index, player_acc_slot_index, slot_index);

	static const fmt_query[] = "INSERT INTO player_accessoires (account_id, accessory_id, slot) VALUES (%d, %d, %d)";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_id), GetAccessoryItemData(Inventory:GetAccIndexByItemID(id), acc_id), slot_index);
	mysql_tquery(mysql, query, "AccInventoryGetCacheInsertId", "ii", playerid, index);
	return true;
}

stock Inventory:GetAccIndexByItemID(id){
	for(new i = 1; i < sizeof(accessories_items_info); i++){
		if(id == GetAccessoryItemData(i, acc_item_id)) return i;
	}
	return 0;
}