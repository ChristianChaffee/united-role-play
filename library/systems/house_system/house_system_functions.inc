
stock House:UpdatePickups(house_index){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;

	if(IsValidDynamicPickup(GetHouseData(house_index, h_enter_pickup))){
		DestroyDynamicPickup(GetHouseData(house_index, h_enter_pickup));
	}
	if(IsValidDynamic3DTextLabel(GetHouseData(house_index, h_enter_text))){
		DestroyDynamic3DTextLabel(GetHouseData(house_index, h_enter_text));
	}
	if(IsValidDynamicPickup(GetHouseData(house_index, h_exit_pickup))){
		DestroyDynamicPickup(GetHouseData(house_index, h_exit_pickup));
	}
	if(IsValidDynamic3DTextLabel(GetHouseData(house_index, h_exit_text))){
		DestroyDynamic3DTextLabel(GetHouseData(house_index, h_exit_text));
	}
	if(IsValidDynamicMapIcon(GetHouseData(house_index, h_map_icon))){
		DestroyDynamicMapIcon(GetHouseData(house_index, h_map_icon));
	}

	GetHouseData(house_index, h_enter_pickup) = CreateDynamicPickup((!GetHouseData(house_index, h_owner_id) ? (1273) : (1272)), 
		23, 
		GetHouseData(house_index, h_pos_x), 
		GetHouseData(house_index, h_pos_y), 
		GetHouseData(house_index, h_pos_z), 
		GetHouseData(house_index, h_vw), 
		GetHouseData(house_index, h_int));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_lime"[Дом №%d]\n\
		"cl_grey"'Left ALT'",
		GetHouseData(house_index, h_id));

	GetHouseData(house_index, h_enter_text) = CreateDynamic3DTextLabel(small_string, -1, 
		GetHouseData(house_index, h_pos_x), 
		GetHouseData(house_index, h_pos_y), 
		GetHouseData(house_index, h_pos_z), 
		15.0, _, _, 1, 
		GetHouseData(house_index, h_vw), 
		GetHouseData(house_index, h_int));
	small_string[0] = EOS;

	GetHouseData(house_index, h_exit_pickup) = CreateDynamicPickup(HOUSE_EXIT_PICKUP_MODEL, 23, 
		house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][0], 
		house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][1], 
		house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][2], 
		GetHouseData(house_index, h_id), 
		GetHouseInteriorData(GetHouseData(house_index, h_interior_index), h_int_int));

	GetHouseData(house_index, h_exit_text) = CreateDynamic3DTextLabel(""cl_white"Выход\n"cl_grey"'Left ALT'", -1, 
		house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][0], 
		house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][1], 
		house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][2], 
		15.0, _, _, 1, 
		GetHouseData(house_index, h_id), 
		GetHouseInteriorData(GetHouseData(house_index, h_interior_index), h_int_int));

	GetHouseData(house_index, h_map_icon) = CreateDynamicMapIcon(GetHouseData(house_index, h_pos_x), 
		GetHouseData(house_index, h_pos_y), 
		GetHouseData(house_index, h_pos_z), 
		(!GetHouseData(house_index, h_owner_id) ? (31) : (32)), 
		-1, 
		GetHouseData(house_index, h_vw), 
		GetHouseData(house_index, h_int));
	return true;
}

stock House:EnterHouse(playerid, house_index){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;
	if(IsPlayerInAnyVehicle(playerid)) return false;

	SetPlayerPos(playerid,
		house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][0],
		house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][1],
		house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][2]);

	SetPlayerFacingAngle(playerid, 
		house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][3]);


	SetPlayerInterior(playerid, 
		GetHouseInteriorData(GetHouseData(house_index, h_interior_index), h_int_int));

	SetPlayerVirtualWorld(playerid,
		GetHouseData(house_index, h_id));
	return true;
}

stock House:HideEnterInterface(playerid){
	cef_emit_event(playerid, "hideHouseEnter");
	return true;
}

