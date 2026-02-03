
forward LoadHouses(house_index);
public LoadHouses(house_index){
	new rows, 
		tick_time = GetTickCount(),
		old_house_index = house_index;

	cache_get_row_count(rows);

	if(rows >= MAX_HOUSES){
		printf("[LoadHouses - ERROR]: Количество записей в базе данных превышает размер массива (%d >= %d)", rows, MAX_HOUSES);
		return false;
	}

	for(new i, temp_string[256], playerid = INVALID_PLAYER_ID; i < rows; i++){
		
		if(old_house_index == -1){
			house_index = i;
		}

		cache_get_value_name_int(i, "id", GetHouseData(house_index, h_id));
		cache_get_value_name_int(i, "owner_id", GetHouseData(house_index, h_owner_id));
		cache_get_value_name_int(i, "class", GetHouseData(house_index, h_class));
		cache_get_value_name_int(i, "price", GetHouseData(house_index, h_price));
		cache_get_value_name_int(i, "payment_days", GetHouseData(house_index, h_payment_days));
		cache_get_value_name_int(i, "door_lock", GetHouseData(house_index, h_lock));

		cache_get_value_name_float(i, "pos_x", GetHouseData(house_index, h_pos_x));
		cache_get_value_name_float(i, "pos_y", GetHouseData(house_index, h_pos_y));
		cache_get_value_name_float(i, "pos_z", GetHouseData(house_index, h_pos_z));
		cache_get_value_name_int(i, "vw", GetHouseData(house_index, h_vw));
		cache_get_value_name_int(i, "interior", GetHouseData(house_index, h_vw));

		cache_get_value_name_int(i, "interior_index", GetHouseData(house_index, h_interior_index));

		cache_get_value_name(i, "upgrades", temp_string);
		sscanf(temp_string, "p<,>a<i>["#HOUSE_UPGRADES_COUNT"]", house_info[house_index][h_upgrades]);

		cache_get_value_name(i, "inventory_items_id", temp_string);
		sscanf(temp_string, "p<,>a<i>["#MAX_HOUSE_INVENTORY_ITEMS"]", house_info[house_index][h_inventory_items_id]);

		cache_get_value_name(i, "inventory_items_count", temp_string);
		sscanf(temp_string, "p<,>a<i>["#MAX_HOUSE_INVENTORY_ITEMS"]", house_info[house_index][h_inventory_items_count]);

		cache_get_value_name(i, "inventory_items_value_one", temp_string);
		sscanf(temp_string, "p<,>a<i>["#MAX_HOUSE_INVENTORY_ITEMS"]", house_info[house_index][h_inventory_items_value_one]);

		cache_get_value_name(i, "inventory_items_value_two", temp_string);
		sscanf(temp_string, "p<,>a<i>["#MAX_HOUSE_INVENTORY_ITEMS"]", house_info[house_index][h_inventory_items_value_two]);

		cache_get_value_name_int(i, "safe_cash", GetHouseData(house_index, h_safe_cash));

		if(GetHouseData(house_index, h_payment_days) < 0 && GetHouseData(house_index, h_owner_id) != 0){
			playerid = INVALID_PLAYER_ID;
			for(new g, j = GetPlayerPoolSize(); g <= j; g++){
				if(!IsPlayerConnected(g) || !player_logged[g]) continue;
				if(GetPlayerData(g, p_id) != GetHouseData(house_index, h_owner_id)) continue;

				playerid = g;
				break;
			}

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				""cl_dark_red"Дом №%d был продан государству за неуплату налога за %d$.",
				GetHouseData(house_index, h_id), GetHouseData(house_index, h_price));

			if(playerid == INVALID_PLAYER_ID){
				AddPlayerNotification(GetHouseData(house_index, h_owner_id), small_string);

				static const fmt_query[] = "UPDATE accounts SET money = money + %d WHERE id = %d";
				new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
				mysql_format(mysql, query, sizeof(query), fmt_query,
					GetHouseData(house_index, h_price), GetHouseData(house_index, h_owner_id));
				mysql_tquery(mysql, query);
			}
			else{
				SendClientMessage(playerid, COLOR_DARK_RED, small_string);

				if(!GetPlayerBankCardData(playerid, pbc_id)){
					GiveMoney(playerid, GetHouseData(house_index, h_price));
				}
				else{
					Bank:GiveMoney(playerid, GetHouseData(house_index, h_price));
				}
			}
			small_string[0] = EOS;

			House:SellHouse(house_index);
		}

		House:UpdatePickups(house_index);
	}

	if(old_house_index == -1){
		printf("[LoadHouses]: Дома загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
		load_info[loaded_houses] = true;
	}
	return true;
}

forward EnterPlayerHouse(playerid, house_index);
public EnterPlayerHouse(playerid, house_index){
	new rows,
		temp_name[MAX_PLAYER_NAME];

	cache_get_row_count(rows);

	if(!rows){
		format(temp_name, sizeof(temp_name), "None");
	}
	else{
		cache_get_value_name(0, "name", temp_name);
	}

	SelectTextDraw(playerid, -1);

	cef_emit_event(playerid, "showHouseEnter",
		CEFINT(GetHouseData(house_index, h_id)),
		CEFSTR(temp_name),
		CEFSTR(house_class_names[GetHouseData(house_index, h_class)]),
		CEFINT(GetHouseData(house_index, h_price)),
		CEFINT(GetHouseData(house_index, h_lock)),
		CEFINT((!GetPlayerBankCardData(playerid, pbc_id)) ? (0) : ((!GetPlayerBankCardData(playerid, pbc_balance)) ? (0) : (1))));
	
	SetPVarInt(playerid, "House_Index", house_index);
	return true;
}

forward houseClose(playerid, const arguments[]);
public houseClose(playerid, const arguments[]){
	DeletePVar(playerid, "House_Index");
	CancelSelectTextDraw(playerid);
	return true;
}

forward houseInspect(playerid, const arguments[]);
public houseInspect(playerid, const arguments[]){
	House:EnterHouse(playerid, GetPVarInt(playerid, "House_Index"));
	House:HideEnterInterface(playerid);
	return true;
}

forward houseEnter(playerid, const arguments[]);
public houseEnter(playerid, const arguments[]){
	House:EnterHouse(playerid, GetPVarInt(playerid, "House_Index"));
	House:HideEnterInterface(playerid);
	return true;
}

forward houseBuyConfirm(playerid, const arguments[]);
public houseBuyConfirm(playerid, const arguments[]){
	new payment_type,
		house_index = GetPVarInt(playerid, "House_Index");

	sscanf(arguments, "i", payment_type);

	if(house_index < 0 || house_index >= MAX_HOUSES) return false;

	if(GetHouseData(house_index, h_owner_id) != 0){
		SendErrorMessage(playerid, "Данный дом уже занят.");
		House:HideEnterInterface(playerid);
		return false;
	}
	if(!payment_type && GetPlayerData(playerid, p_money) < GetHouseData(house_index, h_price)){
		SendErrorMessage(playerid, "У Вас недостаточно наличных денежных средств для покупки этого дома.");
		House:HideEnterInterface(playerid);
		return false;
	}
	if(payment_type && GetPlayerBankCardData(playerid, pbc_balance) < GetHouseData(house_index, h_price)){
		SendErrorMessage(playerid, "У Вас недостаточно денежных средств на банковской карте для покупки этого дома.");
		House:HideEnterInterface(playerid);
		return false;
	}

	if(!payment_type){
		GiveMoney(playerid, -GetHouseData(house_index, h_price));
	}
	else{
		Bank:GiveMoney(playerid, -GetHouseData(house_index, h_price));
	}

	SetHouseData(house_index, h_owner_id, GetPlayerData(playerid, p_id));
	SetHouseData(house_index, h_payment_days, 1);

	static const fmt_query[] = "UPDATE houses SET owner_id = %d, payment_days = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 3)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetHouseData(house_index, h_owner_id), GetHouseData(house_index, h_payment_days),
		GetHouseData(house_index, h_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы купили дом "cl_white"№%d (%s) "cl_lime"за "cl_white"%d$"cl_lime".",
		GetHouseData(house_index, h_id), house_class_names[GetHouseData(house_index, h_class)],
		GetHouseData(house_index, h_price));
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	SendClientMessage(playerid, COLOR_GREY, "- Ваш дом автоматически оплачен на 1 день. Чтобы оплатить дом обратитесь в банк.");
	SendClientMessage(playerid, COLOR_GREY, "- Меню управления домом: "cl_white"/hpanel");

	House:HideEnterInterface(playerid);
	return true;
}

forward houseMenuTenantsOpen(playerid, const arguments[]);
public houseMenuTenantsOpen(playerid, const arguments[]){
	if(!GetPVarType(playerid, "House_Index")) return false;

	static const fmt_query[] = "SELECT id, name FROM accounts WHERE tenant_house_id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetHouseData(GetPVarInt(playerid, "House_Index"), h_id));
	mysql_tquery(mysql, query, "ShowPlayerHouseTenantMenu", "i", playerid);
	return true;
}

forward ShowPlayerHouseTenantMenu(playerid);
public ShowPlayerHouseTenantMenu(playerid){
	if(!GetPVarType(playerid, "House_Index")) return false;

	new rows;
	cache_get_row_count(rows);

	for(new i, temp_name[MAX_PLAYER_NAME], temp_id; i < rows; i++){
		cache_get_value_name_int(i, "id", temp_id);
		cache_get_value_name(i, "name", temp_name);

		cef_emit_event(playerid, "addHouseMenuTenant",
			CEFSTR(temp_name),
			CEFINT((GetPlayerId(temp_name) == INVALID_PLAYER_ID) ? (0) : (1)));

		house_tenants_listitems[playerid][i] = temp_id;
	}
	return true;
}

forward houseMenuDoor(playerid, const arguments[]);
public houseMenuDoor(playerid, const arguments[]){
	new house_index = (!GetPVarType(playerid, "House_Index")) ? (-1) : (GetPVarInt(playerid, "House_Index"));
	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в собственном доме.");
		House:HideMenu(playerid);
		return false;
	}

	new door_status;
	sscanf(arguments, "i", door_status);

	SetHouseData(house_index, h_lock, door_status);

	static const fmt_query[] = "UPDATE houses SET door_lock = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetHouseData(house_index, h_lock), GetHouseData(house_index, h_id));
	mysql_tquery(mysql, query);
	return true;
}

forward houseMenuUpgradeBuy(playerid, const arguments[]);
public houseMenuUpgradeBuy(playerid, const arguments[]){
	new house_index = (!GetPVarType(playerid, "House_Index")) ? (-1) : (GetPVarInt(playerid, "House_Index"));
	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в собственном доме.");
		House:HideMenu(playerid);
		return false;
	}

	new upgrade_index;
	sscanf(arguments, "i", upgrade_index);

	if(House:IsHouseHaveUpgrade(house_index, upgrade_index)){
		House:ShowMenuError(playerid, "У Вас уже есть данное улучшение.");
		House:UpdateUpgradesList(playerid, house_index);
		return false;
	}
	if(!GetPlayerBankCardData(playerid, pbc_id)){
		House:ShowMenuError(playerid, "У Вас нет банковской карты.");
		return false;
	}
	if(GetPlayerBankCardData(playerid, pbc_balance) < house_upgrade_info[upgrade_index][hu_price]){
		House:ShowMenuError(playerid, "На балансе Вашей банковской карты недостаточно средств.");
		return false;
	}

	if(upgrade_index != HOUSE_UPGRADE_GARAGE){
		Bank:GiveMoney(playerid, -house_upgrade_info[upgrade_index][hu_price]);

		house_info[house_index][h_upgrades][upgrade_index] = 1;
		House:SaveUpgrades(house_index);

		House:UpdateUpgradesList(playerid, house_index);
	}

	switch(upgrade_index){
		case HOUSE_UPGRADE_GARAGE:{
			House:HideMenu(playerid);

			SetPlayerPos(playerid,
				GetHouseData(house_index, h_pos_x),
				GetHouseData(house_index, h_pos_y),
				GetHouseData(house_index, h_pos_z));

			SetPlayerVirtualWorld(playerid,
				GetHouseData(house_index, h_vw));

			SetPlayerInterior(playerid,
				GetHouseData(house_index, h_int));

			SendClientMessage(playerid, COLOR_LIME, "Выберите место, где хотите поставить гараж и введите "cl_white"'/savegarage'"cl_lime".");
			SetPVarInt(playerid, "House_IsCreateGarage", house_index);
		}	
	}
	return true;
}

forward houseMenuSellToState(playerid, const arguments[]);
public houseMenuSellToState(playerid, const arguments[]){
	new house_index = (!GetPVarType(playerid, "House_Index")) ? (-1) : (GetPVarInt(playerid, "House_Index"));
	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в собственном доме.");
		House:HideMenu(playerid);
		return false;
	}
	if(!GetPlayerBankCardData(playerid, pbc_id)){
		House:ShowMenuError(playerid, "У Вас нет бакновской карты.");
		return false;
	}

	Bank:GiveMoney(playerid, GetHouseData(house_index, h_price));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы продали дом "cl_white"№%d "cl_lime"государству за "cl_white"%d$"cl_lime".",
		GetHouseData(house_index, h_id), GetHouseData(house_index, h_price));
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	House:HideMenu(playerid);
	House:SellHouse(house_index);
	return true;
}

forward houseMenuTenantEvict(playerid, const arguments[]);
public houseMenuTenantEvict(playerid, const arguments[]){
	new house_index = (!GetPVarType(playerid, "House_Index")) ? (-1) : (GetPVarInt(playerid, "House_Index"));
	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в собственном доме.");
		House:HideMenu(playerid);
		return false;
	}

	new tenant_index;
	sscanf(arguments, "i", tenant_index);

	if(!house_tenants_listitems[playerid][tenant_index]){
		House:ShowMenuError(playerid, "Данного сожителя не существует.");
		House:HideMenu(playerid);
		return false;
	}

	static const fmt_query[] = "UPDATE accounts SET tenant_house_id = 0 WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		house_tenants_listitems[playerid][tenant_index]);
	mysql_tquery(mysql, query);

	new scdplayerid = GetPlayerIdBySqlId(house_tenants_listitems[playerid][tenant_index]);
	if(scdplayerid != INVALID_PLAYER_ID){
		SetPlayerData(scdplayerid, p_tenant_house_id, 0);

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Владелец жилья %s[%d] выселил(-а) Вас из дома №%d.",
			GetPlayerData(playerid, p_name), playerid, GetHouseData(house_index, h_id));
		SendClientMessage(scdplayerid, COLOR_DARK_RED, small_string);
		small_string[0] = EOS;
	}
	else{
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Владелец жилья %s выселил(-а) Вас из дома №%d.",
			GetPlayerData(playerid, p_name), GetHouseData(house_index, h_id));
		AddPlayerNotification(house_tenants_listitems[playerid][tenant_index], small_string);
		small_string[0] = EOS;
	}

	House:HideMenu(playerid);
	return true;
}

