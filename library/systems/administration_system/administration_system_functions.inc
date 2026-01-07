
stock Admin:SendAdminMessage(color, const str[]){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(!GetPlayerAdminData(i, pa_id)) continue;
		if(!GetPlayerAdminData(i, pa_logged)) continue;

		SendClientMessage(i, color, str);
	}
}

stock Admin:SendAdminActionMessage(color, const str[]) { SendClientMessageToAll(color, str); }

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

stock Admin:ShowCreateBusinessPrice(playerid){
	Dialog_Open(playerid, Dialog:D_ADMIN_BIZ_CREATE_PRICE, DIALOG_STYLE_INPUT,
		"Гос. стоимость",
		""cl_blue"- "cl_white"Введите государственную стоимость создаваемого бизнеса",
		"Далее", "Назад");
	return true;
}

stock Admin:ShowCreateBusinessName(playerid){
	Dialog_Open(playerid, Dialog:D_ADMIN_BIZ_CREATE_NAME, DIALOG_STYLE_INPUT,
		"Название",
		""cl_blue"- "cl_white"Введите название создаваемого бизнеса",
		"Далее", "Назад");
	return true;
}

stock Admin:ShowBusinessCreateAccept(playerid){
	new business_name[64];
	GetPVarString(playerid, "Admin_BusinessCreateName", business_name, sizeof(business_name));

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"Название бизнеса: "cl_white"%s\n\
		"cl_blue"Тип бизнеса: "cl_white"%s\n\
		"cl_blue"Цена бизнеса: "cl_white"%d$\n\n\
		\\c"cl_grey"[Вы действительно хотите создать данный бизнес?]",
		business_name, business_type_names[GetPVarInt(playerid, "Admin_BusinessCreateType")],
		GetPVarInt(playerid, "Admin_BusinessCreatePrice"));

	Dialog_Open(playerid, Dialog:D_ADMIN_BIZ_CREATE_ACCEPT, DIALOG_STYLE_MSGBOX,
		"Создание бизнеса", big_string,
		"Создать", "Назад");
	big_string[0] = EOS;
	return true;
}