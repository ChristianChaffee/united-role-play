
#if defined _chathider_included
  #endinput
#endif
#define _chathider_included

forward Chathider_OnKeyPressed(playerid, key);
forward OnUserChangeLayout(playerid, layout[]);

native SetChatStatus(playerid, bool:status);
native GetUserLayout(playerid, string_return[], size = sizeof(string_return));