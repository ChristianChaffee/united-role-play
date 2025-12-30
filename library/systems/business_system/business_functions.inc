
stock Business:ShowMenu(playerid, business_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;

	switch(GetBusinessData(business_index, b_type)){
		case BUSINESS_TYPE_PIZZA:{
			big_string[0] = EOS;
			format(big_string, sizeof(big_string),
				""cl_grey"Название\t"cl_grey"Цена");

			new counter;
			for(new i; i < MAX_BUSINESS_MENU_POSITIONS; i++){
				if(!strcmp(GetBusinessMenuData(GetBusinessData(business_index, b_type), i, bm_name), "-")) continue;
				
				format(big_string, sizeof(big_string),
					"%s\n\
					"cl_lime"%d. "cl_white"%s\t%d$",
					big_string, counter + 1,
					GetBusinessMenuData(GetBusinessData(business_index, b_type), i, bm_name),
					business_info[business_index][b_products_prices][i]);

				player_listitems[playerid][counter] = i;
				counter++;
			}

			player_listitems[playerid][counter] = PLAYER_PAYMENT_METHOD_LISTITEM;

			format(big_string, sizeof(big_string),
				"%s\n\
				"cl_grey"[Способ оплаты]\t"cl_white"%s",
				big_string,
				!GetPVarType(playerid, "PaymentMethodCard") ? ("Наличные") : ("Банковская карта"));

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				""cl_lime"%s",
				GetBusinessData(business_index, b_name));

			Dialog_Open(playerid, Dialog:D_BUSINESS_MENU, DIALOG_STYLE_TABLIST_HEADERS,
				small_string, big_string,
				"Выбарть", "Закрыть");

			big_string[0] = EOS;
			small_string[0] = EOS;
		}
	}
	return true;
}

stock Business:ShowInfoMenu(playerid){
	Dialog_Open(playerid, Dialog:D_BUSINESS_INFO_MENU, DIALOG_STYLE_LIST,
		"Информация о бизнесах", 
		""cl_blue"- "cl_white"Список всех бизнесов\n\
		"cl_blue"- "cl_white"Список всех свободных бизнесов",
		"Выбрать", "Закрыть");
	return true;
}

stock Business:ShowInfoAllBusinesses(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Название\t"cl_grey"Тип бизнеса\t"cl_grey"Владелец\t"cl_grey"Гос. стоимость");

	new counter;
	for(new i, owner_str[64]; i < MAX_BUSINESS; i++){
		if(!GetBusinessData(i, b_id)) continue;

		if(!GetBusinessData(i, b_owner_id)){
			format(owner_str, sizeof(owner_str), ""cl_green"Владелец отсутствует");
		}
		else format(owner_str, sizeof(owner_str), ""cl_white"%s", GetNameBySqlID(GetBusinessData(i, b_owner_id)));

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s\t"cl_white"%s\t%s\t"cl_white"%d$",
			big_string, counter + 1,
			GetBusinessData(i, b_name), business_type_names[GetBusinessData(i, b_type)],
			owner_str, GetBusinessData(i, b_price));

		player_listitems[playerid][counter] = i;
		counter++;
	}

	Dialog_Open(playerid, Dialog:D_BUSINESS_INFO_LIST, DIALOG_STYLE_TABLIST_HEADERS,
		"Список всех бизнесов", big_string,
		"Выбрать", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Business:ShowInfoFreeBusinesses(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Название\t"cl_grey"Тип бизнеса\t"cl_grey"Владелец\t"cl_grey"Гос. стоимость");

	new counter;
	for(new i; i < MAX_BUSINESS; i++){
		if(!GetBusinessData(i, b_id)) continue;
		if(GetBusinessData(i, b_owner_id) > 0) continue;

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s\t"cl_white"%s\t"cl_green"Владелец отсутствует\t"cl_white"%d$",
			big_string, counter + 1,
			GetBusinessData(i, b_name), business_type_names[GetBusinessData(i, b_type)], GetBusinessData(i, b_price));

		player_listitems[playerid][counter] = i;
		counter++;
	}

	if(!counter){
		SendErrorMessage(playerid, "В данный момент нет свободных бизнесов.");
		Business:ShowInfoMenu(playerid);
		return false;
	}

	Dialog_Open(playerid, Dialog:D_BUSINESS_INFO_LIST, DIALOG_STYLE_TABLIST_HEADERS,
		"Список свободных бизнесов", big_string,
		"Выбрать", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Business:ShowInfo(playerid, business_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;

	new owner_str[32];
	if(!GetBusinessData(business_index, b_owner_id)){
		format(owner_str, sizeof(owner_str), ""cl_green"Владелец отсутствует");
	}
	else format(owner_str, sizeof(owner_str), ""cl_white"%s", GetNameBySqlID(GetBusinessData(business_index, b_owner_id)));

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"Номер бизнеса: "cl_white"%d\n\
		"cl_blue"Тип бизнеса: "cl_white"%s\n\
		"cl_blue"Название бизнеса: "cl_white"%s\n\n\
		"cl_blue"Владелец бизнеса: "cl_white"%s\n\
		"cl_blue"Цена бизнеса: "cl_white"%d$",
		GetBusinessData(business_index, b_id),
		business_type_names[GetBusinessData(business_index, b_type)],
		GetBusinessData(business_index, b_name),
		owner_str,
		GetBusinessData(business_index, b_price));

	Dialog_Open(playerid, Dialog:D_BUSINESS_INFO, DIALOG_STYLE_MSGBOX,
		"Информация о бизнесе", big_string,
		(!GetBusinessData(business_index, b_owner_id)) ? ("Купить") : (""),
		"Назад");
	big_string[0] = EOS;
	return true;
}

stock Business:ShowPlayerSelectBusiness(playerid){
	new counter;

	big_string[0] = EOS;
	for(new i; i < MAX_BUSINESS; i++){
		if(!GetBusinessData(i, b_id)) continue;
		if(GetBusinessData(i, b_owner_id) != GetPlayerData(playerid, p_id)) continue;

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s",
			big_string, counter + 1, GetBusinessData(i, b_name));

		player_listitems[playerid][counter] = i;
		counter++;
	}

	if(!counter){
		SendErrorMessage(playerid, "У Вас нет бизнесов во владении.");
		return false;
	}

	Dialog_Open(playerid, Dialog:D_BUSINESS_PLAYER_SELECT, DIALOG_STYLE_LIST,
		"Выберите бизнес", big_string,
		"Выбрать", "Закрыть");

	big_string[0] = EOS;
	return true;
}

