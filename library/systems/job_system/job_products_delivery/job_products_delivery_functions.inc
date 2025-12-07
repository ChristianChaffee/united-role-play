
stock Job:ShowProdDeliveryInvite(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_PRODUCTS_DELIVERY){
		Job:ProductDeliveryLeaveAccept(playerid);
		return false;
	}

	Dialog_Open(playerid, Dialog:D_JOB_PROD_DELIVERY_INVITE, DIALOG_STYLE_MSGBOX,
		"Директор службы доставки",
		""cl_blue"- "cl_white"Ты действительно хочешь устроиться на работу доставщика продуктов?\n\n\
		"cl_grey"Описание работы: "cl_white"доставка продуктов по бизнесам штата.",
		"Да", "Нет");
	return true;
}

stock Job:ShowProdDeliveryMenu(playerid){
	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_brown"Продуктов в фургоне "cl_grey"["cl_white"%d/%d"cl_grey"]",
		GetPVarInt(playerid, "JobProductsDelivery_ProductsCount"), JOB_PD_MAX_TRUCK_PRODUCTS);

	Dialog_Open(playerid, Dialog:D_JOB_PROD_DEL_MENU, DIALOG_STYLE_LIST,
		small_string,
		""cl_brown"- "cl_white"Купить продукты\n\
		"cl_brown"- "cl_white"Контракты на доставку\n\
		"cl_light_red"- Завершить рабочий день доставщика продуктов",
		"Выбрать", "Закрыть");
	small_string[0] = EOS;
	return true;
}

stock Job:ShowProdDeliveryBuyProd(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_brown"- "cl_white"Введите количество продуктов, которое хотите купить\n\n\
		"cl_brown"Цена за 1 единицу: "cl_white"%d$\n\
		"cl_brown"Доступно для покупки: "cl_white"%d ед",
		job_products_delivery_price,
		JOB_PD_MAX_TRUCK_PRODUCTS - GetPVarInt(playerid, "JobProductsDelivery_ProductsCount"));

	Dialog_Open(playerid, Dialog:D_JOB_PROD_DEL_BUY_PRODUCTS, DIALOG_STYLE_INPUT,
		"Покупка продуктов", big_string,
		"Купить", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Job:ShowBusinessEnterDelivery(playerid){
	if(!GetPVarType(playerid, "Business_Index")) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите количество продуктов и цену за 1 единицу товара\n\
		"cl_blue"Доступно для заказа: "cl_white"%d ед\n\n\
		"cl_grey"Пример: "cl_white"1000, 5",
		BUSINESS_MAX_PRODUCTS - GetBusinessData(GetPVarInt(playerid, "Business_Index"), b_products));
	Dialog_Open(playerid, Dialog:D_JOB_BUSINESS_ENTER_DEL, DIALOG_STYLE_INPUT,
		"Доставка продуктов", big_string,
		"Заказать", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Job:ShowDelivryOrdersList(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Бизнес\t"cl_grey"Количество продуктов\t"cl_grey"Итоговая цена\t"cl_grey"Расстояние");

	new count,
		Float:distance;

	for(new i; i < MAX_PRODUCTS_DELIVERY_ORDERS; i++){
		if(GetProdDeliveryData(i, pd_order_business_index) == -1) continue;
		if(!Job:ProductDeliveryFreeOrder(i)) continue;

		distance = GetPlayerDistanceFromPoint(playerid, 
			GetBusinessData(GetProdDeliveryData(i, pd_order_business_index), b_pos_x),
			GetBusinessData(GetProdDeliveryData(i, pd_order_business_index), b_pos_y),
			GetBusinessData(GetProdDeliveryData(i, pd_order_business_index), b_pos_z));

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_brown"%d. "cl_white"%s\t%d ед\t%d$\t%0.2f м",
			big_string, count + 1,
			GetBusinessData(GetProdDeliveryData(i, pd_order_business_index), b_name),
			GetProdDeliveryData(i, pd_order_count),
			GetProdDeliveryData(i, pd_order_count) * GetProdDeliveryData(i, pd_order_price),
			distance);

		player_listitems[playerid][count] = i;
		count++;
	}

	if(!count){
		SendErrorMessage(playerid, "В данный момент нет свободных заказов.");
		big_string[0] = EOS;
		return false;
	}

	Dialog_Open(playerid, Dialog:D_JOB_PROD_DELIVERY_LIST, DIALOG_STYLE_TABLIST_HEADERS,
		"Список заказов", big_string,
		"Взять", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Job:ProductDeliveryLeaveAccept(playerid){
	Dialog_Open(playerid, Dialog:D_JOB_PROD_DEL_LEAVE_ACCEPT, DIALOG_STYLE_MSGBOX,
		"Развозчик продуктов",
		""cl_blue"- "cl_white"Вы действительно хотите уволиться с работы развозчика продуктов?\n\
		"cl_grey"Все продукты в Вашем фургоне будут проданы за минимальную стоимость в "#JOB_PD_MIN_PRICE" за единицу",
		"Да", "Нет");
	return true;
}

//==============================================================================
stock Job:GetDeliveryVehicleRenterID(vehicleid){
	if(GetVehicleData(vehicleid, v_status) != VEHICLE_STATUS_PROD_DELIVERY) return INVALID_PLAYER_ID;

	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(GetPVarInt(i, "JobProductsDelivery_RentVehicleID") != vehicleid) continue;
		return i;
	}
	return INVALID_PLAYER_ID;
}

stock Job:IsBusinessHaveDeliveryOrder(business_index){
	for(new i; i < MAX_PRODUCTS_DELIVERY_ORDERS; i++){
		if(GetProdDeliveryData(i, pd_order_business_index) != business_index) continue;
		return true;
	}
	return false;
}

stock Job:GetDeliveryFreeOrderIndex(){
	for(new i; i < MAX_PRODUCTS_DELIVERY_ORDERS; i++){
		if(GetProdDeliveryData(i, pd_order_business_index) == -1) return i;
	}
	return -1;
}

stock Job:AddOrder(order_index, business_index, order_count, order_price){
	if(order_index < 0 || order_index >= MAX_PRODUCTS_DELIVERY_ORDERS) return false;
	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;

	SetProdDeliveryData(order_index, pd_order_business_index, business_index);
	SetProdDeliveryData(order_index, pd_order_count, order_count);
	SetProdDeliveryData(order_index, pd_order_price, order_price);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Поступил новый заказ от "cl_white"%s "cl_brown"на "cl_white"%d ед "cl_brown"продуктов.",
		GetBusinessData(business_index, b_name), order_count);

	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(GetPlayerData(i, p_job) != JOB_PRODUCTS_DELIVERY) continue;

		SendClientMessage(i, COLOR_BROWN, small_string);
		SendClientMessage(i, COLOR_GREY, "- Чтобы узнать подробнее о заказе используйте '/delivery'.");
	}
	small_string[0] = EOS;
	return true;
}

