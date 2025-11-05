
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM inventory_items", "LoadInventoryItems");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook stock OnPlayerAccountLoaded(playerid){
	static const fmt_query[] = "SELECT * FROM player_inventory_items p JOIN accounts a ON p.account_id = a.id WHERE a.name = '%s'";
	new query[sizeof(fmt_query) - 2 + 24];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_name));
	mysql_tquery(mysql, query, "LoadPlayerInventoryItems", "i", playerid);
	return continue(playerid);
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){
	switch(dialogid){
		case D_INVENTORY_ACTION:{
			if(!response){
				DeletePVar(playerid, "Inventory_DialogSlot");
				SelectTextDraw(playerid, COLOR_GREY);
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			switch(listitem){
				case 0: return Inventory:ShowInfoDialog(playerid);
				case 1: return Inventory:UseItem(playerid);
				case 2: return Inventory:ShowHandOverEnterID(playerid);
				case 3: return Inventory:DropItem(playerid);
			}
			return Y_HOOKS_CONTINUE_RETURN_0;
		}

		case D_INVENTORY_INFO:{
			Inventory:ShowActionDialog(playerid, GetPVarInt(playerid, "Inventory_DialogSlot"));
			return Y_HOOKS_CONTINUE_RETURN_1;
		}

		case D_INVENTORY_DROP_COUNT:{
			if(!GetPVarType(playerid, "Inventory_DialogSlot")) return Y_HOOKS_CONTINUE_RETURN_0;
			if(!response) return Inventory:ShowInfoDialog(playerid);
			
			new 
				slot_index = GetPVarInt(playerid, "Inventory_DialogSlot"),
				count = strval(inputtext);

			if(count > GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count)){
				SendErrorMessage(playerid, "У Вас нет такого количества данного предмета.");
				Inventory:ShowInfoDialog(playerid);
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			new Float:pos[3];
			GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

			Inventory:CreateDropItem(pos[0], pos[1], pos[2] - 1.0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), count, GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_one), GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_two), GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_str));
			if(count == GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count)){
				Inventory:SetPlayerSlotNull(playerid, slot_index);
			}
			else{
				SetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count, GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) - count);

				static const fmt_query[] = "UPDATE player_inventory_items SET item_count = %d WHERE id = %d LIMIT 1";
				new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
				mysql_format(mysql, query, sizeof(query), fmt_query, 
					GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count),
					GetPlayerInventoryItemData(playerid, slot_index, inv_player_id));
				mysql_tquery(mysql, query);
			}

			DeletePVar(playerid, "Inventory_DialogSlot");
			Inventory:Hide(playerid);
			return Y_HOOKS_CONTINUE_RETURN_1;
		}

		case D_INVENTORY_HAND_OVER_ID:{
			if(!GetPVarType(playerid, "Inventory_DialogSlot")) return Y_HOOKS_CONTINUE_RETURN_0;
			if(!response) return Inventory:ShowInfoDialog(playerid);

			new targetplayerid = strval(inputtext);
			if(targetplayerid == playerid){
				Inventory:ShowHandOverEnterID(playerid);
				SendErrorMessage(playerid, "Вы не можете передать предмет самому себе.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
			if(!IsPlayerInRangeOfPlayer(playerid, targetplayerid, 5.0)){
				Inventory:ShowHandOverEnterID(playerid);
				SendErrorMessage(playerid, "Данный игрок далеко от Вас.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
			SetPVarInt(playerid, "Inventory_HandOverID", targetplayerid);
			Inventory:ShowHandOverEnterCount(playerid);
			return Y_HOOKS_CONTINUE_RETURN_1;
		}
		case D_INVENTORY_HAND_OVER_COUNT:{
			if(!GetPVarType(playerid, "Inventory_DialogSlot") || !GetPVarType(playerid, "Inventory_HandOverID")) return Y_HOOKS_CONTINUE_RETURN_0;
			if(!response){
				DeletePVar(playerid, "Inventory_HandOverID");
				Inventory:ShowHandOverEnterID(playerid);
				return Y_HOOKS_CONTINUE_RETURN_0;
			}

			new 
				targetplayerid = GetPVarInt(playerid, "Inventory_HandOverID"),
				count = strval(inputtext),
				slot_index = GetPVarInt(playerid, "Inventory_DialogSlot"),
				target_slot_index = Inventory:GetFreeSlotIndex(targetplayerid);

			if(targetplayerid == playerid){
				Inventory:ShowHandOverEnterID(playerid);
				SendErrorMessage(playerid, "Вы не можете передать предмет самому себе.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
			if(!IsPlayerInRangeOfPlayer(playerid, targetplayerid, 5.0)){
				Inventory:ShowHandOverEnterID(playerid);
				SendErrorMessage(playerid, "Данный игрок далеко от Вас.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
			if(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) < count){
				Inventory:ShowHandOverEnterID(playerid);
				SendErrorMessage(playerid, "У Вас нет такого количества данного предмета в выбранном слоте.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
			if(!GetPlayerInventoryItemData(playerid, slot_index, inv_player_can_drop)){
				Inventory:ShowHandOverEnterID(playerid);
				SendErrorMessage(playerid, "Вы не можете передавать данный предмет.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
			if(count <= 0){
				Inventory:ShowHandOverEnterID(playerid);
				SendErrorMessage(playerid, "Корректно введите количество передаваемого предмета.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
			if(targetplayerid == -1){
				Inventory:ShowHandOverEnterID(playerid);
				SendErrorMessage(playerid, "У игрока недостаточно места в инвентаре.");
				return Y_HOOKS_CONTINUE_RETURN_0;
			}
		
			Inventory:GiveItem(targetplayerid, target_slot_index, GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), count, GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_one), GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_two), GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_str));
			SetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count, GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) - count);
			if(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) <= 0){
				Inventory:SetPlayerSlotNull(playerid, slot_index);
			}

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"Вы передали %s[%d] предмет \"%s\" (%d шт)",
				GetPlayerData(targetplayerid, p_name), targetplayerid,
				GetInventoryItemData(GetPlayerInventoryItemData(targetplayerid, target_slot_index, inv_player_item_id), item_name),
				count);
			SendClientMessage(playerid, COLOR_BLUE, small_string);

			format(small_string, sizeof(small_string),
				"Игрок %s[%d] передал(-а) Вам предмет \"%s\" (%d шт)",
				GetPlayerData(playerid, p_name), playerid,
				GetInventoryItemData(GetPlayerInventoryItemData(targetplayerid, target_slot_index, inv_player_item_id), item_name),
				count);
			SendClientMessage(targetplayerid, COLOR_BLUE, small_string);
			small_string[0] = EOS;

			Inventory:Hide(playerid);
			Inventory:Hide(targetplayerid);
			return Y_HOOKS_CONTINUE_RETURN_1;
		}
	}

	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
stock Inventory:SetPlayerSlotNull(playerid, slot_index){
	if(slot_index < 0 || slot_index >= MAX_INVENTORY_SLOTS) return false;

	static const fmt_query[] = "DELETE from player_inventory_items WHERE id = %d LIMIT 1";
	new query[sizeof(fmt_query) - 2 + 11];
	mysql_format(mysql, query, sizeof(query), fmt_query, inventory_player[playerid][slot_index][inv_player_id]);
	mysql_tquery(mysql, query);

	SetPlayerInventoryItemData(playerid, slot_index, inv_player_id, 0);
	SetPlayerInventoryItemData(playerid, slot_index, inv_player_account_id, 0);
	SetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id, 0);
	SetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count, 0);
	SetPlayerInventoryItemData(playerid, slot_index, inv_player_item_slot, 0);
	SetPlayerInventoryItemData(playerid, slot_index, inv_player_value_one, 0);
	SetPlayerInventoryItemData(playerid, slot_index, inv_player_value_two, 0);
	format(inventory_player[playerid][slot_index][inv_player_value_str], 256, "-");
	return true;
}

stock Inventory:SaveItem(playerid, slot_index){
	if(slot_index < 0 || slot_index >= MAX_INVENTORY_SLOTS) return false;

	static const fmt_query[] = "UPDATE player_inventory_items SET item_id = %d, item_count = %d, item_slot = %d, value_one = %d, value_two = %d, value_str = '%s', can_drop = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11) + (-2 + 11) + (-2 + 11) + (-2 + 11) + (-2 + 256) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id),
		GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count),
		GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_slot),
		GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_one),
		GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_two),
		GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_str),
		GetPlayerInventoryItemData(playerid, slot_index, inv_player_can_drop),
		GetPlayerInventoryItemData(playerid, slot_index, inv_player_id));
	mysql_tquery(mysql, query);
	return true;
}

