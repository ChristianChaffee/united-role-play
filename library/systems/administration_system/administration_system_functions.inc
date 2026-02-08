
stock Admin:SendAdminMessage(color, const str[]){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(!GetPlayerAdminData(i, pa_id)) continue;
		if(!GetPlayerAdminData(i, pa_logged)) continue;

		SendClientMessage(i, color, str);
	}
}

stock Admin:SendAdminActionMessage(color, const str[]) { SendClientMessageToAll(color, str); }

stock Admin:DeleteFractionPVars(playerid){
	DeletePVar(playerid, "Admin_FracCreateName");
	DeletePVar(playerid, "Admin_FracCreateType");
	DeletePVar(playerid, "Admin_FracCreateColor");
	return true;
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

stock Admin:ShowCreateHousePrice(playerid){
	Dialog_Open(playerid, Dialog:D_ADMIN_HOUSE_CREATE_PRICE, DIALOG_STYLE_INPUT,
		"Гос. стоимость",
		""cl_blue"- "cl_white"Введите государственную стоимость создаваемого дома",
		"Далее", "Назад");
	return true;
}

stock Admin:ShowCreateHouseAccept(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"Класс дома: "cl_white"%s\n\
		"cl_blue"Цена дома: "cl_white"%d$\n\n\
		\\c"cl_grey"[Вы действительно хотите создать данный дом?]",
		house_class_names[GetPVarInt(playerid, "Admin_HouseCreateType")],
		GetPVarInt(playerid, "Admin_HouseCreatePrice"));

	Dialog_Open(playerid, Dialog:D_ADMIN_HOUSE_CREATE_ACCEPT, DIALOG_STYLE_MSGBOX,
		"Создание дома", big_string,
		"Создать", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Admin:ShowFractionMenu(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_green"- Создать новую фракцию");

	for(new i, count; i < MAX_FRACTIONS; i++){
		if(!GetFractionData(i, f_id)) continue;

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_grey"%d. "cl_white"%s "cl_grey"[%s]",
			big_string, count,
			GetFractionData(i, f_name),
			fraction_type_names[GetFractionData(i, f_type)]);

		player_listitems[playerid][count] = i;
	}

	Dialog_Open(playerid, Dialog:D_ADMIN_FRACMENU, DIALOG_STYLE_LIST,
		"Управление фракциями", big_string,
		"Далее", "Закрыть");

	big_string[0] = EOS;
	return true;
}

stock Admin:ShowCreateFraction(playerid){
	new fraction_name_str[64], fraction_color_str[32], fraction_create_str[32];

	if(!GetPVarType(playerid, "Admin_FracCreateName")){
		format(fraction_name_str, sizeof(fraction_name_str), "-");
	}
	else{
		GetPVarString(playerid, "Admin_FracCreateName", fraction_name_str, sizeof(fraction_name_str));
	}

	if(!GetPVarType(playerid, "Admin_FracCreateColor")){
		format(fraction_color_str, sizeof(fraction_color_str), "-");
	}
	else{
		format(fraction_color_str, sizeof(fraction_color_str),
			"%s%s",
			color_info[GetPVarInt(playerid, "Admin_FracCreateColor")][color_str],
			color_info[GetPVarInt(playerid, "Admin_FracCreateColor")][color_name]);
	}

	//-----
	if(!GetPVarType(playerid, "Admin_FracCreateName") || !GetPVarType(playerid, "Admin_FracCreateType")
		|| !GetPVarType(playerid, "Admin_FracCreateColor")){

		format(fraction_create_str, sizeof(fraction_create_str), "");
	}
	else{
		format(fraction_create_str, sizeof(fraction_create_str), "\n"cl_green"- Создать фракцию");
	}

	//------
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Параметр\t"cl_grey"Значение\n\
		"cl_grey"1. "cl_white"Название фракции\t%s\n\
		"cl_grey"2. "cl_white"Тип фракции\t%s\n\
		"cl_grey"3. "cl_white"Цвет фракции\t%s%s",
		fraction_name_str,
		(!GetPVarType(playerid, "Admin_FracCreateType")) ? ("-") : (fraction_type_names[GetPVarInt(playerid, "Admin_FracCreateType")]),
		fraction_color_str, fraction_create_str);

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_CREATE, DIALOG_STYLE_TABLIST_HEADERS,
		"Создание фракции", big_string,
		"Выбрать", "Назад");
	//-----

	big_string[0] = EOS;
	return true;
}

stock Admin:ShowCreateFractionName(playerid, error_text[] = ""){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите название создаваемой фракции\n\
		%s",
		error_text);

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_CREATE_NAME, DIALOG_STYLE_INPUT,
		"Создание фракции", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Admin:ShowCreateFractionType(playerid){
	big_string[0] = EOS;

	for(new i; i < FRACTIONS_TYPES_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s",
			big_string, i + 1,
			fraction_type_names[i]);
	}

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_CREATE_TYPE, DIALOG_STYLE_LIST,
		"Выберите тип фракции", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Admin:ShowCreateFractionColor(playerid){
	big_string[0] = EOS;

	for(new i; i < COLORS_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			%s%s",
			big_string, color_info[i][color_str], color_info[i][color_name]);
	}

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_CREATE_COLOR, DIALOG_STYLE_LIST,
		"Выберите цвет фракции", big_string,
		"Далее", "Назад");
	big_string[0] = EOS;
	return true;
}