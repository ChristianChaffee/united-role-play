
stock Job:ShowDeallerInvite(playerid){
	if(GetPlayerData(playerid, p_job) != JOB_NONE && GetPlayerData(playerid, p_job) != JOB_DEALLER){
		SendErrorMessage(playerid, "Для начала увольтесь с другой работы.");
		return false;
	}

	if(GetPlayerData(playerid, p_job) == JOB_DEALLER){
		Dialog_Open(playerid, Dialog:D_JOB_DEALLER_UNINVITE, DIALOG_STYLE_MSGBOX,
			"Диллер", 
			""cl_blue"- "cl_white"Ты действительно хочешь покончить с нашим \"делом\"?",
			"Да", "Нет");
		return false;
	}

	Dialog_Open(playerid, Dialog:D_JOB_DEALLER_INVITE, DIALOG_STYLE_MSGBOX,
		"Диллер", 
		""cl_blue"- "cl_white"Пс, если хочешь немного подзаработать, можешь взять у меня "#JOB_DEALLER_DRUGS_COUNT" скрутков\n\
		и доставить их по точкам. Плачу "cl_green""#JOB_DEALLER_PAY"$ "cl_white"за скруток.",
		"Взять", "Отказаться");
	return true;
}

stock Job:DeallerCheckStage(playerid){
	new is_find;
	for(new i; i < MAX_INVENTORY_SLOTS; i++){
		if(!GetPlayerInventoryItemData(playerid, i, inv_player_id)) continue;
		if(GetPlayerInventoryItemData(playerid, i, inv_player_item_id) != ITEM_SUGAR_DRUG_ID) continue;
		if(GetPlayerInventoryItemData(playerid, i, inv_player_value_one) != 1) continue;

		is_find = 1;
		break;
	}

	if(!is_find){
		SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Вы разнесли все скрутки. Можете отправиться за новыми или заниматься своими делами.");
		Job:DeallerUninvite(playerid);
		return false;
	}

	new drug_pos_index = -1;
	if(GetPVarType(playerid, "JobDealler_PosIndex")){
		drug_pos_index = random(sizeof(dealler_drug_pos));
		while(drug_pos_index == GetPVarInt(playerid, "JobDealler_PosIndex")){
			drug_pos_index = random(sizeof(dealler_drug_pos));
		}
	}
	else drug_pos_index = random(sizeof(dealler_drug_pos));

	if(drug_pos_index == -1){
		SendClientMessage(playerid, COLOR_DARK_RED, "Произошла ошибка. Обратитесь к разработчикам.");
		return false;
	}

	DisablePlayerCheckpoint(playerid);
	GPSDisable(playerid);

	SetPlayerCheckpoint(playerid, dealler_drug_pos[drug_pos_index][0], dealler_drug_pos[drug_pos_index][1], dealler_drug_pos[drug_pos_index][2], 1.0);
	SetPVarInt(playerid, "JobDealler_PosIndex", drug_pos_index);
	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Место закладки отмечено на Вашей мини-карте.");
	return true;
}

stock Job:DeallerRemoveItems(playerid){
	for(new i; i < MAX_INVENTORY_SLOTS; i++){
		if(!GetPlayerInventoryItemData(playerid, i, inv_player_id)) continue;
		if(GetPlayerInventoryItemData(playerid, i, inv_player_item_id) != ITEM_SUGAR_DRUG_ID) continue;
		if(GetPlayerInventoryItemData(playerid, i, inv_player_value_one) != 1) continue;

		Inventory:SetPlayerSlotNull(playerid, i);
	}
	return true;
}

stock Job:DeallerUninvite(playerid){
	SetPlayerData(playerid, p_job, JOB_NONE);

	DeletePVar(playerid, "JobDealler_PosIndex");
	DeletePVar(playerid, "JobDealler_Timer");

	Job:DeallerRemoveItems(playerid);
	DisablePlayerCheckpoint(playerid);
	return true;
}