forward houseMenuTenantAdd(playerid, const arguments[]);
public houseMenuTenantAdd(playerid, const arguments[]){
	new house_index = (!GetPVarType(playerid, "House_Index")) ? (-1) : (GetPVarInt(playerid, "House_Index"));
	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в собственном доме.");
		House:HideMenu(playerid);
		return false;
	}

	new scdplayerid;
	sscanf(arguments, "i", scdplayerid);

	if(!IsPlayerConnected(scdplayerid) || !player_logged[scdplayerid]){
		House:ShowMenuError(playerid, "Данный игрок не в сети.");
		return false;
	}
	if(scdplayerid == playerid){
		House:ShowMenuError(playerid, "Вы не можете предложить подселиться самому себе.");
		return false;
	}
	if(!IsPlayerInRangeOfPlayer(playerid, scdplayerid, 10.0)){
		House:ShowMenuError(playerid, "Данный игрок далек от от Вас.");
		return false;
	}
	if(House:GetTenantsCount(house_index) >= MAX_HOUSE_TENANTS){
		House:ShowMenuError(playerid, "В Вашем доме максимальное количество гостей.");
		return false;
	}
	if(GetPlayerData(scdplayerid, p_tenant_house_id) == GetHouseData(house_index, h_id)){
		House:ShowMenuError(playerid, "Данный игрок и так живёт в Вашем доме.");
		return false;
	}
	if(GetPlayerData(scdplayerid, p_tenant_house_id) != 0){
		House:ShowMenuError(playerid, "Данный игрок уже живёт в другом доме.");
		return false;
	}

	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(scdplayerid, p_id)) continue;

		House:ShowMenuError(playerid, "У данного игрока уже есть дом.");
		return false;
	}

	House:HideMenu(playerid);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы предложили "cl_white"%s[%d] "cl_lime"подселиться в Ваш дом.",
		GetPlayerData(scdplayerid, p_name), scdplayerid);
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	cef_emit_event(scdplayerid, "showHouseMenuTenantAccept",
		CEFINT(GetHouseData(house_index, h_id)),
		CEFSTR(GetPlayerData(playerid, p_name)),
		CEFINT(playerid));
	return true;
}

