
forward Phone_Call(playerid, const arguments[]);
public Phone_Call(playerid, const arguments[]){
	new number = -1;
	if(strfind(arguments, "*") == -1 && strfind(arguments, "#") == -1){
		sscanf(arguments, "i", number);
	}

	if(number == -1){
		Phone:ShowError(playerid, "Корректно введите номер.");
		return false;
	}

	new scdplayerid = GetPlayerIdByPhoneNumber(number);

	if(scdplayerid == INVALID_PLAYER_ID){
		Phone:ShowError(playerid, "Абонент вне зоны действия сети!");
		return false;
	}
	if(scdplayerid == playerid){
		Phone:ShowError(playerid, "Вы не можете позвонить самому себе.");
		return false;
	}
	if(GetPVarType(scdplayerid, "Phone_CallID")){
		Phone:ShowError(playerid, "Абонент занят.");
		return false;
	}
	if(GetPlayerCallerId(playerid) != INVALID_PLAYER_ID){
		Phone:ShowError(playerid, "У Вас уже есть активный вызов.");
		return false;
	}

	SetPVarInt(playerid, "Phone_CallID", scdplayerid);

	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
	SendMeAction(playerid, "звонит по телефону.");

	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: Чтобы сбросить трубку введите '/h' или откройте телефон с помощью '/phone'.");
	SendClientMessage(playerid, COLOR_LIME, "У Вас звонит телефон: "cl_green"'/p' "cl_white"- поднять трубку"cl_lime", "cl_light_red"'/h' "cl_white"- сбросить вызов"cl_lime".");
	
	cef_emit_event(playerid, "startCallInterface",
		CEFSTR(GetPlayerData(scdplayerid, p_name)));

	return true;
}

forward Phone_CallHangup(playerid, const arguments[]);
public Phone_CallHangup(playerid, const arguments[]){
	new scdplayerid = GetPlayerCallerId(playerid);
	
	if(scdplayerid != INVALID_PLAYER_ID){
		SendClientMessage(scdplayerid, COLOR_GREY, "Абонент сбросил трубку.");
		Phone:StopCall(scdplayerid);
	}

	SendClientMessage(playerid, COLOR_GREY, "Вы сбросили трубку.");
	Phone:StopCall(playerid);
	return true;
}

forward Phone_BankTransfer(playerid, const arguments[]);
public Phone_BankTransfer(playerid, const arguments[]){
	new scdplayerid, 
		sum;

	sscanf(arguments, "ii", scdplayerid, sum);

	new result = Bank:TransferMoney(playerid, scdplayerid, sum, true);
	if(result) Phone:HidePhone(playerid);
	return result;
}

forward Phone_BusinessDeposit(playerid, const arguments[]);
public Phone_BusinessDeposit(playerid, const arguments[]){
	new business_index,
		amount,
		business_interface_index;

	sscanf(arguments, "ii", business_index, amount);
	business_interface_index = business_index;
	business_index = player_listitems[playerid][business_index];

	if(GetBusinessData(business_index, b_owner_id) != GetPlayerData(playerid, p_id)){
		Phone:ShowError(playerid, "Вы не владеете данным бизнесом.");
		return false;
	}
	if(GetPlayerBankCardData(playerid, pbc_balance) < amount){
		Phone:ShowError(playerid, "У Вас недостаточно средств на банковском счёте.");
		return false;
	}

	Bank:GiveMoney(playerid, -amount);

	AddBusinessData(business_index, b_balance, amount);
	static const fmt_query[] = "UPDATE business SET balance = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetBusinessData(business_index, b_balance), GetBusinessData(business_index, b_id));
	mysql_tquery(mysql, query);

	cef_emit_event(playerid, "setPhoneBankData",
		CEFINT(GetPlayerBankCardData(playerid, pbc_balance)));

	cef_emit_event(playerid, "updateBusinessBalance",
		CEFINT(business_interface_index),
		CEFINT(GetBusinessData(business_index, b_balance)));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы пополнили баланс бизнеса "cl_white"\"%s\" "cl_green"на "cl_white"%d$"cl_green".",
		GetBusinessData(business_index, b_name), amount);
	SendClientMessage(playerid, COLOR_GREEN, small_string);
	small_string[0] = EOS;
	return true;
}

