
stock Job:ShowProdDeliveryInvite(playerid){
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