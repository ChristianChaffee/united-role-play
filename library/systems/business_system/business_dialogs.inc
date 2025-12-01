
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