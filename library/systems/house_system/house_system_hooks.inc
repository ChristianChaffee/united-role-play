
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM houses", "LoadHouses", "i", -1);

	cef_subscribe("houseClose", "houseClose");
	cef_subscribe("houseInspect", "houseInspect");
	cef_subscribe("houseEnter", "houseEnter");
	cef_subscribe("houseBuyConfirm", "houseBuyConfirm");

	cef_subscribe("houseMenuTenantsOpen", "houseMenuTenantsOpen");
	cef_subscribe("houseMenuDoor", "houseMenuDoor");
	cef_subscribe("houseMenuUpgradeBuy", "houseMenuUpgradeBuy");
	cef_subscribe("houseMenuSellToState", "houseMenuSellToState");
	cef_subscribe("houseMenuTenantEvict", "houseMenuTenantEvict");
	cef_subscribe("houseMenuTenantAdd", "houseMenuTenantAdd");
	cef_subscribe("houseMenuTenantAcceptResponse", "houseMenuTenantAcceptResponse");
	cef_subscribe("houseMenuSellToPlayer", "houseMenuSellToPlayer");
	cef_subscribe("houseAcceptBuyResponse", "houseAcceptBuyResponse");
	cef_subscribe("houseMenuInventoryTake", "houseMenuInventoryTake");
	cef_subscribe("houseMenuSafeDeposit", "houseMenuSafeDeposit");
	cef_subscribe("houseMenuSafeWithdraw", "houseMenuSafeWithdraw");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK){
		if(!IsPlayerInAnyVehicle(playerid)){
			for(new house_index; house_index < MAX_HOUSES; house_index++){
				if(!GetHouseData(house_index, h_id)) continue;

				if(IsPlayerInRangeOfPoint(playerid, 2.0, GetHouseData(house_index, h_pos_x), GetHouseData(house_index, h_pos_y), GetHouseData(house_index, h_pos_z))){
					static const fmt_query[] = "SELECT a.name FROM houses h JOIN accounts a ON a.id = h.owner_id WHERE h.id = '%d'";
					new query[sizeof(fmt_query) + (-2 + 11)];
					mysql_format(mysql, query, sizeof(query), fmt_query,
						GetHouseData(house_index, h_id));
					mysql_tquery(mysql, query, "EnterPlayerHouse", "ii", playerid, house_index);
					break;
				}

				if(GetPlayerVirtualWorld(playerid) == GetHouseData(house_index, h_id)
					&& GetPlayerInterior(playerid) == GetHouseInteriorData(GetHouseData(house_index, h_interior_index), h_int_int)){

					if(IsPlayerInRangeOfPoint(playerid, 2.0,
						house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][0],
						house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][1],
						house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][2])){

						SetPlayerPos(playerid, 
							GetHouseData(house_index, h_pos_x),
							GetHouseData(house_index, h_pos_y),
							GetHouseData(house_index, h_pos_z));

						SetPlayerInterior(playerid,
							GetHouseData(house_index, h_int));

						SetPlayerVirtualWorld(playerid,
							GetHouseData(house_index, h_vw));
						break;
					}

				}
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook stock PayDay(hour){
	if(hour == 5){
		mysql_tquery(mysql, "UPDATE `houses` SET `payment_days` = `payment_days` - '1' WHERE `owner_id` != '0'");
	}
	return continue(hour);
}