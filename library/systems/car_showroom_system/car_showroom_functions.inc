
stock CarShowroom:GetOnSaleCount(car_showroom_index){
	if(car_showroom_index < 0 || car_showroom_index >= CAR_SHOWROOMS_COUNT) return 0;

	new count;
	for(new i; i < CAR_SHOWROOMS_MAX_POS_COUNT; i++){
		if(car_showroom_info[car_showroom_index][cs_vehicle_on_sale][i] == -1 || !IsValidVehicle(car_showroom_info[car_showroom_index][cs_vehicle_on_sale][i])) continue;
		count++;
	}
	return count;
}

stock CarShowroom:GetMaxSlots(car_showroom_index){
	if(car_showroom_index < 0 || car_showroom_index >= CAR_SHOWROOMS_COUNT) return 0;
	
	new count;
	for(new i; i < CAR_SHOWROOMS_MAX_POS_COUNT; i++){
		if(car_showroom_veh_pos[car_showroom_index][i][0] == 0.0 && car_showroom_veh_pos[car_showroom_index][i][1] == 0.0 && car_showroom_veh_pos[car_showroom_index][i][2] == 0.0 && car_showroom_veh_pos[car_showroom_index][i][3] == 0.0) continue;
		count++;
	}
	return count;
}

stock CarShowroom:GetFreeSlotsCount(car_showroom_index){
	if(car_showroom_index < 0 || car_showroom_index >= CAR_SHOWROOMS_COUNT) return 0;

	new count;
	for(new i, j = CarShowroom:GetMaxSlots(car_showroom_index); i < j; i++){
		if(car_showroom_info[car_showroom_index][cs_vehicle_on_sale][i] != -1) continue;
		count++;
	}
	return count;
}

stock CarShowroom:GetRandomModelsCount(car_showroom_index){
	if(car_showroom_index < 0 || car_showroom_index >= CAR_SHOWROOMS_COUNT) return 0;

	new count;
	for(new i; i < CAR_SHOWROOM_MAX_VEH_MODELS; i++){
		if(car_showroom_info[car_showroom_index][cs_vehicle_possible_models][i] == -1) continue;
		count++;
	}
	return count;
}

stock CarShowroom:IsOccupiedSlot(car_showroom_index, pos_index){
	if(car_showroom_index < 0 || car_showroom_index >= CAR_SHOWROOMS_COUNT) return 1;

	if(car_showroom_info[car_showroom_index][cs_vehicle_on_sale][pos_index] != -1) return 1;
	return 0;
}

stock CarShowroom:GetFreeSlot(car_showroom_index){
	if(car_showroom_index < 0 || car_showroom_index >= CAR_SHOWROOMS_COUNT) return -1;

	for(new i, j = CarShowroom:GetMaxSlots(car_showroom_index); i < j; i++){
		if(car_showroom_info[car_showroom_index][cs_vehicle_on_sale][i] != -1) continue;
		return i;
	}
	return -1;
}

stock CarShowroom:GetVehicleShowroom(vehicleid){
	if(!IsValidVehicle(vehicleid)) return -1;
	if(GetVehicleData(vehicleid, v_status) != VEHICLE_STATUS_CAR_SHOWROOM) return -1;

	for(new i; i < CAR_SHOWROOMS_COUNT; i++){
		for(new g, j = CarShowroom:GetMaxSlots(i); g < j; g++){
			if(car_showroom_info[i][cs_vehicle_on_sale][g] == vehicleid) return i;
		}
	}
	return -1;
}

stock CarShowroom:GetVehiclePrice(vehicleid){
	if(!IsValidVehicle(vehicleid)) return -1;
	if(GetVehicleData(vehicleid, v_status) != VEHICLE_STATUS_CAR_SHOWROOM) return -1;

	new car_showroom_index = CarShowroom:GetVehicleShowroom(vehicleid);
	if(car_showroom_index == -1) return -1;

	for(new i; i < CAR_SHOWROOM_MAX_VEH_MODELS; i++){
		if(car_showroom_info[car_showroom_index][cs_vehicle_possible_models][i] == GetVehicleModel(vehicleid)) return car_showroom_info[car_showroom_index][cs_vehicle_prices][i];
	}
	return -1;
}

