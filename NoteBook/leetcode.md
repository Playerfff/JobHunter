# String

### 3. longest-substring-without-repeating-characters

We should use `set` to maintain a **slide window**.

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

