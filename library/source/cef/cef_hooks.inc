
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	cef_subscribe("Authorization", "OnPlayerCefAuthorization");
	return Y_HOOKS_CONTINUE_RETURN_1;
}