
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

	#warning DEBUG
	printf("Phone_BusinessDeposit");

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

	#warning DEBUG
	printf("Phone_BusinessWithdraw");

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