
stock Bank:ShowMainMenu(playerid){
	if(GetPlayerBankCardData(playerid, pbc_is_loaded) && !GetPlayerBankCardData(playerid, pbc_id)){

		Dialog_Open(playerid, Dialog:D_BANK_CARD_ACCEPT_CREATE, DIALOG_STYLE_MSGBOX,
			"Банк",
			""cl_blue"- "cl_white"У Вас нет банковской карты.\n\
			\\c"cl_grey"["cl_white"Вы хотите приобрести банковскую карту за "cl_green""#BANK_CARD_CREATE_PRICE"$"cl_white"?"cl_grey"]\n\n\
			\\c"cl_grey"Время ожидания: "cl_white""#BANK_CARD_CREATE_TIME" минут.",
			"Да", "Нет");
		return true;
	}
	if(GetPlayerBankCardData(playerid, pbc_is_loaded) && GetPlayerBankCardData(playerid, pbc_id)
		&& GetPlayerBankCardData(playerid, pbc_status) == BC_STATUS_WAIT){

		if(GetPlayerBankCardData(playerid, pbc_timer) > 0){
			SendErrorMessage(playerid, "Ваша банковская карта ещё не готова.");
			return false;
		}

		new slot_index = Inventory:GetFreeSlotIndex(playerid);
		if(slot_index == -1){
			SendErrorMessage(playerid, "У Вас нет места в инвентаре для получения банковской карты.");
			return false;
		}
		Inventory:GiveItem(playerid, slot_index, ITEM_BANK_CARD, _, GetPlayerBankCardData(playerid, pbc_id), _, _, 0);

		SetPlayerBankCardData(playerid, pbc_pin_code, RandomEx(1000, 9999));
		SetPlayerBankCardData(playerid, pbc_create_time, gettime());
		SetPlayerBankCardData(playerid, pbc_status, BC_STATUS_ACTIVE);

		static const fmt_query[] = "UPDATE bank_cards SET pin_code = %d, create_time = %d, status = %d WHERE id = %d";
		new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11) + (-2 + 11) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetPlayerBankCardData(playerid, pbc_pin_code),
			GetPlayerBankCardData(playerid, pbc_create_time),
			GetPlayerBankCardData(playerid, pbc_status),
			GetPlayerBankCardData(playerid, pbc_id));
		mysql_tquery(mysql, query);

		big_string[0] = EOS;
		format(big_string, sizeof(big_string),
			""cl_blue"- "cl_white"Вы успешно получили банковскую карту!\n\n\
			\\c"cl_grey"Пин-код: "cl_white"%d\n\
			\\c"cl_green"Запомните его!",
			GetPlayerBankCardData(playerid, pbc_pin_code));
		Dialog_Open(playerid, Dialog:D_NULL, DIALOG_STYLE_MSGBOX,
			"Банк", big_string,
			"Хорошо", "");
	}
	if(GetPlayerBankCardData(playerid, pbc_is_loaded) && GetPlayerBankCardData(playerid, pbc_id)
		&& GetPlayerBankCardData(playerid, pbc_status) == BC_STATUS_ACTIVE){

		if(!Bank:IsValidPinCode(playerid)){
			return Bank:ShowEnterPinCode(playerid);
		}

		Bank:ShowBankMenu(playerid);
		return true;
		
	}
	return true;
}

stock Bank:IsValidPinCode(playerid){
	if(!GetPlayerBankCardData(playerid, pbc_is_loaded) || !GetPlayerBankCardData(playerid, pbc_id)) return false;
	return (GetPlayerBankCardData(playerid, pbc_pin_code_timer) > 0) ? (true) : (false);
}

stock Bank:UpdateTypeSettings(playerid){
	if(!GetPlayerBankCardData(playerid, pbc_is_loaded) || !GetPlayerBankCardData(playerid, pbc_id)) return false;

	SetPlayerBankCardData(playerid, pbc_deposit_percent, bank_deposit_percents[GetPlayerBankCardData(playerid, pbc_type)]);

	static const fmt_query[] = "UPDATE bank_cards SET deposit_percent = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerBankCardData(playerid, pbc_deposit_percent),
		GetPlayerBankCardData(playerid, pbc_id));
	mysql_tquery(mysql, query);
	return true;
}

stock Bank:GiveMoney(playerid, money){
	if(!GetPlayerBankCardData(playerid, pbc_is_loaded) || !GetPlayerBankCardData(playerid, pbc_id)) return false;

	AddPlayerBankCardData(playerid, pbc_balance, money);

	static const fmt_query[] = "UPDATE bank_cards SET balance = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerBankCardData(playerid, pbc_balance), GetPlayerBankCardData(playerid, pbc_id));
	mysql_tquery(mysql, query);
	return true;
}