forward houseMenuTenantAcceptResponse(playerid, const arguments[]);
public houseMenuTenantAcceptResponse(playerid, const arguments[]){
	new response,
		house_id,
		scdplayerid,
		house_index = -1;

	sscanf(arguments, "iii", response, house_id, scdplayerid);

	if(!response){
		if(IsPlayerConnected(scdplayerid)){
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"%s[%d] "cl_lime"отказался(-ась) подселяться в Ваш дом.",
				GetPlayerData(playerid, p_name), playerid);
			SendClientMessage(scdplayerid, COLOR_LIME, small_string);
		
			format(small_string, sizeof(small_string),
				"Вы отказались от предложения "cl_white"%s[%d] "cl_lime"подселиться в его(-её) дом.",
				GetPlayerData(scdplayerid, p_name), scdplayerid);
			SendClientMessage(playerid, COLOR_LIME, small_string);
			small_string[0] = EOS;
		}
		else{
			SendClientMessage(playerid, COLOR_LIME, "Вы отказались подселяться в дом.");
		}

		return false;
	}

	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(playerid, p_id)) continue;

		SendErrorMessage(playerid, "Вы не можете подселиться, так как уже владеете другим домом.");
		return false;
	}

	if(!IsPlayerConnected(scdplayerid) || !player_logged[scdplayerid]){
		SendErrorMessage(playerid, "Владелец дома не в сети.");
		return false;
	}
	if(!IsPlayerInRangeOfPlayer(playerid, scdplayerid, 10.0)){
		SendErrorMessage(playerid, "Владелец дома далеко от Вас.");
		return false;
	}

	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_id) != house_id) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(scdplayerid, p_id)) continue;

		house_index = i;
		break;
	}

	if(house_index == -1){
		SendErrorMessage(playerid, "Данный игрок уже не владеет данным домом.");
		return false;
	}
	if(House:GetTenantsCount(house_index) >= MAX_HOUSE_TENANTS){
		SendErrorMessage(playerid, "В доме максимальное количество гостей.");
		return false;
	}
	if(GetPlayerData(playerid, p_tenant_house_id) != 0){
		SendErrorMessage(playerid, "Вы уже живёте в другом доме. Используйте '/leavehouse' чтобы выселиться.");
		return false;
	}

	SetPlayerData(playerid, p_tenant_house_id, GetHouseData(house_index, h_id));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"%s[%d] "cl_lime"подселился(-ась) в Ваш дом!",
		GetPlayerData(playerid, p_name), playerid);
	SendClientMessage(scdplayerid, COLOR_WHITE, small_string);

	format(small_string, sizeof(small_string),
		"Вы приняли предложение "cl_white"%s[%d] "cl_lime"подселиться в дом "cl_white"№%d"cl_lime"!",
		GetPlayerData(scdplayerid, p_name), scdplayerid, GetHouseData(house_index, h_id));
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	static const fmt_query[] = "UPDATE accounts SET tenant_house_id = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_tenant_house_id), GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query);

	if(GetPVarType(scdplayerid, "House_InMenu")){
		House:HideMenu(scdplayerid);
	}
	return true;
}

