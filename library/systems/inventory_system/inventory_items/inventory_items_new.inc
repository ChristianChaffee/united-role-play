
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
#define ITEM_FRIED_POTATO_ID 					25
#define ITEM_NUGGETS_ID 						26
#define ITEM_HOT_DOG_ID 						27
#define ITEM_MED_KIT_ID 						28
#define ITEM_PHONE_ID 							29
#define ITEM_SIM_CARD_ID 						30
#define ITEM_MASK_ID 							31
#define ITEM_CAMERA_ID 							32
#define ITEM_FLOWERS_ID 						33
#define ITEM_CANISTER_ID 						34
#define ITEM_REPAIR_ID 							35
#define ITEM_SKIN_ID 							36

#define ITEM_DEAGLE_ID 							37
#define ITEM_SHOTGUN_ID 						38
#define ITEM_MP5_ID 							39
#define ITEM_M4_ID 								40
#define ITEM_DEAGLE_AMMO_ID 					41
#define ITEM_SHOTGUN_AMMO_ID 					42
#define ITEM_MP5_AMMO_ID 						43
#define ITEM_M4_AMMO_ID 						44
#define ITEM_PEPPER_SPRAY_ID 					45
#define ITEM_AMMO_ID 							46

#warning TODO: Add image to ITEM_CANISTER_ID
#warning TODO: Add image to ITEM_REPAIR_ID
#warning TODO: Add image to ITEM_SKIN_ID
#warning TODO: Add image to ITEM_DEAGLE_ID
#warning TODO: Add image to ITEM_SHOTGUN_ID
#warning TODO: Add image to ITEM_MP5_ID
#warning TODO: Add image to ITEM_M4_ID
#warning TODO: Add image to ITEM_DEAGLE_AMMO_ID
#warning TODO: Add image to ITEM_SHOTGUN_AMMO_ID
#warning TODO: Add image to ITEM_MP5_AMMO_ID
#warning TODO: Add image to ITEM_M4_AMMO_ID
#warning TODO: Add image to ITEM_PEPPER_SPRAY_ID
#warning TODO: Add image to ITEM_AMMO_ID


#define MAX_INVENTORY_ITEMS 					46
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