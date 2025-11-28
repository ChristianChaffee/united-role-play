
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook stock OnMySQLConnect(){
	static const fmt_query[] = "DELETE FROM reports WHERE `timestamp` + (86400 * 30) < %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, gettime());
	mysql_tquery(mysql, query);
	return continue();
}

hook stock SetPlayerSpawn(playerid, spawn_type){
	Report:CheckPlayerAnswers(playerid);
	return continue(playerid, spawn_type);
}
