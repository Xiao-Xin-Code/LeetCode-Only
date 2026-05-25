#pragma once

#include <algorithm>
#include <climits>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <stack>

#include "Extension.h"

namespace Solution {
	class Solution {
	public:
		//001-两数之和 https://leetcode.com/problems/two-sum/
		std::vector<int> twoSum(std::vector<int>& nums, int target) {
			if(nums.size() <= 1) return {};	
			std::unordered_map<int, int> maps;
			for (int i = 0; i < nums.size(); ++i) {
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

		//006-Z字形变换 https://leetcode.com/problems/zigzag-conversion/
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
				auto cur = queue.front();
				std::string str = std::get<0>(cur);
				int left = std::get<1>(cur);
				int right = std::get<2>(cur);
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
			ListNode* head = lists[0];
			for(int i = 1; i < lists.size(); ++i){
				head = mergeTwoLists(head, lists[i]);
			}
			return head;
		}

		//024-交换链表中的节点 https://leetcode.com/problems/swap-nodes-in-pairs/
		ListNode* swapPairs(ListNode* head) {
			ListNode* l1 = head;
			ListNode* l2 = head->next;
			if(l2 == nullptr) return l1;
			l1->next = swapPairs(l2->next);
			l2->next = l1;
			return nullptr;
		}

		//025-K个一组反转链表 https://leetcode.com/problems/reverse-nodes-in-k-group/
		ListNode* reverseKGroup(ListNode* head, int k) {
			ListNode* l1 = head;
			ListNode* l2 = head;
			int n = 0;
			while(l2!=nullptr&&n<k){
				l2 = l2->next;
				n++;
			}
			if(n > 0) return head;
			ListNode* l3 = l1->next;
			l1->next = reverseKGroup(l2->next, k);
			l2->next = l3;
			return l2;
		}

		//026-从已排序数组中移除重复元素 https://leetcode.com/problems/remove-duplicates-from-sorted-array/
		int removeDuplicates(std::vector<int>& nums) {
			int count = 1;
			for(int i = 1; i < nums.size(); ++i){
				if(nums[i] != nums[i-1]){
					nums[count] = nums[i];
					count++;
				}
			}
			return count;
		}

		//027-移除元素 https://leetcode.com/problems/remove-element/
		int removeElement(std::vector<int>& nums, int val) {
			int count = 0;
			for(int i = 0; i < nums.size(); ++i){
				if(nums[i] != val){
					nums[count] = nums[i];
					count++;
				}
			}
			return count;
		}
		
		//028-实现strStr() https://leetcode.com/problems/implement-strstr/
		int strStr(std::string haystack, std::string needle) {
			for(int  i = 0; i <= haystack.size() - needle.size(); ++i){
				if(haystack.substr(i,needle.size()) == needle){
					return i;
				}
			}
			return -1;
		}

		//029-整除 https://leetcode.com/problems/divide-two-integers/
		int divide(int dividend, int divisor) {
			int count = 0;
			while(dividend >= divisor){
				int tempDivisor = divisor;
				int curCount = 1;
				while(dividend - tempDivisor >= tempDivisor){
					tempDivisor += tempDivisor;
					curCount += curCount;
				}
				dividend -= tempDivisor;
				count += curCount;
			}
			return count;
		}

		//030-串联所有单词的子串 https://leetcode.com/problems/substring-with-concatenation-of-all-words/
		std::vector<int> findSubstring(std::string s, std::vector<std::string>& words) {
			std::unordered_map<std::string,int> wordMap;
			std::vector<int> result;
			int windowSize = 0;
			for(int i = 0; i < words.size(); ++i){
				wordMap[words[i]]++;
				windowSize += words[i].size();
			}
			for(int i = 0; i <= s.size() - windowSize; ++i){
				int cur = i;
				std::unordered_map<std::string,int> tempMap = wordMap;
				for(int j = cur; j < cur + windowSize;){
					bool isMatch = false;
					for(auto it = tempMap.begin(); it != tempMap.end(); ++it){
						if(it->second > 0){
							isMatch = true;
							for(int w = 0; w < it->first.size(); ++w){
								if(s[j + w] != it->first[w]){
									isMatch = false;
									break;
								}
							}
							if(isMatch){
								tempMap[it->first]--;
								j += it->first.size();
								break;
							}
						}
					}
					if(!isMatch){
						break;
					}
				}
				bool isAll = true;
				for(auto it = tempMap.begin(); it != tempMap.end(); ++it){
					if(it->second != 0){
						isAll = false;
						break;
					}
				}
				if(isAll){
					result.push_back(cur);
				}
			}
			return result;
		}

		//031-下一个排列 https://leetcode.com/problems/next-permutation/
		void nextPermutation(std::vector<int>& nums) {
			int index = nums.size() - 2;
			while(index >= 0 && nums[index] >= nums[index + 1]) index--;

			if(index >= 0){
				int i = nums.size() - 1;
				while(i > index && nums[i] <= nums[index]) i--;
				std::swap(nums[index],nums[i]);
			}
			std::reverse(nums.begin() + index + 1, nums.end());
		}

		//032-最长有效括号 https://leetcode.com/problems/longest-valid-parentheses/
		int longestValidParentheses(std::string s) {
			int max = 0;
			std::stack<int> stack;
			stack.push(-1);
			for(int i = 0; i < s.size(); ++i){
				if(s[i] == '('){
					stack.push(i);
				}
				else{
					stack.pop();
					if(stack.empty()) stack.push(i);
					else max = std::max(max, i - stack.top());
				}
			}
			return max;
		}

		//033-搜索旋转排序数组 https://leetcode.com/problems/search-in-rotated-sorted-array/
		int search(std::vector<int>& nums, int target) {
			int left = 0;
			int right = nums.size() - 1;
			while(left <= right){
				int mid = (left + right) / 2;
				if(nums[mid] == target) return mid;
				if(nums[left] <= nums[mid]){
					if(nums[left] <= target && target <= nums[mid]){
						right = mid - 1;
					}
					else
					{
						left = mid + 1;
					}
				}
				else{
					if(nums[mid] <= target && target <= nums[right]){
						left = mid + 1;
					}
					else{
						right = mid - 1;
					}
				}
			}
			return -1;
		}

		//034-在排序数组中查找元素的第一个和最后一个位置 https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/
		std::vector<int> searchRange(std::vector<int>& nums, int target) {
			int left = 0,right = nums.size() - 1;
			std::vector<int> result = {-1,-1};
			int l = left,r = right;
			while(l <= r){
				int mid = (l + r) / 2;
				if(nums[mid] >= target){
					if(nums[mid] == target) result[0] = mid;
					r = mid - 1;
				}
				else{
					l = mid + 1;
				}
			}
			l = left,r = right;
			while(l <= r){
				int mid = (l + r) / 2;
				if(nums[mid] <= target){
					if(nums[mid] == target) result[1] = mid;
					l = mid + 1;
				}
				else{
					r = mid - 1;
				}
			}
			return result;
		}

		//035-搜索插入位置 https://leetcode.com/problems/search-insert-position/
		int searchInsert(std::vector<int>& nums, int target) {
			int left = 0,right = nums.size() - 1;
			while(left <= right){
				int mid = (left + right) / 2;
				if(nums[mid] == target) return mid;
				else if(nums[mid] < target) left = mid + 1;
				else right = mid - 1;
			}
			return left;
		}

		//036-有效的数独 https://leetcode.com/problems/valid-sudoku/
		bool isValidSudoku(std::vector<std::vector<char>>& board) {
			bool row[9][9];
			bool col[9][9];
			bool box[9][9];
			for(int i = 0; i < board.size(); ++i){
				for(int j = 0; j < board[i].size(); ++j){
					int index = board[i][j] - '1';
					if(row[i][index]) return false;
					if(col[j][index]) return false;
					if(box[j / 3 * 3 + i / 3][index]) return false;
					row[i][index] = true;
					col[j][index] = true;
					box[j / 3 * 3 + i / 3][index] = true;
				}
			}
			return true;
		}

		//037-解数独 https://leetcode.com/problems/sudoku-solver/
		void solveSudoku(std::vector<std::vector<char>>& board) {
			solveSudokuHelper(board);
		}
	private:
		void solveSudokuHelper(std::vector<std::vector<char>>& board){
			for(int i = 0; i < board.size(); ++i){
				for(int j = 0; j < board[i].size(); ++j){
					if(board[i][j] != '.') continue;
					for(char c = '1'; c <= '9'; ++c){
						bool isValid = true;
						for(int k = 0; k < 9; ++k){
							if(board[i][k] != '.' && board[i][k] == c) {
								isValid = false;
								break;
							}
							if(board[k][j] != '.' && board[k][j] == c){
								isValid = false;
								break;
							}
							if(board[i / 3 * 3 + k / 3][j / 3 * 3 + k % 3] != '.' && board[i / 3 * 3 + k / 3][j / 3 * 3 + k % 3] == c){
								isValid = false;
								break;
							}
						}
						if(isValid){
							board[i][j] = c;
							solveSudokuHelper(board);
							board[i][j] = '.';
						}
					}
				}
			}
		}

		//038-外观数列 https://leetcode.com/problems/count-and-say/
		std::string countAndSay(int n) {
			if(n == 1) return "1";
			std::string pre = countAndSay(n - 1);
			std::string result;
			int count = 1;
			for(int i = 1; i < n; ++i){
				if(pre[i] == pre[i - 1]){
					count++;
				}
				else{
					result += std::to_string(count) + pre[i - 1];
					count = 1;
				}
			}
			result += std::to_string(count) + pre.back();
			return result;

		}

		//039-组合总和 https://leetcode.com/problems/combination-sum/
		std::vector<std::vector<int>> combinationSum(std::vector<int>& candidates, int target) {
			std::vector<std::vector<int>> result;
			sort(candidates.begin(),candidates.end());
			combinationSumHelper(candidates,target,0,{},result);
			return result;
		}
	private:
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

	public:
		//040-组合总和 II https://leetcode.com/problems/combination-sum-ii/
		std::vector<std::vector<int>> combinationSum2(std::vector<int>& candidates, int target) {
			std::vector<std::vector<int>> result;
			sort(candidates.begin(),candidates.end());
			combinationSum2Helper(candidates,target,0,{},result);
			return result;
		}
	private:
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

	public:
		//041-缺失的第一个正数 https://leetcode.com/problems/first-missing-positive/
		int firstMissingPositive(std::vector<int>& nums) {
			for(int i = 0; i < nums.size(); ++i){
				if(nums[i] < 0) nums[i] = 0;
			}
			for(int i = 0; i < nums.size(); ++i){
				if(nums[i] != 0 && abs(nums[i]) <= nums.size()){
					nums[abs(nums[i]) - 1] = -abs(nums[abs(nums[i]) - 1]);
				}
			}
			for(int i = 0; i < nums.size(); ++i){
				if(nums[i] > 0) return i + 1;
			}
			return nums.size() + 1;
		}

		//042-接雨水 https://leetcode.com/problems/trapping-rain-water/
		int trap(std::vector<int>& height) {
			int left = 0;
			int right = height.size() - 1;
			int total = 0;
			int leftHeight = height[left];
			int rightHeight = height[right];
			while(left < right){
				while(left < right && height[left] == 0) left++;
				while(left < right && height[right] == 0) right--;
				if(left >= right) break;
				leftHeight = height[left];
				rightHeight = height[right];
				if(leftHeight <= rightHeight){
					left++;
					while(left < right && height[left] < leftHeight){
						total += leftHeight - height[left];
						left++;
					}
					leftHeight = height[left];
				}
				else{
					right--;
					while(left < right && height[right] < rightHeight){
						total += rightHeight - height[right];
						right--;
					}
					rightHeight = height[right];
				}
			}
			return total;
		}

		//043-乘字符串 https://leetcode.com/problems/multiply-strings/
		std::string multiply(std::string num1, std::string num2) {
			std::string result(num1.length() + num2.length(),'0');
			for(int i = num2.length() - 1; i >= 0; --i){
				for(int j = num1.length() - 1; j >= 0; --j){
					int mul = (num2[i] - '0') * (num1[j] - '0');
					int sum = mul + (result[i + j + 1] - '0');
					result[i + j + 1] = sum % 10 + '0';
					int carry = sum / 10;
					int index = i + j;
					while(carry > 0 && index >= 0){
						int cur = (result[index] - '0') + carry;
						result[index] = cur % 10 + '0';
						carry = cur / 10;
						index--;
					}
				}
			}
			result.erase(0,result.find_first_not_of("0"));
			return result.empty() ? "0" : result;
		}

		//044-通配符匹配 https://leetcode.com/problems/wildcard-matching/
		bool isMatchWildcard(std::string s, std::string p) {
			return false;
		}

		//045-跳跃游戏II https://leetcode.com/problems/jump-game-ii/
		int jump(std::vector<int>& nums) {
			int count = 0,currentEnd = 0,farthest = 0;
			for(int i = 0; i < nums.size() - 1; ++i){
				farthest = std::max(farthest,i + nums[i]);
				if(i == currentEnd){
					currentEnd = farthest;
					count++;
				}
			}
			return count;
		}

		//046-全排列 https://leetcode.com/problems/permutations/
		std::vector<std::vector<int>> permute(std::vector<int>& nums) {
			std::vector<std::vector<int>> result;
			permuteHelper(nums, std::vector<bool>(nums.size(),false), std::vector<int>(),result);
			return result;
		}
	private:
		void permuteHelper(std::vector<int>& nums,std::vector<bool> usedFrag,std::vector<int> curResult,std::vector<std::vector<int>>& result){
			if(curResult.size() == nums.size()){
				result.push_back(curResult);
				return;
			}
			for(int i = 0; i < nums.size(); ++i){
				if(usedFrag[i]) continue;
				curResult.push_back(nums[i]);
				usedFrag[i] = true;
				permuteHelper(nums,usedFrag,curResult,result);
				usedFrag[i] = false;
				curResult.pop_back();
			}
		}

	public:
		//047-全排列II https://leetcode.com/problems/permutations-ii/
		std::vector<std::vector<int>> permuteUnique(std::vector<int>& nums) {
			sort(nums.begin(),nums.end());
			std::vector<std::vector<int>> result;
			permuteUniqueHelper(nums, std::vector<bool>(nums.size(),false), std::vector<int>(),result);
			return result;
		}
	private:
		void permuteUniqueHelper(std::vector<int>& nums,std::vector<bool> usedFrag,std::vector<int> curResult,std::vector<std::vector<int>>& result){
			if(curResult.size() == nums.size()){
				result.push_back(curResult);
				return;
			}
			for(int i = 0; i < nums.size(); ++i){
				if(usedFrag[i]) continue;
				if(i > 0 && nums[i] == nums[i - 1] && !usedFrag[i - 1]) continue;
				curResult.push_back(nums[i]);
				usedFrag[i] = true;
				permuteUniqueHelper(nums,usedFrag,curResult,result);
				usedFrag[i] = false;
				curResult.pop_back();
			}
		}

	public:
		//048-旋转图像 https://leetcode.com/problems/rotate-image/
		void rotate(std::vector<std::vector<int>>& matrix) {
			int left = 0,right = matrix.size() - 1;
			while(left < right){
				for(int i = 0; i <= right - left; ++i){
					int temp = matrix[left][left + i];
					matrix[left][left + i] = matrix[right - i][left];
					matrix[right - i][left] = matrix[right][right - i];
					matrix[right][right - i] = matrix[left + i][right];
					matrix[left + i][right] = temp;
				}
				left++;
				right--;
			}
			
		}

		//049 字母异位词分组 https://leetcode.com/problems/group-anagrams/
		std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) {
			std::vector<std::vector<std::string>> result;
			std::unordered_map<std::string,std::vector<std::string>> map;
			for(int i = 0; i < strs.size(); ++i){
				std::string temp = strs[i];
				sort(temp.begin(),temp.end());
				map[temp].push_back(strs[i]);
			}
			for(auto it = map.begin(); it != map.end(); ++it){
				result.push_back(it->second);
			}
			return result;
		}
		
