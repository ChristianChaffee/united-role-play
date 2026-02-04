
stock Donate:ShowUI(playerid){
	cef_emit_event(playerid, "showDonateShop",
		CEFINT(GetPlayerData(playerid, p_money)),
		CEFINT(GetPlayerData(playerid, p_donate)));

	// --- Транспорт
	cef_emit_event(playerid, "clearDonateVehicles");
	for(new i; i < DONATE_SHOP_VEHICLES_COUNT; i++){
		cef_emit_event(playerid, "addDonateVehicle",
			CEFSTR(GetVehicleParamsData(GetVehicleParamIndexByModel(donate_vehicle_info[i][dv_model_id]), vp_name)),
			CEFINT(donate_vehicle_info[i][dv_price]),
			CEFINT(donate_vehicle_info[i][dv_discount]),
			CEFINT(donate_vehicle_info[i][dv_rarity]),
			CEFSTR(donate_vehicle_info[i][dv_image_url]),
			CEFINT(i));

		if(donate_vehicle_info[i][dv_discount] != 0){
			cef_emit_event(playerid, "addDonateDiscountBlock",
				CEFSTR(donate_vehicle_info[i][dv_image_url]),
				CEFINT(donate_vehicle_info[i][dv_price]),
				CEFINT(donate_vehicle_info[i][dv_discount]),
				CEFINT(donate_vehicle_info[i][dv_rarity]));
		}
	}

	cef_emit_event(playerid, "sortDonateVehicles");

	// --- Привелегии
	cef_emit_event(playerid, "setDonateUpgradesPrices",
		CEFINT(donate_upgrades_info[DONATE_UPGRADE_LEVEL][du_price]),
		CEFINT(donate_upgrades_info[DONATE_UPGRADE_LICENSES][du_price]),
		CEFINT(donate_upgrades_info[DONATE_UPGRADE_WANTED][du_price]),
		CEFINT(donate_upgrades_info[DONATE_UPGRADE_NAME][du_price]));

	// --- Скины
	cef_emit_event(playerid, "clearDonateSkins");

	for(new i; i < DONATE_SHOP_SKINS_COUNT; i++){
		cef_emit_event(playerid, "addDonateSkins",
			CEFSTR(donate_skin_info[i][ds_name]),
			CEFINT(donate_skin_info[i][ds_price]),
			CEFINT(donate_skin_info[i][ds_discount]),
			CEFINT(donate_skin_info[i][ds_rarity]),
			CEFSTR(donate_skin_info[i][ds_image_url]),
			CEFINT(i));

		if(donate_skin_info[i][ds_discount] != 0){
			cef_emit_event(playerid, "addDonateDiscountBlock",
				CEFSTR(donate_skin_info[i][ds_image_url]),
				CEFINT(donate_skin_info[i][ds_price]),
				CEFINT(donate_skin_info[i][ds_discount]),
				CEFINT(donate_skin_info[i][ds_rarity]));
		}
	}

	cef_emit_event(playerid, "sortDonateSkins");

	// --- Денежные наборы
	cef_emit_event(playerid, "clearDonateCashBoxes");
	for(new i; i < DONATE_SHOP_CASHBOXES_COUNT; i++){
		cef_emit_event(playerid, "addDonateCashBox",
			CEFSTR(donate_cashbox_info[i][dc_name]),
			CEFINT(donate_cashbox_info[i][dc_price]),
			CEFINT(donate_cashbox_info[i][dc_give_money]),
			CEFINT(i));
	}

	// --- Кейсы
	cef_emit_event(playerid, "clearDonateCases");
	for(new i; i < DONATE_SHOP_CASES_COUNT; i++){
		cef_emit_event(playerid, "DonateShop_AddCase",
			CEFSTR(donate_case_info[i][dci_name]),
			CEFINT(donate_case_info[i][dci_price]),
			CEFINT(donate_case_info[i][dci_discount]),
			CEFSTR(donate_case_info[i][dci_image_url]),
			CEFINT(i));

		if(donate_case_info[i][dci_discount] != 0){
			cef_emit_event(playerid, "addDonateDiscountBlock",
				CEFSTR(donate_case_info[i][dci_image_url]),
				CEFINT(donate_case_info[i][dci_price]),
				CEFINT(donate_case_info[i][dci_discount]),
				CEFINT(RARITY_TYPE_COMMON));
		}
	}

	Donate:AddCasePrizes(playerid);

	// ---

	SelectTextDraw(playerid, -1);
	SetPVarInt(playerid, "Donate_IsOpen", 1);
	return true;
}

