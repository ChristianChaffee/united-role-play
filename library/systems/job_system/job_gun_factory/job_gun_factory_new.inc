
/*
Используемые PVar`ы:

	JobGunFactory_Stage
	JobGunFactory_MachineIndex
	JobGunFactory_Timer
	JobGunFactory_Count

*/

//==============================================================================
enum
{
	JOB_GF_STAGE_NONE,
	JOB_GF_STAGE_TO_MATERIALS,
	JOB_GF_STAGE_TO_MACHINE,
	JOB_GF_STAGE_TO_WAREHOUSE
}

//==============================================================================
new const Float:job_gun_factory_machine_pos[4][4] = {
	{1378.54, -22.83, 1000.99, 180.0},
	{1378.54, -24.74, 1000.99, 0.0},
	{1378.54, -28.49, 1000.99, 0.0},
	{1378.54, -26.58, 1000.99, 180.0}
};

new const job_gun_factory_guns_model_id[4] = {
	355, 356, 349, 357 
};

//==============================================================================
#define JOB_GUN_FACTORY_ACTOR_ID 					12
#define JOB_GUN_FACTORY_PAY 						15
#define JOB_GUN_FACTORY_MATERIALS_POS 				1384.92, -17.95, 1000.99
#define JOB_GUN_FACTORY_WAREHOUSE_POS 				1391.65, -21.86, 1000.99
#define JOB_GUN_FACTORY_CENTRAL_POINT 				1383.99, -21.40, 1000.99
#define JOB_GUN_FACTORY_VW 							5
#define JOB_GUN_FACTORY_INT 						7
#define JOB_GUN_FACTORY_TIMER 						10