stock Inventory:ChangePlayerItemDTS(playerid, from_index, to_index){
	if((from_index < 0 || from_index >= MAX_INVENTORY_SLOTS)
		|| (to_index < 0 || to_index >= MAX_INVENTORY_SLOTS)) return false;

	new save_index = GetPlayerInventoryItemData(playerid, from_index, inv_player_item_slot);

	SetPlayerInventoryItemData(playerid, from_index, inv_player_item_slot, GetPlayerInventoryItemData(playerid, to_index, inv_player_item_slot));
	SetPlayerInventoryItemData(playerid, to_index, inv_player_item_slot, save_index);

	static const fmt_query[] = "UPDATE player_inventory_items SET item_slot = %d WHERE id = %d LIMIT 1";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	
	mysql_format(mysql, query, sizeof(query), fmt_query, 
		GetPlayerInventoryItemData(playerid, from_index, inv_player_item_slot),
		GetPlayerInventoryItemData(playerid, from_index, inv_player_id));
	mysql_tquery(mysql, query);

	mysql_format(mysql, query, sizeof(query), fmt_query, 
		GetPlayerInventoryItemData(playerid, to_index, inv_player_item_slot),
		GetPlayerInventoryItemData(playerid, to_index, inv_player_id));
	mysql_tquery(mysql, query);
	return true;
}

