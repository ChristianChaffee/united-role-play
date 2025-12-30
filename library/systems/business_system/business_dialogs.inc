
DialogResponse:D_BUSINESS_MENU(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	new business_index = Business:GetBusinessIndexByVW(GetPlayerVirtualWorld(playerid));

	if(business_index == -1) return false;

	new business_type = GetBusinessData(business_index, b_type),
		selected_position = player_listitems[playerid][listitem];

	if(selected_position == PLAYER_PAYMENT_METHOD_LISTITEM){
		if(GetPVarType(playerid, "PaymentMethodCard")) DeletePVar(playerid, "PaymentMethodCard");
		else{
			if(!GetPlayerBankCardData(playerid, pbc_id)){
				SendErrorMessage(playerid, "У Вас нет бакнвоской карты!");
			}
			else SetPVarInt(playerid, "PaymentMethodCard", 1);
		}

		Business:ShowMenu(playerid, business_index);
		return true;
	}

	new price = business_info[business_index][b_products_prices][selected_position];

	switch(business_type){
		case BUSINESS_TYPE_PIZZA:{
			if(GetPlayerData(playerid, p_money) < price && !GetPVarType(playerid, "PaymentMethodCard")){
				SendErrorMessage(playerid, "У Вас недостаточно наличных денежных средств.");
				Business:ShowMenu(playerid, business_index);
				return false;
			}
			if(GetPlayerBankCardData(playerid, pbc_balance) < price && GetPVarType(playerid, "PaymentMethodCard")){
				SendErrorMessage(playerid, "У Вас недостаточно денежных средств на карте.");
				Business:ShowMenu(playerid, business_index);
				return false;
			}
			if(GetBusinessData(business_index, b_products) - GetBusinessMenuData(business_type, selected_position, bm_products_price) < 0){
				SendErrorMessage(playerid, "В бизнесе недостаточно продуктов.");
				Business:ShowMenu(playerid, business_index);
				return false;
			}

			new slot_index = Inventory:GetFreeSlotIndex(playerid);
			if(slot_index == -1){
				SendErrorMessage(playerid, "У Вас в инвнтаре недостаточно места.");
				Business:ShowMenu(playerid, business_index);
				return false;
			}

			Inventory:GiveItem(playerid, slot_index, GetBusinessMenuData(business_type, selected_position, bm_item_id));

			if(!GetPVarType(playerid, "PaymentMethodCard")){
				GiveMoney(playerid, -price);
			}
			else{
				Bank:GiveMoney(playerid, -price);
			}

			AddBusinessData(business_index, b_balance, price);
			AddBusinessData(business_index, b_products, -GetBusinessMenuData(business_type, selected_position, bm_products_price));

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				""cl_lime"Вы купили: "cl_white"%s.",
				GetBusinessMenuData(business_type, selected_position, bm_name));
			SendClientMessage(playerid, -1, small_string);
			SendClientMessage(playerid, COLOR_GREY, "- Предмет помещён в инвентарь (/inv).");
			small_string[0] = EOS;

			static const fmt_query[] = "UPDATE business SET products = %d, balance = %d WHERE id = %d";
			new query[sizeof(fmt_query) + ((-2 + 11) * 3)];
			mysql_format(mysql, query, sizeof(query), fmt_query, 
				GetBusinessData(business_index, b_products), GetBusinessData(business_index, b_balance),
				GetBusinessData(business_index, b_id));
			mysql_tquery(mysql, query);
		}
	}
	return true;
}

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