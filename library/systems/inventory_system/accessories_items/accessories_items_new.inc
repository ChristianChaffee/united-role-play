
#define MAX_ACCESSORIES_ITEMS 					1
#define PLAYER_ACCESSORIES_SLOTS_COUNT 			6

#define GetAccessoryItemData(%0,%1) 			accessories_items_info[%0][%1]
#define SetAccessoryItemData(%0,%1,%2) 			accessories_items_info[%0][%1] = %2

#define GetPlayerAccessoryData(%0,%1,%2)  		player_accessories_items_info[%0][%1][%2]
#define SetPlayerAccessoryData(%0,%1,%2,%3)  	player_accessories_items_info[%0][%1][%2] = %3

//==============================================================================
enum accessories_items_info_struct
{
	acc_id,
	acc_item_id,
	acc_bone,
	Float:acc_offset_x,
	Float:acc_offset_y,
	Float:acc_offset_z,
	Float:acc_offset_rot_x,
	Float:acc_offset_rot_y,
	Float:acc_offset_rot_z,
	Float:acc_scalse_x,
	Float:acc_scalse_y,
	Float:acc_scalse_z,
}
new accessories_items_info[MAX_ACCESSORIES_ITEMS + 1][accessories_items_info_struct];


//==============================================================================
enum player_accessories_items
{
	player_acc_id,
	player_acc_item_id,
	player_acc_slot_index,
}
new player_accessories_items_info[MAX_PLAYERS][PLAYER_ACCESSORIES_SLOTS_COUNT][player_accessories_items];