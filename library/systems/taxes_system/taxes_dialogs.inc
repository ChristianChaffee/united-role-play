
DialogResponse:D_TAXES_MENU(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Taxes_AddNewListitem");
		return false;
	}

	if(GetPVarType(playerid, "Taxes_AddNewListitem") && listitem == GetPVarInt(playerid, "Taxes_AddNewListitem")){
		DeletePVar(playerid, "Taxes_AddNewListitem");
		Taxes:ShowAddNewTax(playerid);
		return true;
	}

	Taxes:ShowInfo(playerid, player_listitems[playerid][listitem]);
	DeletePVar(playerid, "Taxes_AddNewListitem");
	return true;
}

//==============================================================================
DialogResponse:D_TAXES_ADD_NEW_TAX(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:DeleteAddTaxPVars(playerid);
		callcmd::taxes(playerid);
		return false;
	}
	switch(listitem){
		case 0: Taxes:ShowAddNewTaxName(playerid);
		case 1: Taxes:ShowAddNewTaxLevel(playerid);
		case 2: Taxes:ShowAddNewTaxSum(playerid);
		case 3: Taxes:ShowAddNewTimeType(playerid);
		case 4: Taxes:ShowAddNewType(playerid);
		case 5:{
			if(GetPVarType(playerid, "Taxes_AddIsOffline")){
				DeletePVar(playerid, "Taxes_AddIsOffline");
			}
			else{
				SetPVarInt(playerid, "Taxes_AddIsOffline", 1);
			}
			Taxes:ShowAddNewTax(playerid);
		}
		case 6:{
			if(!GetPVarType(playerid, "Taxes_AddName")){
				SendErrorMessage(playerid, "Введите название налога.");
				Taxes:ShowAddNewTax(playerid);
				return false;
			}
			if(!GetPVarType(playerid, "Taxes_AddSum")){
				SendErrorMessage(playerid, "Введите сумму/процент налога.");
				Taxes:ShowAddNewTax(playerid);
				return false;
			}

			new temp_tax_name[64];
			GetPVarString(playerid, "Taxes_AddName", temp_tax_name, sizeof(temp_tax_name));
			FixSVarString(temp_tax_name);

			static const fmt_query[] = "INSERT INTO taxes (name, requried_level, sum, time_type, type, is_offline) \
				VALUES ('%s', %d, %d, %d, %d, %d)";
			
			new query[sizeof(fmt_query) + (-2 + 64) + ((-2 + 11) * 5)];
			mysql_format(mysql, query, sizeof(query), fmt_query,
				temp_tax_name, GetPVarInt(playerid, "Taxes_AddRequriedLevel"),
				GetPVarInt(playerid, "Taxes_AddSum"), GetPVarInt(playerid, "Taxes_AddTimeType"),
				GetPVarInt(playerid, "Taxes_AddType"), GetPVarInt(playerid, "Taxes_AddIsOffline"));
			mysql_tquery(mysql, query, "CreateNewTax", "i", playerid);
		}
	}
	return true;
}

DialogResponse:D_TAXES_ADD_NEW_NAME(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:ShowAddNewTax(playerid);
		return false;
	}
	if(strlen(inputtext) < 5 || strlen(inputtext) >= 64){
		Taxes:ShowAddNewTaxName(playerid, ""cl_grey"Ошибка: "cl_white"название налога должно быть от 5 до 64 символов.");
		return false;
	}

	SetPVarString(playerid, "Taxes_AddName", inputtext);
	Taxes:ShowAddNewTax(playerid);
	return true;
}

DialogResponse:D_TAXES_ADD_NEW_LEVEL(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:ShowAddNewTax(playerid);
		return false;
	}
	new temp_requried_level;
	if(sscanf(inputtext, "i", temp_requried_level)){
		Taxes:ShowAddNewTaxLevel(playerid, ""cl_grey"Ошибка: "cl_white"Корректно введите уровень игрока.");
		return false;
	}
	if(temp_requried_level < 0){
		Taxes:ShowAddNewTaxLevel(playerid, ""cl_grey"Ошибка: "cl_white"Уровень игрока не может быть меньше 0.");
		return false;
	}

	SetPVarInt(playerid, "Taxes_AddRequriedLevel", temp_requried_level);
	Taxes:ShowAddNewTax(playerid);
	return true;
}

