
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