#pragma once

#include <climits>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <stack>

#include "Extension.h"
#include "Sort.h"

namespace Solution {
	class Solution {
	public:
		//001-两数之和 https://leetcode.com/problems/two-sum/
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

		//002-两数相加 https://leetcode.com/problems/add-two-numbers/
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

		//003-无重复字符的最长子串 https://leetcode.com/problems/longest-substring-without-repeating-characters/
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

		//004-两个有序数组的中位数 https://leetcode.com/problems/median-of-two-sorted-arrays/
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

		//005-最长回文子串 https://leetcode.com/problems/longest-palindromic-substring/
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
			if(s.empty()||s.size() == 1) return s;
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

		//006-ZZ字形变换 https://leetcode.com/problems/zigzag-conversion/
		std::string convert(std::string s, int numRows) {
			if(s.empty()||numRows == 1) return s;
			std::vector<std::string> rows(numRows, "");
			int cycle = (numRows - 1) * 2;
			for(int i = 0; i < s.size(); ++i){
				int offset = i % cycle;
				int index = offset < numRows ? offset : cycle - offset;
				rows[index] += s[i];
			}
			std::string result;
			for(const auto& row : rows){
				result += row;
			}
			return result;
		}

		//007-整数反转 https://leetcode.com/problems/reverse-integer/
		int reverse(int x) {
			if(x == 0) return 0;
			int result = 0;
			while(x!=0){
				int pop = x % 10;
				x /= 10;
				if(x > 0 && result > (INT_MAX - pop)/10) return 0;
				if(x < 0 && result < (INT_MIN - pop)/10) return 0;
				result = result * 10 + pop;
			}
			return result;
		}

		//008-字符串转换整数 (atoi) https://leetcode.com/problems/string-to-integer-atoi/
		int myAtoi(std::string str) {
			if(str.empty()) return 0;
			int sign = 1;
			int result = 0;
			bool isBegin = false;
			for(int i = 0; i < str.size(); ++i){
				if(str[i] >= '0' && str[i] <= '9'){
					isBegin = true;
					int pop = str[i] - '0';
					if(sign == 1 && result > (INT_MAX - pop) / 10) return INT_MAX;
					if(sign == -1 && -result < (INT_MIN + pop) / 10) return INT_MIN;
					result = result * 10 + pop;
				}
				else if(i == 0){
					if(str[i] == '+' || str[i] == '-'){
						sign = str[i] == '-' ? -1 : 1;
						isBegin = true;
					}
					else if(str[i] == ' '){
						continue;
					}
					else{
						break;
					}
				}
				else if(!isBegin && str[i] == ' '){
					continue;
				}
				else{
					break;
				}
			}
			return result * sign;
		}

		//009-回文数 https://leetcode.com/problems/palindrome-number/
		bool isPalindrome(int x) {
			if(x < 0) return false;
			if(x == 0) return true;
			int right = 0;
			while(x > right){
				right = right * 10 + x % 10;
				if(x == right) return true;
				x /= 10;
			}
			if(x == right) return true;
			return false;
		}
		
		//010-正则表达式匹配 https://leetcode.com/problems/regular-expression-matching/
		bool isMatch(std::string s, std::string p) {
			return false;
		}
		
		//011-盛最多水的容器 https://leetcode.com/problems/container-with-most-water/
		int maxArea(std::vector<int>& height) {
			int left = 0,right = height.size() - 1;
			int maxHeight = 0;
			while(left < right){
				int count = right - left;
				int curHeight = std::min(height[left],height[right]) * count;
				int maxHeight = std::max(maxHeight, curHeight);
				if(height[left] < height[right]){
					left++;
				}
				else{
					right--;
				}
			}
			return maxHeight;
		}

