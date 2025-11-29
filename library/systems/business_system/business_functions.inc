
stock Business:GetBusinessIndexByVW(vw){
	for(new i; i < MAX_BUSINESS; i++){
		if(!GetBusinessData(i, b_id)) continue;
		if(GetBusinessData(i, b_id) == vw) return i;
	}
	return -1;
}