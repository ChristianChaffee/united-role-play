
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
		"cl_blue"2. "cl_white"Заказать продукты "cl_grey"[%d шт]\n\
		"cl_blue"3. "cl_white"Управление ценами\n\
		"cl_blue"4. "cl_white"Улучшения бизнеса",
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

stock Business:ShowPricesList(playerid, business_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS){
		DeletePVar(playerid, "Business_Index");
		return false;
	}

	new count;
	big_string[0] = EOS;

	format(big_string, sizeof(big_string),
		""cl_grey"Наименование позиции\t"cl_grey"Цена");

	for(new i; i < MAX_BUSINESS_MENU_POSITIONS; i++){
		if(!strcmp("-", GetBusinessMenuData(GetBusinessData(business_index, b_type), i, bm_name))) continue;

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s\t"cl_white"%d$",
			big_string, count + 1, 
			GetBusinessMenuData(GetBusinessData(business_index, b_type), i, bm_name),
			business_info[business_index][b_products_prices][i]);

		player_listitems[playerid][count] = i;
		count++;
	}

	if(!count){
		SendErrorMessage(playerid, "Произошла ошибка. Попробуйте позже.");
		DeletePVar(playerid, "Business_Index");
		return false;
	}

	Dialog_Open(playerid, Dialog:D_BUSINESS_PANEL_PRICES, DIALOG_STYLE_TABLIST_HEADERS,
		GetBusinessData(business_index, b_name), big_string,
		"Изменить", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Business:ShowChangePrice(playerid, business_index, menu_postition_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS){
		DeletePVar(playerid, "Business_Index");
		DeletePVar(playerid, "Business_MenuPositionIndex");
		return false;
	}
	if(menu_postition_index < 0 || menu_postition_index >= MAX_BUSINESS_MENU_POSITIONS){
		DeletePVar(playerid, "Business_Index");
		DeletePVar(playerid, "Business_MenuPositionIndex");
		return false;
	}

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите новую цену для товара "cl_grey"\"%s\""cl_white".\n\
		"cl_grey"Текущая цена: "cl_white"%d$.\n\
		"cl_grey"Цена должна быть от %d$ до %d$.",
		GetBusinessMenuData(GetBusinessData(business_index, b_type), menu_postition_index, bm_name),
		business_info[business_index][b_products_prices][menu_postition_index],
		BUSINESS_MIN_PRODUCT_PRICE, BUSINESS_MAX_PRODUCT_PRICE);

	Dialog_Open(playerid, Dialog:D_BUSINESS_CHANGE_PRICE, DIALOG_STYLE_INPUT,
		GetBusinessData(business_index, b_name), big_string,
		"Изменить", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Business:ShowPerksList(playerid, business_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string), ""cl_grey"Название улучшения\tЦена\tНаличие");

	for(new perk_index; perk_index < BUSINESS_PERKS_COUNT; perk_index++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s\t"cl_white"%d$\t%s",
			big_string, perk_index + 1,
			GetBusinessPerkData(perk_index, bp_name), GetBusinessPerkData(perk_index, bp_price),
			(business_info[business_index][b_perks][perk_index]) ? (""cl_green"Есть") : (""cl_light_red"Отсутствует"));
	}

	Dialog_Open(playerid, Dialog:D_BUSINESS_PERKS_LIST, DIALOG_STYLE_TABLIST_HEADERS,
		"Список улучшения", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Business:ShowPerk(playerid, business_index, perk_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;
	if(perk_index < 0 || perk_index >= BUSINESS_PERKS_COUNT) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"Название улучшения: "cl_white"%s\n\
		"cl_blue"Описание улучшения: "cl_white"%s\n\
		"cl_blue"Стоимость улучшения: "cl_white"%d$",
		GetBusinessPerkData(perk_index, bp_name), GetBusinessPerkData(perk_index, bp_description),
		GetBusinessPerkData(perk_index, bp_price));

	if(!business_info[business_index][b_perks][perk_index]){
		format(big_string, sizeof(big_string),
			"%s\n\n\
			\\c"cl_grey"[Вы желаете купить улучшение?]",
			big_string);
	}

	Dialog_Open(playerid, Dialog:D_BUSINESS_PERK_INFO, DIALOG_STYLE_MSGBOX,
		GetBusinessPerkData(perk_index, bp_name), big_string,
		(!business_info[business_index][b_perks][perk_index]) ? ("Купить") : ("Назад"),
		(!business_info[business_index][b_perks][perk_index]) ? ("Назад") : (""));

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

stock Business:LoadBusinessPerks(business_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;
	if(!GetBusinessData(business_index, b_id)) return false;

	for(new perk_index = 0; perk_index < BUSINESS_PERKS_COUNT; perk_index++){
		if(IsValidDynamicActor(business_info[business_index][b_perks_actors_id][perk_index])){
			DestroyDynamicActor(business_info[business_index][b_perks_actors_id][perk_index]);
			business_info[business_index][b_perks_actors_id][perk_index] = 0;
		}

		if(!business_info[business_index][b_perks][perk_index]) continue;

		switch(perk_index){
			case BUSINESS_PERK_SECURITY, BUSINESS_PERK_CASHBOX:{

				business_info[business_index][b_perks_actors_id][perk_index] = CreateDynamicActor(business_interior_info[GetBusinessData(business_index, b_type)][bi_perks_skins][perk_index], 
					business_interior_info[GetBusinessData(business_index, b_type)][bi_perks_x][perk_index], 
					business_interior_info[GetBusinessData(business_index, b_type)][bi_perks_y][perk_index], 
					business_interior_info[GetBusinessData(business_index, b_type)][bi_perks_z][perk_index], 
					business_interior_info[GetBusinessData(business_index, b_type)][bi_perks_angle][perk_index], 
					_, _, 
					GetBusinessData(business_index, b_id), 
					GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_int));

				if(!strcmp("", GetBusinessPerkData(perk_index, bp_anim_lib)) && !strcmp("", GetBusinessPerkData(perk_index, bp_anim_name))){
					ApplyDynamicActorAnimation(business_info[business_index][b_perks_actors_id][perk_index], GetBusinessPerkData(perk_index, bp_anim_lib), GetBusinessPerkData(perk_index, bp_anim_name), 4.1, true, true, true, false, 0);
				}
			}
		}
	}
	return true;
}

