
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	cef_subscribe("FactionMenu_Members", "FactionMenu_Members");
	cef_subscribe("FactionMembers_Close", "FactionMembers_Close");
	cef_subscribe("FactionMember_ChangeRank", "FactionMember_ChangeRank");
	cef_subscribe("FactionMember_Fire", "FactionMember_Fire");
	return Y_HOOKS_CONTINUE_RETURN_1;
}