stock CarShowroom:AddRandomVehicle(car_showroom_index){
	if(car_showroom_index < 0 || car_showroom_index >= CAR_SHOWROOMS_COUNT) return false;
	if(!CarShowroom:GetFreeSlotsCount(car_showroom_index)) return false;

	new random_vehicle_index = random(CarShowroom:GetRandomModelsCount(car_showroom_index) - 1);
	new random_pos_index = CarShowroom:GetFreeSlot(car_showroom_index);

	if(random_pos_index == -1) return false;

	while(car_showroom_info[car_showroom_index][cs_vehicle_possible_models][random_vehicle_index] == -1){
		random_vehicle_index = random(CarShowroom:GetRandomModelsCount(car_showroom_index) - 1);
	}

	car_showroom_info[car_showroom_index][cs_vehicle_on_sale][random_pos_index] = CreateVehicle(car_showroom_info[car_showroom_index][cs_vehicle_possible_models][random_vehicle_index], 
		car_showroom_veh_pos[car_showroom_index][random_pos_index][0], 
		car_showroom_veh_pos[car_showroom_index][random_pos_index][1],
		car_showroom_veh_pos[car_showroom_index][random_pos_index][2],
		car_showroom_veh_pos[car_showroom_index][random_pos_index][3], 
		1, 1, 1000);

	new car_params_index = GetVehicleParamIndex(car_showroom_info[car_showroom_index][cs_vehicle_on_sale][random_pos_index]);
	SetVehicleStatus(car_showroom_info[car_showroom_index][cs_vehicle_on_sale][random_pos_index], VEHICLE_STATUS_CAR_SHOWROOM);

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_lime"Модель: "cl_white"%s\n\
		"cl_lime"Цена: "cl_white"%d$",
		GetVehicleParamsData(car_params_index, vp_name),
		car_showroom_info[car_showroom_index][cs_vehicle_prices][random_vehicle_index]);

	CreateVehicle3DText(car_showroom_info[car_showroom_index][cs_vehicle_on_sale][random_pos_index], big_string, -1, 20.0);
	big_string[0] = EOS;
	return true;
}

stock CarShowroom:UpdateText(car_showroom_index){
	if(car_showroom_index < 0 || car_showroom_index >= CAR_SHOWROOMS_COUNT) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_lime"%s\n\
		"cl_grey"Всего машин на продаже: "cl_white"%d шт",
		car_showroom_info[car_showroom_index][cs_name], CarShowroom:GetOnSaleCount(car_showroom_index));

	if(!IsValidDynamic3DTextLabel(car_showroom_info[car_showroom_index][cs_text])){
		car_showroom_info[car_showroom_index][cs_text] = CreateDynamic3DTextLabel(big_string, -1, 
			car_showroom_info[car_showroom_index][cs_pos][0], 
			car_showroom_info[car_showroom_index][cs_pos][1], 
			car_showroom_info[car_showroom_index][cs_pos][2], 
			25.0, _, _, 1, 0, 0);
	}
	else UpdateDynamic3DTextLabelText(car_showroom_info[car_showroom_index][cs_text], -1, big_string);
	big_string[0] = EOS;
	return true;
}

//==============================================================================
stock CarShowroom:ShowVehicleInfo(playerid, car_showroom_index){
	if(car_showroom_index < 0 || car_showroom_index >= CAR_SHOWROOMS_COUNT) return RemovePlayerFromVehicle(playerid);
	if(!GetPlayerVehicleID(playerid)) return RemovePlayerFromVehicle(playerid);
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return RemovePlayerFromVehicle(playerid);

	new 
		price = CarShowroom:GetVehiclePrice(GetPlayerVehicleID(playerid)),
		vehicle_params_index = GetVehicleParamIndex(GetPlayerVehicleID(playerid));

	if(price == -1) return RemovePlayerFromVehicle(playerid);

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_lime"Модель: "cl_white"%s\n\
		"cl_lime"Цена: "cl_white"%d$\n\n\
		\\c"cl_grey"[Характеристики]\n\
		"cl_lime"Максимальная скорость: "cl_white"%d км/ч\n\
		"cl_lime"Разгон 0-100 км/ч: %02.f сек\n\
		"cl_lime"Марка топлива: "cl_white"%s",
		GetVehicleParamsData(vehicle_params_index, vp_name), price,
		GetVehicleParamsData(vehicle_params_index, vp_max_speed),
		GetVehicleParamsData(vehicle_params_index, vp_acceleration),
		fuel_type_names[GetVehicleParamsData(vehicle_params_index, vp_fuel_type]));

	Dialog_Open(playerid, Dialog:D_CAR_SHOWROOM_VEHICLE_INFO, DIALOG_STYLE_MSGBOX,
    	"Информация о т/с", big_string,
    	"Купить", "Закрыть");
	big_string[0] = EOS;
	return true;
}