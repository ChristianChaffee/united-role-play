
forward LoadGarages(garage_index);
public LoadGarages(garage_index){
	new rows, 
		tick_time = GetTickCount(),
		old_garage_index = garage_index;

	cache_get_row_count(rows);

	if(rows >= MAX_GARAGES){
		printf("[LoadGarages - ERROR]: Количество записей в базе данных превышает размер массива (%d >= %d)", rows, MAX_GARAGES);
		return false;
	}

	for(new i; i < rows; i++){
		if(old_garage_index == -1){
			garage_index = i;
		}

		cache_get_value_name_int(i, "id", GetGarageData(garage_index, g_id));
		cache_get_value_name_int(i, "house_id", GetGarageData(garage_index, g_house_id));
		cache_get_value_name_float(i, "pos_x", GetGarageData(garage_index, g_pos_x));
		cache_get_value_name_float(i, "pos_y", GetGarageData(garage_index, g_pos_y));
		cache_get_value_name_float(i, "pos_z", GetGarageData(garage_index, g_pos_z));

		House:GarageUpdate(garage_index);
	}

	if(old_garage_index == -1){
		printf("[LoadGarages]: Гаражи загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
		load_info[loaded_garages] = true;
	}
	return true;
}

forward LoadCreateGarage(playerid);
public LoadCreateGarage(playerid){
	new garage_id = cache_insert_id();

	if(GetPlayerBankCardData(playerid, pbc_balance) < house_upgrade_info[HOUSE_UPGRADE_GARAGE][hu_price]){
		static const fmt_query[] = "DELETE FROM garages WHERE id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, garage_id);

		SendErrorMessage(playerid, "У Вас недостаточно средств для покупки гаража.");
		mysql_tquery(mysql, query);
		return false;
	}

	new free_index = -1;
	for(new i; i < MAX_GARAGES; i++){
		if(GetGarageData(i, g_id) != 0) continue;
		
		free_index = i;
		break;
	}

	if(free_index == -1){
		static const fmt_query[] = "DELETE FROM garages WHERE id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, garage_id);

		SendErrorMessage(playerid, "На сервере максимальное количество гаражей.");
		mysql_tquery(mysql, query);
		return false;
	}

	SendClientMessage(playerid, COLOR_LIME, "Гараж успешно создан!");

	static const fmt_query[] = "SELECT * FROM garages WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		garage_id);
	mysql_tquery(mysql, query, "LoadGarages", "i", free_index);
	return true;
}

forward garageRepairConfirm(playerid, const arguments[]);
public garageRepairConfirm(playerid, const arguments[]){
	new vehicleid,
		Float:veh_health;

	sscanf(arguments, "i", vehicleid);

	if(!IsValidVehicle(vehicleid)){
		SendErrorMessage(playerid, "Данного транспорта не существует.");
		House:HideGarageRepair(playerid);
		return false;
	}

	GetVehicleHealth(vehicleid, veh_health);
	new price = floatround((100.0 - (veh_health * 100.0) / 1000.0) * GARAGE_REPAIR_PERCENT_PRICE);

	if(GetPlayerData(playerid, p_money) < price){
		SendErrorMessage(playerid, "У Вас недостаточно наличных денежных средств.");
		return false;
	}

	GiveMoney(playerid, -price);

	RepairVehicle(vehicleid);
	SetVehicleHealth(vehicleid, 1000.0);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы починили транспорт "cl_white"\"%s\" "cl_lime"за "cl_white"%d$"cl_lime".",
		GetVehicleParamsData(GetVehicleParamIndexByModel(GetVehicleModel(vehicleid)), vp_name),
		price);
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	House:HideGarageRepair(playerid);
	return true;
}