stock Inventory:FindItemWithSlot(playerid, slot_index){
	for(new i; i < MAX_INVENTORY_SLOTS; i++){
		if(GetPlayerInventoryItemData(playerid, i, inv_player_item_slot) == slot_index && GetPlayerInventoryItemData(playerid, i, inv_player_id) > 0) return i;
	}
	return -1;
}

stock Inventory:GetFreeSlotIndex(playerid){
	for(new i; i < MAX_INVENTORY_SLOTS; i++){
		if(Inventory:FindItemWithSlot(playerid, i) == -1) return i;
	}
	return -1;
}

stock Inventory:GiveItem(playerid, slot_index, id, count = 1, value_one = 0, value_two = 0, value_str[] = "-", can_drop = 1){
	if(Inventory:FindItemWithSlot(playerid, slot_index) != -1) return false;
	new index = -1;
	for(new i; i < MAX_INVENTORY_SLOTS; i++){
		if(!GetPlayerInventoryItemData(playerid, i, inv_player_id)){
			index = i;
			break;
		}
	}

	if(index == -1) return false;

	SetPlayerInventoryItemData(playerid, index, inv_player_account_id, GetPlayerData(playerid, p_id));
	SetPlayerInventoryItemData(playerid, index, inv_player_item_id, id);
	SetPlayerInventoryItemData(playerid, index, inv_player_item_count, count);
	SetPlayerInventoryItemData(playerid, index, inv_player_value_one, value_one);
	SetPlayerInventoryItemData(playerid, index, inv_player_value_two, value_two);
	SetPlayerInventoryItemData(playerid, index, inv_player_can_drop, can_drop);
	SetPlayerInventoryItemData(playerid, index, inv_player_item_slot, slot_index);
	format(inventory_player[playerid][index][inv_player_value_str], 256, "%s", value_str);

	static const fmt_query[] = "INSERT INTO player_inventory_items \
		(`account_id`, `item_id`, `item_count`, `item_slot`, `value_one`, `value_two`, `value_str`, `can_drop`) VALUES \
		('%d', '%d', '%d', '%d', '%d', '%d', '%s', '%d')";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11) + (-2 + 11) + (-2 + 11) + (-2 + 11) + (-2 + 11) + (-2 + 256) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_id), id, count, slot_index, value_one, value_two, value_str, can_drop);
	mysql_tquery(mysql, query, "InventoryGetCacheInsertId", "ii", playerid, index);
	return true;
}

