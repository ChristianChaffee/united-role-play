
DialogResponse:D_JOB_DEALLER_INVITE(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	SetPlayerData(playerid, p_job, JOB_DEALLER);

	DisablePlayerCheckpoint(playerid);
	GPSDisable(playerid);

	new slot_index = Inventory:GetFreeSlotIndex(playerid);
	if(slot_index == -1){
		SendErrorMessage(playerid, "У Вас нет места в инвентаре.");
		return false;
	}

	Inventory:GiveItem(playerid, slot_index, ITEM_SUGAR_DRUG_ID, JOB_DEALLER_DRUGS_COUNT, 1, _, _, 0);
	return true;
}

DialogResponse:D_JOB_DEALLER_UNINVITE(playerid, response, listitem, inputtext[]){
	if(response) Job:DeallerUninvite(playerid);
}