stock Business:ShowBusinessPanel(playerid, business_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"1. "cl_white"Информация о бизнесе\n\
		"cl_blue"2. "cl_white"Заказать продукты "cl_grey"[%d шт]",
		GetBusinessData(business_index, b_products));

	Dialog_Open(playerid, Dialog:D_BUSINESS_PANEL, DIALOG_STYLE_LIST,
		GetBusinessData(business_index, b_name), big_string,
		"Далее", "Закрыть");
	big_string[0] = EOS;
	return true;
}

stock Business:ShowInfoBpanel(playerid, business_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"ID бизнеса: "cl_white"%d\n\
		"cl_blue"Название бизнеса: "cl_white"%s\n\
		"cl_blue"Тип бизнеса: "cl_white"%s\n\n\
		"cl_blue"Продуктов в бизнесе: "cl_white"%d ед\n\
		"cl_blue"Баланс бизнеса: "cl_white"%d$\n\
		"cl_blue"Гос. стоимость бизнеса: "cl_white"%d$\n\n\
		\\c"cl_grey"[Налог на бизнес составляет %d$ в сутки. Он снимается с баланса бизнеса в 5:00.\n\
		\\cВ случае отрицательного баланса бизнес будет продан государству за %d процентов от стоимости (%d$).\n\
		\\cПополнить/снять с баланса бизнеса можно в банке]",
		GetBusinessData(business_index, b_id), GetBusinessData(business_index, b_name),
		business_type_names[GetBusinessData(business_index, b_type)],
		GetBusinessData(business_index, b_products), GetBusinessData(business_index, b_balance),
		GetBusinessData(business_index, b_price),
		BUSINESS_DAY_TAX, floatround(BUSINESS_SELL_PERCENT * 100.0),
		floatround(BUSINESS_SELL_PERCENT * float(GetBusinessData(business_index, b_price))));

	Dialog_Open(playerid, Dialog:D_BUSINESS_INFO_BPANEL, DIALOG_STYLE_MSGBOX,
		GetBusinessData(business_index, b_name), big_string,
		"Назад", "");

	big_string[0] = EOS;
	return true;
}

//==============================================================================
stock Business:GetBusinessIndexByVW(vw){
	for(new i; i < MAX_BUSINESS; i++){
		if(!GetBusinessData(i, b_id)) continue;
		if(GetBusinessData(i, b_id) == vw) return i;
	}
	return -1;
}