		//012-整数转罗马数字 https://leetcode.com/problems/integer-to-roman/
		std::string intToRoman(int x) {
			std::unordered_map<int, std::string> map = {{1000, "M"},{900, "CM"},{500, "D"},{400, "CD"},
				{400, "CD"},{100, "C"},{90, "XC"},{50, "L"},{40, "XL"},{10, "X"},{9, "IX"},
				{5, "V"},{4, "IV"},{1, "I"}
			};
			std::string result;
			for(const auto& pair : map){
				if(x >= pair.first){
					int count = x / pair.first;
					x %= pair.first;
					for(int i = 0; i < count; ++i){
						result.append(pair.second);
					}
				}
			}
			return result;
		}

		//013-罗马数字转整数 https://leetcode.com/problems/roman-to-integer/
		int romanToInt(std::string s) {
			std::unordered_map<std::string,int> map = {{"I",1}, {"V",5}, {"X",10}, {"L",50}, {"C",100}, {"D",500}, {"M",1000}};
			int index = 0;
			int result = 0;
			while(index < s.size()){
				if(map.find(s.substr(index,2)) != map.end()){
					result += map[s.substr(index,2)];
					index += 2;
				}
				else if(map.find(s.substr(index,1)) != map.end()){
					result += map[s.substr(index,1)];
					index++;
				}
				else{
					return 0;
				}
			}
			return result;
		}

		//014-最长公共前缀 https://leetcode.com/problems/longest-common-prefix/
		std::string longestCommonPrefix(std::vector<std::string>& strs) {
			if(strs.empty()) return "";
			std::string result;
			for(int i = 0; i < strs[0].length(); ++i){
				for(int j = 1; j < strs.size(); ++j){
					if(strs[j][i] != strs[0][i]){
						return result;
					}
				}
				result += strs[0][i];
			}
			return result;
		}

		//015-三数之和 https://leetcode.com/problems/3sum/
		std::vector<std::vector<int>> threeSum(std::vector<int>& nums) {
			std::sort(nums.begin(), nums.end());
			std::vector<std::vector<int>> result;
			for(int i = 0; i < nums.size() - 2; ++i){
				int target = -nums[i];
				int left = i + 1;
				int right = nums.size() - 1;
				while(left < right){
					int sum = nums[left] +nums[right];
					if(sum == target){
						result.push_back({nums[i], nums[left], nums[right]});
						while(left < right && nums[left + 1] == nums[left]) left++;
						while(left < right && nums[right - 1] == nums[right]) right--;
						left++;
						right--;
					}
					else if(sum < target){
						left++;
					}
					else{
						right--;
					}
				}
			}
			return result;
		}

		//016-三数之和最近 https https://leetcode.com/problems/3sum-closest/
		int threeSumClosest(std::vector<int>& nums, int target) {
			std::sort(nums.begin(), nums.end());
			int closest = nums[0] + nums[1] + nums[2];
			for(int i = 0; i < nums.size() - 2; ++i){
				if(i > 0 && nums[i] == nums[i - 1]) continue;
				int left = i + 1;
				int right = nums.size() - 1;
				while(left < right){
					int sum = nums[i] + nums[left] + nums[right];
					if(std::abs(sum - target) < std::abs(closest - target)){
						closest = sum;
					}
					else if(sum == target){
						return sum;
					}
					else if(sum < target){
						left++;
					}
					else{
						right--;
					}
				}
			}
			return closest;
		}

		//017-电话号码的字母组合 https://leetcode.com/problems/letter-combinations-of-a-phone-number/
		std::vector<std::string> letterCombinations(std::string digits) {
			if(digits.empty()) return {};
			std::unordered_map<char, std::string> map = {{'2', "abc"}, {'3', "def"}, {'4', "ghi"}, {'5', "jkl"}, {'6', "mno"}, {'7', "pqrs"}, {'8', "tuv"}, {'9', "wxyz"}};
			std::queue<std::string> queue;
			queue.push("");
			for(char c : digits){
				int size = queue.size();
				std::string cur = map[c];
				for(int i = 0; i < size; ++i){
					std::string current = queue.front();
					queue.pop();
					for(char d : cur){
						queue.push(current + d);
					}
				}
			}
			std::vector<std::string> result;
			while(!queue.empty()){
				result.push_back(queue.front());
				queue.pop();
			}
			return result;		
		}

