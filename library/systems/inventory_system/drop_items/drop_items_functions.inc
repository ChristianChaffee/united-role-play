
stock Inventory:CreateDropItem(Float:x, Float:y, Float:z, vw, int, inv_item_id, count, value_one, value_two, value_str[]){
	if(inv_item_id > MAX_INVENTORY_ITEMS || inv_item_id <= 0) return false;

	new drop_index = -1;
	for(new i; i < MAX_DROP_ITEMS; i++){
		if(!drop_item_info[i][drop_item_id]){
			drop_index = i;
			break;
		}
	}

	if(drop_index == -1) return false;

	drop_item_info[drop_index][drop_item_id] = inv_item_id;
	drop_item_info[drop_index][drop_item_count] = count;
	drop_item_info[drop_index][drop_item_pos][0] = x;
	drop_item_info[drop_index][drop_item_pos][1] = y;
	drop_item_info[drop_index][drop_item_pos][2] = z;
	drop_item_info[drop_index][drop_item_vw] = vw;
	drop_item_info[drop_index][drop_item_int] = int;

	drop_item_info[drop_index][drop_item_value_one] = value_one;
	drop_item_info[drop_index][drop_item_value_two] = value_two;
	format(drop_item_info[drop_index][drop_item_value_str], 256, "%s", value_str);

	drop_item_info[drop_index][drop_item_object] = CreateDynamicObject(GetInventoryItemData(inv_item_id, item_model_id), 
		x, y, z, 0.0, 0.0, 0.0, vw, int);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_blue"%s\n\
		"cl_blue"Количество: "cl_white"%d шт\n\
		"cl_grey"[/takedrop]",
		GetInventoryItemData(inv_item_id, item_name), count);

	drop_item_info[drop_index][drop_item_text] = CreateDynamic3DTextLabel(small_string, -1, x, y, z, 10.0, _, _, _, vw, int);
	small_string[0] = EOS;
	return true;
}

stock Inventory:DestroyDrop(drop_index){
	if(drop_index < 0 || drop_index >= MAX_DROP_ITEMS) return false;

	DestroyDynamicObject(drop_item_info[drop_index][drop_item_object]);
	DestroyDynamic3DTextLabel(drop_item_info[drop_index][drop_item_text]);

	drop_item_info[drop_index][drop_item_id] = 0;
	drop_item_info[drop_index][drop_item_count] = 0;
	drop_item_info[drop_index][drop_item_pos][0] = 0.0;
	drop_item_info[drop_index][drop_item_pos][1] = 0.0;
	drop_item_info[drop_index][drop_item_pos][2] = 0.0;
	drop_item_info[drop_index][drop_item_vw] = 0;
	drop_item_info[drop_index][drop_item_int] = 0;

	drop_item_info[drop_index][drop_item_value_one] = 0;
	drop_item_info[drop_index][drop_item_value_two] = 0;
	drop_item_info[drop_index][drop_item_value_str][0] = EOS;
	return true;
}