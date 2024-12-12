#include <iostream>
#include <vector>
#include <queue>
#include <stack>

class Graph
{
public:
    Graph(std::size_t vertices_num): _V(vertices_num){
        _adjList.reserve(_V);
    }

    // add edge, u -> v
    void addEdge(int u, int v){
        _adjList[u].emplace_back(v);
    }

    void BFS(int start){
        std::vector<bool> visited(_V, false);
        std::queue<int> q;
        q.push(start);
        visited[start] = true;

        while(!q.empty()){
            int x = q.front();
            std::cout << "processing: " << x << std::endl;
            q.pop();
            for(int nxt: _adjList[x]){
                if(!visited[nxt]){
                    q.push(nxt);
                    visited[nxt] = true;
                }
            }
        }
    }

    // loop-implementation
    // void DFS(int start){
    //     std::vector<bool> visited(_V, false);
    //     std::stack<int> st;
    //     st.push(start);
    //     visited[start] = true;

    //     while(!st.empty()){
    //         int x = st.top();
    //         std::cout << "processing: " << x << std::endl;
    //         st.pop();
    //         for(int nxt: _adjList[x]){
    //             if(!visited[nxt]){
    //                 st.push(nxt);
    //                 visited[nxt] = true;
    //             }
    //         }
    //     }
    // }

    void DFS(int start){
        std::vector<bool> visited(_V, false);
        std::stack<int> topoStack;
        DFSUtil(start, visited, topoStack);
    }

    void topologicalSort() {
        std::vector<bool> visited(_V, false);
        std::stack<int> topoStack;

        for(int i = 0; i < _V; ++i) {
            if (!visited[i]) {
                DFSUtil(i, visited, topoStack);
            }
        }

        std::cout << "拓扑排序结果：";
        while (!topoStack.empty()) {
            std::cout << topoStack.top() << " ";
            topoStack.pop();
        }
        std::cout << std::endl;
    }

private:
    // recursive-implementation
    void DFSUtil(int node, std::vector<bool>& visited, std::stack<int>& topoStack){
        visited[node] = true;
        std::cout << "DFS processing: " << node << std::endl;

        for (int nxt : _adjList[node]) {
            if (!visited[nxt]) {
                DFSUtil(nxt, visited, topoStack);
            }
        }
        topoStack.push(node);
    }

    std::size_t _V;
    std::vector<std::vector<int>> _adjList;
};

int main() {
    Graph g(5);
    g.addEdge(0, 1);
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(3, 4);

    std::cout << "从节点 0 开始的 BFS 遍历结果：" << std::endl;
    g.BFS(0);
    std::cout << "从节点 0 开始的 DFS 遍历结果：" << std::endl;
    g.DFS(0);
    std::cout << "topological sorting结果：" << std::endl;
    g.topologicalSort();
    return 0;
}