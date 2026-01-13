
DialogResponse:D_INV_AMMO(playerid, response, listitem, inputtext[]){
	if(!response){
		Inventory:Show(playerid);
		return false;
	}

	new count = strval(inputtext),
		slot_index = GetPVarInt(playerid, "Inventory_SlotIndex");

	DeletePVar(playerid, "Inventory_SlotIndex");

	if(count <= 0){
		Inventory:Show(playerid);
		Inventory:ShowError(playerid, " оличество патронов должно быть больше 0.");
		return false;
	}
	if(count > Inventory:GetAmmoForWeapon(playerid, GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_id))){
		Inventory:Show(playerid);
		Inventory:ShowError(playerid, "” ¬ас нет такого количества патронов.");
		return false;
	}

	new weaponid, ammo_id;

	switch(GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_id)){
		case ITEM_DEAGLE_ID: weaponid = WEAPON_DEAGLE, ammo_id = ITEM_DEAGLE_AMMO_ID;
		case ITEM_SHOTGUN_ID: weaponid = WEAPON_SHOTGUN, ammo_id = ITEM_SHOTGUN_AMMO_ID;
		case ITEM_MP5_ID: weaponid = WEAPON_MP5, ammo_id = ITEM_MP5_AMMO_ID;
		case ITEM_M4_ID: weaponid = WEAPON_M4, ammo_id = ITEM_M4_AMMO_ID;
	}

	GivePlayerWeapon(playerid, weaponid, count);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"достал(-а) %s.",
		GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_action_text));
	SendMeAction(playerid, small_string, true);
	small_string[0] = EOS;

	Inventory:SetPlayerSlotNull(playerid, slot_index);

	for(new i, slot_ammo; i < MAX_INVENTORY_SLOTS; i++){
	    if(!count) break;
	    if(!GetPlayerInventoryItemData(playerid, i, inv_player_id)) continue;
	    if(GetPlayerInventoryItemData(playerid, i, inv_player_item_id) != ammo_id) continue;
	    
	    slot_ammo = GetPlayerInventoryItemData(playerid, i, inv_player_item_count);
	    
	    if(slot_ammo > count){
	        // ¬ слоте больше патронов, чем нужно - уменьшаем количество
	        SetPlayerInventoryItemData(playerid, i, inv_player_item_count, slot_ammo - count);
	        Inventory:SaveItem(playerid, i);
	        count = 0;
	    }
	    else{
	        // ¬ слоте меньше или равно нужному - удал€ем слот полностью
	        count -= slot_ammo;
	        Inventory:SetPlayerSlotNull(playerid, i);
	    }
	}
	return true;
}