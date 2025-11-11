
forward BuyPlayerVehicle(playerid, vehicleid);
public BuyPlayerVehicle(playerid, vehicleid){
	if(!cache_insert_id()) return RemovePlayerFromVehicle(playerid);
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerVehicleID(playerid) != vehicleid) return RemovePlayerFromVehicle(playerid);
	if(GetPlayerData(playerid, p_money) < CarShowroom:GetVehiclePrice(GetPlayerVehicleID(playerid))){
		SendErrorMessage(playerid, "У Вас недостаточно средств для покупки данного транспорта.");
		RemovePlayerFromVehicle(playerid);
		return false;
	}

	GiveMoney(playerid, -CarShowroom:GetVehiclePrice(vehicleid));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы купили транспорт \"%s\" за %d$!",
		GetVehicleParamsData(GetVehicleParamIndex(vehicleid), vp_name),
		CarShowroom:GetVehiclePrice(vehicleid));
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;
	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Чтобы Ваш транспорт не забрали на штраф-стоянку, припаркуйте его "cl_grey"('/park')"cl_white".");

	new car_showroom_index = CarShowroom:GetVehicleShowroom(vehicleid);
	for(new i, j = CarShowroom:GetMaxSlots(car_showroom_index); i < j; i++){
		if(car_showroom_info[car_showroom_index][cs_vehicle_on_sale][i] == vehicleid){
			car_showroom_info[car_showroom_index][cs_vehicle_on_sale][i] = -1;
			break;
		}
	}

	GetVehicleData(vehicleid, v_id) = cache_insert_id();
	SetVehicleStatus(vehicleid, VEHICLE_STATUS_PLAYER);

	format(GetVehicleData(vehicleid, v_owner), MAX_PLAYER_NAME, "%s", GetPlayerData(playerid, p_name));
	DestroyVehicleText3D(vehicleid);

	CarShowroom:UpdateText(car_showroom_index);
	return true;
}