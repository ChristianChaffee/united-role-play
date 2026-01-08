
DialogResponse:D_BUSINESS_INFO_MENU(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	switch(listitem){
		case 0: Business:ShowInfoAllBusinesses(playerid);
		case 1: Business:ShowInfoFreeBusinesses(playerid);
	}
	return true;
}

DialogResponse:D_BUSINESS_INFO_LIST(playerid, response, listitem, inputtext[]){
	if(!response){
		Business:ShowInfoMenu(playerid);
		return true;
	}

	SetPVarInt(playerid, "Business_Index", player_listitems[playerid][listitem]);
	Business:ShowInfo(playerid, GetPVarInt(playerid, "Business_Index"));
	return true;
}

DialogResponse:D_BUSINESS_INFO(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Business_Index");
		Business:ShowInfoMenu(playerid);
		return false;
	}

	new business_index = GetPVarInt(playerid, "Business_Index");
	DeletePVar(playerid, "Business_Index");

	if(business_index < 0 || business_index >= MAX_BUSINESS) return false;
	if(GetBusinessData(business_index, b_owner_id) > 0){
		SendErrorMessage(playerid, "У данного бизнеса уже есть владелец.");
		Business:ShowInfoMenu(playerid);
		return false;
	}
	if(GetPlayerData(playerid, p_money) < GetBusinessData(business_index, b_price) && GetPlayerBankCardData(playerid, pbc_balance) < GetBusinessData(business_index, b_price)){
		SendErrorMessage(playerid, "У Вас недостаточно средств для покупки данного бизнеса.");
		Business:ShowInfoMenu(playerid);
		return false;
	}

	if(GetPlayerData(playerid, p_money) < GetBusinessData(business_index, b_price)){
		Bank:GiveMoney(playerid, -GetBusinessData(business_index, b_price));
	}
	else GiveMoney(playerid, -GetBusinessData(business_index, b_price));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы приобрели бизнес "cl_white"%s "cl_lime"за %d$.",
		GetBusinessData(business_index, b_name), GetBusinessData(business_index, b_price));
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	SendClientMessage(playerid, COLOR_GREY, "- Чтобы открыть панель управления бизнесом введите '/bpanel'");

	SetBusinessData(business_index, b_owner_id, GetPlayerData(playerid, p_id));

	static const fmt_query[] = "UPDATE business SET owner_id = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetBusinessData(business_index, b_owner_id), GetBusinessData(business_index, b_id));
	mysql_tquery(mysql, query);
	return true;
}

DialogResponse:D_BUSINESS_PLAYER_SELECT(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	SetPVarInt(playerid, "Business_Index", player_listitems[playerid][listitem]);
	Business:ShowBusinessPanel(playerid, GetPVarInt(playerid, "Business_Index"));
	return true;
}

DialogResponse:D_BUSINESS_PANEL(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Business_Index");
		return false;
	}

	switch(listitem){
		case 0:{ // - Информация
			Business:ShowInfoBpanel(playerid, GetPVarInt(playerid, "Business_Index"));
		}
		case 1:{ // - Заказать продукты
			if(Job:IsBusinessHaveDeliveryOrder(GetPVarInt(playerid, "Business_Index"))){
				SendErrorMessage(playerid, "У Вашего бизнеса уже есть активный заказ на доставку продуктов.");
				Business:ShowBusinessPanel(playerid, GetPVarInt(playerid, "Business_Index"));
				return false;
			}
			
			Job:ShowBusinessEnterDelivery(playerid);
		}
		case 2:{ // - Управление ценами
			Business:ShowPricesList(playerid, GetPVarInt(playerid, "Business_Index"));
		}
		case 3:{ // - Улучшения бизнеса
			Business:ShowPerksList(playerid, GetPVarInt(playerid, "Business_Index"));
		}
		case 4:{ // - Продать бизнес
			Business:ShowSellSelectType(playerid);
		}
	}

	return true;
}

DialogResponse:D_BUSINESS_INFO_BPANEL(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Business_Index")) return false;
	Business:ShowBusinessPanel(playerid, GetPVarInt(playerid, "Business_Index"));
	return true;
}

