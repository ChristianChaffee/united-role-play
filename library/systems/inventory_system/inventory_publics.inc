
forward Inventory_SwapItems(playerid, const arguments[]);
public Inventory_SwapItems(playerid, const arguments[]){
	new from_slot,
		to_slot,
		is_null_second_slot = true;

	sscanf(arguments, "ii", from_slot, to_slot);

	for(new i; i < MAX_INVENTORY_SLOTS; i++){
		if(!GetPlayerInventoryItemData(playerid, i, inv_player_id)) continue;
		if(GetPlayerInventoryItemData(playerid, i, inv_player_item_slot) == from_slot){
			from_slot = i;
		}
		if(GetPlayerInventoryItemData(playerid, i, inv_player_item_slot) == to_slot && GetPlayerInventoryItemData(playerid, i, inv_player_id) != 0){
			to_slot = i;
			is_null_second_slot = false;
		}
	}

	Inventory:ChangePlayerItemDTS(playerid, from_slot, to_slot, is_null_second_slot);
	return true;
}

forward Inventory_UseItem(playerid, const arguments[]);
public Inventory_UseItem(playerid, const arguments[]){
	new slot_index;
	sscanf(arguments, "i", slot_index);

	for(new i; i < MAX_INVENTORY_SLOTS; i++){
		if(!GetPlayerInventoryItemData(playerid, i, inv_player_id)) continue;
		if(GetPlayerInventoryItemData(playerid, i, inv_player_item_slot) != slot_index) continue;

		slot_index = i;
		break;
	}

	Inventory:UseItem(playerid, slot_index);
	Inventory:Hide(playerid);
	return true;
}

forward Inventory_DropItem(playerid, const arguments[]);
public Inventory_DropItem(playerid, const arguments[]){
	new slot_index,
		amount;

	sscanf(arguments, "ii", slot_index, amount);

	for(new i; i < MAX_INVENTORY_SLOTS; i++){
		if(!GetPlayerInventoryItemData(playerid, i, inv_player_id)) continue;
		if(GetPlayerInventoryItemData(playerid, i, inv_player_item_slot) != slot_index) continue;

		slot_index = i;
		break;
	}

	Inventory:DropItem(playerid, slot_index, amount);
	return true;
}

forward Inventory_AccessoryRemove(playerid, const arguments[]);
public Inventory_AccessoryRemove(playerid, const arguments[]){
	new acc_slot_index;
	sscanf(arguments, "i", acc_slot_index);

	for(new i; i < PLAYER_ACCESSORIES_SLOTS_COUNT; i++){
		if(GetPlayerAccessoryData(playerid, i, player_acc_slot_index) == acc_slot_index && GetPlayerAccessoryData(playerid, i, player_acc_id) != 0){
			acc_slot_index = i;
			break;
		}
	}

	new slot_index = Inventory:GetFreeSlotIndex(playerid);
	if(slot_index == -1){
		Inventory:ShowError(playerid, "У Вас нет места в инвентаре.");
		return false;
	}

	Inventory:GiveItem(playerid, slot_index, Inventory:GetItemIdByAccSlot(playerid, acc_slot_index));
	Inventory:SetAccSlotNull(playerid, acc_slot_index);
	
	cef_emit_event(playerid, "setAccessorySlot",
		CEFINT(acc_slot_index),
		CEFSTR(""),
		CEFSTR(""));

	cef_emit_event(playerid, "setInventoryItem",
		CEFINT(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_slot)),
		CEFSTR(GetInventoryItemData(inventory_player[playerid][slot_index][inv_player_item_id], item_name)),
		CEFINT(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count)),
		CEFSTR(GetInventoryItemData(inventory_player[playerid][slot_index][inv_player_item_id], item_img_url)),
		CEFSTR(GetInventoryItemData(inventory_player[playerid][slot_index][inv_player_item_id], item_description)));
	return true;
}

forward Inventory_TransferItem(playerid, const arguments[]);
public Inventory_TransferItem(playerid, const arguments[]){
	new slot_index,
		scdplayerid,
		amount;

	sscanf(arguments, "iii", slot_index, scdplayerid, amount);

	for(new i; i < MAX_INVENTORY_SLOTS; i++){
		if(!GetPlayerInventoryItemData(playerid, i, inv_player_id)) continue;
		if(GetPlayerInventoryItemData(playerid, i, inv_player_item_slot) != slot_index) continue;

		slot_index = i;
		break;
	}

	if(!GetPlayerInventoryItemData(playerid, slot_index, inv_player_can_drop)){
		Inventory:ShowError(playerid, "Данный предмет нельзя передать.");
		return false;
	}
	if(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) < amount){
		Inventory:ShowError(playerid, "У Вас нет такого количества данного предмета.");
		return false;
	}
	if(scdplayerid == playerid){
		Inventory:ShowError(playerid, "Вы не можете передать предмет самому себе.");
		return false;
	}
	if(!IsPlayerConnected(scdplayerid)){
		Inventory:ShowError(playerid, "Данный игрок не в сети.");
		return false;
	}
	if(!IsPlayerInRangeOfPlayer(playerid, scdplayerid, 10.0)){
		Inventory:ShowError(playerid, "Данный игрок далеко от Вас.");
		return false;
	}

	Inventory:Hide(playerid);

	cef_emit_event(scdplayerid, "showInventoryTransferOffer",
		CEFSTR(GetPlayerData(playerid, p_name)),
		CEFSTR(GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_name)),
		CEFSTR(GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_img_url)),
		CEFINT(amount), 
		CEFINT(slot_index),
		CEFINT(playerid));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы отправили предложение на передачу предмета игроку %s[%d].",
		GetPlayerData(scdplayerid, p_name), scdplayerid);
	SendClientMessage(playerid, COLOR_GREEN, small_string);
	small_string[0] = EOS;
	return true;
}

