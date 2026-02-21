
stock Taxes:ShowMainMenu(playerid){
	big_string[0] = EOS;

	format(big_string, sizeof(big_string),
		""cl_grey"Название\t"cl_grey"Сумма/процент\t"cl_grey"Тип времени\t"cl_grey"Тип налога");

	new counter;
	for(new i; i < MAX_TAXES; i++){
		if(!GetTaxData(i, t_id)) continue;

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s\t%d\t%s\t%s",
			big_string, counter + 1,
			GetTaxData(i, t_name), GetTaxData(i, t_sum),
			tax_type_time_names[GetTaxData(i, t_time_type)],
			tax_type_names[GetTaxData(i, t_type)]);

		player_listitems[playerid][counter] = i;
		counter++;
	}

	if(counter < MAX_TAXES - 1){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_lime"- Добавить новый налог", big_string);

		SetPVarInt(playerid, "Taxes_AddNewListitem", counter);
	}
	
	Dialog_Open(playerid, Dialog:D_TAXES_MENU, DIALOG_STYLE_TABLIST_HEADERS,
		"Налогообложение штата", big_string,
		"Выбрать", "Закрыть");

	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowAddNewTax(playerid){
	new temp_tax_name[64], temp_requried_level[32];
	GetPVarString(playerid, "Taxes_AddName", temp_tax_name, sizeof(temp_tax_name));

	if(GetPVarInt(playerid, "Taxes_AddRequriedLevel") == 0 || !GetPVarType(playerid, "Taxes_AddRequriedLevel")){
		format(temp_requried_level, sizeof(temp_requried_level),
			"Для всех граждан");
	}
	else{
		format(temp_requried_level, sizeof(temp_requried_level),
			"Для граждан с %d уровня",
			GetPVarInt(playerid, "Taxes_AddRequriedLevel"));
	}

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		"Параметр\tЗначение\n\
		"cl_blue"1. "cl_grey"Название налога\t"cl_white"%s\n\
		"cl_blue"2. "cl_grey"Требуемый уровень гражданина для налогообложения\t"cl_white"%s\n\
		"cl_blue"3. "cl_grey"Сумма/процент налога\t"cl_white"%d\n\
		"cl_blue"4. "cl_grey"Временной промежуток налогообложения\t"cl_white"%s\n\
		"cl_blue"5. "cl_grey"Тип налогообложения\t"cl_white"%s\n\
		"cl_blue"6. "cl_grey"Налогообложение в офлайне\t"cl_white"%s\n\
		"cl_lime"- Создать налог",
		temp_tax_name, temp_requried_level,
		GetPVarInt(playerid, "Taxes_AddSum"),
		tax_type_time_names[GetPVarInt(playerid, "Taxes_AddTimeType")],
		tax_type_names[GetPVarInt(playerid, "Taxes_AddType")],
		(GetPVarInt(playerid, "Taxes_AddIsOffline")) ? ("Да") : ("Нет"));
	
	Dialog_Open(playerid, Dialog:D_TAXES_ADD_NEW_TAX, DIALOG_STYLE_TABLIST_HEADERS,
		"Создание налога", big_string,
		"Выбрать", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowAddNewTaxName(playerid, error_text[] = ""){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите название для нового налога\n\n\
		%s",
		error_text);

	Dialog_Open(playerid, Dialog:D_TAXES_ADD_NEW_NAME, DIALOG_STYLE_INPUT,
		"Создание налога", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowAddNewTaxLevel(playerid, error_text[] = ""){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите уровень игрока, с которого будет проходить налогообложение\n\
		"cl_grey"Если хотите чтобы под налогообложение попадали все игроки, введите 0\n\n\
		%s",
		error_text);

	Dialog_Open(playerid, Dialog:D_TAXES_ADD_NEW_LEVEL, DIALOG_STYLE_INPUT,
		"Создание налога", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowAddNewTaxSum(playerid, error_text[] = ""){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Сумму или процент налога\n\n\
		%s",
		error_text);

	Dialog_Open(playerid, Dialog:D_TAXES_ADD_NEW_SUM, DIALOG_STYLE_INPUT,
		"Создание налога", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowAddNewTimeType(playerid){
	big_string[0] = EOS;
	for(new i; i < TAX_TIME_TYPES_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s",
			big_string, i + 1, tax_type_time_names[i]);
	}

	Dialog_Open(playerid, Dialog:D_TAXES_ADD_NEW_TIME_TYPE, DIALOG_STYLE_LIST,
		"Выберите период налогообложения", big_string,
		"Выбрать", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowAddNewType(playerid){
	big_string[0] = EOS;
	for(new i; i < TAX_TYPES_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s",
			big_string, i + 1, tax_type_names[i]);
	}

	Dialog_Open(playerid, Dialog:D_TAXES_ADD_NEW_TYPE, DIALOG_STYLE_LIST,
		"Выберите тип налогообложения", big_string,
		"Выбрать", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowInfo(playerid, tax_index){
	if(tax_index < 0 || tax_index >= MAX_TAXES) return false;

	new tax_requried_level_string[128];
	if(GetTaxData(tax_index, t_requried_level) == 0){
		format(tax_requried_level_string, sizeof(tax_requried_level_string),
			"Для всех граждан");
	}
	else{
		format(tax_requried_level_string, sizeof(tax_requried_level_string),
			"Для граждан с %d уровня", GetTaxData(tax_index, t_requried_level));
	}

	SetPVarInt(playerid, "Taxes_TaxIndex", tax_index);

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		"Параметр\tЗначение\n\
		"cl_blue"1. "cl_grey"Название налога\t"cl_white"%s\n\
		"cl_blue"2. "cl_grey"Требуемый уровень гражданина для налогообложения\t"cl_white"%s\n\
		"cl_blue"3. "cl_grey"Сумма/процент налога\t"cl_white"%d\n\
		"cl_blue"4. "cl_grey"Временной промежуток налогообложения\t"cl_white"%s\n\
		"cl_blue"5. "cl_grey"Тип налогообложения\t"cl_white"%s\n\
		"cl_blue"6. "cl_grey"Налогообложение в офлайне\t"cl_white"%s\n\
		"cl_dark_red"- Удалить налог",
		GetTaxData(tax_index, t_name), tax_requried_level_string,
		GetTaxData(tax_index, t_sum), tax_type_time_names[GetTaxData(tax_index, t_time_type)],
		tax_type_names[GetTaxData(tax_index, t_type)],
		(GetTaxData(tax_index, t_is_offline)) ? ("Да") : ("Нет"));

	Dialog_Open(playerid, Dialog:D_TAXES_INFO, DIALOG_STYLE_TABLIST_HEADERS,
		GetTaxData(tax_index, t_name), big_string,
		"Выбрать", "Назад");

	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowTaxChangeName(playerid, tax_index, error_text[] = ""){
	if(tax_index < 0 || tax_index >= MAX_TAXES) return false;
	if(!GetTaxData(tax_index, t_id)) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите новое название для налога\n\
		"cl_grey"Текущее название налога: "cl_white"%s\n\n%s",
		GetTaxData(tax_index, t_name), error_text);

	Dialog_Open(playerid, Dialog:D_TAXES_CHANGE_NAME, DIALOG_STYLE_INPUT,
		"Название налога", big_string,
		"Далее", "Закрыть");
	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowTaxChangeLevel(playerid, tax_index, error_text[] = ""){
	if(tax_index < 0 || tax_index >= MAX_TAXES) return false;
	if(!GetTaxData(tax_index, t_id)) return false;

	new tax_requried_level_string[128];
	if(GetTaxData(tax_index, t_requried_level) == 0){
		format(tax_requried_level_string, sizeof(tax_requried_level_string),
			"все граждане");
	}
	else{
		format(tax_requried_level_string, sizeof(tax_requried_level_string),
			"граждане, достигшие %d уровня",
			GetTaxData(tax_index, t_requried_level));
	}

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите уровень игрока, с которого будет проходить налогообложение\n\
		"cl_grey"Если Вы хотите чтобы под налогообложение попадали все игроки, введите 0\n\
		"cl_grey"Под текущий уровень налогооблажения попадают: "cl_white"%s\n\n%s",
		tax_requried_level_string, error_text);

	Dialog_Open(playerid, Dialog:D_TAXES_CHANGE_LEVEL, DIALOG_STYLE_INPUT,
		"Уровень игрока", big_string,
		"Далее", "Закрыть");
	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowTaxChangeSum(playerid, tax_index, error_text[] = ""){
	if(tax_index < 0 || tax_index >= MAX_TAXES) return false;
	if(!GetTaxData(tax_index, t_id)) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите новую сумму/процент налога\n\
		"cl_grey"Текущая сумма/процент налога: "cl_white"%s\n\n%s",
		GetTaxData(tax_index, t_sum), error_text);

	Dialog_Open(playerid, Dialog:D_TAXES_CHANGE_SUM, DIALOG_STYLE_INPUT,
		"Сумма/процент", big_string,
		"Далее", "Закрыть");
	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowTaxChangeTimeType(playerid, tax_index){
	if(tax_index < 0 || tax_index >= MAX_TAXES) return false;
	if(!GetTaxData(tax_index, t_id)) return false;

	big_string[0] = EOS;
	for(new i; i < TAX_TIME_TYPES_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s",
			big_string, i + 1,
			tax_type_time_names[i]);
	}

	Dialog_Open(playerid, Dialog:D_TAXES_CHANGE_TIME_TYPE, DIALOG_STYLE_LIST,
		"Выберите временной промежуток", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Taxes:ShowTaxChangeType(playerid, tax_index){
	if(tax_index < 0 || tax_index >= MAX_TAXES) return false;
	if(!GetTaxData(tax_index, t_id)) return false;

	big_string[0] = EOS;
	for(new i; i < TAX_TYPES_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s",
			big_string, i + 1,
			tax_type_names[i]);
	}

	Dialog_Open(playerid, Dialog:D_TAXES_CHANGE_TYPE, DIALOG_STYLE_LIST,
		"Выберите тип налогообложения", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}

//==============================================================================
stock Taxes:DeleteAddTaxPVars(playerid){
	DeletePVar(playerid, "Taxes_AddName");
	DeletePVar(playerid, "Taxes_AddRequriedLevel");
	DeletePVar(playerid, "Taxes_AddSum");
	DeletePVar(playerid, "Taxes_AddTimeType");
	DeletePVar(playerid, "Taxes_AddType");
	DeletePVar(playerid, "Taxes_AddIsOffline");
	return true;
}

stock Taxes:GiveOnlineTax(tax_index){
	if(tax_index < 0 || tax_index >= MAX_TAXES) return false;
	if(!GetTaxData(tax_index, t_id)) return false;
	if(GetTaxData(tax_index, t_is_offline)) return false;

	static const fmt_query[] = "INSERT INTO player_taxes (account_id, tax_id, sum, timestamp) VALUES (%d, %d, %d, %d)";
	new query[sizeof(fmt_query) + ((-2 + 11) * 4)];

	for(new playerid, j = GetPlayerPoolSize(), sum; playerid <= j; playerid++){
		if(!IsPlayerConnected(playerid) || !player_logged[playerid]) continue;
		if(GetPlayerData(playerid, p_level) < GetTaxData(tax_index, t_requried_level)) continue;

		switch(GetTaxData(tax_index, t_type)){
			case TAX_TYPE_DEFAULT:{
				sum = GetTaxData(tax_index, t_sum);
			}
			case TAX_TYPE_PAY_PERCENT:{
				sum = floatround(float(GetPlayerData(playerid, p_pay)) * (float(GetTaxData(tax_index, t_sum)) / 100.0));
			}
			case TAX_TYPE_VEHICLE:{
				sum = GetPlayerVehiclesCount(playerid) * GetTaxData(tax_index, t_sum);
			}
		}

		if(!sum) continue;

		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetPlayerData(playerid, p_id), GetTaxData(tax_index, t_id), sum, gettime());
		mysql_tquery(mysql, query);

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Вам начислен %s в размере %d$. Для оплаты используйте '/mytaxes'.",
			GetTaxData(tax_index, t_name), sum);
		SendClientMessage(playerid, COLOR_YELLOW, small_string);
		small_string[0] = EOS;
	}
	return true;
}

stock Taxes:GiveOfflineTax(tax_index){
	if(tax_index < 0 || tax_index >= MAX_TAXES) return false;
	if(!GetTaxData(tax_index, t_id)) return false;
	if(!GetTaxData(tax_index, t_is_offline)) return false;

	static const fmt_query[] = "SELECT id, pay FROM accounts WHERE level >= %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetTaxData(tax_index, t_requried_level));
	mysql_tquery(mysql, query, "GiveOfflineTax", "i", tax_index);
	return true;
}