stock Bank:GiveDepositMoney(playerid, money){
	if(!GetPlayerBankCardData(playerid, pbc_is_loaded) || !GetPlayerBankCardData(playerid, pbc_id)) return false;

	AddPlayerBankCardData(playerid, pbc_deposit_balance, money);

	static const fmt_query[] = "UPDATE bank_cards SET deposit_balance = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerBankCardData(playerid, pbc_deposit_balance), GetPlayerBankCardData(playerid, pbc_id));
	mysql_tquery(mysql, query);
	return true;
}

//==============================================================================
stock Bank:ShowEnterPinCode(playerid){
	Dialog_Open(playerid, Dialog:D_BANK_CARD_ENTER_PIN_CODE, DIALOG_STYLE_INPUT,
		"Банк",
		""cl_blue"- "cl_white"Введите пин-код от банковской карты\n\
		"cl_grey"Восстановить пин-код можно в отдельном окне в банке",
		"Ввод", "Закрыть");
	return true;
}

stock Bank:ShowBankMenu(playerid){
	Dialog_Open(playerid, Dialog:D_BANK_MENU, DIALOG_STYLE_LIST,
		"Банк",
		""cl_blue"1. "cl_white"Информация по счёту\n\
		"cl_blue"2. "cl_white"Управление статусом карты\n\
		"cl_blue"3. "cl_white"Пополнить баланс\n\
		"cl_blue"4. "cl_white"Снять с баланса\n\
		"cl_blue"5. "cl_white"Управление депозитом",
		"Далее", "Закрыть");
	return true;
}

stock Bank:ShowCardInfo(playerid){
	new year, month, day, hour, minute;
	ConvertTime(GetPlayerBankCardData(playerid, pbc_create_time), year, month, day, hour, minute);

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"Владелец: "cl_white"%s[%d]\n\
		"cl_blue"Дата оформления счёта: "cl_white"%02d.%02d.%d в %02d:%02d\n\
		"cl_blue"Статус карты: %s\n\n\
		"cl_blue"Баланс: "cl_white"%d$\n\
		"cl_blue"Баланс депозита (процент депозита): "cl_white"%d$ (%d процентов)",
		GetPlayerData(playerid, p_name), playerid,
		day, month, year, hour, minute,
		bank_card_types[GetPlayerBankCardData(playerid, pbc_type)],
		GetPlayerBankCardData(playerid, pbc_balance),
		GetPlayerBankCardData(playerid, pbc_deposit_balance), GetPlayerBankCardData(playerid, pbc_deposit_percent));
	
	Dialog_Open(playerid, Dialog:D_BANK_MENU_CARD_INFO, DIALOG_STYLE_MSGBOX,
		"Банк", big_string,
		"Назад", "");
	big_string[0] = EOS;
	return true;
}