stock House:ShowMenu(playerid, house_index){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;
	if(GetHouseData(house_index, h_owner_id) != GetPlayerData(playerid, p_id)) return false;

	SelectTextDraw(playerid, -1);

	cef_emit_event(playerid, "showHouseMenu");
	cef_emit_event(playerid, "setHouseMenuData",
		CEFINT(GetHouseData(house_index, h_id)),
		CEFINT(GetHouseData(house_index, h_lock)));

	cef_emit_event(playerid, "setHouseSellStatePrice",
		CEFINT(GetHouseData(house_index, h_price)));


	House:UpdateInventoryList(playerid, house_index);
	House:UpdateUpgradesList(playerid, house_index);

	SetPVarInt(playerid, "House_Index", house_index);
	SetPVarInt(playerid, "House_InMenu", 1);
	return true;
}

stock House:HideMenu(playerid){
	cef_emit_event(playerid, "hideHouseMenu");

	DeletePVar(playerid, "House_Index");
	DeletePVar(playerid, "House_InMenu");

	CancelSelectTextDraw(playerid);
	return true;
}

stock House:ShowMenuError(playerid, text[]){
	cef_emit_event(playerid, "showHouseMenuError",
		CEFSTR(text));
	return true;
}

stock House:UpdateUpgradesList(playerid, house_index){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;

	cef_emit_event(playerid, "clearHouseMenuUpgrades");
	for(new i; i < HOUSE_UPGRADES_COUNT; i++){
		cef_emit_event(playerid, "addHouseMenuUpgrade",
			CEFSTR(house_upgrade_info[i][hu_name]),
			CEFINT(house_upgrade_info[i][hu_price]),
			CEFINT(house_info[house_index][h_upgrades][i]));	
	}

	cef_emit_event(playerid, "setHouseInventoryButtonVisible",
		CEFINT(House:IsHouseHaveUpgrade(house_index, HOUSE_UPGRADE_INVENTORY)));

	cef_emit_event(playerid, "setHouseMenuSafeData",
		CEFINT(House:IsHouseHaveUpgrade(house_index, HOUSE_UPGRADE_SAFE)),
		CEFINT(GetHouseData(house_index, h_safe_cash)));
	return true;
}

stock House:UpdateInventoryList(playerid, house_index){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;

	cef_emit_event(playerid, "clearHouseInventoryItems");
	for(new i, counter; i < MAX_HOUSE_INVENTORY_ITEMS; i++){
		if(!house_info[house_index][h_inventory_items_id][i]) continue;
		if(!house_info[house_index][h_inventory_items_count][i]) continue;

		cef_emit_event(playerid, "addHouseInventoryItem",
			CEFSTR(GetInventoryItemData(house_info[house_index][h_inventory_items_id][i], item_name)),
			CEFINT(house_info[house_index][h_inventory_items_count][i]));

		house_inventory_listitems[playerid][counter] = i;
		counter++;
	}
	return true;
}

stock House:IsHouseHaveUpgrade(house_index, upgrade_index){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;
	if(upgrade_index < 0 || upgrade_index >= HOUSE_UPGRADE_COUNT) return false;

	return house_info[house_index][h_upgrades][upgrade_index];
}

stock House:GetPlayerHouseIndex(playerid){
	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(playerid, p_id)) continue;
		if(GetPlayerVirtualWorld(playerid) != GetHouseData(i, h_id)) continue;
		if(GetPlayerInterior(playerid) != GetHouseInteriorData(GetHouseData(i, h_interior_index), h_int_int)) continue;
		if(!IsPlayerInRangeOfPoint(playerid, 100.0, 
			house_interior_info[GetHouseData(i, h_interior_index)][h_enter_pos][0],
			house_interior_info[GetHouseData(i, h_interior_index)][h_enter_pos][1],
			house_interior_info[GetHouseData(i, h_interior_index)][h_enter_pos][2])) continue;

		return i;
	}
	return -1;
}

stock House:SaveUpgrades(house_index){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;
	if(!GetHouseData(house_index, h_id)) return false;

	new upgrades_string[128];
	for(new i; i < HOUSE_UPGRADE_COUNT; i++){
		format(upgrades_string, sizeof(upgrades_string),
			"%s%d%s",
			upgrades_string,
			house_info[house_index][h_upgrades][i],
			(i == HOUSE_UPGRADE_COUNT - 1) ? ("") : (", "));
	}

	static const fmt_query[] = "UPDATE houses SET upgrades = '%s' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + sizeof(upgrades_string)) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		upgrades_string, GetHouseData(house_index, h_id));
	mysql_tquery(mysql, query);
	return true;
}