stock Inventory:UseItem(playerid){
	if(!GetPVarType(playerid, "Inventory_DialogSlot")) return false;
	new slot_index = GetPVarInt(playerid, "Inventory_DialogSlot");

	if(Inventory:IsAccessoryItem(GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_id))){
		for(new i; i < PLAYER_ACCESSORIES_SLOTS_COUNT; i++){
			if(GetInventoryItemData(GetAccessoryItemData(GetPlayerAccessoryData(playerid, i, player_acc_item_id), acc_item_id), item_id) == GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_id)){
				SendErrorMessage(playerid, "У Вас уже есть данный аксессуар.");
				return false;
			}
		}
		new free_acc_slot_index = Inventory:GetFreeAccSlotIndex(playerid);
		if(free_acc_slot_index == -1){
			SendErrorMessage(playerid, "У Вас заняты все слоты для аксессуаров.");
			return false;
		}

		Inventory:GivePlayerAccessory(playerid, free_acc_slot_index, GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_id));
		Inventory:UpdatePlayersAccessories(playerid);
		Inventory:SetPlayerSlotNull(playerid, slot_index);
	}
	else{
		switch(GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_id)){
			case ITEM_LETTER_ID:{
				big_string[0] = EOS;
				format(big_string, sizeof(big_string),
					""cl_blue"Текст письма: "cl_white"%s",
					GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_str));
				SPD(playerid, D_NULL, DIALOG_STYLE_MSGBOX,
					"Письмо", big_string,
					"Закрыть", "");
				big_string[0] = EOS;

				if(GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_two) == SQ_WELCOME_TO_STATE){
					StartQuest:SetStage(playerid, SQ_WELCOME_TO_STATE, SQ_STAGE_FIND_FRIEND);
				}

				Inventory:SetPlayerSlotNull(playerid, slot_index);
			}
			case ITEM_MONEY_ID:{
				GiveMoney(playerid, GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_one));

				small_string[0] = EOS;
				format(small_string, sizeof(small_string),
					"Вы получили: "cl_white"%d$",
					GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_one));
				SendClientMessage(playerid, COLOR_GREEN, small_string);
				small_string[0] = EOS;

				Inventory:SetPlayerSlotNull(playerid, slot_index);
			}
			case ITEM_TOMATO_ID, ITEM_ORANGE_ID, ITEM_RED_APPLE_ID, ITEM_GREEN_APPLE_ID, ITEM_CHEESBURGER_ID,
				ITEM_HAMBURGER_ID, ITEM_KEBAB_ID, ITEM_BANAN_ID, ITEM_PIZZA_ID:{

				new 
					give_satiety = GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_static_value_one),
					give_thirst = GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_static_value_two);
			
				ApplyAnimation(playerid, "VENDING", "VEND_EAT1_P", 4.0, 0, 1, 1, 0, 0);

				small_string[0] = EOS;
				format(small_string, sizeof(small_string),
					"съел(-а) %s.",
					GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_action_text));
				SendMeAction(playerid, small_string, true);
				small_string[0] = EOS;

				AddPlayerNeedValue(playerid, PLAYER_NEED_HUNGRY, give_satiety);
				AddPlayerNeedValue(playerid, PLAYER_NEED_THIRST, give_thirst);

				for(new i; i < PLAYER_NEEDS_COUNT; i++){
					if(GetPlayerNeedValue(playerid, i) > PLAYER_NEED_MAX_VALUE) SetPlayerNeedValue(playerid, i, PLAYER_NEED_MAX_VALUE);
					if(GetPlayerNeedValue(playerid, i) < 0) SetPlayerNeedValue(playerid, i, 0);
				}
				PlayerNeeds:SaveNeeds(playerid);

				SetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count, GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) - 1);
				if(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) <= 0){
					Inventory:SetPlayerSlotNull(playerid, slot_index);
				}
				else{
					Inventory:SaveItem(playerid, slot_index);
				}
			}
			case ITEM_COFFEE_ID, ITEM_MILK_ID, ITEM_ORANGE_JUICE_ID, ITEM_APPLE_JUICE_ID:{
				new 
					give_satiety = GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_static_value_one),
					give_thirst = GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_static_value_two);
			
				ApplyAnimation(playerid, "VENDING", "VEND_DRINK_P", 4.0, 0, 1, 1, 0, 0);

				small_string[0] = EOS;
				format(small_string, sizeof(small_string),
					"выпил(-а) %s.",
					GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_action_text));
				SendMeAction(playerid, small_string, true);
				small_string[0] = EOS;

				AddPlayerNeedValue(playerid, PLAYER_NEED_HUNGRY, give_satiety);
				AddPlayerNeedValue(playerid, PLAYER_NEED_THIRST, give_thirst);

				for(new i; i < PLAYER_NEEDS_COUNT; i++){
					if(GetPlayerNeedValue(playerid, i) > PLAYER_NEED_MAX_VALUE) SetPlayerNeedValue(playerid, i, PLAYER_NEED_MAX_VALUE);
					if(GetPlayerNeedValue(playerid, i) < 0) SetPlayerNeedValue(playerid, i, 0);
				}
				PlayerNeeds:SaveNeeds(playerid);

				SetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count, GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) - 1);
				if(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) <= 0){
					Inventory:SetPlayerSlotNull(playerid, slot_index);
				}
				else{
					Inventory:SaveItem(playerid, slot_index);
				}
			}
			case ITEM_BEER_ID, ITEM_WHISKEY_ID, ITEM_COGNAC_ID, ITEM_VODKA_ID:{
				new 
					give_drunkenness = GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_static_value_one),
					give_satiety = GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_static_value_two);
			
				ApplyAnimation(playerid, "VENDING", "VEND_DRINK_P", 4.0, 0, 1, 1, 0, 0);

				small_string[0] = EOS;
				format(small_string, sizeof(small_string),
					"выпил(-а) %s.",
					GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_action_text));
				SendMeAction(playerid, small_string, true);
				small_string[0] = EOS;

				SetPlayerDrunkLevel(playerid, GetPlayerData(playerid, p_drunked_level) + give_drunkenness);
				AddPlayerNeedValue(playerid, PLAYER_NEED_HUNGRY, give_satiety);
				
				for(new i; i < PLAYER_NEEDS_COUNT; i++){
					if(GetPlayerNeedValue(playerid, i) > PLAYER_NEED_MAX_VALUE) SetPlayerNeedValue(playerid, i, PLAYER_NEED_MAX_VALUE);
					if(GetPlayerNeedValue(playerid, i) < 0) SetPlayerNeedValue(playerid, i, 0);
				}
				PlayerNeeds:SaveNeeds(playerid);

				SetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count, GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) - 1);
				if(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) <= 0){
					Inventory:SetPlayerSlotNull(playerid, slot_index);
				}
				else{
					Inventory:SaveItem(playerid, slot_index);
				}
			}
		}
	}

	DeletePVar(playerid, "Inventory_DialogSlot");
	Inventory:Hide(playerid);
	return true;
}

