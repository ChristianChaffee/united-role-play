
stock Inventory:Show(playerid){
	for(new i; i < sizeof(InvParadox_TD); i++){
		TextDrawShowForPlayer(playerid, InvParadox_TD[i]);
	}
	for(new i; i < 21; i++){
		PlayerTextDrawShow(playerid, InvParadox_PTD[playerid][i]);
	}
	for(new i; i < 25; i++){
		PlayerTextDrawShow(playerid, SlotParadox_PTD[playerid][i]);
	}
	SelectTextDraw(playerid, COLOR_GREY);
	DeletePVar(playerid, "Inventory_SelectedSlot");
	SetPVarInt(playerid, "Inventory_Page", 1);
	Inventory:UpdatePages(playerid);
	return true;
}

stock Inventory:Hide(playerid){
	for(new i; i < sizeof(InvParadox_TD); i++){
		TextDrawHideForPlayer(playerid, InvParadox_TD[i]);
	}
	for(new i; i < 21; i++){
		PlayerTextDrawHide(playerid, InvParadox_PTD[playerid][i]);
	}
	for(new i; i < 25; i++){
		PlayerTextDrawHide(playerid, SlotParadox_PTD[playerid][i]);
	}

	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][GetPVarInt(playerid, "Inventory_SelectedSlot")], -1);
	
	CancelSelectTextDraw(playerid);
	DeletePVar(playerid, "Inventory_SelectedSlot");
	DeletePVar(playerid, "Inventory_Page");
	return true;
}

stock Inventory:UpdatePreviewModels(playerid){
	PlayerTextDrawHide(playerid, InvParadox_PTD[playerid][0]);
	PlayerTextDrawSetPreviewModel(playerid, InvParadox_PTD[playerid][0], GetPlayerSkin(playerid));
	PlayerTextDrawShow(playerid, InvParadox_PTD[playerid][0]);

	for(new i, model_id, item_index; i < 25; i++){
		PlayerTextDrawHide(playerid, SlotParadox_PTD[playerid][i]);

		item_index = Inventory:FindItemWithSlot(playerid, (GetPVarInt(playerid, "Inventory_Page") - 1) * 25 + i);

		model_id = (item_index == -1) ? (1649) : (GetInventoryItemData(inventory_player[playerid][item_index][inv_player_item_id], item_model_id));

		PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][i], model_id);
		PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][i], 
			(item_index == -1) ? (0.0) : (GetInventoryItemData(inventory_player[playerid][item_index][inv_player_item_id], item_rot_x)), 
			(item_index == -1) ? (0.0) : (GetInventoryItemData(inventory_player[playerid][item_index][inv_player_item_id], item_rot_y)), 
			(item_index == -1) ? (0.0) : (GetInventoryItemData(inventory_player[playerid][item_index][inv_player_item_id], item_rot_z)), 
			(item_index == -1) ? (0.0) : (GetInventoryItemData(inventory_player[playerid][item_index][inv_player_item_id], item_scale)));

		PlayerTextDrawShow(playerid, SlotParadox_PTD[playerid][i]);
	}

	for(new i = 12, temp_acc_slot_index, model_id, item_index; i <= 17; i++){
		temp_acc_slot_index = i - 12;
		item_index = Inventory:FindAccItemWithSlot(playerid, temp_acc_slot_index);

		model_id = (item_index == -1) ? (1649) : (GetInventoryItemData(GetAccessoryItemData(player_accessories_items_info[playerid][item_index][player_acc_item_id], acc_item_id), item_model_id));
	
		PlayerTextDrawSetPreviewModel(playerid, InvParadox_PTD[playerid][i], model_id);
		PlayerTextDrawSetPreviewRot(playerid, InvParadox_PTD[playerid][i], 
			(item_index == -1) ? (0.0) : (GetInventoryItemData(GetAccessoryItemData(player_accessories_items_info[playerid][item_index][player_acc_item_id], acc_item_id), item_rot_x)), 
			(item_index == -1) ? (0.0) : (GetInventoryItemData(GetAccessoryItemData(player_accessories_items_info[playerid][item_index][player_acc_item_id], acc_item_id), item_rot_y)), 
			(item_index == -1) ? (0.0) : (GetInventoryItemData(GetAccessoryItemData(player_accessories_items_info[playerid][item_index][player_acc_item_id], acc_item_id), item_rot_z)), 
			(item_index == -1) ? (0.0) : (GetInventoryItemData(GetAccessoryItemData(player_accessories_items_info[playerid][item_index][player_acc_item_id], acc_item_id), item_scale)));

		PlayerTextDrawShow(playerid, InvParadox_PTD[playerid][i]);
	}
}

stock Inventory:UpdatePages(playerid){
	if(!GetPVarType(playerid, "Inventory_Page")) return false;

	for(new i = 18; i <= 20; i++){
		PlayerTextDrawHide(playerid, InvParadox_PTD[playerid][i]);
		PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][i], (GetPVarInt(playerid, "Inventory_Page") == (i - 18 + 1)) ? (-1503414785) : (2134219775));
		PlayerTextDrawShow(playerid, InvParadox_PTD[playerid][i]);
	}

	Inventory:UpdatePreviewModels(playerid);
	return true;
}