DialogResponse:D_BUSINESS_PANEL_PRICES(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Business_Index")) return false;
	if(!response){
		Business:ShowBusinessPanel(playerid, GetPVarInt(playerid, "Business_Index"));
		return false;
	}

	SetPVarInt(playerid, "Business_MenuPositionIndex", player_listitems[playerid][listitem]);
	Business:ShowChangePrice(playerid, GetPVarInt(playerid, "Business_Index"), GetPVarInt(playerid, "Business_MenuPositionIndex"));
	return true;
}

DialogResponse:D_BUSINESS_CHANGE_PRICE(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Business_MenuPositionIndex");
		return false;
	}
	if(strval(inputtext) < BUSINESS_MIN_PRODUCT_PRICE){
		SendErrorMessage(playerid, "Цена должна быть от "#BUSINESS_MIN_PRODUCT_PRICE"$.");
		Business:ShowChangePrice(playerid, GetPVarInt(playerid, "Business_Index"), GetPVarInt(playerid, "Business_MenuPositionIndex"));
		return false;
	}
	if(strval(inputtext) > BUSINESS_MAX_PRODUCT_PRICE){
		SendErrorMessage(playerid, "Цена должна быть до "#BUSINESS_MAX_PRODUCT_PRICE"$.");
		Business:ShowChangePrice(playerid, GetPVarInt(playerid, "Business_Index"), GetPVarInt(playerid, "Business_MenuPositionIndex"));
		return false;
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы изменили цену товара \"%s\" с %d$ на %d$.",
		GetBusinessMenuData(GetBusinessData(GetPVarInt(playerid, "Business_Index"), b_type), GetPVarInt(playerid, "Business_MenuPositionIndex"), bm_name),
		business_info[GetPVarInt(playerid, "Business_Index")][b_products_prices][GetPVarInt(playerid, "Business_MenuPositionIndex")],
		strval(inputtext));
	SendClientMessage(playerid, COLOR_GREEN, small_string);

	business_info[GetPVarInt(playerid, "Business_Index")][b_products_prices][GetPVarInt(playerid, "Business_MenuPositionIndex")] = strval(inputtext);

	small_string[0] = EOS;
	for(new i; i < MAX_BUSINESS_MENU_POSITIONS; i++){
		format(small_string, sizeof(small_string),
			"%s%d%s",
			small_string,
			business_info[GetPVarInt(playerid, "Business_Index")][b_products_prices][i],
			(i == MAX_BUSINESS_MENU_POSITIONS - 1) ? ("") : (", "));
	}

	static const fmt_query[] = "UPDATE business SET products_prices = '%s' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + sizeof(small_string)) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		small_string, GetBusinessData(GetPVarInt(playerid, "Business_Index"), b_id));
	mysql_tquery(mysql, query);
	small_string[0] = EOS;

	DeletePVar(playerid, "Business_Index");
	DeletePVar(playerid, "Business_MenuPositionIndex");
	return true;
}

DialogResponse:D_BUSINESS_PERKS_LIST(playerid, response, listitem, inputtext[]){
	if(!response){
		Business:ShowBusinessPanel(playerid, GetPVarInt(playerid, "Business_Index"));
		return false;
	}

	SetPVarInt(playerid, "Business_PerkIndex", listitem);
	Business:ShowPerk(playerid, GetPVarInt(playerid, "Business_Index"), GetPVarInt(playerid, "Business_PerkIndex"));
	return true;
}

DialogResponse:D_BUSINESS_PERK_INFO(playerid, response, listitem, inputtext[]){
	if(!response && business_info[GetPVarInt(playerid, "Business_Index")][b_perks][GetPVarInt(playerid, "Business_PerkIndex")]){
		DeletePVar(playerid, "Business_PerkIndex");
		Business:ShowPerksList(playerid, GetPVarInt(playerid, "Business_Index"));
		return true;
	}

	new business_index = GetPVarInt(playerid, "Business_Index"),
		perk_index = GetPVarInt(playerid, "Business_PerkIndex");

	if(response && !business_info[business_index][b_perks][perk_index]){
		if(GetBusinessData(business_index, b_balance) < GetBusinessPerkData(perk_index, bp_price)){
			SendErrorMessage(playerid, "На балансе бизнеса недостаточно средств.");
			Business:ShowPerk(playerid, business_index, perk_index);
			return false;
		}

		business_info[business_index][b_perks][perk_index] = 1;
		Business:SavePerks(business_index);

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Вы купили улучшения \"%s\" за %d$.",
			GetBusinessPerkData(perk_index, bp_name),
			GetBusinessPerkData(perk_index, bp_price));
		SendClientMessage(playerid, COLOR_GREEN, small_string);
		small_string[0] = EOS;
	}
	else{
		DeletePVar(playerid, "Business_PerkIndex");
		Business:ShowPerksList(playerid, GetPVarInt(playerid, "Business_Index"));
	}
	return true;
}

