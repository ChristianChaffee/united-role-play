
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	cef_subscribe("DonateShop_BuyVehicle", "DonateShop_BuyVehicle");

	cef_subscribe("DonateShop_BuyLevel", "DonateShop_BuyLevel");
	cef_subscribe("DonateShop_BuyLicenses", "DonateShop_BuyLicenses");
	cef_subscribe("DonateShop_BuyWanted", "DonateShop_BuyWanted");
	cef_subscribe("DonateShop_BuyNickname", "DonateShop_BuyNickname");

	cef_subscribe("DonateShop_BuyCashBox", "DonateShop_BuyCashBox");
	return Y_HOOKS_CONTINUE_RETURN_1;
}