forward houseMenuSellToPlayer(playerid, const arguments[]);
public houseMenuSellToPlayer(playerid, const arguments[]){
	new house_index = (!GetPVarType(playerid, "House_Index")) ? (-1) : (GetPVarInt(playerid, "House_Index"));
	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в собственном доме.");
		House:HideMenu(playerid);
		return false;
	}

	new scdplayerid,
		price;

	sscanf(arguments, "ii", scdplayerid, price);

	if(price > GetHouseData(house_index, h_price) * 2 || price < 0){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Цена дома должна быть от 0$ до %d$.",
			GetHouseData(house_index, h_price) * 2);
		House:ShowMenuError(playerid, small_string);
		small_string[0] = EOS;
		return false;
	}
	if(!IsPlayerConnected(scdplayerid) || !player_logged[scdplayerid]){
		House:ShowMenuError(playerid, "Данный игрок не в сети.");
		return false;
	}
	if(scdplayerid == playerid){
		House:ShowMenuError(playerid, "Вы не можете предложить купить дом самому себе.");
		return false;
	}
	if(!IsPlayerInRangeOfPlayer(playerid, scdplayerid, 10.0)){
		House:ShowMenuError(playerid, "Данный игрок далек от от Вас.");
		return false;
	}
	
	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(scdplayerid, p_id)) continue;

		House:ShowMenuError(playerid, "У данного игрока уже есть дом.");
		return false;
	}

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы предложили "cl_white"%s[%d] "cl_lime"купить дом "cl_white"№%d "cl_lime"за "cl_white"%d$"cl_lime".",
		GetPlayerData(scdplayerid, p_name), scdplayerid,
		GetHouseData(house_index, h_id), price);
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	House:HideMenu(playerid);

	cef_emit_event(scdplayerid, "showHouseAcceptBuy",
		CEFINT(GetHouseData(house_index, h_id)),
		CEFSTR(GetPlayerData(playerid, p_name)),
		CEFINT(playerid),
		CEFINT(price));
	return true;
}

