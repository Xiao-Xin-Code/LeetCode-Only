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

		//两个有序数组的中位数 https://leetcode.com/problems/median-of-two-sorted-arrays/
		double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
			if(nums1.empty() && nums2.empty()) return 0.0;
			if(nums1.size() > nums2.size()) findMedianSortedArrays(nums2, nums1);
			int m = nums1.size();
			int n = nums2.size();
			int totalLeft = (m + n + 1) / 2;
			int left = 0, right = m;
			while(left <= right){
				int i = (left + right) / 2;
				int j = totalLeft - i;
				int nums1LeftMax = i == 0 ? INT_MIN : nums1[i-1];
				int nums2LeftMax = j == 0 ? INT_MIN : nums2[j-1];
				int nums1RightMin = i == m ? INT_MAX : nums1[i];
				int nums2RightMin = j == n ? INT_MAX : nums2[j];
				if(nums1LeftMax <= nums2RightMin && nums2LeftMax <= nums1RightMin){
					if((m + n) % 2 == 0){
						return (std::max(nums1LeftMax, nums2LeftMax) + std::min(nums1RightMin, nums2RightMin)) / 2.0;
					}else{
						return std::max(nums1LeftMax, nums2LeftMax);
					}
				}
				else if(nums1LeftMax > nums2RightMin){
					right = i - 1;
				}
				else{
					left = i + 1;
				}
			}
			return 0.0;
		}

		//最长回文子串 https://leetcode.com/problems/longest-palindromic-substring/
		//思路：
		//首先就是对于（奇数）回文，对于一个以C为中心的回文，在范围内的任意一个I，会存在一个J，满足（I+J）/ 2 = C
		//所以对于Manacher算法就是利用奇数型的这个性质，来优化时间复杂度的
		//那么，就需要我们将任意一个字符串构建成一个奇数型的字符串，才能确保存在中心，来使用这个性质
		//然后这样我们就避免了分别处理奇数与偶数的情况，优化了时间复杂度
		//此时我们按奇数型处理，直接遍历字符串，假设每个字符就是中心
		//此时就要使用到对称性了，
		//我们定义 C 表示当前的中心，R 表示以当前C为中心的最右侧
		//那么，此时我们遍历字符串时，就会存在这样的情况，如果遍历的I在R的左侧，表明会有一个J与I保持对称，
		//基于对称性，可以确定：在回文范围内，对于I与J他们周围的字符是一样的，所以如果对于以J为中心的回文的半径应用在I上，不会让I超出R，表明I的回文半径至少会与J一样
		//如果J的半径比较大，导致超出了，表明在仅有的R到I之间对于I来说都是回文的，之后的话就需要在进行判断，无论J有没有超出
		//这时候就需要对I中心基于已经设置过的最小半径来扩展判断后续是否也是回文：在扩展中就需要确保左侧右侧不超出字符串的范围
		//这样就计算出当前I的回文半径，
		//之后就是更新数据：为了让后续的遍历尽可能使用到对称性，所以我们应该保持让R尽量向后扩展，也就有了当前I的右边界超出原本的记录，就更新当前记录的中心与半径
		std::string longestPalindrome(std::string s) {
			std::string t = "$#";
			for(char c : s){
				t += c;
				t += "#";
			}
			t += "@";
			int length = t.length();
			std::vector<int> p(length, 0);
			int center = 0,right = 0;
			int max_center = 0;
			int max_length = 0;
			for(int i = 0; i < length; ++i){
				if(i < right){
					p[i] = std::min(right - i, p[2 * center - i]);
				}
				else{
					p[i] = 1;
				}
				while(i-p[i] >= 0 && i+p[i] < length && t[i-p[i]] == t[i+p[i]]){
					p[i]++;
				}
				if(i+p[i] > right){
					right = i + p[i];
					center = i;
				}
				if(p[i] > max_length){
					max_length = p[i];
					max_center = i;
				}
			}
			return s.substr((max_center - max_length + 1) / 2, max_length - 1);
		}

		//Z字形变换 https://leetcode.com/problems/zigzag-conversion/
		//空白方法
		std::string convert(std::string s, int numRows) {
			return s;
		}

	};
}


