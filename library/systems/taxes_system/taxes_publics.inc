
forward LoadTaxes(tax_index);
public LoadTaxes(tax_index){
	new rows, 
		tick_time = GetTickCount(),
		old_tax_index = tax_index;

	cache_get_row_count(rows);

	if(rows >= MAX_TAXES){
		printf("[LoadTaxes - ERROR]: Количество записей в базе данных превышает размер массива (%d >= %d)", rows, MAX_TAXES);
		return false;
	}

	for(new i; i < rows; i++){
		if(old_tax_index == -1){
			tax_index = i;
		}

		cache_get_value_name_int(i, "id", GetTaxData(tax_index, t_id));
		cache_get_value_name(i, "name", GetTaxData(tax_index, t_name));
		cache_get_value_name_int(i, "requried_level", GetTaxData(tax_index, t_requried_level));
		cache_get_value_name_int(i, "sum", GetTaxData(tax_index, t_sum));
		cache_get_value_name_int(i, "time_type", GetTaxData(tax_index, t_time_type));
		cache_get_value_name_int(i, "type", GetTaxData(tax_index, t_type));
		cache_get_value_name_int(i, "is_offline", GetTaxData(tax_index, t_is_offline));
	}

	if(old_tax_index == -1){
		printf("[LoadTaxes]: Налоги загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
		load_info[loaded_taxes] = true;
	}
	return true;
}

forward CreateNewTax(playerid);
public CreateNewTax(playerid){
	new insert_id = cache_insert_id(),
		free_tax_index = -1;

	for(new i; i < MAX_TAXES; i++){
		if(GetTaxData(i, t_id) > 0) continue;
		free_tax_index = i;
		break;
	}

	if(free_tax_index == -1){
		SendErrorMessage(playerid, "В данный момент на сервере максимальное количество налогов.");

		static const fmt_query[] = "DELETE FROM taxes WHERE id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, insert_id);
		mysql_tquery(mysql, query);
	}
	else{
		new temp_tax_name[64];
		GetPVarString(playerid, "Taxes_AddName", temp_tax_name, sizeof(temp_tax_name));

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"%s %s ввёл(-а) новый налог: %s",
			fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
			GetPlayerData(playerid, p_name), temp_tax_name);
		SendClientMessageToAll(COLOR_YELLOW, small_string);
		small_string[0] = EOS;

		static const fmt_query[] = "SELECT * FROM taxes WHERE id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, insert_id);
		mysql_tquery(mysql, query, "LoadTaxes", "i", free_tax_index);
	}

	Taxes:DeleteAddTaxPVars(playerid);
	return true;
}

forward GiveOfflineTax(tax_index);
public GiveOfflineTax(tax_index){
	if(tax_index < 0 || tax_index >= MAX_TAXES) return false;
	if(!GetTaxData(tax_index, t_id)) return false;
	if(!GetTaxData(tax_index, t_is_offline)) return false;

	new rows;
	cache_get_row_count(rows);

	static const fmt_query[] = "INSERT INTO player_taxes (account_id, tax_id, sum, timestamp) VALUES (%d, %d, %d, %d)";
	new query[sizeof(fmt_query) + ((-2 + 11) * 4)];

	for(new i, temp_id, temp_pay, sum; i < rows; i++){
		cache_get_value_name_int(i, "id", temp_id);
		cache_get_value_name_int(i, "pay", temp_pay);

		switch(GetTaxData(tax_index, t_type)){
			case TAX_TYPE_DEFAULT:{
				sum = GetTaxData(tax_index, t_sum);
			}
			case TAX_TYPE_PAY_PERCENT:{
				sum = floatround(float(temp_pay) * (float(GetTaxData(tax_index, t_sum)) / 100.0));
			}
			case TAX_TYPE_VEHICLE:{
				sum = GetPlayerOfflineVehiclesCount(temp_id) * GetTaxData(tax_index, t_sum);
			}
		}

		if(!sum) continue;

		mysql_format(mysql, query, sizeof(query), fmt_query,
			temp_id, GetTaxData(tax_index, t_id), sum, gettime());
		mysql_tquery(mysql, query);

		if(GetPlayerIdBySqlId(temp_id) != INVALID_PLAYER_ID){
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"Вам начислен %s в размере %d$. Для оплаты используйте '/mytaxes'.",
				GetTaxData(tax_index, t_name), sum);
			SendClientMessage(GetPlayerIdBySqlId(temp_id), COLOR_YELLOW, small_string);
			small_string[0] = EOS;
		}
	}
	return true;
}