stock Bank:ShowTypeInfoList(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"Статус карты: "cl_grey"[%s"cl_grey"]\n\
		"cl_blue"- "cl_white"Информация о статусах\n\
		"cl_blue"- "cl_white"Изменить статус карты",
		bank_card_types[GetPlayerBankCardData(playerid, pbc_type)]);
	Dialog_Open(playerid, Dialog:D_BANK_MENU_STATUS_LIST, DIALOG_STYLE_LIST,
		"Банк", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Bank:ShowTypeInfo(playerid){
	big_string[0] = EOS;

	format(big_string, sizeof(big_string),
		"\\c"cl_grey"[%s"cl_grey"]\n\
		"cl_blue"- "cl_white"Нет доступа к депозиту\n\
		"cl_blue"- Стоимость статсуа: "cl_white"бесплатно (начальный)\n\n\n\
		\\c"cl_grey"[%s"cl_grey"]\n\
		"cl_blue"- Процент депозита: "cl_white"%d процента\n\
		"cl_blue"- Стоимость статсуа: "cl_green"%d$\n\n\n\
		\\c"cl_grey"[%s"cl_grey"]\n\
		"cl_blue"- Процент депозита: "cl_white"%d процента\n\
		"cl_blue"- Стоимость статсуа: "cl_green"%d$\n\n\n\
		\\c"cl_grey"[%s"cl_grey"]\n\
		"cl_blue"- Процент депозита: "cl_white"%d процента\n\
		"cl_blue"- Стоимость статсуа: "cl_green"%d$",
		bank_card_types[0],
		bank_card_types[1], bank_deposit_percents[1], bank_type_prices[1],
		bank_card_types[2], bank_deposit_percents[2], bank_type_prices[2],
		bank_card_types[3], bank_deposit_percents[3], bank_type_prices[3]);

	Dialog_Open(playerid, Dialog:D_BANK_MENU_STATUS_INFO, DIALOG_STYLE_MSGBOX,
		"Банк", big_string,
		"Назад", "");
	big_string[0] = EOS;
	return true;
}

stock Bank:ShowChangeType(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Статус\t"cl_grey"Стоимость ($)");

	for(new i = 1; i < sizeof(bank_card_types); i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. %s\t"cl_green"%d$",
			big_string, i,
			bank_card_types[i], bank_type_prices[i]);
	}
	Dialog_Open(playerid, Dialog:D_BANK_MENU_SELECT_STATUS, DIALOG_STYLE_TABLIST_HEADERS,
		"Банк", big_string,
		"Выбрать", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Bank:ShowPutMoney(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите сумму, на которую хотите пополнить баланс банковской карты\n\
		"cl_grey"[Текущий баланс: %d$]",
		GetPlayerBankCardData(playerid, pbc_balance));

	Dialog_Open(playerid, Dialog:D_BANK_MENU_PUT_MONEY, DIALOG_STYLE_INPUT,
		"Банк", big_string,
		"Ввод", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Bank:ShowTakeMoney(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите сумму, которую хотите снять с баланса банковской карты\n\
		"cl_grey"[Текущий баланс: %d$]",
		GetPlayerBankCardData(playerid, pbc_balance));

	Dialog_Open(playerid, Dialog:D_BANK_MENU_TAKE_MONEY, DIALOG_STYLE_INPUT,
		"Банк", big_string,
		"Ввод", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Bank:ShowDepositMenu(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Баланс депозита "cl_grey"["cl_white"%d$"cl_grey"]\n\
		"cl_blue"- "cl_white"Информация о депозите\n\
		"cl_blue"- "cl_white"Пополнить баланс депозита\n\
		"cl_blue"- "cl_white"Снять с баланса депозита",
		GetPlayerBankCardData(playerid, pbc_deposit_balance));

	Dialog_Open(playerid, Dialog:D_BANK_MENU_DEPOSIT_MENU, DIALOG_STYLE_LIST,
		"Банк | Депозиты", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Bank:ShowDepositInfo(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"Депозитный счёт "cl_white"— пассивный баланс, который растёт каждый час (PayDay)\n\
		в зависимости от статуса Вашего банковского счёта. От статуса банковского счёта зависит\n\
		ежечасовой прирост суммы на депозитном счёте.\n\
		Ваш текущий процент прироста депозита: "cl_grey"%d процентов"cl_white".",
		GetPlayerBankCardData(playerid, pbc_deposit_percent));

	Dialog_Open(playerid, Dialog:D_BANK_MENU_DEPOSIT_INFO, DIALOG_STYLE_MSGBOX,
		"Банк | Депозиты", big_string,
		"Назад", "");
	big_string[0] = EOS;
	return true;
}

stock Bank:ShowDepositPutMoney(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите сумму, которую хотите перевести с основного счёта на депозитный\n\n\
		"cl_grey"[Баланс основного счёта: %d$]\n\
		"cl_grey"[Баланс депозитного счётв: %d$]",
		GetPlayerBankCardData(playerid, pbc_balance),
		GetPlayerBankCardData(playerid, pbc_deposit_balance));

	Dialog_Open(playerid, Dialog:D_BANK_DEPOSIT_PUT_MONEY, DIALOG_STYLE_INPUT,
		"Банк | Депозиты", big_string,
		"Назад", "");
	big_string[0] = EOS;
	return true;
}

stock Bank:ShowDepositTakeMoney(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите сумму, которую хотите перевести с депозитного счёта на основной\n\n\
		"cl_grey"[Баланс основного счёта: %d$]\n\
		"cl_grey"[Баланс депозитного счётв: %d$]",
		GetPlayerBankCardData(playerid, pbc_balance),
		GetPlayerBankCardData(playerid, pbc_deposit_balance));

	Dialog_Open(playerid, Dialog:D_BANK_DEPOSIT_TAKE_MONEY, DIALOG_STYLE_INPUT,
		"Банк | Депозиты", big_string,
		"Назад", "");
	big_string[0] = EOS;
	return true;
}