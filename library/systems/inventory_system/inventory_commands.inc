
CMD:inv(playerid){
	if(!player_logged[playerid]) return false;

	Inventory:Show(playerid);
	return true;
}

CMD:takedrop(playerid){
	if(!player_logged[playerid]) return false;

	new drop_index = -1;
	for(new i; i < MAX_DROP_ITEMS; i++){
		if(!drop_item_info[i][drop_item_id]) continue;
		if(!IsPlayerInRangeOfPoint(playerid, 2.0, drop_item_info[i][drop_item_pos][0], drop_item_info[i][drop_item_pos][1], drop_item_info[i][drop_item_pos][2])
			|| GetPlayerVirtualWorld(playerid) != drop_item_info[i][drop_item_vw]
			|| GetPlayerInterior(playerid) != drop_item_info[i][drop_item_int]){

			continue;
		}

		drop_index = i;
		break;
	}

	if(drop_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться около выброшенного предмета.");
		return false;
	}

	new slot_index = Inventory:GetFreeSlotIndex(playerid);
	if(slot_index == -1){
		SendErrorMessage(playerid, "У Вас нет места в инвентаре.");
		return false;
	}

	Inventory:GiveItem(playerid, slot_index, drop_item_info[drop_index][drop_item_id], drop_item_info[drop_index][drop_item_count], drop_item_info[drop_index][drop_item_value_one], drop_item_info[drop_index][drop_item_value_two], drop_item_info[drop_index][drop_item_value_str], drop_item_info[drop_index][drop_item_can_drop]);
	Inventory:DestroyDrop(drop_index);
	return true;
}