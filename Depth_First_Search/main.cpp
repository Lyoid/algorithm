#include <iostream>
#include <vector>
#include <stack>

using namespace std;

typedef int DATA_TYPE;  // 权值为int型
const DATA_TYPE NO_EDGE = 10000000;  // 表示没有该边
// 邻接表
struct AdjTableGraph
{
    vector<vector<int> > adjTable;
    vector<vector<DATA_TYPE> > adjWeights;  // 暂时用不到 维数与adjTable一致
};

// 邻接表深度优先搜索算法递归
void AdjTableDFS2(AdjTableGraph graph, vector<int> &visited, vector<int> &visitOrder, int startNode,int &ways)
{
    visited[startNode] = 1;
    visitOrder.push_back(startNode);

    if (graph.adjTable[startNode].size() > 0)
    {
        for (size_t i = 0; i < graph.adjTable[startNode].size(); ++i)
        {
 //           if (visited[graph.adjTable[startNode][i]] == 0)
 //           {
                AdjTableDFS2(graph, visited, visitOrder, graph.adjTable[startNode][i],ways);
 //           }
            ways += 1;
        }
    }

}

int main(int argc, char *argv[])
{

    // 图的初始化
    // 顶点编号必须为从0开始的连续的整数(若不是，先转换)
    // 图为有向图

    // ================邻接表方式===============
    AdjTableGraph graph;
    graph.adjTable.push_back(vector<int>{1, 2});
    graph.adjTable.push_back(vector<int>{2});
    graph.adjTable.push_back(vector<int>{3});
    graph.adjTable.push_back(vector<int>{});
    //graph.adjTable.push_back(vector<int>{});


    // 邻接表 递归
    vector<int> visited2(graph.adjTable.size(), 0);
    vector<int> visitOrder2;
    int ways = 0;
    AdjTableDFS2(graph, visited2, visitOrder2, 0,ways);

    cout << "邻接表 递归： ";
    for (size_t i = 0; i < visitOrder2.size(); ++i)
    {
        cout << visitOrder2[i] << " ";
    }
    cout << endl;
    cout << "从0出发，总共多少条路（可以不到底）：";
    cout << ways;
/*    for (size_t i = 0; i < graph.adjTable.size(); ++i)
    {
        for(size_t j = 0; j < graph.adjTable[i].size(); ++ j)
        {
            cout << graph.adjTable[i][j]<< endl;

        }
    }

 */



    return 0;
}