stock Business:SavePerks(business_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;

	small_string[0] = EOS;
	for(new perk_index; perk_index < BUSINESS_PERKS_COUNT; perk_index++){
		format(small_string, sizeof(small_string),
			"%s%d%s",
			small_string, business_info[business_index][b_perks][perk_index],
			(perk_index == BUSINESS_PERKS_COUNT - 1) ? ("") : (", "));
	}

	static const fmt_query[] = "UPDATE business SET perks = '%s' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + sizeof(small_string)) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		small_string, GetBusinessData(business_index, b_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;

	Business:LoadBusinessPerks(business_index);
	return true;
}

stock Business:CheckStartEvents(business_index){
	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;

	//------------------- Ограбление

	new chance = random(100);

	if((business_info[business_index][b_perks][BUSINESS_PERK_SECURITY] && chance <= BUSINESS_ROBBERY_SEC_PERCENT)
		|| (!business_info[business_index][b_perks][BUSINESS_PERK_SECURITY] && chance <= BUSINESS_ROBBERY_DEF_PERCENT)){

		new robbery_sum = RandomEx(BUSINESS_ROBBERY_MIN_SUM, BUSINESS_ROBBERY_MAX_SUM);

		AddBusinessData(business_index, b_balance, -robbery_sum);
		if(business_info[business_index][b_balance] < 0) SetBusinessData(business_index, b_balance, 0);

		static const fmt_query[] = "UPDATE business SET balance = %d WHERE id = %d";
		new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetBusinessData(business_index, b_balance), GetBusinessData(business_index, b_id));
		mysql_tquery(mysql, query);

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			""cl_dark_red"Бизнес \"%s\" был ограблен на сумму %d$!",
			GetBusinessData(business_index, b_name), robbery_sum);
		
		AddPlayerNotification(GetBusinessData(business_index, b_owner_id), small_string);
		small_string[0] = EOS;

	}

	//------------------------------
	return true;
}