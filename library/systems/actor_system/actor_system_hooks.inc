
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM actors", "LoadActors");
	return Y_HOOKS_CONTINUE_RETURN_1;
}