DialogResponse:D_TAXES_ADD_NEW_SUM(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:ShowAddNewTax(playerid);
		return false;
	}
	new temp_sum;
	if(sscanf(inputtext, "i", temp_sum)){
		Taxes:ShowAddNewTaxSum(playerid, ""cl_grey"Ошибка: "cl_white"Корректно введите сумму/процент.");
		return false;
	}
	if(temp_sum <= 0){
		Taxes:ShowAddNewTaxSum(playerid, ""cl_grey"Ошибка: "cl_white"Сумма налога должна быть больше 0.");
		return false;
	}
	if(temp_sum > 100 && GetPVarType(playerid, "Taxes_AddType") && GetPVarInt(playerid, "Taxes_AddType") == TAX_TYPE_PAY_PERCENT){
		Taxes:ShowAddNewTaxSum(playerid, ""cl_grey"Ошибка: "cl_white"Сумма процентного налога не должна быть больше 100.");
		return false;
	}

	SetPVarInt(playerid, "Taxes_AddSum", temp_sum);
	Taxes:ShowAddNewTax(playerid);
	return true;
}

DialogResponse:D_TAXES_ADD_NEW_TIME_TYPE(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:ShowAddNewTax(playerid);
		return false;
	}

	SetPVarInt(playerid, "Taxes_AddTimeType", listitem);
	Taxes:ShowAddNewTax(playerid);
	return true;
}

DialogResponse:D_TAXES_ADD_NEW_TYPE(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:ShowAddNewTax(playerid);
		return false;
	}
	if(listitem == TAX_TYPE_PAY_PERCENT && GetPVarType(playerid, "Taxes_AddSum") && GetPVarInt(playerid, "Taxes_AddSum") > 100){
		SetPVarInt(playerid, "Taxes_AddSum", 100);
		SendErrorMessage(playerid, "Процент налога понижен до 100.");
	}

	SetPVarInt(playerid, "Taxes_AddType", listitem);
	Taxes:ShowAddNewTax(playerid);
	return true;
}

//==============================================================================
DialogResponse:D_TAXES_INFO(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Taxes_TaxIndex");
		Taxes:ShowMainMenu(playerid);
		return false;
	}
	switch(listitem){
		case 0: Taxes:ShowTaxChangeName(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		case 1: Taxes:ShowTaxChangeLevel(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		case 2: Taxes:ShowTaxChangeSum(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		case 3: Taxes:ShowTaxChangeTimeType(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		case 4: Taxes:ShowTaxChangeType(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		case 5:{
			SetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_is_offline, !GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_is_offline));

			static const fmt_query[] = "UPDATE taxes SET is_offline = %d WHERE id = %d";
			new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
			mysql_format(mysql, query, sizeof(query), fmt_query,
				GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_is_offline),
				GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_id));
			mysql_tquery(mysql, query);

			Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		}
		case 6:{
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"%s %s удалил(-а) налог \"%s\".",
				fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
				GetPlayerData(playerid, p_name),
				GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_name));
			SendClientMessageToAll(COLOR_YELLOW, small_string);
			small_string[0] = EOS;

			static const fmt_query[] = "DELETE FROM taxes WHERE id = %d";
			new query[sizeof(fmt_query) + (-2 + 11)];
			mysql_format(mysql, query, sizeof(query), fmt_query, GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_id));
			mysql_tquery(mysql, query);

			tax_info[GetPVarInt(playerid, "Taxes_TaxIndex")] = tax_info_default;

			DeletePVar(playerid, "Taxes_TaxIndex");
		}
	}
	return true;
}

DialogResponse:D_TAXES_CHANGE_NAME(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		return false;
	}
	if(strlen(inputtext) < 5 || strlen(inputtext) >= 64){
		Taxes:ShowTaxChangeName(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"), ""cl_grey"Ошибка: "cl_white"название налога должно быть от 5 до 64 символов.");
		return false;
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"%s %s изменил(-а) название налога \"%s\" на \"%s\".",
		fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
		GetPlayerData(playerid, p_name), GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_name),
		inputtext);
	SendClientMessageToAll(COLOR_YELLOW, small_string);
	small_string[0] = EOS;

	format(GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_name), 64, "%s", inputtext);

	static const fmt_query[] = "UPDATE taxes SET name = '%s' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 64) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_name),
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_id));
	mysql_tquery(mysql, query);

	Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
	return true;
}

DialogResponse:D_TAXES_CHANGE_LEVEL(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		return false;
	}
	new tax_level;
	if(sscanf(inputtext, "i", tax_level)){
		Taxes:ShowTaxChangeLevel(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"), ""cl_grey"Ошибка: "cl_white"корректно введите уровень");
		return false;
	}
	if(tax_level < 0){
		Taxes:ShowTaxChangeLevel(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"), ""cl_grey"Ошибка: "cl_white"уровень не может быть меньше 0");
		return false;
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[R] %s %s[%d] измиенил(-а) уровень налогообложения с %d на %d для налога \"%s\".",
		fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
		GetPlayerData(playerid, p_name), playerid,
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_requried_level), tax_level,
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_name));
	Fraction:SendMessage(GetPlayerFractionIndex(playerid), COLOR_YELLOW, small_string);
	small_string[0] = EOS;

	SetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_requried_level, tax_level);

	static const fmt_query[] = "UPDATE taxes SET requried_level = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_requried_level),
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_id));
	mysql_tquery(mysql, query);

	Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
	return true;
}