stock House:SellHouse(house_index){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;

	SetHouseData(house_index, h_owner_id, 0);
	SetHouseData(house_index, h_lock, 1);
	SetHouseData(house_index, h_payment_days, 1);
	SetHouseData(house_index, h_safe_cash, 0);

	for(new i; i < HOUSE_UPGRADE_COUNT; i++){
		house_info[house_index][h_upgrades][i] = 0;
	}
	House:SaveUpgrades(house_index);

	static const fmt_query_1[] = "UPDATE houses SET owner_id = DEFAULT, door_lock = DEFAULT, payment_days = DEFAULT, safe_cash = DEFAULT WHERE id = %d";
	new query_1[sizeof(fmt_query_1) + (-2 + 11)];
	mysql_format(mysql, query_1, sizeof(query_1), fmt_query_1,
		GetHouseData(house_index, h_id));
	mysql_tquery(mysql, query_1);

	static const fmt_query_2[] = "UPDATE accounts SET tenant_house_id = 0 WHERE tenant_house_id = %d";
	new query_2[sizeof(fmt_query_2) + (-2 + 11)];
	mysql_format(mysql, query_2, sizeof(query_2), fmt_query_2,
		GetHouseData(house_index, h_id));
	mysql_tquery(mysql, query_2);

	for(new i; i < MAX_HOUSE_INVENTORY_ITEMS; i++){
		House:SetInventorySlotNull(house_index, i, false);
	}
	House:SaveInventory(house_index);

	House:UpdatePickups(house_index);
	return true;
}

stock House:GetTenantsCount(house_index){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;

	static const fmt_query[] = "SELECT id FROM accounts WHERE tenant_house_id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetHouseData(house_index, h_id));
	mysql_query(mysql, query);

	new rows;
	cache_get_row_count(rows);
	return rows;
}

stock House:SetInventorySlotNull(house_index, slot_index, save = true){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;
	if(slot_index < 0 || slot_index >= MAX_HOUSE_INVENTORY_ITEMS) return false;

	house_info[house_index][h_inventory_items_id][slot_index] = 0;
	house_info[house_index][h_inventory_items_count][slot_index] = 0;
	house_info[house_index][h_inventory_items_value_one][slot_index] = 0;
	house_info[house_index][h_inventory_items_value_two][slot_index] = 0;

	if(save){
		House:SaveInventory(house_index);
	}
	return true;
}

stock House:SaveInventory(house_index){
	if(house_index < 0 || house_index >= MAX_HOUSES) return false;

	new id_string[256],
		count_string[256],
		value_one_string[256],
		value_two_string[256];

	for(new i; i < MAX_HOUSE_INVENTORY_ITEMS; i++){
		format(id_string, sizeof(id_string), "%s%d%s",
			id_string,
			house_info[house_index][h_inventory_items_id][i],
			(i == MAX_HOUSE_INVENTORY_ITEMS - 1) ? ("") : (", "));

		format(count_string, sizeof(count_string), "%s%d%s",
			count_string,
			house_info[house_index][h_inventory_items_count][i],
			(i == MAX_HOUSE_INVENTORY_ITEMS - 1) ? ("") : (", "));

		format(value_one_string, sizeof(value_one_string), "%s%d%s",
			value_one_string,
			house_info[house_index][h_inventory_items_value_one][i],
			(i == MAX_HOUSE_INVENTORY_ITEMS - 1) ? ("") : (", "));

		format(value_two_string, sizeof(value_two_string), "%s%d%s",
			value_two_string,
			house_info[house_index][h_inventory_items_value_two][i],
			(i == MAX_HOUSE_INVENTORY_ITEMS - 1) ? ("") : (", "));
	}

	static const fmt_query[] = "UPDATE houses SET inventory_items_id = '%s', inventory_items_count = '%s', inventory_items_value_one = '%s', inventory_items_value_two = '%s' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + sizeof(id_string)) + (-2 + sizeof(count_string)) + (-2 + sizeof(value_one_string)) + (-2 + sizeof(value_two_string)) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		id_string, count_string, value_one_string, value_two_string,
		GetHouseData(house_index, h_id));
	mysql_tquery(mysql, query);
	return true;
}

stock House:GetHouseIndexByID(house_id){
	if(house_id <= 0) return -1;
	
	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_id) != house_id) continue;

		return i;
	}
	return -1;
}