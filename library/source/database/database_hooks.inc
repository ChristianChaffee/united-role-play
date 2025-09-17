
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){

	mysql = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_BASE);

	switch(mysql_errno()){
		case 0:{
			print("~ Подключение к базе данных \""#MYSQL_BASE"\" успешно");
		}
		default:{
			print("~ Подключение к базе данных \""#MYSQL_BASE"\" НЕ успешно");
		}
	}
	mysql_set_charset("cp1251");
	return Y_HOOKS_CONTINUE_RETURN_1;
}