forward houseAcceptBuyResponse(playerid, const arguments[]);
public houseAcceptBuyResponse(playerid, const arguments[]){
	new response,
		house_id,
		scdplayerid,
		price;

	sscanf(arguments, "iiii", response, house_id, scdplayerid, price);

	if(!response){
		if(IsPlayerConnected(scdplayerid) && player_logged[scdplayerid]){
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"%s[%d] "cl_lime"отказался от покупки Вашего дома за "cl_white"%d$"cl_lime".",
				GetPlayerData(playerid, p_name), playerid, price);
			SendClientMessage(scdplayerid, COLOR_WHITE, small_string);
			small_string[0] = EOS;
		}

		SendClientMessage(playerid, COLOR_LIME, "Вы отказались от покупки дома.");
		return false;
	}

	if(!IsPlayerConnected(scdplayerid) || !player_logged[scdplayerid]){
		SendErrorMessage(playerid, "Данный игрок не в сети.");
		return false;
	}
	if(!IsPlayerInRangeOfPlayer(playerid, scdplayerid, 10.0)){
		SendErrorMessage(playerid, "Данный игрок далек от Вас.");
		return false;
	}

	new house_index = -1;
	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(scdplayerid, p_id)) continue;

		house_index = i;
		break;
	}

	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(playerid, p_id)) continue;

		SendErrorMessage(playerid, "Вы уже владеете домом.");
		return false;
	}

	if(house_index == -1){
		SendErrorMessage(playerid, "Данный игрок уже не владеет домом.");
		return false;
	}
	if(GetPlayerData(playerid, p_money) < price){
		SendErrorMessage(playerid, "У Вас недостаточно денежных средств.");
		return false;
	}

	GiveMoney(playerid, -price);
	GiveMoney(scdplayerid, price);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"%s[%d] "cl_lime"купил(-а) у Вас дом "cl_white"№%d "cl_lime"за "cl_white"%d$"cl_lime".",
		GetPlayerData(playerid, p_name), playerid, GetHouseData(house_index, h_id), price);
	SendClientMessage(scdplayerid, COLOR_WHITE, small_string);

	format(small_string, sizeof(small_string),
		"Вы купили дом "cl_white"№%d "cl_lime"у "cl_white"%s[%d] "cl_lime"за "cl_white"%d$"cl_lime".",
		GetHouseData(house_index, h_id), GetPlayerData(scdplayerid, p_name), scdplayerid, price);
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	SetHouseData(house_index, h_owner_id, GetPlayerData(playerid, p_id));

	static const fmt_query[] = "UPDATE houses SET owner_id = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetHouseData(house_index, h_owner_id), GetHouseData(house_index, h_id));
	mysql_tquery(mysql, query);

	if(GetPlayerData(playerid, p_tenant_house_id) != 0){
		SetPlayerData(playerid, p_tenant_house_id, 0);

		static const fmt_query_2[] = "UPDATE accounts SET tenant_house_id = 0 WHERE id = %d";
		new query_2[sizeof(fmt_query_2) + (-2 + 11)];
		mysql_format(mysql, query_2, sizeof(query_2), fmt_query_2,
			GetPlayerData(playerid, p_id));
		mysql_tquery(mysql, query_2);
	}
	return true;
}

