
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnPlayerSpawn(playerid){
	if(player_logged[playerid]) Job:DeallerRemoveItems(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	if(GetPlayerData(playerid, p_job) == JOB_DEALLER) Job:DeallerUninvite(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterCheckpoint(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_DEALLER
		&& GetPVarType(playerid, "JobDealler_PosIndex")
		&& !GetPVarType(playerid, "JobDealler_Timer")
		&& IsPlayerInRangeOfPoint(playerid, 1.0, 
			dealler_drug_pos[GetPVarInt(playerid, "JobDealler_PosIndex")][0],
			dealler_drug_pos[GetPVarInt(playerid, "JobDealler_PosIndex")][1],
			dealler_drug_pos[GetPVarInt(playerid, "JobDealler_PosIndex")][2])){

		ApplyAnimation(playerid, "BOMBER", "BOM_PLANT", 4.0, 1, 1, 1, 0, 0);
		SetPVarInt(playerid, "JobDealler_Timer", JOB_DEALLER_PLAYER_TIME);
		DisablePlayerCheckpoint(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook stock OnPlayerSecondUpdate(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_DEALLER && GetPVarType(playerid, "JobDealler_Timer")){
		SetPVarInt(playerid, "JobDealler_Timer", GetPVarInt(playerid, "JobDealler_Timer") - 1);
		if(GetPVarInt(playerid, "JobDealler_Timer") <= 0){

			SendClientMessage(playerid, COLOR_YELLOW, "Вы заработали "#JOB_DEALLER_PAY"$.");
			GiveMoney(playerid, JOB_DEALLER_PAY);

			for(new i; i < MAX_INVENTORY_SLOTS; i++){
				if(!GetPlayerInventoryItemData(playerid, i, inv_player_id)) continue;
				if(GetPlayerInventoryItemData(playerid, i, inv_player_item_id) != ITEM_SUGAR_DRUG_ID) continue;
				if(GetPlayerInventoryItemData(playerid, i, inv_player_value_one) != 1) continue;

				SetPlayerInventoryItemData(playerid, i, inv_player_item_count, GetPlayerInventoryItemData(playerid, i, inv_player_item_count) - 1);
				if(GetPlayerInventoryItemData(playerid, i, inv_player_item_count) <= 0){
					Inventory:SetPlayerSlotNull(playerid, i);
				}
				break;
			}

			DeletePVar(playerid, "JobDealler_Timer");
			ClearAnimations(playerid);

			Job:DeallerCheckStage(playerid);
		}
	}
	return continue(playerid);
}

hook stock OnPlayerGiveInvItem(playerid, slot_index){
	if(GetPlayerData(playerid, p_job) == JOB_DEALLER
		&& GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_id) == ITEM_SUGAR_DRUG_ID
		&& GetPlayerInventoryItemData(playerid, slot_index, inv_player_value_one) == 1
		&& GetPlayerInventoryItemData(playerid, slot_index, inv_player_item_count) == JOB_DEALLER_DRUGS_COUNT){

		Job:DeallerCheckStage(playerid);
	}
	return continue(playerid, slot_index);
}