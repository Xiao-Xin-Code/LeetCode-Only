#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "Extension.h"

namespace Solution {
	class Solution {
	public:
		//两数之和 https://leetcode.com/problems/two-sum/
		std::vector<int> twoSum(std::vector<int>& nums, int target) {
			if(nums.size()<=1) return {};	
			std::unordered_map<int, int> maps;
			for (int i = 0;i < nums.size();++i) {
				int diff = target - nums[i];
				auto it = maps.find(diff);
				if(it != maps.end()){
					return {it->second,i};
				}
				maps[nums[i]] = i; 
			}
			return {};
		}

		//两数相加 https://leetcode.com/problems/add-two-numbers/
		ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
			if(l1 == nullptr && l2 == nullptr) return nullptr;
			if(l1 == nullptr) return l2;
			if(l2 == nullptr) return l1;
			int carry = 0;
			ListNode* head = nullptr;
			ListNode* tail = nullptr;
			while(l1 || l2 || carry != 0){
				int sum = carry;
				if(l1){
					sum += l1->val;
					l1 = l1->next;
				}
				if(l2){
					sum += l2->val;
					l2 = l2->next;
				}
				carry = sum/10;
				sum %= 10;
				if(head == nullptr){
					head = new ListNode(sum);
					tail = head;
				}else{
					tail->next = new ListNode(sum);
					tail = tail->next;
				}
			}
			return head;
		}

		//无重复字符的最长子串 https://leetcode.com/problems/longest-substring-without-repeating-characters/
		int lengthOfLongestSubstring(std::string s) {
			if(s.empty()) return 0;
			if(s.size() == 1) return 1;
			std::unordered_map<char, int> charindex;
			int left = 0, right = 0;
			int max_length = 0;
			for(int i = 0; i < s.size(); ++i){
				auto it = charindex.find(s[i]);
				if(it != charindex.end() && it->second >= left){
					left = it->second + 1;
				}
				charindex[s[i]] = i;
				right = i;
				max_length = std::max(max_length, right - left + 1);
			}
			return max_length;
		}

		// 两个有序数组的中位数 https://leetcode.com/problems/median-of-two-sorted-arrays/
		double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
			return 0.0;
		}

		
	};
}


