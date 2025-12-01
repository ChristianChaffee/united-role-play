
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

			player_listitems[playerid][counter + 1] = PLAYER_PAYMENT_METHOD_LISTITEM;

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

//==============================================================================
stock Business:GetBusinessIndexByVW(vw){
	for(new i; i < MAX_BUSINESS; i++){
		if(!GetBusinessData(i, b_id)) continue;
		if(GetBusinessData(i, b_id) == vw) return i;
	}
	return -1;
}