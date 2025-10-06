
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	CreateDynamicMapIcon(BANK_MAP_ICON_POS, BANK_MAP_ICON_TYPE, -1, 0, 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}