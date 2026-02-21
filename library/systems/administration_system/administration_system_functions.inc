
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
			big_string, count + 1,
			GetFractionData(i, f_name),
			fraction_type_names[GetFractionData(i, f_type)]);

		player_listitems[playerid][count] = i;
		count++;
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
		"cl_blue"1. "cl_white"Название фракции\t%s\n\
		"cl_blue"2. "cl_white"Тип фракции\t%s\n\
		"cl_blue"3. "cl_white"Цвет фракции\t%s%s",
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

stock Admin:ShowFractionInfo(playerid, fraction_index){
	if(fraction_index < 0 || fraction_index >= MAX_FRACTIONS){
		SendErrorMessage(playerid, "Произошла ошибка.");
		return false;
	}

	if(!GetFractionData(fraction_index, f_leader_account_id)){
		Fraction:ShowFractionInfoDialog(playerid, fraction_index, "Отсутствует");
	}
	else{
		static const fmt_query[] = "SELECT f.*, a.name AS leader_name FROM fractions f JOIN accounts a ON f.leader_account_id = a.id WHERE f.id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, GetFractionData(fraction_index, f_id));
		mysql_tquery(mysql, query, "ShowFractionInfo", "ii", playerid, fraction_index);
	}

	return true;
}

stock Fraction:ShowFractionInfoDialog(playerid, fraction_index, leader_name[]){
	if(fraction_index < 0 || fraction_index >= MAX_FRACTIONS) return false;

	new color_index;
	for(new i; i < COLORS_COUNT; i++){
		if(color_info[i][color_code] == GetFractionData(fraction_index, f_color_value)){
			color_index = i;
			break;
		}
	}

	SetPVarInt(playerid, "Admin_FractionIndex", fraction_index);

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Параметр\t"cl_grey"Значение\n\
		"cl_blue"Название фракции\t"cl_white"%s\n\
		"cl_blue"Тэг фракции\t"cl_white"%s\n\
		"cl_blue"Тип фракции\t"cl_white"%s\n\
		"cl_blue"Лидер фракции\t"cl_white"%s\n\
		"cl_blue"Цвет фракции\t"cl_white"%s%s\n\
		"cl_blue"- "cl_white"Управление рангами\n\
		"cl_blue"- "cl_white"Изменить место спавна фракции\n\
		"cl_blue"- "cl_white"Изменить место раздевалки фракции\n\
		"cl_blue"- "cl_white"Изменить место оружейной фракции",
		GetFractionData(fraction_index, f_name), GetFractionData(fraction_index, f_tag),
		fraction_type_names[GetFractionData(fraction_index, f_type)],
		leader_name,
		GetFractionData(fraction_index, f_color_str), color_info[color_index][color_name]);

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_INFO, DIALOG_STYLE_TABLIST_HEADERS,
		GetFractionData(fraction_index, f_name), big_string,
		"Выбрать", "Закрыть");

	big_string[0] = EOS;
	return true;
}

stock Admin:ShowChangeFracName(playerid, error_text[] = ""){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите новое название для фракции "cl_grey"\"%s\"\n\
		"cl_grey"Название должно быть от 5 до 64 символов.\n\n%s",
		GetFractionData(GetPVarInt(playerid, "Admin_FractionIndex"), f_name), error_text);

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_CHANGE_NAME, DIALOG_STYLE_INPUT,
		"Название фракции", big_string,
		"ИЗменить", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Admin:ShowChangeFracTag(playerid, error_text[] = ""){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите тэг для фракции "cl_grey"\"%s\"\n\
		"cl_grey"Тэг должен быть от 2 до 6 символов.\n\n%s",
		GetFractionData(GetPVarInt(playerid, "Admin_FractionIndex"), f_name), error_text);

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_CHANGE_TAG, DIALOG_STYLE_INPUT,
		"Тэг фракции", big_string,
		"ИЗменить", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Admin:ShowChangeFracType(playerid){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	big_string[0] = EOS;
	for(new i; i < FRACTIONS_TYPES_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s",
			big_string, i + 1, fraction_type_names[i]);
	}

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_CHANGE_TYPE, DIALOG_STYLE_LIST,
		"Тип фракции", big_string,
		"ИЗменить", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Admin:ShowAcceptUninviteLeader(playerid){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Вы действительно хотите уволить лидера фракции "cl_grey"\"%s\""cl_white"?",
		GetFractionData(GetPVarInt(playerid, "Admin_FractionIndex"), f_name));

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_ACCEPT_LEADER, DIALOG_STYLE_MSGBOX,
		"Снять лидера фракции", big_string,
		"Уволить", "Назад");
	
	big_string[0] = EOS;
	return true;
}

stock Admin:ShowChangeFracColor(playerid){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	big_string[0] = EOS;

	for(new i; i < COLORS_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			%s%s",
			big_string, color_info[i][color_str], color_info[i][color_name]);
	}

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_CHANGE_COLOR, DIALOG_STYLE_LIST,
		"Выберите цвет", big_string,
		"Выбрать", "Назад");
	
	big_string[0] = EOS;
	return true;
}

stock Admin:ShowRanksMenu(playerid){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_RANKS_MENU, DIALOG_STYLE_LIST,
		"Управление рангами", 
		""cl_blue"- "cl_white"Список рангов\n\
		"cl_blue"- "cl_white"Изменить максимальный ранг",
		"Выбрать", "Назад");
	return true;
}

stock Admin:ShowFracRankList(playerid){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	big_string[0] = EOS;
	for(new i; i < GetFractionData(GetPVarInt(playerid, "Admin_FractionIndex"), f_ranks_count); i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s",
			big_string, i + 1,
			fraction_ranks[GetPVarInt(playerid, "Admin_FractionIndex")][i]);
	}

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_RANKS_LIST, DIALOG_STYLE_LIST,
		"Список рангов", big_string,
		"Изменить", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Admin:ShowFracChangeMaxRank(playerid, error_text[] = ""){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите новый максимальный ранг для фракции "cl_grey"\"%s\"\n\
		"cl_grey"Максимальный ранг фракции должен быть от 5 до "#MAX_FRACTION_RANKS"\n\n%s",
		GetFractionData(GetPVarInt(playerid, "Admin_FractionIndex"), f_name), error_text);

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_MAX_RANK, DIALOG_STYLE_INPUT,
		"Максимальный ранг", big_string,
		"Изменить", "Назад");
	big_string[0] = EOS;

	return true;
}

stock Admin:ShowChangeFracRank(playerid, error_text[] = ""){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;
	if(!GetPVarType(playerid, "Admin_FractionRank")) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите новое название для "cl_grey"\"%s\" (Ранг %d)\n\
		"cl_grey"Название ранга должно быть от 2 до 32 символов.\n\n%s",
		fraction_ranks[GetPVarInt(playerid, "Admin_FractionIndex")][GetPVarInt(playerid, "Admin_FractionRank")],
		GetPVarInt(playerid, "Admin_FractionRank") + 1, error_text);

	Dialog_Open(playerid, Dialog:D_ADMIN_FRAC_CHANGE_RANK, DIALOG_STYLE_INPUT,
		"Изменить ранг", big_string,
		"Изменить", "Назад");
	big_string[0] = EOS;

	return true;
}

stock Admin:DestroyCreateFracVehPVars(playerid){
	DeletePVar(playerid, "Admin_AddFracVehFractionIndex");
	DeletePVar(playerid, "Admin_AddFracVehVehicleid");
	DeletePVar(playerid, "Admin_AddFracVehColor1");
	DeletePVar(playerid, "Admin_AddFracVehColor2");
	return true;
}