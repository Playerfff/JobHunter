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

### 28. find-the-index-of-the-first-occurrence-in-a-string

```C++
    int strStr(string haystack, string needle) {
        size_t found = haystack.find(needle);
        if (found != string::npos)
            return static_cast<int>(found);
        else
            return -1;
    }
```

### 459. Repeated Substring Pattern
Given a string `s`, to check if it can be constructed by taking a substring of it and appending multiple copies of the substring together, you can append `s` to `s` and check if there is a `s` in the appended string.

### 翻转数字

1. We should reverse enough memory first of all.
2. We use two pointers: one for the end of the old string and one for the end of the new string. We stop when the two pointers met each other.

# Two-Pointer

## 15. 3Sum

1. sort nums.

2. for i in range, choose a way to deduplicate.

3. make left = i + 1, right = nums.size() - 1;

4. 如果nums[i] + nums[left] + nums[right] > 0 就说明 此时三数之和大了，因为数组是排序后了，所以right下标就应该向左移动，这样才能让三数之和小一些。

   如果 nums[i] + nums[left] + nums[right] < 0 说明 此时 三数之和小了，left 就向右移动，才能让三数之和大一些，直到left与right相遇为止。

## Stack & Queue

### 239. sliding-window-maximum
#### user-defined descending queue
use `std::deque` to simulate a secending queue - MyQueue. Use MyQueue to finish the problem. 

### 347. Top K Frequent Elements
**Classic**  
1. 统计: 把所有元素塞入hash_map中进行统计;
2. 使用`std::priority`进行排序, 统计前k个;

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

### Height & Depth

1. **Height**: 高度是该节点到**最远的**叶子结点的距离; (需要从下往上遍历, 因此需要postorder遍历)

```c++
	int getHeight(TreeNode* node) {
        if (node == NULL) return 0;
        int leftdepth = getdepth(node->left);       // 左
        int rightdepth = getdepth(node->right);     // 右
        int depth = 1 + max(leftdepth, rightdepth); // 中
        return depth;
    }

	// 简化版本
	int getHeight(TreeNode* node) {
        if(node == nullptr) return 0;
        return 1 + std::max(getDepth(node->left), getDepth(node->right));
    }
```



2. **Depth**:  深度是根节点到**最远的**该节点的距离; (需要从上往下遍历, 因此需要preorder遍历)

```c++
	int result;
    void getDepth(TreeNode* node, int depth) {
        result = depth > result ? depth : result; // 中

        if (node->left == NULL && node->right == NULL) return ;

        if (node->left) { // 左
            depth++;    // 深度+1
            getdepth(node->left, depth);
            depth--;    // 回溯，深度-1
        }
        if (node->right) { // 右
            depth++;    // 深度+1
            getdepth(node->right, depth);
            depth--;    // 回溯，深度-1
        }
        return ;
    }
```



### 222. Count Complete Tree Nodes

1. check if the tree is **full binary tree** by calculating the depth of the left subtree and the right subtree.

2. if is full binary tree, use the formula to get the node count.

3. if not a full binary tree, 

      ```C++
      return countNodes(root->left) + countNodes(root->right) + 1;
      ```

### 98. Validate Binary Search Tree

1. To validate BST, you cannot just simply compare the left node value and the right node value.

2. 要知道中序遍历下，输出的二叉搜索树节点的数值是有序序列。

   有了这个特性，**验证二叉搜索树，就相当于变成了判断一个序列是不是递增的了。**

### 501. Find Mode in Binary Search Tree

**Classic**

### 236. Lowest Common Ancestor of a Binary Tree

```c++
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (root == q || root == p || root == NULL) return root;
        TreeNode* left_result = lowestCommonAncestor(root->left, p, q);
        TreeNode* right_result = lowestCommonAncestor(root->right, p, q);
        if(left_result && right_result) return root;
        else if(left_result != nullptr && right_result == nullptr) return left_result;
        else if(right_result != nullptr && left_result == nullptr) return right_result;
        else return nullptr;
    }
```

## Backtracing

**回溯法解决的问题都可以抽象为树形结构**

回溯法模板如下:

```c++
void backtracking(参数) {
    if (终止条件) {
        存放结果;
        return;
    }

    for (选择：本层集合中元素（树中节点孩子的数量就是集合的大小）) {
        处理节点;
        backtracking(路径，选择列表); // 递归
        回溯，撤销处理结果
    }
}
```

### 40. Combination Sum II

注意分清楚，**树层上面的去重**和**树枝上面的去重**
