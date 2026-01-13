
forward LoadBusinesses(business_index);
public LoadBusinesses(business_index){
	new rows,
		tick_time = GetTickCount();

	cache_get_row_count(rows);

	if(rows >= MAX_BUSINESS){
		printf("[LoadBusinesses - ERROR]: Количество записей в базе данных превышает размер массива (%d >= %d)", rows, MAX_BUSINESS);
		return false;
	}

	for(new i, old_business_index = business_index, temp_string[2048]; i < rows; i++){

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
		if(GetBusinessData(business_index, b_type) != BUSINESS_TYPE_SKIN_SHOP){
			sscanf(temp_string, "p<,>a<i>["#MAX_BUSINESS_MENU_POSITIONS"]", business_info[business_index][b_products_prices]);
		}
		else{
			sscanf(temp_string, "p<,>a<i>["#BUSINESS_SKIN_SHOP_COUNT"]", business_skin_shop_prices[business_index]);
		}

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
		
		if(GetBusinessData(business_index, b_type) != BUSINESS_TYPE_GAS_STATION){
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
		}
		else{
			new business_type = GetBusinessData(business_index, b_type);

			big_string[0] = EOS;

			format(big_string, sizeof(big_string),
				""cl_lime"[%s]",
				GetBusinessData(business_index, b_name));

			for(new g; g < MAX_BUSINESS_MENU_POSITIONS; g++){
				if(!strcmp(GetBusinessMenuData(business_type, g, bm_name), "-")) continue;

				format(big_string, sizeof(big_string),
					"%s\n"cl_lime"%s: "cl_white"%d$",
					big_string, GetBusinessMenuData(business_type, g, bm_name),
					business_info[business_index][b_products_prices][g]);
			}

			format(big_string, sizeof(big_string),
				"%s\n\n\
				"cl_grey"Пешком используйте 'Left ALT'\n\
				"cl_grey"В транспорте используйте '/fill'",
				big_string);

			GetBusinessData(business_index, b_cashbox_text) = CreateDynamic3DTextLabel(big_string, -1, 
				GetBusinessData(business_index, b_pos_x), 
				GetBusinessData(business_index, b_pos_y), 
				GetBusinessData(business_index, b_pos_z), 
				15.0, _, _, 1, 
				GetBusinessData(business_index, b_vw), 
				GetBusinessData(business_index, b_int));

			GetBusinessData(business_index, b_cashbox_pickup) = CreateDynamicPickup(GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pickup_model_id), 23, 
				GetBusinessData(business_index, b_pos_x), 
				GetBusinessData(business_index, b_pos_y), 
				GetBusinessData(business_index, b_pos_z), 
				GetBusinessData(business_index, b_vw), 
				GetBusinessData(business_index, b_int));
		}

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

	//---------------------------------
	if(business_type == BUSINESS_TYPE_SHOP 
		&& GetBusinessMenuData(business_type, selected_position, bm_item_id) == ITEM_SIM_CARD_ID){

		new sim_card_number = GenerateSimCardNumber();
		Inventory:GiveItem(playerid, slot_index, GetBusinessMenuData(business_type, selected_position, bm_item_id), _, sim_card_number);
	}
	else if(business_type == BUSINESS_TYPE_SHOP 
		&& GetBusinessMenuData(business_type, selected_position, bm_item_id) == ITEM_CAMERA_ID){

		Inventory:GiveItem(playerid, slot_index, GetBusinessMenuData(business_type, selected_position, bm_item_id), _, 50);
	}
	else{
		Inventory:GiveItem(playerid, slot_index, GetBusinessMenuData(business_type, selected_position, bm_item_id));
	}
	//---------------------------------

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

forward GasStation_Buy(playerid, const arguments[]);
public GasStation_Buy(playerid, const arguments[]){
	new item_index,
		item_count,
		payment_type,
		business_index = -1;

	for(new i; i < MAX_BUSINESS; i++){
		if(!GetBusinessData(i, b_id)) continue;
		if(GetBusinessData(i, b_type) != BUSINESS_TYPE_GAS_STATION) continue;
		if(!IsPlayerInRangeOfPoint(playerid, 5.0, GetBusinessData(i, b_pos_x), GetBusinessData(i, b_pos_y), GetBusinessData(i, b_pos_z))) continue;

		business_index = i;
		break;
	}

	if(business_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться на АЗС.");
		Business:HideGasStation(playerid);
		return false;
	}

	sscanf(arguments, "iii", item_index, item_count, payment_type);

	new price = business_info[business_index][b_products_prices][item_index] * item_count,
		vehicleid = GetPlayerVehicleID(playerid);

	if(GetBusinessData(business_index, b_products) - (GetBusinessMenuData(GetBusinessData(business_index, b_type), item_index, bm_products_price) * item_count) < 0){
		Business:ShowGasStationError(playerid, "В бизнесе недостаточно продуктов.");
		return false;
	}
	if(payment_type && !GetPlayerBankCardData(playerid, pbc_id)){
		Business:ShowGasStationError(playerid, "У Вас нет банковской карты.");
		return false;
	}
	if(payment_type && GetPlayerBankCardData(playerid, pbc_balance) < price){
		Business:ShowGasStationError(playerid, "У Вас недостаточно средств на банковской карте.");
		return false;
	}
	if(!payment_type && GetPlayerData(playerid, p_money) < price){
		Business:ShowGasStationError(playerid, "У Вас недостаточно наличных денежных средств.");
		return false;
	}
	if(item_index <= 3 && !vehicleid){
		Business:ShowGasStationError(playerid, "Вы должны находиться в транспорте.");
		return false;
	}
	if(item_index <= 3 && GetVehicleParamsData(GetVehicleParamIndexByModel(GetVehicleModel(vehicleid)), vp_fuel_type) != item_index + 1){
		Business:ShowGasStationError(playerid, "Вашему транспорту не подходит данное топливо.");
		return false;
	}
	if(item_index <= 3 && floatround(GetVehicleData(vehicleid, v_fuel), floatround_floor) + item_count > 100){
		Business:ShowGasStationError(playerid, "В транспорт не влезет такое количество топлива.");
		return false;
	}

	if(item_index <= 3){
		AddVehicleData(vehicleid, v_fuel, float(item_count));
		cef_emit_event(playerid, "updateFuel", CEFINT(floatround(GetVehicleData(vehicleid, v_fuel))));

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Вы заправили транспорт на "cl_white"%d литров(-а)"cl_lime".",
			item_count);
		SendClientMessage(playerid, COLOR_LIME, small_string);
		small_string[0] = EOS;
	}
	else{
		new slot_index = Inventory:GetFreeSlotIndex(playerid);
		if(slot_index == -1){
			Business:ShowGasStationError(playerid, "У Вас недостаточно места в инвентаре.");
			return false;
		}

		Inventory:GiveItem(playerid, slot_index, GetBusinessMenuData(GetBusinessData(business_index, b_type), item_index, bm_item_id));
	
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			""cl_lime"Вы купили: "cl_white"%s.",
			GetBusinessMenuData(GetBusinessData(business_index, b_type), item_index, bm_name));
		SendClientMessage(playerid, -1, small_string);
		SendClientMessage(playerid, COLOR_GREY, "- Предмет помещён в инвентарь (/inv).");
		small_string[0] = EOS;
	}

	if(!payment_type){
		GiveMoney(playerid, -price);
	}
	else Bank:GiveMoney(playerid, -price);

	Business:HideGasStation(playerid);

	AddBusinessData(business_index, b_balance, price);
	AddBusinessData(business_index, b_products, -(GetBusinessMenuData(GetBusinessData(business_index, b_type), item_index, bm_products_price) * item_count));

	static const fmt_query[] = "UPDATE business SET products = %d, balance = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 3)];
	mysql_format(mysql, query, sizeof(query), fmt_query, 
		GetBusinessData(business_index, b_products), GetBusinessData(business_index, b_balance),
		GetBusinessData(business_index, b_id));
	mysql_tquery(mysql, query);
	return true;
}

forward skinSelectorPrev(playerid, const arguments[]);
public skinSelectorPrev(playerid, const arguments[]){
	SetPVarInt(playerid, "Business_SkinShopSkinIndex", GetPVarInt(playerid, "Business_SkinShopSkinIndex") - 1);
	if(GetPVarInt(playerid, "Business_SkinShopSkinIndex") < GetPVarInt(playerid, "Business_SkinShopSkinStartIndex")){
		SetPVarInt(playerid, "Business_SkinShopSkinIndex", GetPVarInt(playerid, "Business_SkinShopMaxSkinIndex") - 1);
	}

	SetPlayerSkin(playerid, GetBusinessSkinShopMenu(GetPVarInt(playerid, "Business_SkinShopSkinIndex"), bm_item_id));

	cef_emit_event(playerid, "skinShopSetData",
		CEFINT(GetBusinessSkinShopMenu(GetPVarInt(playerid, "Business_SkinShopSkinIndex"), bm_item_id)),
		CEFINT(business_skin_shop_prices[GetPVarInt(playerid, "Business_Index")][GetPVarInt(playerid, "Business_SkinShopSkinIndex")]));
	return true;
}

forward skinSelectorNext(playerid, const arguments[]);
public skinSelectorNext(playerid, const arguments[]){
	SetPVarInt(playerid, "Business_SkinShopSkinIndex", GetPVarInt(playerid, "Business_SkinShopSkinIndex") + 1);
	if(GetPVarInt(playerid, "Business_SkinShopSkinIndex") >= GetPVarInt(playerid, "Business_SkinShopMaxSkinIndex")){
		SetPVarInt(playerid, "Business_SkinShopSkinIndex", GetPVarInt(playerid, "Business_SkinShopSkinStartIndex"));
	}

	SetPlayerSkin(playerid, GetBusinessSkinShopMenu(GetPVarInt(playerid, "Business_SkinShopSkinIndex"), bm_item_id));

	cef_emit_event(playerid, "skinShopSetData",
		CEFINT(GetBusinessSkinShopMenu(GetPVarInt(playerid, "Business_SkinShopSkinIndex"), bm_item_id)),
		CEFINT(business_skin_shop_prices[GetPVarInt(playerid, "Business_Index")][GetPVarInt(playerid, "Business_SkinShopSkinIndex")]));
	return true;
}

forward skinSelectorExit(playerid, const arguments[]);
public skinSelectorExit(playerid, const arguments[]){
	new business_index = GetPVarInt(playerid, "Business_Index");

	SetPlayerPos(playerid, 
		GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pos_x),
		GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pos_y),
		GetBusinessInteriorData(GetBusinessData(business_index, b_type), bi_cashbox_pos_z));	
	
	SetPlayerVirtualWorld(playerid,
		GetBusinessData(business_index, b_id));

	SetPlayerSkin(playerid, GetPVarInt(playerid, "Business_SkinShopOldSkin"));

	DeletePVar(playerid, "Business_Index");
	DeletePVar(playerid, "Business_SkinShopSkinIndex");
	DeletePVar(playerid, "Business_SkinShopSkinStartIndex");
	DeletePVar(playerid, "Business_SkinShopMaxSkinIndex");
	DeletePVar(playerid, "Business_SkinShopOldSkin");

	SetCameraBehindPlayer(playerid);
	CancelSelectTextDraw(playerid);

	cef_emit_event(playerid, "hideSkinSelector");
	return true;
}

