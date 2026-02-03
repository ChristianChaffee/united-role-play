
forward DonateShop_BuyVehicle(playerid, const arguments[]);
public DonateShop_BuyVehicle(playerid, const arguments[]){
	new index;
	sscanf(arguments, "i", index);

	if(index < 0 || index >= DONATE_SHOP_VEHICLES_COUNT){
		Donate:ShowError(playerid, "Произошла ошибка.");
		return false;
	}

	new price = Donate:GetPriceWithDiscount(donate_vehicle_info[index][dv_price], donate_vehicle_info[index][dv_discount]);

	if(GetPlayerData(playerid, p_donate) < price){
		Donate:ShowError(playerid, "У Вас недостаточно U-Coin`s.");
		return false;
	}
	if(GetFreeVehicleSlotsCount(playerid) <= 0){
		Donate:ShowError(playerid, "У Вас недостаточно слотов для транспорта.");
		return false;
	}

	Donate:HideUI(playerid);

	Donate:GiveDonate(playerid, -price);

	static const fmt_query[] = "INSERT INTO vehicles (model_id, status, owner, price, color_1, color_2) VALUES (%d, %d, %d, %d, %d, %d)";
	new query[sizeof(fmt_query) + ((-2 + 11) * 6)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		donate_vehicle_info[index][dv_model_id],
		VEHICLE_STATUS_PLAYER,
		GetPlayerData(playerid, p_id),
		price * DONATE_SHOP_CONVERT,
		1, 1);
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы купили транспорт \"%s\" за %d U-Coins.",
		GetVehicleParamsData(GetVehicleParamIndexByModel(donate_vehicle_info[index][dv_model_id]), vp_name),
		price);
	SendClientMessage(playerid, COLOR_YELLOW, small_string);
	small_string[0] = EOS;
	SendClientMessage(playerid, COLOR_GREY, "- Для загрузки транспорта используйте телефон ('/phone').");

	return true;
}