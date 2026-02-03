
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
			CEFSTR(donate_vehicle_info[i][dv_image_url]));

		if(donate_vehicle_info[i][dv_discount] != 0){
			cef_emit_event(playerid, "addDonateDiscountBlock",
				CEFSTR(donate_vehicle_info[i][dv_image_url]),
				CEFINT(donate_vehicle_info[i][dv_price]),
				CEFINT(donate_vehicle_info[i][dv_discount]),
				CEFINT(donate_vehicle_info[i][dv_rarity]));
		}
	}

	cef_emit_event(playerid, "sortDonateVehicles");
	// ---

	// --- Привелегии
	cef_emit_event(playerid, "setDonateUpgradesPrices",
		CEFINT(donate_upgrades_info[DONATE_UPGRADE_LEVEL][du_price]),
		CEFINT(donate_upgrades_info[DONATE_UPGRADE_LICENSES][du_price]),
		CEFINT(donate_upgrades_info[DONATE_UPGRADE_WANTED][du_price]),
		CEFINT(donate_upgrades_info[DONATE_UPGRADE_NAME][du_price]));
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