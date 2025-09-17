
forward CheckPlayerRegistration(playerid);
public CheckPlayerRegistration(playerid){
	new rows;
	cache_get_row_count(rows);

	if(rows){
		Registration:ShowLogPassword(playerid);
	}
	else{
		Registration:ShowRegPassword(playerid);
	}
}

forward CheckAccountPassword(playerid);
public CheckAccountPassword(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows){
		SendErrorMessage(playerid, "Пароль введён не верно!");
		Registration:ShowLogPassword(playerid);
		return false;
	}
	else{
		Registration:LoadPlayer(playerid, cache_save());
		return true;
	}
}

forward CheckAccountEmail(playerid, email[]);
public CheckAccountEmail(playerid, email[]){
	new rows;
	cache_get_row_count(rows);

	if(rows){
		SendErrorMessage(playerid, "Данный адрес электронной почты уже занят другим игроком!");
		Registration:ShowRegEmail(playerid);
		return false;
	}
	else{
		for(new i, j = GetPlayerPoolSize(); i <= j; i++){
			if(!strcmp(email, GetPlayerData(i, p_email)) && i != playerid){
				SendErrorMessage(playerid, "Данный адрес электронной почты уже занят другим игроком!");
				Registration:ShowRegEmail(playerid);
				return false;
			}
		}
	}

	format(GetPlayerData(playerid, p_email), 32, "%s", email);
	Registration:ShowRegCharacter(playerid);
	return true;
}

forward CreatePlayerAccount(playerid);
public CreatePlayerAccount(playerid){
	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Игрок %s[%d] зарегистрировался на проекте!",
		GetPlayerData(playerid, p_name), playerid);
	SendClientMessageToAll(COLOR_YELLOW, small_string);
	small_string[0] = EOS;

	SetPlayerSpawn(playerid, SPAWN_TYPE_STANDART);
}