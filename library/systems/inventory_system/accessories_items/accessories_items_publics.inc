
forward LoadAccessoriesItems();
public LoadAccessoriesItems(){
	new rows, tick_time = GetTickCount();
	cache_get_row_count(rows);

	if(rows != MAX_ACCESSORIES_ITEMS){
		printf("[LoadAccessoriesItems]: Check table rows count.");
		return false;
	}

	for(new i; i < rows; i++){
		cache_get_value_name_int(i, "id", GetAccessoryItemData(i + 1, acc_id));
		cache_get_value_name_int(i, "item_id", GetAccessoryItemData(i + 1, acc_item_id));
		cache_get_value_name_int(i, "bone", GetAccessoryItemData(i + 1, acc_bone));
		cache_get_value_name_float(i, "offset_x", GetAccessoryItemData(i + 1, acc_offset_x));
		cache_get_value_name_float(i, "offset_y", GetAccessoryItemData(i + 1, acc_offset_y));
		cache_get_value_name_float(i, "offset_z", GetAccessoryItemData(i + 1, acc_offset_z));
		cache_get_value_name_float(i, "offset_rot_x", GetAccessoryItemData(i + 1, acc_offset_rot_x));
		cache_get_value_name_float(i, "offset_rot_y", GetAccessoryItemData(i + 1, acc_offset_rot_y));
		cache_get_value_name_float(i, "offset_rot_z", GetAccessoryItemData(i + 1, acc_offset_rot_z));
		cache_get_value_name_float(i, "scale_x", GetAccessoryItemData(i + 1, acc_scalse_x));
		cache_get_value_name_float(i, "scale_y", GetAccessoryItemData(i + 1, acc_scalse_y));
		cache_get_value_name_float(i, "scale_z", GetAccessoryItemData(i + 1, acc_scalse_z));
	}

	printf("[LoadAccessoriesItems]: Аксессуары загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
	return true;
}

forward LoadPlayerAccessoriesItems(playerid);
public LoadPlayerAccessoriesItems(playerid){
	new rows;
	cache_get_row_count(rows);

	for(new i; i < rows; i++){
		cache_get_value_name_int(i, "id", GetPlayerAccessoryData(playerid, i, player_acc_id));
		cache_get_value_name_int(i, "accessory_id", GetPlayerAccessoryData(playerid, i, player_acc_item_id));
		cache_get_value_name_int(i, "slot", GetPlayerAccessoryData(playerid, i, player_acc_slot_index));
	}
	return true;
}

forward AccInventoryGetCacheInsertId(playerid, slot_index);
public AccInventoryGetCacheInsertId(playerid, slot_index){
	SetPlayerAccessoryData(playerid, slot_index, player_acc_id, cache_insert_id());
	Inventory:UpdatePlayersAccessories(playerid);
}