
DialogResponse:D_REGISTRATION_PASSWORD(playerid, response, listitem, inputtext[]){
	if(!response){
		Kick(playerid);
		return false;
	}
	else{
		new Regex:password_regex = Regex_New("^[-A-Za-z0-9!@#$^&*()_+[\\];\\\\<>,.\\/?~]{4,32}$");
		if(!Regex_Check(inputtext, password_regex)){
			SendErrorMessage(playerid, "Пароль должен состоят от 4 до 32 латинских букв и символов!");
			Registration:ShowRegPassword(playerid);
			Regex_Delete(password_regex);
			return false;
		}
		Regex_Delete(password_regex);

		format(GetPlayerData(playerid, p_password), 32, "%s", inputtext);
		Registration:ShowRegEmail(playerid);
		return true;
	}
}

DialogResponse:D_REGISTRATION_EMAIL(playerid, response, listitem, inputtext[]){
	if(!response){
		Registration:ShowRegCharacter(playerid);
		return false;
	}
	else{
		static const fmt_query[] = "SELECT id FROM accounts WHERE email = '%s' LIMIT 1";
		new query[sizeof(fmt_query) + (-2 + 32)];
		mysql_format(mysql, query, sizeof(query), fmt_query, inputtext);
		mysql_tquery(mysql, query, "CheckAccountEmail", "is", playerid, inputtext);
		return true;
	}
}

DialogResponse:D_REGISTRATION_CHARACTER(playerid, response, listitem, inputtext[]){
	if(!response){
		ResetPlayerStringData(playerid, p_email);
		Registration:ShowRegEmail(playerid);
		return false;
	}
	else{
		switch(listitem){
			case 0: return Registration:ShowRegCharacter(playerid);
			case 1: return Registration:ShowRegAge(playerid);
			case 2: return Registration:ShowRegNationality(playerid);
			case 3:{
				SetPlayerData(playerid, p_sex, !GetPlayerData(playerid, p_sex));
				Registration:ShowRegCharacter(playerid);
				return true;
			}
			case 4:{
				SetPlayerSkin(playerid, registration_skins[GetPlayerData(playerid, p_sex)][0]);
				
				SetPlayerPos(playerid, REGISTRATION_SKIN_POS);
				SetPlayerFacingAngle(playerid, REGISTRATION_SKIN_POS_ANGLE);
				SetPlayerVirtualWorld(playerid, REGISTRATION_SKINS_VW);
				SetPlayerInterior(playerid, REGISTRATION_SKINS_INT);
				SetPlayerCameraPos(playerid, REGISTRATION_SKIN_CAMERA_POS);
				SetPlayerCameraLookAt(playerid, REGISTRATION_SKIN_POS);
				TogglePlayerControllable(playerid, false);
				
				SetPVarInt(playerid, "Registration_SkinIndex", 0);
				SkinSelect:ShowInterface(playerid);
				return true;
			}
			case 5:{
				if(!GetPlayerData(playerid, p_age)){
					SendErrorMessage(playerid, "Выберите возраст персонажа!");
					Registration:ShowRegCharacter(playerid);
					return false;
				}
				if(!GetPlayerData(playerid, p_nationality)){
					SendErrorMessage(playerid, "Выберите гражданство персонажа!");
					Registration:ShowRegCharacter(playerid);
					return false;
				}
				if(!GetPlayerData(playerid, p_skin)){
					SendErrorMessage(playerid, "Выберите внешний вид персонажа!");
					Registration:ShowRegCharacter(playerid);
					return false;
				}
				Registration:FinishRegistration(playerid);
				return true;
			}
		}
		return true;
	}
}

DialogResponse:D_REGISTRATION_AGE(playerid, response, listitem, inputtext[]){
	if(strval(inputtext) < CHARACTER_MIN_AGE || strval(inputtext) > CHARACTER_MAX_AGE){
		Registration:ShowRegAge(playerid);
		return false;
	}

	SetPlayerData(playerid, p_age, strval(inputtext));
	Registration:ShowRegCharacter(playerid);
	return true;
}

DialogResponse:D_REGISTRATION_NATIONALITY(playerid, response, listitem, inputtext[]){
	SetPlayerData(playerid, p_nationality, listitem + 1);
	Registration:ShowRegCharacter(playerid);
	return true;
}

DialogResponse:D_LOGIN_PASSWORD(playerid, response, listitem, inputtext[]){
	if(!response){
		Kick(playerid);
		return false;
	}
	else{
		static const fmt_query[] = "SELECT * FROM accounts WHERE name = '%s' AND password = MD5('%s')";
		new query[sizeof(fmt_query) - 2 + 24 - 2 + 32];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetPlayerData(playerid, p_name), inputtext);
		mysql_tquery(mysql, query, "CheckAccountPassword", "i", playerid);
		return true;
	}
}