		//018-四数之和 https://leetcode.com/problems/4sum/
		std::vector<std::vector<int>> fourSum(std::vector<int>& nums, int target){
			std::sort(nums.begin(), nums.end());
			std::vector<std::vector<int>> result;
			for(int i = 0; i < nums.size() - 3; ++i){
				int cur = target - nums[i];
				for(int j = i + 1; j < nums.size() - 2; ++j){
					int current = cur - nums[j];
					int left = j + 1;
					int right = nums.size() - 1;
					while(left < right){
						int sum = nums[left] + nums[right];
						if(sum == current){
							result.push_back({nums[i], nums[j], nums[left], nums[right]});
							while(left < right && nums[left + 1] == nums[left]) left++;
							while(left < right && nums[right - 1] == nums[right]) right--;
							left++;
							right--;
						}
						else if(sum < current){
							left++;
						}
						else{
							right--;
						}
					}
				}
			}
			return result;
		}

		//019-删除链表的倒数第N个节点 https://leetcode.com/problems/remove-nth-node-from-end-of-list/
		ListNode* removeNthFromEnd(ListNode* head, int n){
			if(head == nullptr) return nullptr;
			ListNode* fast = head;
			ListNode* slow = head;
			for(int i = 0; i < n; ++i){
				if(fast->next == nullptr) return head->next;
				else fast = fast->next;
			}
			while(fast->next != nullptr){
				slow = slow->next;
				fast = fast->next;
			}
			slow->next = slow->next->next;
			return head;
		}

		//020-有效括号 https://leetcode.com/problems/valid-parentheses/
		bool isValid(std::string s) {
			if(s.empty()) return true;
			if(s.size() % 2 != 0) return false;
			std::stack<char> stackLeft;
			for(char c : s){
				if(c == '(' || c == '[' || c == '{'){
					stackLeft.push(c);
				}
				else{
					if(stackLeft.empty()) return false;
					else if(stackLeft.top() == '(' && c == ')'){
						stackLeft.pop();
					}
					else if(stackLeft.top() == '[' && c == ']'){
						stackLeft.pop();
					}
					else if(stackLeft.top() == '{' && c == '}'){
						stackLeft.pop();
					}
					else return false;
				}
			}
			return stackLeft.empty();
		}

		//021-合并两个有序链表 https://leetcode.com/problems/merge-two-sorted-lists/
		ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
			if(l1 == nullptr) return l2;
			if(l2 == nullptr) return l1;
			ListNode* head = l1->val < l2->val ? l1 : l2;
			ListNode* tail = head;
			while(l1 != nullptr && l2 != nullptr){
				if(l1->val < l2->val){
					tail->next = l1;
					l1 = l1->next;
					tail = tail->next;
				}
				else{
					tail->next = l2;
					l2 = l2->next;
					tail = tail->next;
				}
			}
			tail->next = l1 != nullptr ? l1 : l2;
			return head;
		}

		//022-括号生成 https://leetcode.com/problems/generate-parentheses/
		std::vector<std::string> generateParenthesis(int n) {
			if(n == 0) return {};
			std::queue<std::tuple<std::string,int,int>> queue;
			queue.push({"",0,0});
			std::vector<std::string> result;
			while(!queue.empty()){
				auto [str,left,right] = queue.front();
				queue.pop();
				if(str.length() == n * 2){
					result.push_back(str);
					continue;
				}
				else{
					if(left < n){
						queue.push({str + '(',left + 1,right});
					}
					if(right < left){
						queue.push({str + ')',left,right + 1});
					}
				}
			}
			return result;
		}

		//023-合并K个排序链表 https://leetcode.com/problems/merge-k-sorted-lists/
		ListNode* mergeKLists(std::vector<ListNode*>& lists) {
			if(lists.empty()) return nullptr;
			
			return nullptr;
		}

	};
}


