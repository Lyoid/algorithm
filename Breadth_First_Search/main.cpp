#include <iostream>
#include <vector>
#include <stack>
#include<queue>
using namespace std;

const int maxn = 100000+10;
//邻接表结构
struct AdjTableGraph{
    vector<vector <int> > adjTalbeGraph;
};
struct Node
{
    int index;
};

void BFS(AdjTableGraph graph,vector<int> &visited ,vector<int> &visitOrder ,int &startNode,queue<int> q,int &ways){
    if(graph.adjTalbeGraph[startNode].size() > 0 )
    {
        for(size_t i = 0; i < graph.adjTalbeGraph[startNode].size(); ++i)
        {
            if(visited[graph.adjTalbeGraph[startNode][i]] == 0)
            {
                q.push(graph.adjTalbeGraph[startNode][i]);
                //深度计算时用到，对于总路径数计算时无需
 //               visited[graph.adjTalbeGraph[startNode][i]] = 1;
                ways += 1;
            }

        }
        q.pop();
        BFS(graph,visited,visitOrder,q.front(),q,ways);
    }
}


int main() {
    queue<int>q;
    AdjTableGraph graph;
    vector<int> visitOrder;
    int ways = 0 ;
    graph.adjTalbeGraph.push_back(vector<int> {1,2});
    graph.adjTalbeGraph.push_back(vector<int> {3,4});
    graph.adjTalbeGraph.push_back(vector<int> {5});
    graph.adjTalbeGraph.push_back(vector<int> {});
    graph.adjTalbeGraph.push_back(vector<int> {});
    graph.adjTalbeGraph.push_back(vector<int> {});
/*

    graph.adjTalbeGraph.push_back(vector<int> {1,2});
    graph.adjTalbeGraph.push_back(vector<int> {2});
    graph.adjTalbeGraph.push_back(vector<int> {3});
    graph.adjTalbeGraph.push_back(vector<int> {});
*/
    vector<int > visited(graph.adjTalbeGraph.size(),0);
    q.push(0);


    BFS(graph,visited,visitOrder,q.front(),q,ways);

    cout <<ways;
    return 0;
}