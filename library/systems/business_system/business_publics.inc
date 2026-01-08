
forward LoadBusinesses(business_index);
public LoadBusinesses(business_index){
	new rows,
		tick_time = GetTickCount();

	cache_get_row_count(rows);

	if(rows >= MAX_BUSINESS){
		printf("[LoadBusinesses - ERROR]: Количество записей в базе данных превышает размер массива (%d >= %d)", rows, MAX_BUSINESS);
		return false;
	}

	for(new i, old_business_index = business_index, temp_string[256]; i < rows; i++){

		if(old_business_index == -1){
			business_index = i;
		}

		cache_get_value_name_int(i, "id", GetBusinessData(business_index, b_id));
		cache_get_value_name_int(i, "type", GetBusinessData(business_index, b_type));
		cache_get_value_name(i, "name", GetBusinessData(business_index, b_name));
		cache_get_value_name_int(i, "owner_id", GetBusinessData(business_index, b_owner_id));

		cache_get_value_name_float(i, "pos_x", GetBusinessData(business_index, b_pos_x));
		cache_get_value_name_float(i, "pos_y", GetBusinessData(business_index, b_pos_y));
		cache_get_value_name_float(i, "pos_z", GetBusinessData(business_index, b_pos_z));
		cache_get_value_name_int(i, "vw", GetBusinessData(business_index, b_vw));
		cache_get_value_name_int(i, "int", GetBusinessData(business_index, b_int));

		cache_get_value_name_int(i, "products", GetBusinessData(business_index, b_products));
		cache_get_value_name_int(i, "balance", GetBusinessData(business_index, b_balance));
		cache_get_value_name_int(i, "price", GetBusinessData(business_index, b_price));

		cache_get_value_name(i, "products_prices", temp_string);
		sscanf(temp_string, "p<,>a<i>["#MAX_BUSINESS_MENU_POSITIONS"]", business_info[business_index][b_products_prices]);

		cache_get_value_name(i, "perks", temp_string);
		sscanf(temp_string, "p<,>a<i>["#BUSINESS_PERKS_COUNT"]", business_info[business_index][b_perks]);

		if(GetBusinessData(business_index, b_type) >= BUSINESS_TYPES_COUNT){
			printf("[LoadBusinesses - WARNING]: Тип бизнеса (ID: %d, type: %d) превышает максимальное количество типов бизнеса (%d >= %d)", 
				GetBusinessData(business_index, b_id), GetBusinessData(business_index, b_type),
				GetBusinessData(business_index, b_type), BUSINESS_TYPES_COUNT);
			continue;
		}

		if(GetBusinessData(business_index, b_owner_id) != 0 && GetBusinessData(business_index, b_balance) < 0){
			static const fmt_query_1[] = "UPDATE `accounts` SET `money` = `money` + '%d' WHERE id = %d";
			new query_1[sizeof(fmt_query_1) + ((-2 + 11) * 2)];
			mysql_format(mysql, query_1, sizeof(query_1), fmt_query_1,
				floatround(float(GetBusinessData(business_index, b_price)) * BUSINESS_SELL_PERCENT),
				GetBusinessData(business_index, b_owner_id));
			mysql_tquery(mysql, query_1);

			SetBusinessData(business_index, b_owner_id, 0);
			SetBusinessData(business_index, b_balance, 0);

			static const fmt_query_2[] = "UPDATE business SET owner_id = 0, balance = 0 WHERE id = %d";
			new query_2[sizeof(fmt_query_2) + (-2 + 11)];
			mysql_format(mysql, query_2, sizeof(query_2), fmt_query_2,
				GetBusinessData(business_index, b_id));
			mysql_tquery(mysql, query_2);

			for(new perk_index; perk_index < BUSINESS_PERKS_COUNT; perk_index++){
				business_info[business_index][b_perks][perk_index] = 0;
			}
			Business:SavePerks(business_index);
		}
		
		if(GetBusinessData(business_index, b_owner_id) != 0){
			Business:CheckStartEvents(business_index);
		}

		//-------------------
		
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			""cl_lime"[%s]\n"cl_white"%s\n"cl_grey"'Left ALT'",
			business_type_names[GetBusinessData(business_index, b_type)], GetBusinessData(business_index, b_name));

		GetBusinessData(business_index, b_enter_text) = CreateDynamic3DTextLabel(small_string, -1, 
			GetBusinessData(business_index, b_pos_x), GetBusinessData(business_index, b_pos_y), GetBusinessData(business_index, b_pos_z), 
			15.0, _, _, 1, 
			GetBusinessData(business_index, b_vw), GetBusinessData(business_index, b_int));
		small_string[0] = EOS;

		GetBusinessData(business_index, b_enter_pickup) = CreateDynamicPickup(BUSINESS_ENTER_PICKUP_MODEL_ID, 23, 
			GetBusinessData(business_index, b_pos_x), GetBusinessData(business_index, b_pos_y), GetBusinessData(business_index, b_pos_z), 
			GetBusinessData(business_index, b_vw), GetBusinessData(business_index, b_int));
		
		//-------------------

		GetBusinessData(business_index, b_exit_text) = CreateDynamic3DTextLabel(""cl_white"Выход\n"cl_grey"'Left ALT'", -1, 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_exit_pos_x), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_exit_pos_y), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_exit_pos_z), 
			15.0, _, _, 1, 
			GetBusinessData(business_index, b_id), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_int));

		GetBusinessData(business_index, b_exit_pickup) = CreateDynamicPickup(BUSINESS_ENTER_PICKUP_MODEL_ID, 23, 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_exit_pos_x), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_exit_pos_y), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_exit_pos_z), 
			GetBusinessData(business_index, b_id), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_int));
		
		//-------------------

		GetBusinessData(business_index, b_cashbox_text) = CreateDynamic3DTextLabel(GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_text), -1, 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pos_x), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pos_y), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pos_z), 
			15.0, _, _, 1, 
			GetBusinessData(business_index, b_id), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_int));

		GetBusinessData(business_index, b_cashbox_pickup) = CreateDynamicPickup(GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pickup_model_id), 23, 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pos_x), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pos_y), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pos_z), 
			GetBusinessData(business_index, b_id), 
			GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_int));

		//-------------------

		Business:LoadBusinessPerks(business_index);
	}

	printf("[LoadBusinesses]: Бизнесы загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
	load_info[loaded_businesses] = true;
	return true;
}

