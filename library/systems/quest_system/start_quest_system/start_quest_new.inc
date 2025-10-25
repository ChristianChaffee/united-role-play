
#define StartQuest:%0(%1)						StrtQst_%0(%1)

//==============================================================================
#define START_QUESTS_COUNT 						2

#define GetStartQuestData(%0,%1)  				start_quest_info[%0][%1]

#define GetPlayerStartQuestData(%0,%1,%2)  		start_quest_player_info[%0][%1][%2]
#define SetPlayerStartQuestData(%0,%1,%2,%3)  	start_quest_player_info[%0][%1][%2] = %3

//==============================================================================
enum
{
	SQ_STATUS_NONE,
	SQ_STATUS_IN_PROGRESS,
	SQ_STATUS_COMPLETED,
}

//==============================================================================
enum start_quest_info_struct
{
	sq_id,
	sq_name[32],
	sq_description[256],
}

new start_quest_info[START_QUESTS_COUNT + 1][start_quest_info_struct];

enum start_quest_player_info_struct
{
	sqp_id,
	sqp_account_id,
	sqp_status,
	sqp_quest_id,
}
new start_quest_player_info[MAX_PLAYERS][START_QUESTS_COUNT + 1][start_quest_player_info_struct];