
#if defined _chathider_included
  #endinput
#endif
#define _chathider_included

forward Chathider_OnKeyPressed(playerid, key);
forward OnUserChangeLayout(playerid, layout[]);
forward OnPlayerEnterAFK(playerid);
forward OnPlayerExitAFK(playerid);

native SetChatStatus(playerid, bool:status);
native GetUserLayout(playerid, string_return[], size = sizeof(string_return));
native QuitGameForPlayer(playerid);
native IsPlayerAFK(playerid);