DialogResponse:D_BUSINESS_SELL_TYPE(playerid, response, listitem, inputtext[]){
	if(!response){
		Business:ShowBusinessPanel(playerid, GetPVarInt(playerid, "Business_Index"));
		return false;
	}

	switch(listitem){
		case 0: Business:ShowAcceptSellGov(playerid, GetPVarInt(playerid, "Business_Index"));
		case 1: Business:ShowAcceptSellPlayer(playerid, GetPVarInt(playerid, "Business_Index"));
	}
	return true;
}

DialogResponse:D_BUSINESS_SELL_ACCEPT_GOV(playerid, response, listitem, inputtext[]){
	if(!response){
		Business:ShowSellSelectType(playerid);
		return false;
	}

	new business_index = GetPVarInt(playerid, "Business_Index"),
		sell_sum = floatround(BUSINESS_SELL_PERCENT * float(GetBusinessData(business_index, b_price)));

	Bank:GiveMoney(playerid, sell_sum);
	SetBusinessData(business_index, b_owner_id, 0);

	static const fmt_query[] = "UPDATE business SET owner_id = 0 WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetBusinessData(business_index, b_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы продали бизнес \"%s\" за %d$. Деньги зачислены на банковский счёт.",
		GetBusinessData(business_index, b_name), sell_sum);
	SendClientMessage(playerid, COLOR_GREEN, small_string);
	small_string[0] = EOS;

	for(new perk_index; perk_index < BUSINESS_PERKS_COUNT; perk_index++){
		business_info[business_index][b_perks][perk_index] = 0;
	}
	Business:SavePerks(business_index);

	DeletePVar(playerid, "Business_Index");
	return true;
}

DialogResponse:D_BUSINESS_SELL_ACC_PLAYER(playerid, response, listitem, inputtext[]){
	if(!response){
		Business:ShowSellSelectType(playerid);
		return false;
	}

	new scdplayerid,
		price,
		business_index = GetPVarInt(playerid, "Business_Index");

	if(sscanf(inputtext, "p<,>dd", scdplayerid, price) || price < 0){
		Business:ShowAcceptSellPlayer(playerid, business_index);
		return false;
	}
	if(!IsPlayerConnected(scdplayerid)){
		SendErrorMessage(playerid, "Игрок не в сети.");
		Business:ShowAcceptSellPlayer(playerid, business_index);
		return false;
	}
	if(scdplayerid == playerid){
		SendErrorMessage(playerid, "Вы не можете продать бизнес самому себе.");
		Business:ShowAcceptSellPlayer(playerid, business_index);
		return false;
	}
	if(!IsPlayerInRangeOfPlayer(playerid, scdplayerid, 10.0)){
		SendErrorMessage(playerid, "Игрок находится далеко от Вас.");
		Business:ShowAcceptSellPlayer(playerid, business_index);
		return false;
	}
	if(price > GetBusinessData(business_index, b_price)){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Цена не может превышать гос. стоимость (%d$)",
			GetBusinessData(business_index, b_price));
		SendErrorMessage(playerid, small_string);
		small_string[0] = EOS;

		Business:ShowAcceptSellPlayer(playerid, business_index);
		return false;
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы предложили игроку %s[%d] купить бизнес \"%s\" за %d$.",
		GetPlayerData(scdplayerid, p_name), scdplayerid, GetBusinessData(business_index, b_name), price);
	SendClientMessage(playerid, COLOR_GREEN, small_string);
	small_string[0] = EOS;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Игрок "cl_grey"%s[%d] "cl_white"предлагает Вам купить бизнес "cl_grey"\"%s\" "cl_white"за "cl_green"%d$"cl_white".\n\
		\\c"cl_grey"[Вы согласны?]",
		GetPlayerData(playerid, p_name), playerid,
		GetBusinessData(business_index, b_name), price);

	Dialog_Open(scdplayerid, Dialog:D_BUSINESS_BUY_ACCEPT, DIALOG_STYLE_MSGBOX,
		GetBusinessData(business_index, b_name), big_string,
		"Купить", "Отказаться");
	big_string[0] = EOS;

	SetPVarInt(scdplayerid, "Business_BuyIndex", business_index);
	SetPVarInt(scdplayerid, "Business_SellPrice", price);
	SetPVarInt(scdplayerid, "Business_BuyPlayerid", playerid);
	SetPVarInt(scdplayerid, "Business_BuyTime", 10);
	DeletePVar(playerid, "Business_Index");
	return true;
}