stock Donate:HideUI(playerid){
	cef_emit_event(playerid, "hideDonateShop");
	DeletePVar(playerid, "Donate_IsOpen");
	CancelSelectTextDraw(playerid);
	return true;
}

stock Donate:ShowError(playerid, text[]){
	if(!GetPVarType(playerid, "Donate_IsOpen")) return false;
	cef_emit_event(playerid, "showDonateShopError", CEFSTR(text));
	return true;
}

stock Donate:GetPriceWithDiscount(price, discount){
	return price - floatround((float(discount) / 100.0) * float(price));
}

stock Donate:GiveDonate(playerid, amount){
	AddPlayerData(playerid, p_donate, amount);

	static const fmt_query[] = "UPDATE accounts SET donate = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_donate), GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);
	return true;
}

stock Donate:AddCasePrizes(playerid){
	for(new i; i < DONATE_SHOP_CASES_COUNT; i++){
		cef_emit_event(playerid, "clearDonateCasePrizes", CEFINT(i));
	}

	for(new i, name[64]; i < DONATE_SHOP_CASE_PRIZES_COUNT; i++){

		switch(donate_case_prize_info[i][dcp_type]){

			case CASE_PRIZE_TYPE_MONEY: format(name, sizeof(name), "%d$", donate_case_prize_info[i][dcp_count]);
			case CASE_PRIZE_TYPE_DONATE: format(name, sizeof(name), "%d U-Coin`s", donate_case_prize_info[i][dcp_count]);
			case CASE_PRIZE_TYPE_SKIN: format(name, sizeof(name), "Одежда №%d", donate_case_prize_info[i][dcp_count]);
			case CASE_PRIZE_TYPE_VEHICLE: format(name, sizeof(name), "%s", GetVehicleParamsData(GetVehicleParamIndexByModel(donate_case_prize_info[i][dcp_count]), vp_name));
		}

		cef_emit_event(playerid, "addDonateCasePrize",
			CEFSTR(name),
			CEFINT(donate_case_prize_info[i][dcp_rarity]),
			CEFSTR(donate_case_prize_info[i][dcp_image_url]),
			CEFINT(donate_case_prize_info[i][dcp_case_index]),
			CEFINT(i),
			CEFFLOAT(donate_case_prize_info[i][dcp_chance]));
	}

	for(new i; i < DONATE_SHOP_CASES_COUNT; i++){
		cef_emit_event(playerid, "sortDonateCasePrizes", CEFINT(i));
	}
}

#warning CREATE_PHOTOS
//==============================================================================
new createPhotosVehicleID = INVALID_VEHICLE_ID;

CMD:addveh(playerid, params[]){
	if(sscanf(params, "iii", params[0], params[1], params[2])) return SendErrorMessage(playerid, "Используйте '/addveh [ID] [Цвет 1] [Цвет 2]'");

	if(createPhotosVehicleID != INVALID_VEHICLE_ID && IsValidVehicle(createPhotosVehicleID)){
		DestroyVehicle(createPhotosVehicleID);
	}

	createPhotosVehicleID = CreateVehicle(params[0], 
		370.1483, -2032.8489, 7.6394, 322.7732, 
		params[1], params[2], -1);

	SetPlayerPos(playerid, 369.7998,-2024.8151,7.6719);
	SetPlayerFacingAngle(playerid, 180.0);

	GivePlayerWeapon(playerid, WEAPON_CAMERA, 100);

	SetWeather(0);
	SetWorldTime(20);
	return true;
}