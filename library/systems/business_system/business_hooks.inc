
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	cef_subscribe("Shop_BuyItem", "Shop_BuyItem");

	CreateDynamicPickup(1239, 23, BUSINESS_INFO_PICKUP_POS, BUSINESS_INFO_PICKUP_VW, BUSINESS_INFO_PICKUP_INT);
	CreateDynamic3DTextLabel(""cl_lime"Информация по бизнесам\n"cl_grey"'Left ALT'", -1, 
		BUSINESS_INFO_PICKUP_POS, 15.0, _, _, 1, 
		BUSINESS_INFO_PICKUP_VW, BUSINESS_INFO_PICKUP_INT);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK){
		if(IsPlayerInRangeOfPoint(playerid, 2.0, BUSINESS_INFO_PICKUP_POS)
			&& GetPlayerVirtualWorld(playerid) == BUSINESS_INFO_PICKUP_VW
			&& GetPlayerInterior(playerid) == BUSINESS_INFO_PICKUP_INT){

			Business:ShowInfoMenu(playerid);
			return Y_HOOKS_CONTINUE_RETURN_1;
		}
	}

	if(newkeys == KEY_WALK){
		for(new i; i < MAX_BUSINESS; i++){
			if(!GetBusinessData(i, b_id)) continue;
		
			if(IsPlayerInRangeOfPoint(playerid, 2.0, GetBusinessData(i, b_pos_x), GetBusinessData(i, b_pos_y), GetBusinessData(i, b_pos_z))){

				SetPlayerPos(playerid,
					GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_x),
					GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_y),
					GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_z));
				SetPlayerFacingAngle(playerid,
					GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_angle));
				SetPlayerInterior(playerid,
					GetBusinessInteriorData(GetBusinessData(i, b_type), bi_int));
				SetPlayerVirtualWorld(playerid,
					GetBusinessData(i, b_id));
				SetCameraBehindPlayer(playerid);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
		}

		for(new i, business_index; i < BUSINESS_TYPES_COUNT; i++){
			if(!IsPlayerInRangeOfPoint(playerid, 2.0, GetBusinessInteriorData(i, bi_exit_pos_x), GetBusinessInteriorData(i, bi_exit_pos_y), GetBusinessInteriorData(i, bi_exit_pos_z))) continue;
			if(GetPlayerInterior(playerid) != GetBusinessInteriorData(i, bi_int)) continue;

			business_index = Business:GetBusinessIndexByVW(GetPlayerVirtualWorld(playerid));
			if(business_index == -1) continue;

			SetPlayerPos(playerid,
				GetBusinessData(business_index, b_pos_x),
				GetBusinessData(business_index, b_pos_y),
				GetBusinessData(business_index, b_pos_z));
			SetPlayerVirtualWorld(playerid,
				GetBusinessData(business_index, b_vw));
			SetPlayerInterior(playerid,
				GetBusinessData(business_index, b_int));
			SetCameraBehindPlayer(playerid);

			return Y_HOOKS_CONTINUE_RETURN_1;
		}

		for(new i, business_index; i < BUSINESS_TYPES_COUNT; i++){
			if(!IsPlayerInRangeOfPoint(playerid, 2.0, GetBusinessInteriorData(i, bi_cashbox_pos_x), GetBusinessInteriorData(i, bi_cashbox_pos_y), GetBusinessInteriorData(i, bi_cashbox_pos_z))) continue;
			if(GetPlayerInterior(playerid) != GetBusinessInteriorData(i, bi_int)) continue;

			business_index = Business:GetBusinessIndexByVW(GetPlayerVirtualWorld(playerid));
			if(business_index == -1) continue;

			Business:ShowMenu(playerid, business_index);
			return Y_HOOKS_CONTINUE_RETURN_1;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock OnMySQLConnect(){
	mysql_tquery(mysql, "SELECT * FROM business", "LoadBusinesses", "i", -1);
	return continue();
}

hook stock PayDay(hour){
	if(hour == 5){
		mysql_tquery(mysql, "UPDATE `business` SET `balance` = `balance` - '"#BUSINESS_DAY_TAX"' WHERE `owner_id` != '0'");
	}
	return continue(hour);
}

hook stock OnPlayerSecondUpdate(playerid){
	if(GetPVarType(playerid, "Business_BuyTime") && GetPVarInt(playerid, "Business_BuyTime") > 0){
		SetPVarInt(playerid, "Business_BuyTime", GetPVarInt(playerid, "Business_BuyTime") - 1);
		if(GetPVarInt(playerid, "Business_BuyTime") == 0){
			Business:DeleteSellPVars(playerid);
		}
	}
	return continue(playerid);
}