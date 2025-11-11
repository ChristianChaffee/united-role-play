
DialogResponse:D_PARK_FINE_LIST(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	if(GetPlayerData(playerid, p_money) < PARKING_FINE_PRICE){
		SendErrorMessage(playerid, "У Вас недостаточно средств для возврата транспорта со штраф-стоянки.");
		return false;
	}

	GiveMoney(playerid, -PARKING_FINE_PRICE);
	CreatePlayerVehicle(playerid, player_listitems[playerid][listitem], PARKING_FINE_VEH_POS);
	
	SendClientMessage(playerid, COLOR_GREEN, "Вы забрали транспорт со штраф-стоянки.");
	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Чтобы Ваш транспорт не забрали на штраф-стоянку, припаркуйте его "cl_grey"('/park')"cl_white".");
	return true;
}