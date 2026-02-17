
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	cef_subscribe("FactionMenu_Members", "FactionMenu_Members");
	cef_subscribe("FactionMembers_Close", "FactionMembers_Close");
	cef_subscribe("FactionMember_ChangeRank", "FactionMember_ChangeRank");
	cef_subscribe("FactionMember_Fire", "FactionMember_Fire");
	cef_subscribe("FactionMenu_Info", "FactionMenu_Info");
	cef_subscribe("FactionInfo_Close", "FactionInfo_Close");
	cef_subscribe("FactionMenu_Close", "FactionMenu_Close");
	return Y_HOOKS_CONTINUE_RETURN_1;
}