stock Job:DeleteOrder(order_index){
	if(order_index < 0 || order_index >= MAX_PRODUCTS_DELIVERY_ORDERS) return false;

	SetProdDeliveryData(order_index, pd_order_business_index, -1);
	SetProdDeliveryData(order_index, pd_order_count, 0);
	SetProdDeliveryData(order_index, pd_order_price, 0);
	return true;
}

stock Job:ProductDeliveryFreeOrder(order_index){
	if(order_index < 0 || order_index >= MAX_PRODUCTS_DELIVERY_ORDERS) return false;
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(!GetPVarType(i, "JobProductsDelivery_OrderIndex")) continue;

		if(GetPVarInt(i, "JobProductsDelivery_OrderIndex") == order_index) return false;
	}
	return true;
}

stock Job:ProductDeliveryUninvite(playerid){
	if(GetPVarInt(playerid, "JobProductsDelivery_ProductsCount") > 0){
		GiveMoney(playerid, GetPVarInt(playerid, "JobProductsDelivery_ProductsCount") * JOB_PD_MIN_PRICE);
	}

	new vehicleid = INVALID_VEHICLE_ID;
	if(GetPVarType(playerid, "JobProductsDelivery_RentVehicleID") && IsValidVehicle(GetPVarInt(playerid, "JobProductsDelivery_RentVehicleID"))){
		vehicleid = GetPVarInt(playerid, "JobProductsDelivery_RentVehicleID");
	}

	DeletePVar(playerid, "JobProductsDelivery_ProductsCount");
	DeletePVar(playerid, "JobProductsDelivery_RentVehicleID");
	DeletePVar(playerid, "JobProductsDelivery_OrderIndex");

	if(GetPVarType(playerid, "GPS_Enabled")){
		GPSDisable(playerid);
	}

	SetPlayerData(playerid, p_job, JOB_NONE);

	SendClientMessage(playerid, COLOR_BLUE, "Вы уволены с работы развозчика продуктов.");
	if(vehicleid != INVALID_VEHICLE_ID){
		SetVehicleToRespawn(vehicleid);
	}
	return true;
}