forward Phone_BusinessWithdraw(playerid, const arguments[]);
public Phone_BusinessWithdraw(playerid, const arguments[]){
	new business_index,
		amount,
		business_interface_index;

	sscanf(arguments, "ii", business_index, amount);
	business_interface_index = business_index;
	business_index = player_listitems[playerid][business_index];

	if(GetBusinessData(business_index, b_owner_id) != GetPlayerData(playerid, p_id)){
		Phone:ShowError(playerid, "Вы не владеете данным бизнесом.");
		return false;
	}
	if(GetBusinessData(business_index, b_balance) < amount){
		Phone:ShowError(playerid, "У Вас недостаточно средств на балансе бизнеса.");
		return false;
	}

	Bank:GiveMoney(playerid, amount);

	AddBusinessData(business_index, b_balance, -amount);
	static const fmt_query[] = "UPDATE business SET balance = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetBusinessData(business_index, b_balance), GetBusinessData(business_index, b_id));
	mysql_tquery(mysql, query);

	cef_emit_event(playerid, "setPhoneBankData",
		CEFINT(GetPlayerBankCardData(playerid, pbc_balance)));

	cef_emit_event(playerid, "updateBusinessBalance",
		CEFINT(business_interface_index),
		CEFINT(GetBusinessData(business_index, b_balance)));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы сняли с баланса бизнеса "cl_white"\"%s\" "cl_green"сумму "cl_white"%d$"cl_green".",
		GetBusinessData(business_index, b_name), amount);
	SendClientMessage(playerid, COLOR_GREEN, small_string);
	small_string[0] = EOS;
	return true;
}

forward Phone_Close(playerid, const arguments[]);
public Phone_Close(playerid, const arguments[]){
	DeletePVar(playerid, "Phone_Show");
	return true;
}

forward Phone_VehicleLoad(playerid, const arguments[]);
public Phone_VehicleLoad(playerid, const arguments[]){
	new selected_vehicle_id;
	sscanf(arguments, "i", selected_vehicle_id);
	selected_vehicle_id = phone_vehicle_listitems[playerid][selected_vehicle_id];

	static const fmt_query[] = "SELECT * FROM vehicles WHERE id = %d AND owner = %d AND status = %d LIMIT 1";
	new query[sizeof(fmt_query) + ((-2 + 11) * 3)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		selected_vehicle_id, GetPlayerData(playerid, p_id), VEHICLE_STATUS_PLAYER);
	mysql_tquery(mysql, query, "CheckPlayerVehicle", "i", playerid);
	return true;
}

forward Phone_VehicleUnload(playerid, const arguments[]);
public Phone_VehicleUnload(playerid, const arguments[]){
	new selected_vehicle_id;
	sscanf(arguments, "i", selected_vehicle_id);
	selected_vehicle_id = phone_vehicle_listitems[playerid][selected_vehicle_id];

	new vehicleid = FindVehicleById(selected_vehicle_id);

	if(vehicleid == INVALID_VEHICLE_ID){
		Phone:ShowError(playerid, "Данного транспорта не существует.");
		return false;
	}
	if(GetVehicleData(vehicleid, v_owner) != GetPlayerData(playerid, p_id)){
		SendErrorMessage(playerid, "Данный транспорт принадлежит не Вам.");
		Phone:HidePhone(playerid);
		return false;
	}

	SendClientMessage(playerid, COLOR_LIGHT_RED, "Вы выгрузили транспорт с сервера.");
	DestroyVehicle(vehicleid);

	sscanf(arguments, "i", selected_vehicle_id);
	cef_emit_event(playerid, "updateVehicleStatus",
		CEFINT(selected_vehicle_id),
		CEFINT(0));

	Phone:HidePhone(playerid);
	return true;
}

forward Phone_BankHousePay(playerid, const arguments[]);
public Phone_BankHousePay(playerid, const arguments[]){
	Phone:HidePhone(playerid);

	new days_count, house_index;
	sscanf(arguments, "ii", days_count, house_index);

	if(house_index < 0 || house_index >= MAX_HOUSES){
		SendErrorMessage(playerid, "Произошла ошибка.");
		return false;
	}
	if(GetHouseData(house_index, h_owner_id) != GetPlayerData(playerid, p_id)){
		SendErrorMessage(playerid, "Вы не владелец этого дома.");
		return false;
	}
	if(days_count + GetHouseData(house_index, h_payment_days) > 30){
		SendErrorMessage(playerid, "Произошла ошибка.");
		return false;
	}
	if(GetPlayerBankCardData(playerid, pbc_balance) < days_count * house_class_payment_price[GetHouseData(house_index, h_class)]){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Для оплаты дома на %d дней необходимо %d$ на банковской карте.", 
			days_count,
			days_count * house_class_payment_price[GetHouseData(house_index, h_class)]);
		SendErrorMessage(playerid, small_string);
		small_string[0] = EOS;
		return false;
	}

	Bank:GiveMoney(playerid, -(days_count * house_class_payment_price[GetHouseData(house_index, h_class)]));

	AddHouseData(house_index, h_payment_days, days_count);
	static const fmt_query[] = "UPDATE houses SET payment_days = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetHouseData(house_index, h_payment_days),
		GetHouseData(house_index, h_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы оплатили дом №%d на %d дней(-я).",
		GetHouseData(house_index, h_id), days_count);
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	return true;
}