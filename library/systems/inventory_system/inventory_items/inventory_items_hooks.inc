
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ){
	
	UpdatePlayerWeapons(playerid);

	new slot_index = Inventory:GetFreeSlotIndex(playerid);
	if(slot_index == -1){
		SavePlayerWeapons(playerid);
		return Y_HOOKS_CONTINUE_RETURN_1;
	}

	switch(weaponid){
		
		case WEAPON_DEAGLE:{
			if(player_info[playerid][p_weapons_ammo][2] == 1){
				Inventory:GiveItem(playerid, slot_index, ITEM_DEAGLE_ID);

				player_info[playerid][p_weapons][2] = 0;
				player_info[playerid][p_weapons_ammo][2] = 0;
			}
		}

		case WEAPON_SHOTGUN:{
			if(player_info[playerid][p_weapons_ammo][3] == 1){
				Inventory:GiveItem(playerid, slot_index, ITEM_SHOTGUN_ID);

				player_info[playerid][p_weapons][3] = 0;
				player_info[playerid][p_weapons_ammo][3] = 0;
			}
		}

		case WEAPON_MP5:{
			if(player_info[playerid][p_weapons_ammo][4] == 1){
				Inventory:GiveItem(playerid, slot_index, ITEM_MP5_ID);

				player_info[playerid][p_weapons][4] = 0;
				player_info[playerid][p_weapons_ammo][4] = 0;
			}
		}

		case WEAPON_M4:{
			if(player_info[playerid][p_weapons_ammo][5] == 1){
				Inventory:GiveItem(playerid, slot_index, ITEM_M4_ID);

				player_info[playerid][p_weapons][5] = 0;
				player_info[playerid][p_weapons_ammo][5] = 0;
			}
		}
	}

	SavePlayerWeapons(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}