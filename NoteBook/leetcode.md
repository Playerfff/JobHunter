# Array

## 27. remove-element

```c++
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        nums.erase(remove(nums.begin(), nums.end(), val), nums.end());
        return nums.size();
    }
};
```

### Or

```c++
class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        std::erase(nums, val); // since C++20
        return nums.size();
    }
};
```

## 977. Squares of a Sorted Array

You can insert into a vector's end the larger abs of the sorted array.

## 209. Minimum Size Subarray Sum

**sliding window**

# Linked List

## 142. Linked List Cycle II

1. put a fast pointer jumping two steps one time
2. put a slow pointer jumping one step one time
3. If the cycle existed, they will meet each other at the joint point
4. put a pointer starting from the head, another from the joint point, they will meet each other at the node where the cycle begins.

# Hash_table

我再强调一下 **什么时候使用哈希法**，当我们需要查询一个元素是否出现过，或者一个元素是否在集合里的时候，就要第一时间想到哈希法。

## 202. Happy Number

If you want to break from an endless loop that only ends for some special reasons, what should you do?

- Utilize a hash_set to store all outcomes, and within each iteration, verify if the outcome is present in the hash_set; if so, terminate the loop.

# String

### 3. longest-substring-without-repeating-characters

We should use `set` to maintain a **slide window**.

# Two-Pointer

## 15. 3Sum

1. sort nums.

2. for i in range, choose a way to deduplicate.

3. make left = i + 1, right = nums.size() - 1;

4. 如果nums[i] + nums[left] + nums[right] > 0 就说明 此时三数之和大了，因为数组是排序后了，所以right下标就应该向左移动，这样才能让三数之和小一些。

   如果 nums[i] + nums[left] + nums[right] < 0 说明 此时 三数之和小了，left 就向右移动，才能让三数之和大一些，直到left与right相遇为止。

# Binary Tree

### How to traversal a binary tree?

#### Preorder Traversal

```c++
void traversal(TreeNode* cur, vector<int>& res) {
    if(cur == nullptr) return;
    res.push_back(cur->val);
    traversal(cur->left, res);
    traversal(cur->right, res);
}
```



#### Level Traversal

**queue** is the key.

```c++
vector<vector<int>> levelOrder(TreeNode* root) {
        queue<TreeNode*> que;
        if (root != NULL) que.push(root);
        vector<vector<int>> result;
        while (!que.empty()) {
            int size = que.size();
            vector<int> vec;
         // 这里一定要使用固定大小size，不要使用que.size()，因为que.size是不断变化的
            for (int i = 0; i < size; i++) {
                TreeNode* node = que.front();
                que.pop();
                vec.push_back(node->val);
                if (node->left) que.push(node->left);
                if (node->right) que.push(node->right);
            }
            result.push_back(vec);
        }
        return result;
    }
```

