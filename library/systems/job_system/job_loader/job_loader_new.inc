
/*
Используемые PVar`ы:

	JobLoader_Stage
	JobLoader_BoxesCount

*/

#define JOB_LOADER_ACTOR_ID 							8
#define JOB_LOADER_MAX_PLAYERS 							30
#define JOB_LOADER_STATIC_PAY 							5
#define JOB_LOADER_CENTRAL_POINT 						2191.27, -2277.48, 13.55

//==============================================================================
enum
{
	JOB_LOADER_STAGE_NONE,
	JOB_LOADER_STAGE_TO_BOX,
	JOB_LOADER_STAGE_TO_WAREHOUSE,
}

//==============================================================================
new const Float:job_loader_boxes_pos[3][3] = {
	{2214.02, -2283.73, 14.76},
	{2215.82, -2285.53, 14.76},
	{2218.27, -2287.98, 14.76}
};

new const Float:job_loader_warehouse_pos[][3] = {
	{2173.16, -2248.59, 13.30},
	{2165.35, -2254.91, 13.30},
	{2159.17, -2247.29, 13.30},
	{2162.03, -2236.19, 13.30},
	{2157.54, -2243.35, 13.30},
	{2151.78, -2242.56, 13.30},
	{2149.76, -2250.27, 13.30},
	{2161.12, -2254.83, 13.30},
	{2158.60, -2261.48, 13.30},
	{2150.20, -2255.84, 13.30}
};