
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

forward DonateShop_BuyLevel(playerid, const arguments[]);
public DonateShop_BuyLevel(playerid, const arguments[]){
	new level;
	sscanf(arguments, "i", level);

	if(GetPlayerData(playerid, p_donate) < donate_upgrades_info[DONATE_UPGRADE_LEVEL][du_price] * level){
		Donate:ShowError(playerid, "У Вас недостаточно U-Coin`s.");
		return false;
	}
	if(GetPlayerData(playerid, p_level) + level > MAX_PLAYER_LEVEL){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Максимальное количество уровней, которое вы можете купить составляет %d.",
			MAX_PLAYER_LEVEL - GetPlayerData(playerid, p_level));

		Donate:ShowError(playerid, small_string);
		return false;
	}

	Donate:HideUI(playerid);
	Donate:GiveDonate(playerid, -(donate_upgrades_info[DONATE_UPGRADE_LEVEL][du_price] * level));

	SetPlayerData(playerid, p_level, GetPlayerData(playerid, p_level) + level);
	SetPlayerData(playerid, p_exp, GetExpToNextLevel(GetPlayerData(playerid, p_level)));
	SetPlayerScore(playerid, GetPlayerData(playerid, p_level));

	static const fmt_query[] = "UPDATE accounts SET level = %d, exp = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 3)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_level), GetPlayerData(playerid, p_exp),
		GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы купили +%d игровых уровней за %d U-Coins.",
		level, donate_upgrades_info[DONATE_UPGRADE_LEVEL][du_price] * level);
	SendClientMessage(playerid, COLOR_YELLOW, small_string);
	small_string[0] = EOS;
	return true;
}

forward DonateShop_BuyLicenses(playerid, const arguments[]);
public DonateShop_BuyLicenses(playerid, const arguments[]){
	if(GetPlayerData(playerid, p_donate) < donate_upgrades_info[DONATE_UPGRADE_LICENSES][du_price]){
		Donate:ShowError(playerid, "У Вас недостаточно U-Coin`s.");
		return false;
	}

	for(new i; i < LICENSES_COUNT; i++){
		GivePlayerLicense(playerid, i, 365);
	}

	Donate:HideUI(playerid);
	Donate:GiveDonate(playerid, -donate_upgrades_info[DONATE_UPGRADE_LICENSES][du_price]);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string), 
		"Вы купили пакет лицензия на 1 год за %d U-Coins.",
		donate_upgrades_info[DONATE_UPGRADE_LICENSES][du_price]);
	SendClientMessage(playerid, COLOR_YELLOW, small_string);
	small_string[0] = EOS;
	return true;
}

forward DonateShop_BuyWanted(playerid, const arguments[]);
public DonateShop_BuyWanted(playerid, const arguments[]){
	if(GetPlayerData(playerid, p_donate) < donate_upgrades_info[DONATE_UPGRADE_WANTED][du_price]){
		Donate:ShowError(playerid, "У Вас недостаточно U-Coin`s.");
		return false;
	}
	if(GetPlayerData(playerid, p_wanted) <= 0){
		Donate:ShowError(playerid, "Вы не объявлены в розыск.");
		return false;
	}

	Donate:HideUI(playerid);
	Donate:GiveDonate(playerid, -donate_upgrades_info[DONATE_UPGRADE_WANTED][du_price]);

	SetPlayerData(playerid, p_wanted, 0);
	SetPlayerWantedLevel(playerid, 0);

	static const fmt_query[] = "UPDATE accounts SET wanted = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_wanted), GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы сняли розыск за %d U-Coins.",
		donate_upgrades_info[DONATE_UPGRADE_WANTED][du_price]);
	SendClientMessage(playerid, COLOR_YELLOW, small_string);
	small_string[0] = EOS;
	return true;
}

forward DonateShop_BuyNickname(playerid, const arguments[]);
public DonateShop_BuyNickname(playerid, const arguments[]){
	new name[MAX_PLAYER_NAME];
	sscanf(arguments, "s[24]", name);

	static const fmt_query[] = "SELECT id FROM accounts WHERE name = '%s'";
	new query[sizeof(fmt_query) + (-2 + MAX_PLAYER_NAME)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		name);
	mysql_tquery(mysql, query, "DonateShop_BuyNicknameCheck", "is", playerid, name);
}

forward DonateShop_BuyNicknameCheck(playerid, name[]);
public DonateShop_BuyNicknameCheck(playerid, name[]){
	new rows;
	cache_get_row_count(rows);

	if(rows){
		Donate:ShowError(playerid, "Данный ник-нейм уже занят.");
		return false;
	}
	if(GetPlayerData(playerid, p_donate) < donate_upgrades_info[DONATE_UPGRADE_NAME][du_price]){
		Donate:ShowError(playerid, "У Вас недостаточно U-Coin`s.");
		return false;
	}

	Donate:HideUI(playerid);
	Donate:GiveDonate(playerid, -donate_upgrades_info[DONATE_UPGRADE_NAME][du_price]);

	static const fmt_query[] = "UPDATE accounts SET name = '%s' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + MAX_PLAYER_NAME) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		name, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);

	SetPlayerName(playerid, name);
	format(GetPlayerData(playerid, p_name), MAX_PLAYER_NAME, "%s", name);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы сменили ник-нейм на %s за %d U-Coins.",
		name, donate_upgrades_info[DONATE_UPGRADE_NAME][du_price]);
	SendClientMessage(playerid, COLOR_YELLOW, small_string);
	small_string[0] = EOS;
	return true;
}

forward DonateShop_BuyCashBox(playerid, const arguments[]);
public DonateShop_BuyCashBox(playerid, const arguments[]){
	new index;
	sscanf(arguments, "i", index);

	if(index < 0 || index >= DONATE_SHOP_CASHBOXES_COUNT){
		Donate:ShowError(playerid, "Произошла ошибка.");
		return false;
	}
	if(donate_cashbox_info[index][dc_price] > GetPlayerData(playerid, p_donate)){
		Donate:ShowError(playerid, "У Вас недостаточно U-Coin`s.");
		return false;
	}

	Donate:HideUI(playerid);
	Donate:GiveDonate(playerid, -donate_cashbox_info[index][dc_price]);

	GiveMoney(playerid, donate_cashbox_info[index][dc_give_money]);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы купили валютный набор \"%s\" (%d$) за %d U-Coins.",
		donate_cashbox_info[index][dc_name], donate_cashbox_info[index][dc_give_money], donate_cashbox_info[index][dc_price]);
	SendClientMessage(playerid, COLOR_YELLOW, small_string);
	small_string[0] = EOS;
	return true;
}