//==============================================================================
forward ShowPlayerTaxes(playerid);
public ShowPlayerTaxes(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows){
		SendErrorMessage(playerid, "У Вас нет неоплаченных налогов.");
		return false;
	}

	new temp_day, temp_month, temp_year, temp_hour, temp_minute;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		"Название налога\tСумма налога\tСрок оплаты\n\
		"cl_lime"- Оплатить все налоги");

	for(new i, temp_tax_id, temp_sum, temp_timestamp, temp_tax_index; i < rows; i++){
		cache_get_value_name_int(i, "id", player_listitems[playerid][i]);
		cache_get_value_name_int(i, "tax_id", temp_tax_id);
		cache_get_value_name_int(i, "sum", temp_sum);
		cache_get_value_name_int(i, "timestamp", temp_timestamp);

		for(new tax_index; tax_index < MAX_TAXES; tax_index++){
			if(!GetTaxData(tax_index, t_id)) continue;
			if(GetTaxData(tax_index, t_id) != temp_tax_id) continue;
			temp_tax_index = tax_index;
			break;
		}

		ConvertTime(temp_timestamp + TAX_PAYMENT_TIME, temp_year, temp_month, temp_day, temp_hour, temp_minute);

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s\t"cl_white"%d$\tдо %02d.%02d.%d %02d:%02d%s",
			big_string, i + 1,
			GetTaxData(temp_tax_index, t_name), temp_sum,
			temp_day, temp_month, temp_year, temp_hour, temp_minute,
			(temp_timestamp + TAX_PAYMENT_TIME < gettime()) ? (" "cl_light_red"(ПРОСРОЧЕН)") : (""));
	}

	Dialog_Open(playerid, Dialog:D_MYTAXES, DIALOG_STYLE_TABLIST_HEADERS,
		"Мои налоги", big_string,
		"Оплатить", "Закрыть");

	big_string[0] = EOS;
	return true;
}

forward PayPlayerTax(playerid);
public PayPlayerTax(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows || rows > 1){
		SendErrorMessage(playerid, "Произошла ошибка.");
		return false;
	}

	new temp_id, temp_tax_id, temp_sum, 
		temp_tax_index = -1;

	cache_get_value_name_int(0, "id", temp_id);
	cache_get_value_name_int(0, "tax_id", temp_tax_id);
	cache_get_value_name_int(0, "sum", temp_sum);

	for(new i; i < MAX_TAXES; i++){
		if(!GetTaxData(i, t_id)) continue;
		if(GetTaxData(i, t_id) != temp_tax_id) continue;
		temp_tax_index = i;
		break;
	}

	if(temp_tax_index == -1){
		SendErrorMessage(playerid, "Произошла ошибка.");
		return false;
	}

	if(!GetPlayerBankCardData(playerid, pbc_id) || GetPlayerBankCardData(playerid, pbc_balance) < temp_sum){
		if(GetPlayerData(playerid, p_money) < temp_sum){
			SendErrorMessage(playerid, "У Вас недостаточно средств для оплаты налога.");
			return false;
		}
		else{
			GiveMoney(playerid, -temp_sum);
		}
	}
	else{
		Bank:GiveMoney(playerid, -temp_sum);
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы оплатили %s в размере %d$.",
		GetTaxData(temp_tax_index, t_name), temp_sum);
	SendClientMessage(playerid, COLOR_GREEN, small_string);
	small_string[0] = EOS;

	static const fmt_query[] = "DELETE FROM player_taxes WHERE id = %d AND account_id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		temp_id, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);

	return true;
}

forward PayAllPlayerTaxes(playerid);
public PayAllPlayerTaxes(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows){
		SendErrorMessage(playerid, "Произошла ошибка.");
	}

	new temp_sum, total_sum;

	for(new i; i < rows; i++){
		cache_get_value_name_int(i, "sum", temp_sum);
		total_sum += temp_sum;
	}

	if(!GetPlayerBankCardData(playerid, pbc_id) || GetPlayerBankCardData(playerid, pbc_balance) < total_sum){
		if(GetPlayerData(playerid, p_money) < total_sum){
			SendErrorMessage(playerid, "У Вас недостаточно средств для оплаты налога.");
			return false;
		}
		else{
			GiveMoney(playerid, -total_sum);
		}
	}
	else{
		Bank:GiveMoney(playerid, -total_sum);
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы оплатили все налоги в размере %d$.",
		total_sum);
	SendClientMessage(playerid, COLOR_GREEN, small_string);
	small_string[0] = EOS;

	static const fmt_query[] = "DELETE FROM player_taxes WHERE account_id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);

	return true;
}

forward TaxWantedPlayer(playerid);
public TaxWantedPlayer(playerid){
	new rows;
	cache_get_row_count(rows);

	if(GetPlayerData(playerid, p_wanted) + rows > 6){
		rows = 6 - GetPlayerData(playerid, p_wanted);
	}

	AddPlayerData(playerid, p_wanted, rows);
	SetPlayerWantedLevel(playerid, GetPlayerData(playerid, p_wanted));

	static const fmt_query[] = "UPDATE accounts SET wanted = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_wanted), GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);

	SendClientMessage(playerid, COLOR_DARK_RED, "Вы объявлены в розыск за неуплату налогов!");
	return true;
}