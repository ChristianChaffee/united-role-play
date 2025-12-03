
CMD:delivery(playerid){
	if(!player_logged[playerid]) return false;
	if(GetPlayerData(playerid, p_job) != JOB_PRODUCTS_DELIVERY){
		SendErrorMessage(playerid, "Данная функция доступна только работнику службы доставки продуктов.");
		return false;
	}
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER && GetPlayerVehicleID(playerid) != GetPVarInt(playerid, "JobProductsDelivery_RentVehicleID")){
		SendErrorMessage(playerid, "Вы должны находиться в арендованном рабочем транспорте.");
		return false;
	}

	Job:ShowProdDeliveryMenu(playerid);
	return true;
}