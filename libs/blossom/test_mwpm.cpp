#include "PerfectMatching.h"
#include <iostream>

using namespace std;

int main()

{

    int node_num = 4;
    int edge_num = 3;
    int *edges = new int[6]{0, 1, 1, 2, 2, 3};
    int *weights = new int[3]{1, 3, 4};
    // int *edges = new int[6];
    // for (int i = 0; i < 6; i++)
    // {
    //     edges[i] = i;
    // }
    // int *weights = new int[3];
    // for (int i = 0; i < 3; i++)
    // {
    //     edges[i] = i + 2;
    // }
    int mates[node_num];

    struct PerfectMatching::Options options;
    options.verbose = false;

    PerfectMatching *pm = new PerfectMatching(node_num, edge_num);
    for (int i = 0; i < edge_num; i++)
        pm->AddEdge(edges[2 * i], edges[2 * i + 1], weights[i]);

    pm->options = options;

    pm->Solve();

    for (int i = 0; i < node_num; i++)
    {
        mates[i] = pm->GetMatch(i);
    }

    for (int i = 0; i < node_num; i++)
    {
        cout << mates[i] << endl;
    }
    return 0;
}