#include <vector>

//039-
void combinationSumHelper(std::vector<int>& candidates,int target,int index,std::vector<int> curResult,std::vector<std::vector<int>>& result){
    for(int i = index; i < candidates.size(); ++i){
	    if(target - candidates[i] < 0) break;
		curResult.push_back(candidates[i]);
		if(target - candidates[i] == 0){
			result.push_back(curResult);
		}
		else{
			combinationSumHelper(candidates,target - candidates[i],i,curResult,result);
		}
		curResult.pop_back();
	}
}
    
//040-
void combinationSum2Helper(std::vector<int>& candidates,int target,int index,std::vector<int> curResult,std::vector<std::vector<int>>& result){
	for(int i = index; i < candidates.size(); ++i){
		if(i > index && candidates[i] == candidates[i - 1]) continue;
		if(target - candidates[i] < 0) break;
		curResult.push_back(candidates[i]);
		if(target - candidates[i] == 0){
			result.push_back(curResult);
		}
		else{
			combinationSum2Helper(candidates,target - candidates[i],i + 1,curResult,result);
		}
		curResult.pop_back();
	}
}