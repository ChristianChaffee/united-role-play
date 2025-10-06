
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM accessories_items", "LoadAccessoriesItems");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook LoadPlayerInventoryItems(playerid){
	static const fmt_query[] = "SELECT * FROM player_accessoires p JOIN accounts a ON p.account_id = a.id WHERE a.name = '%s'";
	new query[sizeof(fmt_query) + (-2 + MAX_PLAYER_NAME)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_name));
	mysql_tquery(mysql, query, "LoadPlayerAccessoriesItems", "i", playerid);
}

hook OnPlayerSpawn(playerid){
	if(player_logged[playerid]) Inventory:UpdatePlayersAccessories(playerid);
}

hook OnPlayerClickPTD(playerid, PlayerText:playertextid){
	if(InvParadox_PTD[playerid][12] <= playertextid <= InvParadox_PTD[playerid][17]){
		new slot_index = -1;
		for(new i = 12; i <= 17; i++){
			if(playertextid == InvParadox_PTD[playerid][i]){
				slot_index = i - 12;
				break;
			}
		}

		new item_index = Inventory:FindAccItemWithSlot(playerid, slot_index);
		if(item_index == -1) return Y_HOOKS_CONTINUE_RETURN_0;

		PlayerPlaySound(playerid, 1083, 0.0, 0.0, 0.0);

		new free_inv_slot_index = Inventory:GetFreeSlotIndex(playerid);
		if(free_inv_slot_index == -1){
			SendErrorMessage(playerid, "У Вас нет места в инвентаре.");
			return Y_HOOKS_CONTINUE_RETURN_0;
		}

		Inventory:GiveItem(playerid, free_inv_slot_index, GetInventoryItemData(GetAccessoryItemData(GetPlayerAccessoryData(playerid, item_index, player_acc_item_id), acc_item_id), item_id));
		Inventory:SetAccSlotNull(playerid, item_index);
		Inventory:UpdatePlayersAccessories(playerid);
		Inventory:UpdatePreviewModels(playerid);
		Inventory:Hide(playerid);
		return Y_HOOKS_CONTINUE_RETURN_1;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}