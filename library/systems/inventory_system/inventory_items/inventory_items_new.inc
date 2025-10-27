
#define ITEM_LETTER_ID 							1
#define ITEM_MONEY_ID 							2
#define ITEM_JOURNEY_BACKPACK 					3
#define ITEM_BANK_CARD 							4
#define ITEM_SUGAR_DRUG_ID 						5

#define MAX_INVENTORY_ITEMS 					5
#define MAX_INVENTORY_SLOTS  					75

#define GetInventoryItemData(%0,%1) 			inventory_item_info[%0][%1]

#define GetPlayerInventoryItemData(%0,%1,%2) 	inventory_player[%0][%1][%2]
#define SetPlayerInventoryItemData(%0,%1,%2,%3) inventory_player[%0][%1][%2] = %3

//==============================================================================
enum inventory_item_info_struct
{
	item_id,
	item_name[32],
	item_description[256],
	item_model_id,
	Float:item_rot_x,
	Float:item_rot_y,
	Float:item_rot_z,
	Float:item_scale,
}
new inventory_item_info[MAX_INVENTORY_ITEMS + 1][inventory_item_info_struct];

//==============================================================================
enum inventory_player_struct
{
	inv_player_id,
	inv_player_account_id,
	inv_player_item_id,
	inv_player_item_count,
	inv_player_item_slot,
	inv_player_value_one,
	inv_player_value_two,
	inv_player_value_str[256],
	inv_player_can_drop,
}
new inventory_player[MAX_PLAYERS][MAX_INVENTORY_SLOTS][inventory_player_struct];