forward houseMenuInventoryTake(playerid, const arguments[]);
public houseMenuInventoryTake(playerid, const arguments[]){
	new house_index = (!GetPVarType(playerid, "House_Index")) ? (-1) : (GetPVarInt(playerid, "House_Index"));
	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в собственном доме.");
		House:HideMenu(playerid);
		return false;
	}

	new item_index,
		item_amount,
		slot_index = Inventory:GetFreeSlotIndex(playerid);

	if(slot_index == -1){
		House:ShowMenuError(playerid, "У Вас нет места в инвентаре.");
		return false;
	}

	sscanf(arguments, "ii", item_index, item_amount);

	item_index = house_inventory_listitems[playerid][item_index];

	if(!house_info[house_index][h_inventory_items_id][item_index]){
		House:ShowMenuError(playerid, "Данного предмета нет в инвентаре дома.");
		House:UpdateInventoryList(playerid, house_index);
		return false;
	}
	if(house_info[house_index][h_inventory_items_count][item_index] < item_amount){
		House:ShowMenuError(playerid, "В инвентаре дома нет такого количества данного предмета.");
		House:UpdateInventoryList(playerid, house_index);
		return false;
	}

	Inventory:GiveItem(playerid, slot_index, house_info[house_index][h_inventory_items_id][item_index], item_amount, house_info[house_index][h_inventory_items_value_one][item_index], house_info[house_index][h_inventory_items_value_two][item_index]);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Предмет "cl_white"'%s' (%d шт) "cl_lime"добавлен в Ваш инвентарь "cl_grey"(/inv)"cl_lime".",
		GetInventoryItemData(house_info[house_index][h_inventory_items_id][item_index], item_name),
		item_amount);
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	house_info[house_index][h_inventory_items_count][item_index] -= item_amount;
	if(!house_info[house_index][h_inventory_items_count][item_index]){
		House:SetInventorySlotNull(house_index, item_index);
	}
	else{
		House:SaveInventory(house_index);
	}

	House:HideMenu(playerid);
	return true;
}

