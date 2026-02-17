
#define Hud:%0(%1) 						Hd_%0(%1)

//==============================================================================
#define HUD_UPDATE_TIME 				200

//==============================================================================
enum player_hud_info_struct
{
	Float:hud_health,
	Float:hud_armour,
	hud_money,
	hud_weaponid,
	hud_ammo,
	hud_wanted
}
new player_hud_info[MAX_PLAYERS][player_hud_info_struct];

new const player_hud_info_clear[player_hud_info_struct] = {
	0.0,
	0.0,
	0,
	0,
	0,
	0
};