
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
	if(GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerVehicleID(playerid) != GetPVarInt(playerid, "JobProductsDelivery_RentVehicleID")){
		SendErrorMessage(playerid, "Вы должны находиться в арендованном рабочем транспорте.");
		return false;
	}

	switch(listitem){
		case 0:{
			if(!IsPlayerInRangeOfPoint(playerid, 5.0, JOB_PD_LOAD_POS)){
				if(GetPVarType(playerid, "JobProductsDelivery_OrderIndex")){
					SendErrorMessage(playerid, "Вы должны находиться около склада продуктов.");
				}
				else{
					GPSEnable(playerid, JOB_PD_LOAD_POS);
					SendClientMessage(playerid, COLOR_LIME, "Склад отмечен на Вашем GPS.");
				}
				return false;
			}
			if(GetPVarInt(playerid, "JobProductsDelivery_ProductsCount") >= JOB_PD_MAX_TRUCK_PRODUCTS){
				SendErrorMessage(playerid, "У Вас в фургоне максимальное количество продуктов.");
				return false;
			}

			Job:ShowProdDeliveryBuyProd(playerid);
		}
		case 1:{
			Job:ShowDelivryOrdersList(playerid);
		}
		case 2:{
			Job:ProductDeliveryLeaveAccept(playerid);
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

DialogResponse:D_JOB_BUSINESS_ENTER_DEL(playerid, response, listitem, inputtext[]){
	if(!response) return Business:ShowBusinessPanel(playerid, GetPVarInt(playerid, "Business_Index"));

	new order_count,
		order_price;

	if(sscanf(inputtext, "p<,>dd", order_count, order_price)){
		Job:ShowBusinessEnterDelivery(playerid);
		return false;
	}

	if(order_count <= 0 || order_count > BUSINESS_MAX_PRODUCTS){
		SendErrorMessage(playerid, "Корректно введите количество продуктов.");
		Job:ShowBusinessEnterDelivery(playerid);
		return false;
	}
	if(order_price < 0 || order_price > 100){
		SendErrorMessage(playerid, "Корректно введите цену за 1 единицу продуктов.");
		Job:ShowBusinessEnterDelivery(playerid);
		return false;
	}
	if(order_count + GetBusinessData(GetPVarInt(playerid, "Business_Index"), b_products) > BUSINESS_MAX_PRODUCTS){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Максимально возможное количество продуктов для заказа составляет %d единиц.",
			BUSINESS_MAX_PRODUCTS - GetBusinessData(GetPVarInt(playerid, "Business_Index"), b_products));
		SendErrorMessage(playerid, small_string);
		small_string[0] = EOS;

		Job:ShowBusinessEnterDelivery(playerid);
		return true;
	}
	if(GetBusinessData(GetPVarInt(playerid, "Business_Index"), b_balance) < order_count * order_price){
		SendErrorMessage(playerid, "На балансе бизнеса недостаточно средств.");
		Job:ShowBusinessEnterDelivery(playerid);
		return true;
	}

	new order_index = Job:GetDeliveryFreeOrderIndex();
	if(order_index == -1){
		SendErrorMessage(playerid, "В данный момент нет свободных слотов для доставки.");
		DeletePVar(playerid, "Business_Index");
		return false;
	}

	AddBusinessData(GetPVarInt(playerid, "Business_Index"), b_balance, -(order_count * order_price));
	static const fmt_query[] = "UPDATE business SET balance = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetBusinessData(GetPVarInt(playerid, "Business_Index"), b_balance),
		GetBusinessData(GetPVarInt(playerid, "Business_Index"), b_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы заказали в бизнес "cl_white"%d ед "cl_lime"продуктов за "cl_white"%d$"cl_lime". Ожидайте доставки.",
		order_count, order_count * order_price);
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	Job:AddOrder(order_index, GetPVarInt(playerid, "Business_Index"), order_count, order_price);

	DeletePVar(playerid, "Business_Index");
	return true;
}

DialogResponse:D_JOB_PROD_DELIVERY_LIST(playerid, response, listitem, inputtext[]){
	if(!response) return callcmd::delivery(playerid);
	if(GetPVarType(playerid, "JobProductsDelivery_OrderIndex")){
		SendErrorMessage(playerid, "У Вас уже есть активный заказ.");
		callcmd::delivery(playerid);
		return false;
	}

	new order_index = player_listitems[playerid][listitem];
	if(!Job:ProductDeliveryFreeOrder(order_index)){
		SendErrorMessage(playerid, "Данный заказ уже кто то взял.");
		callcmd::delivery(playerid);
		return false;
	}
	if(!GetPVarType(playerid, "JobProductsDelivery_ProductsCount") || !GetPVarInt(playerid, "JobProductsDelivery_ProductsCount")){
		SendErrorMessage(playerid, "Для начала купите продукты на складе (/delivery -> Купить продукты).");
		callcmd::delivery(playerid);
		return false;
	}

	GPSEnable(playerid, GetBusinessData(GetProdDeliveryData(order_index, pd_order_business_index), b_pos_x), GetBusinessData(GetProdDeliveryData(order_index, pd_order_business_index), b_pos_y), GetBusinessData(GetProdDeliveryData(order_index, pd_order_business_index), b_pos_z));
	SetPVarInt(playerid, "JobProductsDelivery_OrderIndex", order_index);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_white"%s "cl_brown"указана на Вашей мини-карте. Отвезите туда продукты.",
		GetBusinessData(GetProdDeliveryData(order_index, pd_order_business_index), b_name));
	SendClientMessage(playerid, -1, small_string);
	small_string[0] = EOS;
	return true;
}

DialogResponse:D_JOB_PROD_DEL_LEAVE_ACCEPT(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	Job:ProductDeliveryUninvite(playerid);
	return true;
}