forward Inventory_TransferOfferAccept(playerid, const arguments[]);
public Inventory_TransferOfferAccept(playerid, const arguments[]){
	new scdplayerid,
		slot_index,
		amount;

	sscanf(arguments, "iii", scdplayerid, slot_index, amount);

	if(scdplayerid == playerid){
		SendErrorMessage(playerid, "Вы не можете передать предмет самому себе.");
		return false;
	}
	if(!IsPlayerConnected(scdplayerid)){
		SendErrorMessage(playerid, "Данный игрок не в сети.");
		return false;
	}
	if(!IsPlayerInRangeOfPlayer(playerid, scdplayerid, 10.0)){
		SendErrorMessage(playerid, "Игрок далеко от Вас.");
		SendErrorMessage(scdplayerid, "Для передачи предмета требуется находиться ближе к игроку.");
		return false;
	}
	if(!GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_id)){
		SendErrorMessage(playerid, "У игрока уже нет данного предмета.");
		SendErrorMessage(scdplayerid, "Вы не смогли передать данный предмет, так как он отсутствует у Вас.");
		return false;
	}
	if(!GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_can_drop)){
		SendErrorMessage(playerid, "Игрок не может передать данный предмет.");
		SendErrorMessage(scdplayerid, "Вы не можете передать данный предмет, так как его запрещено передавать.");
		return false;
	}
	if(GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_item_count) < amount){
		SendErrorMessage(playerid, "У игрока нет такого количества данного предмета.");
		SendErrorMessage(scdplayerid, "У Вас нет такого количества данного предмета.");
		return false;
	}

	new free_slot_index = Inventory:GetFreeSlotIndex(playerid);
	if(free_slot_index == -1){
		SendErrorMessage(playerid, "У Вас нет места в инвентаре.");
		SendErrorMessage(scdplayerid, "У игрока нет места в инвентаре.");
		return false;
	}

	//---------------------------------
	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы передали игроку "cl_white"%s[%d] "cl_green"предмет "cl_white"%s "cl_grey"(%d шт)"cl_green".",
		GetPlayerData(playerid, p_name), playerid,
		GetInventoryItemData(GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_item_id), item_name),
		amount);
	SendClientMessage(scdplayerid, COLOR_GREEN, small_string);

	format(small_string, sizeof(small_string),
		"Игрок "cl_white"%s[%d] "cl_green"передал(-а) Вам предмет "cl_white"%s "cl_grey"(%d шт)"cl_green".",
		GetPlayerData(scdplayerid, p_name), scdplayerid,
		GetInventoryItemData(GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_item_id), item_name),
		amount);
	SendClientMessage(playerid, COLOR_GREEN, small_string);
	small_string[0] = EOS;
	//---------------------------------

	Inventory:GiveItem(playerid, free_slot_index, GetInventoryItemData(GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_item_id), item_id), amount, GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_value_one), GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_value_two), GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_value_str));

	SetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_item_count, GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_item_count) - amount);
	if(GetPlayerInventoryItemData(scdplayerid, slot_index, inv_player_item_count) <= 0){
		Inventory:SetPlayerSlotNull(scdplayerid, slot_index);
	}
	else{
		Inventory:SaveItem(playerid, slot_index);
	}
	return true;
}

forward Inventory_TransferOfferDecline(playerid, const arguments[]);
public Inventory_TransferOfferDecline(playerid, const arguments[]){
	new scdplayerid,
		slot_index,
		amount;

	sscanf(arguments, "iii", scdplayerid, slot_index, amount);

	if(IsPlayerConnected(scdplayerid)){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Игрок %s[%d] отказался от Вашего предложения по передаче предмета.",
			GetPlayerData(playerid, p_name), playerid);
		SendErrorMessage(scdplayerid, small_string);
		small_string[0] = EOS;
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы отказались от предложения %s[%d] о передаче предмета.",
		GetPlayerData(scdplayerid, p_name), scdplayerid);
	SendErrorMessage(playerid, small_string);
	small_string[0] = EOS;
	return true;
}