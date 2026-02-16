
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM fractions", "LoadFractions", "i", -1);
	mysql_tquery(mysql, "SELECT * FROM fraction_skins", "LoadFractionSkins", "i", -1);
	return Y_HOOKS_CONTINUE_RETURN_1;
}