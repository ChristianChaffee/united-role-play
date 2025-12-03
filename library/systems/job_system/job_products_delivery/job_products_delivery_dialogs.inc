
DialogResponse:D_JOB_PROD_DELIVERY_INVITE(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	SetPlayerData(playerid, p_job, JOB_PRODUCTS_DELIVERY);

	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: Вы устроились на работу доставщика продуктов. Арендуйте рабочий фургон на стоянке.");
	return true;
}

DialogResponse:D_JOB_PROD_DEL_ACCEPT_RENT(playerid, response, listitem, inputtext[]){
	if(!response) return RemovePlayerFromVehicle(playerid);

	new vehicleid = GetPlayerVehicleID(playerid);
	if(!vehicleid || GetVehicleData(vehicleid, v_status) != VEHICLE_STATUS_PROD_DELIVERY) return false;
	new renterid = Job:GetDeliveryVehicleRenterID(vehicleid);

	if(renterid != INVALID_PLAYER_ID && renterid != playerid){
		SendErrorMessage(playerid, "Данный транспорт уже арендуют.");
		RemovePlayerFromVehicle(playerid);
		return false;
	}
	if(GetPlayerData(playerid, p_money) < JOB_PD_CAR_RENT_PRICE){
		SendErrorMessage(playerid, "У Вас недостаточно средств для аренды транспорта.");
		RemovePlayerFromVehicle(playerid);
		return false;
	}
	if(GetPVarType(playerid, "JobProductsDelivery_RentVehicleID")){
		SendErrorMessage(playerid, "Вы уже арендуете фургон.");
		RemovePlayerFromVehicle(playerid);
		return Y_HOOKS_CONTINUE_RETURN_1;
	}

	GiveMoney(playerid, -JOB_PD_CAR_RENT_PRICE);
	SetPVarInt(playerid, "JobProductsDelivery_RentVehicleID", vehicleid);

	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: Вы арендовали фургон. Для начала купите продукты на складе для разгрузки (/delivery).");
	return true;
}

DialogResponse:D_JOB_PROD_DEL_MENU(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	if(GetPlayerData(playerid, p_job) != JOB_PRODUCTS_DELIVERY){
		SendErrorMessage(playerid, "Данная функция доступна только работнику службы доставки продуктов.");
		return false;
	}
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER && GetPlayerVehicleID(playerid) != GetPVarInt(playerid, "JobProductsDelivery_RentVehicleID")){
		SendErrorMessage(playerid, "Вы должны находиться в арендованном рабочем транспорте.");
		return false;
	}

	switch(listitem){
		case 0:{
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, JOB_PD_LOAD_POS)){
				SendErrorMessage(playerid, "Вы должны находиться около склада продуктов.");
				return false;
			}
			if(GetPVarInt(playerid, "JobProductsDelivery_ProductsCount") >= JOB_PD_MAX_TRUCK_PRODUCTS){
				SendErrorMessage(playerid, "У Вас в фургоне максимальное количество продуктов.");
				return false;
			}

			Job:ShowProdDeliveryBuyProd(playerid);
		}
	}
	return true;
}

DialogResponse:D_JOB_PROD_DEL_BUY_PRODUCTS(playerid, response, listitem, inputtext[]){
	if(!response) return callcmd::delivery(playerid);

	if(GetPlayerData(playerid, p_job) != JOB_PRODUCTS_DELIVERY){
		SendErrorMessage(playerid, "Данная функция доступна только работнику службы доставки продуктов.");
		return false;
	}
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER && GetPlayerVehicleID(playerid) != GetPVarInt(playerid, "JobProductsDelivery_RentVehicleID")){
		SendErrorMessage(playerid, "Вы должны находиться в арендованном рабочем транспорте.");
		return false;
	}
	if(!IsPlayerInRangeOfPoint(playerid, 5.0, JOB_PD_LOAD_POS)){
		SendErrorMessage(playerid, "Вы должны находиться около склада продуктов.");
		return false;
	}
	if(strval(inputtext) <= 0 || strval(inputtext) > JOB_PD_MAX_TRUCK_PRODUCTS){
		SendErrorMessage(playerid, "Корректно введите количество продуктов для покупки.");
		Job:ShowProdDeliveryBuyProd(playerid);
		return false;
	}
	if(GetPVarInt(playerid, "JobProductsDelivery_ProductsCount") + strval(inputtext) > JOB_PD_MAX_TRUCK_PRODUCTS){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Максимальное количество продуктов, доступных для покупки составляет %d единиц.",
			JOB_PD_MAX_TRUCK_PRODUCTS - GetPVarInt(playerid, "JobProductsDelivery_ProductsCount"));
		SendErrorMessage(playerid, small_string);
		small_string[0] = EOS;

		Job:ShowProdDeliveryBuyProd(playerid);
		return false;
	}
	if(GetPlayerData(playerid, p_money) < strval(inputtext) * job_products_delivery_price){
		SendErrorMessage(playerid, "У Вас недостаточно наличных денежных средств.");
		return false;
	}

	GiveMoney(playerid, -(strval(inputtext) * job_products_delivery_price));
	SetPVarInt(playerid, "JobProductsDelivery_ProductsCount", GetPVarInt(playerid, "JobProductsDelivery_ProductsCount") + strval(inputtext));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы купили "cl_white"%d ед "cl_brown"продуктов за %d$. У Вас в фургоне "cl_white"%d ед "cl_brown"продуктов.",
		strval(inputtext), strval(inputtext) * job_products_delivery_price, GetPVarInt(playerid, "JobProductsDelivery_ProductsCount"));
	SendClientMessage(playerid, COLOR_BROWN, small_string);
	small_string[0] = EOS;

	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: Выберите контракт на доставку с помощью команды '/delivery'.");
	return true;
}