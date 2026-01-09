
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	cef_subscribe("Inventory_SwapItems", "Inventory_SwapItems");
	cef_subscribe("Inventory_UseItem", "Inventory_UseItem");
	cef_subscribe("Inventory_DropItem", "Inventory_DropItem");
	cef_subscribe("Inventory_AccessoryRemove", "Inventory_AccessoryRemove");
	cef_subscribe("Inventory_TransferItem", "Inventory_TransferItem");
	cef_subscribe("Inventory_TransferOfferAccept", "Inventory_TransferOfferAccept");
	cef_subscribe("Inventory_TransferOfferDecline", "Inventory_TransferOfferDecline");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock OnPlayerSecondUpdate(playerid){
	if(GetPVarType(playerid, "Inventory_EatDrinkAnimationTimer") && GetPVarInt(playerid, "Inventory_EatDrinkAnimationTimer") > 0){
		SetPVarInt(playerid, "Inventory_EatDrinkAnimationTimer", GetPVarInt(playerid, "Inventory_EatDrinkAnimationTimer") - 1);
		if(GetPVarInt(playerid, "Inventory_EatDrinkAnimationTimer")){
			
			ClearAnimations(playerid);
			DeletePVar(playerid, "Inventory_EatDrinkAnimationTimer");
		}
	}
}