//==============================================================================
stock Inventory:ShowActionDialog(playerid, clicked_index){
	//CancelSelectTextDraw(playerid);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_blue"%s "cl_grey"[ID: "cl_white"%d"cl_grey"]",
		GetInventoryItemData(GetPlayerInventoryItemData(playerid, clicked_index, inv_player_item_id), item_name),
		GetInventoryItemData(GetPlayerInventoryItemData(playerid, clicked_index, inv_player_item_id), item_id));

	SPD(playerid, D_INVENTORY_ACTION, DIALOG_STYLE_LIST,
		small_string,
		""cl_blue"1. "cl_white"Информация о предмете\n\
		"cl_blue"2. "cl_white"Использовать предмет\n\
		"cl_blue"3. "cl_white"Передать предмет\n\
		"cl_blue"4. "cl_white"Выкинуть предмет",
		"Выбрать", "Закрыть");
	small_string[0] = EOS;

	SetPVarInt(playerid, "Inventory_DialogSlot", clicked_index);
	return true;
}

stock Inventory:ShowInfoDialog(playerid){
	if(!GetPVarType(playerid, "Inventory_DialogSlot")) return false;
	new slot_index = GetPVarInt(playerid, "Inventory_DialogSlot");

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"Название: "cl_white"%s\n\
		"cl_blue"Количество: "cl_white"%d шт\n\n\
		"cl_grey"["cl_white"%s"cl_grey"]",
		GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_name),
		GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count),
		GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_description));

	switch(GetInventoryItemData(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), item_id)){
		case ITEM_MONEY_ID:{
			format(big_string, sizeof(big_string),
				"%s\n\n\
				"cl_grey"В данной пачке денег: "cl_white"%d$",
				big_string, 
				GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_one));
		}
	}

	SPD(playerid, D_INVENTORY_INFO, DIALOG_STYLE_MSGBOX,
		"Информация о предмете", big_string,
		"Назад", "");
	big_string[0] = EOS;
	return true;
}

