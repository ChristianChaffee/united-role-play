
stock Admin:SendAdminMessage(color, const str[]){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(!GetPlayerAdminData(i, pa_id)) continue;
		if(!GetPlayerAdminData(i, pa_logged)) continue;

		SendClientMessage(i, color, str);
	}
}

//==============================================================================
stock Admin:ShowCreatePassword(playerid){
	if(!GetPlayerAdminData(playerid, pa_id)) return false;

	Dialog_Open(playerid, Dialog:D_ADMIN_CREATE_PASSWORD, DIALOG_STYLE_INPUT,
		"Панель администратора",
		""cl_blue"- "cl_white"Придумайте пароль для доступа к панели администратора.\n\n\
		"cl_grey"Примечание: "cl_white"пароль должен состоять от 8 до 32 латинских букв и символов.",
		"Далее", "Закрыть");
	return true;
}

stock Admin:ShowLogin(playerid){
	if(!GetPlayerAdminData(playerid, pa_id)) return false;

	Dialog_Open(playerid, Dialog:D_ADMIN_LOGIN, DIALOG_STYLE_INPUT,
		"Панель администратора",
		""cl_blue"- "cl_white"Введите пароль от панели администратора.",
		"Ввод", "Закрыть");
	return true;
}