DialogResponse:D_BUSINESS_BUY_ACCEPT(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Business_BuyPlayerid") || !GetPVarType(playerid, "Business_BuyIndex") || !GetPVarType(playerid, "Business_SellPrice")){
		SendErrorMessage(playerid, "Произошла ошибка.");
		Business:DeleteSellPVars(playerid);
		return false;
	}

	new sellerid = GetPVarInt(playerid, "Business_BuyPlayerid"),
		business_index = GetPVarInt(playerid, "Business_BuyIndex"),
		price = GetPVarInt(playerid, "Business_SellPrice");

	if(!response){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"%s[%d] отказался от покупки бизнеса \"%s\".",
			GetPlayerData(playerid, p_name), playerid, GetBusinessData(business_index, b_name));
		SendClientMessage(sellerid, COLOR_GREY, small_string);

		format(small_string, sizeof(small_string),
			"Вы отказались от покупки бизнеса \"%s\" за %d$.",
			GetBusinessData(business_index, b_name), price);
		SendClientMessage(playerid, COLOR_GREY, small_string);
		small_string[0] = EOS;

		Business:DeleteSellPVars(playerid);
		return false;
	}
	if(!IsPlayerConnected(sellerid)){
		SendClientMessage(playerid, COLOR_GREY, "Владелец бизнеса не в сети.");
		Business:DeleteSellPVars(playerid);
		return false;
	}
	if(!IsPlayerInRangeOfPlayer(playerid, sellerid, 10.0)){
		SendClientMessage(playerid, COLOR_GREY, "Игрок далеко от Вас.");
		Business:DeleteSellPVars(playerid);
		return false;
	}
	if(GetPlayerData(sellerid, p_id) != GetBusinessData(business_index, b_owner_id)){
		SendClientMessage(playerid, COLOR_GREY, "Игрок не является владельцем бизнеса.");
		Business:DeleteSellPVars(playerid);
		return false;
	}
	if(GetPlayerData(playerid, p_money) < price){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"У %s[%d] недостаточно средств для покупки бизнеса \"%s\" за %d$.",
			GetPlayerData(playerid, p_name), playerid, GetBusinessData(business_index, b_name), price);
		SendClientMessage(sellerid, COLOR_GREY, small_string);

		format(small_string, sizeof(small_string),
			"У Вас недостаточно средств для покупки бизнеса \"%s\" за %d$.",
			GetBusinessData(business_index, b_name), price);
		SendClientMessage(playerid, COLOR_GREY, small_string);
		small_string[0] = EOS;

		Business:DeleteSellPVars(playerid);
		return false;
	}

	GiveMoney(playerid, -price);
	GiveMoney(sellerid, price);
	SetBusinessData(business_index, b_owner_id, GetPlayerData(playerid, p_id));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы продали бизнес \"%s\" игроку %s[%d] за %d$!",
		GetBusinessData(business_index, b_name), GetPlayerData(playerid, p_name), playerid, price);
	SendClientMessage(sellerid, COLOR_GREEN, small_string);

	format(small_string, sizeof(small_string),
		"Вы купили бизнес \"%s\" у игрока %s[%d] за %d$!",
		GetBusinessData(business_index, b_name), GetPlayerData(sellerid, p_name), sellerid, price);
	SendClientMessage(playerid, COLOR_GREEN, small_string);
	small_string[0] = EOS;

	static const fmt_query[] = "UPDATE business SET owner_id = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetBusinessData(business_index, b_owner_id), GetBusinessData(business_index, b_id));
	mysql_tquery(mysql, query);
	return true;
}