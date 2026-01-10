
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	cef_subscribe("Phone_BankTransfer", "Phone_BankTransfer");
	cef_subscribe("Phone_BusinessDeposit", "Phone_BusinessDeposit");
	cef_subscribe("Phone_BusinessWithdraw", "Phone_BusinessWithdraw");
	return Y_HOOKS_CONTINUE_RETURN_1;
}