stock Inventory:DropItem(playerid){
	if(!GetPVarType(playerid, "Inventory_DialogSlot")) return false;
	new slot_index = GetPVarInt(playerid, "Inventory_DialogSlot");

	if(!GetPlayerInventoryItemData(playerid, slot_index, inv_player_can_drop)){
		SendErrorMessage(playerid, "Вы не можете выбросить данный предмет.");
		Inventory:ShowActionDialog(playerid, slot_index);
		return false;
	}

	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	if(GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) == 1){
		Inventory:CreateDropItem(pos[0], pos[1], pos[2] - 1.0, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid), GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id), GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count), GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_one), GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_two), GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_str));
		Inventory:SetPlayerSlotNull(playerid, slot_index);
		DeletePVar(playerid, "Inventory_DialogSlot");
		Inventory:Hide(playerid);
	}
	else{
		big_string[0] = EOS;
		format(big_string, sizeof(big_string),
			""cl_blue"- "cl_white"Введите количество, которое хотите выбросить\n\
			"cl_grey"Доступное количество: "cl_white"%d",
			GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count));

		SPD(playerid, D_INVENTORY_DROP_COUNT, DIALOG_STYLE_INPUT,
			"Выбросить предмет", big_string,
			"Выбросить", "Назад");
		big_string[0] = EOS;
	}

	return true;
}

stock Inventory:ShowHandOverEnterID(playerid){
	if(!GetPVarType(playerid, "Inventory_DialogSlot")) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите ID игрока, которому хотите передать "cl_grey"\"%s\"",
		GetInventoryItemData(GetPlayerInventoryItemData(playerid, GetPVarInt(playerid, "Inventory_DialogSlot"), inv_player_item_id), item_name));

	SPD(playerid, D_INVENTORY_HAND_OVER_ID, DIALOG_STYLE_INPUT,
		"Передать предмет", big_string,
		"Ввод", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Inventory:ShowHandOverEnterCount(playerid){
	if(!GetPVarType(playerid, "Inventory_DialogSlot") || !GetPVarType(playerid, "Inventory_HandOverID")) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите количество предмета "cl_grey"\"%s\""cl_white", которое хотите передать %s[%d]",
		GetInventoryItemData(GetPlayerInventoryItemData(playerid, GetPVarInt(playerid, "Inventory_DialogSlot"), inv_player_item_id), item_name),
		GetPlayerData(GetPVarInt(playerid, "Inventory_HandOverID"), p_name),
		GetPVarInt(playerid, "Inventory_HandOverID"));

	SPD(playerid, D_INVENTORY_HAND_OVER_COUNT, DIALOG_STYLE_INPUT,
		"Передать предмет", big_string,
		"Ввод", "Назад");
	big_string[0] = EOS;
	return true;
}