forward NewBusinessCreated();
public NewBusinessCreated(){
	new insert_id = cache_insert_id(),
		business_index = -1;

	for(new i; i < MAX_BUSINESS; i++){
		if(GetBusinessData(i, b_id) != 0) continue;
		business_index = i;
		break;
	}

	static const fmt_query[] = "SELECT * FROM business WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, insert_id);
	mysql_tquery(mysql, query, "LoadBusinesses", "i", business_index);
}

forward Shop_BuyItem(playerid, const arguments[]);
public Shop_BuyItem(playerid, const arguments[]){
	new item_index, money_type;
	sscanf(arguments, "ii", item_index, money_type);

	new business_index = Business:GetBusinessIndexByVW(GetPlayerVirtualWorld(playerid)),
		selected_position = player_listitems[playerid][item_index],
		price = business_info[business_index][b_products_prices][selected_position],
		business_type = GetBusinessData(business_index, b_type);

	if(business_index == -1){
		cef_emit_event(playerid, "hideShopWindow");
		return false;
	}

	if(GetBusinessData(business_index, b_products) - GetBusinessMenuData(business_type, selected_position, bm_products_price) < 0){
		cef_emit_event(playerid, "showShopNotification",
			CEFSTR("В бизнесе недостаточно продуктов."));
		return false;
	}
	if(GetPlayerData(playerid, p_money) < price && !money_type){
		cef_emit_event(playerid, "showShopNotification",
			CEFSTR("У Вас недостаточно наличных денежных средств."));
		return false;
	}
	if(!GetPlayerBankCardData(playerid, pbc_id) && money_type){
		cef_emit_event(playerid, "showShopNotification",
			CEFSTR("У Вас нет банковской карты."));
		return false;
	}
	if(GetPlayerBankCardData(playerid, pbc_balance) < price && money_type){
		cef_emit_event(playerid, "showShopNotification",
			CEFSTR("У Вас недостаточно денежных средств на банковской карте."));
		return false;
	}

	new slot_index = Inventory:GetFreeSlotIndex(playerid);
	if(slot_index == -1){
		SendErrorMessage(playerid, "У Вас в инвнтаре недостаточно места.");
		Business:ShowMenu(playerid, business_index);
		return false;
	}

	Inventory:GiveItem(playerid, slot_index, GetBusinessMenuData(business_type, selected_position, bm_item_id));

	if(!money_type){
		GiveMoney(playerid, -price);
	}
	else{
		Bank:GiveMoney(playerid, -price);
	}

	if(GetPlayerData(playerid, p_id) != GetBusinessData(business_index, b_owner_id)){
		new tips_chance = random(100);
		if((business_info[business_index][b_perks][BUSINESS_PERK_CASHBOX] && tips_chance <= BUSINESS_TIPS_CASH_PERCENT)
			|| (!business_info[business_index][b_perks][BUSINESS_PERK_CASHBOX] && tips_chance <= BUSINESS_TIPS_DEF_PERCENT)){

			AddBusinessData(business_index, b_balance, RandomEx(BUSINESS_TIPS_MIN_SUM, BUSINESS_TIPS_MAX_SUM));
		}
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

	cef_emit_event(playerid, "hideShopWindow");
	return true;
}