		//050-Power(x,n) https://leetcode.com/problems/powx-n/
		double myPow(double x, int n) {
			if(n == 0) return 1.0;
			if(x == 1.0 || n == 1) return x;
			int absN = std::abs(n);
			double result = 1.0;
			while(absN > 0){
				if(absN % 2 == 1){
					result *= x;
				}
				x *= x;
				absN /= 2;
			}
			return n > 0 ? result : 1.0 / result;
		}

		//051-N皇后 https://leetcode.com/problems/n-queens/
		std::vector<std::vector<std::string>> solveNQueens(int n) {
			std::vector<std::vector<std::string>> results;
			std::vector<std::vector<std::string>> board(n,std::vector<std::string>(n,""));
			int maxcount = 0;
			solveNQueensHelper(0,n,0,maxcount,results,board);
			return results;
		}
	private:
		void solveNQueensHelper(int row,int n,int count,int& maxcount,std::vector<std::vector<std::string>>& results,std::vector<std::vector<std::string>> board){
			if(row == n){
				if(count > maxcount){
					maxcount = count;
					results = board;
				} 
				return;
			}

			for(int i = 0; i < n; ++i){
				if(board[row][i] != "") continue;
				board[row][i] = "Q";
				std::vector<std::vector<int>> frags; 
				//行
				for(int h = 0; h < n; ++h){
					if(board[row][h] != "") continue;
					board[row][h] = ".";
					frags.push_back({row,h});
				}
				//列
				for(int l = row; l < n; ++l){
					if(board[l][i] != "") continue;
					board[l][i] = ".";
					frags.push_back({l,i});
				}
				//左下
				for(int h = row + 1,l = i - 1; h < n && l >= 0; ++h,--l){
					if(board[h][l] != "") continue;
					board[h][l] = ".";
					frags.push_back({h,l});
				}
				//右下
				for(int h = row + 1,l = i + 1; h < n && l < n; ++h,++l){
					if(board[h][l] != "") continue;
					board[h][l] = ".";
					frags.push_back({h,l});
				}
				solveNQueensHelper(row + 1,n,count + 1,maxcount,results,board);
				board[row][i] = "";
				for(auto frag : frags){
					board[frag[0]][frag[1]] = "";
				}
			}
		}
		

	};
}