DialogResponse:D_TAXES_CHANGE_SUM(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		return false;
	}
	new tax_sum;
	if(sscanf(inputtext, "i", tax_sum)){
		Taxes:ShowTaxChangeSum(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"), ""cl_grey"Ошибка: "cl_white"корректно введите сумму/процент налога");
		return false;
	}
	if(tax_sum < 0){
		Taxes:ShowTaxChangeSum(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"), ""cl_grey"Ошибка: "cl_white"сумма/процент налога не может быть меньше или равен 0");
		return false;
	}
	if(GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_type) == TAX_TYPE_PAY_PERCENT && tax_sum > 100){
		Taxes:ShowTaxChangeSum(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"), ""cl_grey"Ошибка: "cl_white"для процентного налога процент не может быть больше 100");
		return false;
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"%s %s изменил(-а) сумму/процент налога с %d до %d для налога \"%s\".",
		fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
		GetPlayerData(playerid, p_name), GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_sum),
		tax_sum, GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_name));
	SendClientMessageToAll(COLOR_YELLOW, small_string);
	small_string[0] = EOS;

	SetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_sum, tax_sum);

	static const fmt_query[] = "UPDATE taxes SET sum = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_sum),
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_id));
	mysql_tquery(mysql, query);

	Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
	return true;
}

DialogResponse:D_TAXES_CHANGE_TIME_TYPE(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		return false;
	}
	if(listitem == GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_time_type)){
		SendErrorMessage(playerid, "Выберите временной промежуток, отличный от текущего.");
		Taxes:ShowTaxChangeTimeType(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		return false;
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[R] %s %s[%d] изменил(-а) временной промежуток налогообложения с %s на %s для налога \"%s\".",
		fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
		GetPlayerData(playerid, p_name), playerid,
		tax_type_time_names[GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_time_type)],
		tax_type_time_names[listitem],
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_name));
	Fraction:SendMessage(GetPlayerFractionIndex(playerid), COLOR_YELLOW, small_string);
	small_string[0] = EOS;

	SetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_time_type, listitem);

	static const fmt_query[] = "UPDATE taxes SET time_type = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_time_type), GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_id));
	mysql_query(mysql, query);

	Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
	return true;
}

DialogResponse:D_TAXES_CHANGE_TYPE(playerid, response, listitem, inputtext[]){
	if(!response){
		Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		return false;
	}
	if(listitem == GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_type)){
		SendErrorMessage(playerid, "Выберите тип налогообложения, отличный от текущего.");
		Taxes:ShowTaxChangeType(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		return false;
	}
	if(listitem == TAX_TYPE_PAY_PERCENT && GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_sum) > 100){
		SendErrorMessage(playerid, "Сначала измените процент налога. Для процентного налога он не может быть больше 100.");
		Taxes:ShowTaxChangeType(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
		return false;
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[R] %s %s[%d] изменил(-а) тип налога с %s на %s для налога \"%s\".",
		fraction_ranks[GetPlayerFractionIndex(playerid)][GetPlayerData(playerid, p_fraction_rank) - 1],
		GetPlayerData(playerid, p_name), playerid,
		tax_type_names[GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_type)],
		tax_type_names[listitem],
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_name));
	Fraction:SendMessage(GetPlayerFractionIndex(playerid), COLOR_YELLOW, small_string);
	small_string[0] = EOS;

	SetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_type, listitem);

	static const fmt_query[] = "UPDATE taxes SET type = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_type),
		GetTaxData(GetPVarInt(playerid, "Taxes_TaxIndex"), t_id));
	mysql_tquery(mysql, query);

	Taxes:ShowInfo(playerid, GetPVarInt(playerid, "Taxes_TaxIndex"));
	return false;
}

//==============================================================================
DialogResponse:D_MYTAXES(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	if(listitem == 0){
		static const fmt_query[] = "SELECT * FROM player_taxes WHERE account_id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id));
		mysql_tquery(mysql, query, "PayAllPlayerTaxes", "i", playerid);
	}
	else{
		static const fmt_query[] = "SELECT * FROM player_taxes WHERE id = %d AND account_id = %d LIMIT 1";
		new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			player_listitems[playerid][listitem - 1], GetPlayerData(playerid, p_id));
		mysql_tquery(mysql, query, "PayPlayerTax", "i", playerid);
	}
	return true;
}