forward houseMenuSafeDeposit(playerid, const arguments[]);
public houseMenuSafeDeposit(playerid, const arguments[]){
	new house_index = (!GetPVarType(playerid, "House_Index")) ? (-1) : (GetPVarInt(playerid, "House_Index"));
	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в собственном доме.");
		House:HideMenu(playerid);
		return false;
	}

	new amount;
	sscanf(arguments, "i", amount);

	if(GetPlayerData(playerid, p_money) < amount){
		House:ShowMenuError(playerid, "У Вас недостаточно наличных денежных средств.");
		return false;
	}
	if(amount <= 0){
		House:ShowMenuError(playerid, "Корректно введите количество денежных средств.");
		return false;
	}

	GiveMoney(playerid, -amount);
	AddHouseData(house_index, h_safe_cash, amount);

	static const fmt_query[] = "UPDATE houses SET safe_cash = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetHouseData(house_index, h_safe_cash), GetHouseData(house_index, h_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы положили в сейф дома "cl_white"%d$"cl_lime". В сейфе дома: "cl_white"%d$"cl_lime".",
		amount, GetHouseData(house_index, h_safe_cash));
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	House:HideMenu(playerid);
	return true;
}

forward houseMenuSafeWithdraw(playerid, const arguments[]);
public houseMenuSafeWithdraw(playerid, const arguments[]){
	new house_index = (!GetPVarType(playerid, "House_Index")) ? (-1) : (GetPVarInt(playerid, "House_Index"));
	if(house_index == -1){
		SendErrorMessage(playerid, "Вы должны находиться в собственном доме.");
		House:HideMenu(playerid);
		return false;
	}

	new amount;
	sscanf(arguments, "i", amount);

	if(GetHouseData(house_index, h_safe_cash) < amount){
		House:ShowMenuError(playerid, "В сейфе дома недостаточно денежных средств.");
		return false;
	}
	if(amount <= 0){
		House:ShowMenuError(playerid, "Корректно введите количество денежных средств.");
		return false;
	}

	GiveMoney(playerid, amount);
	AddHouseData(house_index, h_safe_cash, -amount);

	static const fmt_query[] = "UPDATE houses SET safe_cash = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetHouseData(house_index, h_safe_cash), GetHouseData(house_index, h_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы взяли из сейфа дома "cl_white"%d$"cl_lime". В сейфе дома: "cl_white"%d$"cl_lime".",
		amount, GetHouseData(house_index, h_safe_cash));
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	House:HideMenu(playerid);
	return true;
}

forward NewHouseCreated();
public NewHouseCreated(){
	new free_index = -1;
	for(new i; i < MAX_HOUSES; i++){
		if(GetHouseData(i, h_id) != 0) continue;
		
		free_index = i;
		break;
	}

	if(free_index == -1){
		static const fmt_query[] = "DELETE FROM houses WHERE id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			cache_insert_id());
		mysql_tquery(mysql, query);

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"[A] Ошибка создания дома №%d. На сервере максимальное количество домов.",
			cache_insert_id());
		Admin:SendAdminMessage(COLOR_LIGHT_RED, small_string);
		small_string[0] = EOS;
		return false;
	}

	static const fmt_query[] = "SELECT * FROM houses WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		cache_insert_id());
	mysql_tquery(mysql, query, "LoadHouses", "i", free_index);
	return true;
}