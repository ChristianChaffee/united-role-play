
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	job_products_delivery_price = RandomEx(JOB_PD_MIN_PRICE, JOB_PD_MAX_PRICE);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_lime"[Загрузка продуктов]\n\
		"cl_grey"Закупочная цена: "cl_white"%d$\n\
		"cl_grey"Для закупки используйте '/delivery'",
		job_products_delivery_price);

	job_products_delivery_text = CreateDynamic3DTextLabel(small_string, -1, JOB_PD_LOAD_POS, 25.0, _, _, 1, 0, 0);
	small_string[0] = EOS;

	job_products_change_price_time = JOB_PD_CHANGE_PRICE_TIME;

	for(new i; i < MAX_PRODUCTS_DELIVERY_ORDERS; i++){
		SetProdDeliveryData(i, pd_order_business_index, -1);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_PRODUCTS_DELIVERY){
		Job:ProductDeliveryUninvite(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleDeath(vehicleid, killerid){
	if(Job:GetDeliveryVehicleRenterID(vehicleid) != INVALID_PLAYER_ID){
		Job:ProductDeliveryUninvite(Job:GetDeliveryVehicleRenterID(vehicleid));
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnVehicleSpawn(vehicleid){
	if(Job:GetDeliveryVehicleRenterID(vehicleid) != INVALID_PLAYER_ID){
		Job:ProductDeliveryUninvite(Job:GetDeliveryVehicleRenterID(vehicleid));
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate){
	if(newstate == PLAYER_STATE_DRIVER && GetVehicleData(GetPlayerVehicleID(playerid), v_status) == VEHICLE_STATUS_PROD_DELIVERY){
		
		new vehicleid = GetPlayerVehicleID(playerid),
			renterid = Job:GetDeliveryVehicleRenterID(vehicleid);

		if(renterid != INVALID_PLAYER_ID && renterid != playerid){
			SendErrorMessage(playerid, "Данный транспорт уже арендуют.");
			RemovePlayerFromVehicle(playerid);
			return Y_HOOKS_CONTINUE_RETURN_1;
		}
		if(renterid == INVALID_PLAYER_ID){
			if(GetPVarType(playerid, "JobProductsDelivery_RentVehicleID")){
				SendErrorMessage(playerid, "Вы уже арендуете фургон.");
				RemovePlayerFromVehicle(playerid);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
			Dialog_Open(playerid, Dialog:D_JOB_PROD_DEL_ACCEPT_RENT, DIALOG_STYLE_MSGBOX,
				"Аренда транспорта",
				""cl_blue"- "cl_white"Вы действительно хотите арендовать фургон за "cl_green""#JOB_PD_CAR_RENT_PRICE"$"cl_white"?",
				"Да", "Нет");
		}

	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeExit(){
	static const fmt_query[] = "UPDATE business SET balance = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];

	for(new i; i < MAX_PRODUCTS_DELIVERY_ORDERS; i++){
		if(GetProdDeliveryData(i, pd_order_business_index) == -1) continue;
		if(!GetProdDeliveryData(i, pd_order_price)) continue;

		AddBusinessData(GetProdDeliveryData(i, pd_order_business_index), b_balance, GetProdDeliveryData(i, pd_order_price) * GetProdDeliveryData(i, pd_order_count));
		
		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetBusinessData(GetProdDeliveryData(i, pd_order_business_index), b_balance),
			GetBusinessData(GetProdDeliveryData(i, pd_order_business_index), b_id));
		mysql_tquery(mysql, query);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterRCP(playerid){
	if(GetPVarType(playerid, "GPS_Enabled") && GetPlayerData(playerid, p_job) == JOB_PRODUCTS_DELIVERY
		&& GetPVarType(playerid, "JobProductsDelivery_OrderIndex")){

		new order_index = GetPVarInt(playerid, "JobProductsDelivery_OrderIndex"),
			business_index = GetProdDeliveryData(order_index, pd_order_business_index);

		if(!IsPlayerInRangeOfPoint(playerid, 5.0, GetBusinessData(business_index, b_pos_x), GetBusinessData(business_index, b_pos_y), GetBusinessData(business_index, b_pos_z))){
			SendErrorMessage(playerid, "Произошла ошибка. Активный заказ сброшен.");
			DeletePVar(playerid, "JobProductsDelivery_OrderIndex");
			return Y_HOOKS_CONTINUE_RETURN_1;
		}

		new products_count;
		if(GetProdDeliveryData(order_index, pd_order_count) - GetPVarInt(playerid, "JobProductsDelivery_ProductsCount") <= 0){
			products_count = GetProdDeliveryData(order_index, pd_order_count);
		}
		else{
			products_count = GetPVarInt(playerid, "JobProductsDelivery_ProductsCount");
		}

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Вы доставили "cl_white"%d ед "cl_brown"продуктов в "cl_white"%s "cl_brown"и заработали "cl_white"%d$"cl_brown".",
			products_count, GetBusinessData(business_index, b_name),
			products_count * GetProdDeliveryData(order_index, pd_order_price));
		SendClientMessage(playerid, COLOR_BROWN, small_string);
		small_string[0] = EOS;

		AddBusinessData(business_index, b_products, products_count);

		static const fmt_query[] = "UPDATE business SET products = %d WHERE id = %d";
		new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetBusinessData(business_index, b_products), GetBusinessData(business_index, b_id));
		mysql_tquery(mysql, query);
		
		SetPVarInt(playerid, "JobProductsDelivery_ProductsCount", GetPVarInt(playerid, "JobProductsDelivery_ProductsCount") - products_count);
		if(GetPVarInt(playerid, "JobProductsDelivery_ProductsCount") <= 0){
			DeletePVar(playerid, "JobProductsDelivery_ProductsCount");
		}

		AddProdDeliveryData(order_index, pd_order_count, -products_count);
		if(GetProdDeliveryData(order_index, pd_order_count) <= 0){
			Job:DeleteOrder(order_index);
		}

		GiveMoney(playerid, products_count * GetProdDeliveryData(order_index, pd_order_price));
		DeletePVar(playerid, "JobProductsDelivery_OrderIndex");
		return true;
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock SecondTimerStock(){
	if(job_products_change_price_time > 0){
		job_products_change_price_time--;

		if(job_products_change_price_time <= 0){
			job_products_delivery_price = RandomEx(JOB_PD_MIN_PRICE, JOB_PD_MAX_PRICE);

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				""cl_lime"[Загрузка продуктов]\n\
				"cl_grey"Закупочная цена: "cl_white"%d$\n\
				"cl_grey"Для закупки используйте '/delivery'",
				job_products_delivery_price);

			UpdateDynamic3DTextLabelText(job_products_delivery_text, -1, small_string);
			small_string[0] = EOS;

			job_products_change_price_time = JOB_PD_CHANGE_PRICE_TIME;

		}
	}
	return continue();
}