forward skinSelectorBuy(playerid, const arguments[]);
public skinSelectorBuy(playerid, const arguments[]){
	new business_index = GetPVarInt(playerid, "Business_Index"),
		skin_index = GetPVarInt(playerid, "Business_SkinShopSkinIndex"),
		price = business_skin_shop_prices[business_index][skin_index],
		payment_type;

	sscanf(arguments, "i", payment_type);

	if(GetBusinessData(business_index, b_products) - GetBusinessSkinShopMenu(skin_index, bm_products_price) < 0){
		Business:ShowSkinShopError(playerid, "В бизнесе недостаточно продуктов.");
		return false;
	}
	if(payment_type && !GetPlayerBankCardData(playerid, pbc_id)){
		Business:ShowSkinShopError(playerid, "У Вас нет банковской карты.");
		return false;
	}
	if(payment_type && GetPlayerBankCardData(playerid, pbc_balance) < price){
		Business:ShowSkinShopError(playerid, "У Вас недостаточно средств на банковской карте.");
		return false;
	}
	if(!payment_type && GetPlayerData(playerid, p_money) < price){
		Business:ShowSkinShopError(playerid, "У Вас недостаточно наличных денежных средств.");
		return false;
	}

	new slot_index = Inventory:GetFreeSlotIndex(playerid);
	if(slot_index == -1){
		Business:ShowSkinShopError(playerid, "У Вас недостаточно места в инвентаре.");
		return false;
	}

	Inventory:GiveItem(playerid, slot_index, ITEM_SKIN_ID, _, GetBusinessSkinShopMenu(skin_index, bm_item_id));

	small_string[0] = EOS;
	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_lime"Вы купили: "cl_white"%s",
		GetBusinessSkinShopMenu(skin_index, bm_name));
	SendClientMessage(playerid, -1, small_string);
	SendClientMessage(playerid, COLOR_GREY, "- Предмет помещён в инвентарь (/inv).");
	small_string[0] = EOS;

	if(!payment_type){
		GiveMoney(playerid, -price);
	}
	else{
		Bank:GiveMoney(playerid, -price);
	}

	AddBusinessData(business_index, b_balance, price);
	AddBusinessData(business_index, b_products, -GetBusinessSkinShopMenu(skin_index, bm_products_price));

	static const fmt_query[] = "UPDATE business SET products = %d, balance = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 3)];
	mysql_format(mysql, query, sizeof(query), fmt_query, 
		GetBusinessData(business_index, b_products), GetBusinessData(business_index, b_balance),
		GetBusinessData(business_index, b_id));
	mysql_tquery(mysql, query);

	skinSelectorExit(playerid, "");
	return true;
}