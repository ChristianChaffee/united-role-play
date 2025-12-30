
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================

hook stock SecondTimerStock(){
	gettime(global_hour, global_minute, global_second);

	if(global_minute == 0 && global_second == 0){
		PayDay(global_hour);
	}
	return continue();
}