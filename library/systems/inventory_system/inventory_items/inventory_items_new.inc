
#define ITEM_LETTER_ID 							1
#define ITEM_MONEY_ID 							2
#define ITEM_JOURNEY_BACKPACK 					3
#define ITEM_BANK_CARD 							4
#define ITEM_SUGAR_DRUG_ID 						5
#define ITEM_TOMATO_ID 							6
#define ITEM_ORANGE_ID 							7
#define ITEM_RED_APPLE_ID 						8
#define ITEM_GREEN_APPLE_ID 					9
#define ITEM_CHEESBURGER_ID 					10
#define ITEM_HAMBURGER_ID 						11
#define ITEM_KEBAB_ID 							12
#define ITEM_BANAN_ID 							13
#define ITEM_PIZZA_ID 							14
#define ITEM_COFFEE_ID 							15
#define ITEM_MILK_ID 							16
#define ITEM_ORANGE_JUICE_ID 					17
#define ITEM_APPLE_JUICE_ID 					18
#define ITEM_BEER_ID 							19
#define ITEM_WHISKEY_ID 						20
#define ITEM_COGNAC_ID 							21
#define ITEM_VODKA_ID 							22
#define ITEM_SPRUNK_ID 							23
#define ITEM_ECOLA_ID 							24


#define MAX_INVENTORY_ITEMS 					24
#define MAX_INVENTORY_SLOTS  					27

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
	item_action_text[128],
	Float:item_rot_x,
	Float:item_rot_y,
	Float:item_rot_z,
	Float:item_scale,
	item_static_value_one,
	item_static_value_two,
	item_img_url[64]
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