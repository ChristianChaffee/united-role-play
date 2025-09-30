
forward LoadInventoryItems();
public LoadInventoryItems(){
	new rows, tick_time = GetTickCount();
	cache_get_row_count(rows);

	for(new i; i < rows; i++){
		cache_get_value_name_int(i, "id", inventory_item_info[i + 1][item_id]);
		cache_get_value_name(i, "name", inventory_item_info[i + 1][item_name]);
		cache_get_value_name(i, "description", inventory_item_info[i + 1][item_description]);
		cache_get_value_name_int(i, "model_id", inventory_item_info[i + 1][item_model_id]);

		cache_get_value_name_float(i, "rot_x", inventory_item_info[i + 1][item_rot_x]);
		cache_get_value_name_float(i, "rot_y", inventory_item_info[i + 1][item_rot_y]);
		cache_get_value_name_float(i, "rot_z", inventory_item_info[i + 1][item_rot_z]);
		cache_get_value_name_float(i, "scale", inventory_item_info[i + 1][item_scale]);
	}

	printf("[LoadInventoryItems]: Предметы инвентаря загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
	return true;
}

forward LoadPlayerInventoryItems(playerid);
public LoadPlayerInventoryItems(playerid){
	new rows, tick_time = GetTickCount();
	cache_get_row_count(rows);

	for(new i; i < rows; i++){
		cache_get_value_name_int(i, "id", inventory_player[playerid][i][inv_player_id]);
		cache_get_value_name_int(i, "account_id", inventory_player[playerid][i][inv_player_account_id]);
		cache_get_value_name_int(i, "item_id", inventory_player[playerid][i][inv_player_item_id]);
		cache_get_value_name_int(i, "item_count", inventory_player[playerid][i][inv_player_item_count]);
		cache_get_value_name_int(i, "item_slot", inventory_player[playerid][i][inv_player_item_slot]);
		cache_get_value_name_int(i, "value_one", inventory_player[playerid][i][inv_player_value_one]);
		cache_get_value_name_int(i, "value_two", inventory_player[playerid][i][inv_player_value_two]);
		cache_get_value_name(i, "value_str", inventory_player[playerid][i][inv_player_value_str]);
		cache_get_value_name_int(i, "can_drop", inventory_player[playerid][i][inv_player_can_drop]);
	}

	printf("[LoadPlayerInventoryItems]: Предметы инвентаря %s[%d] загружены <%d шт> <%d мс>", 
		GetPlayerData(playerid, p_name), playerid, 
		rows, GetTickCount() - tick_time);
	return true;
}

forward InventoryGetCacheInsertId(playerid, slot_index);
public InventoryGetCacheInsertId(playerid, slot_index){
	SetPlayerInventoryItemData(playerid, slot_index, inv_player_id, cache_insert_id());
}