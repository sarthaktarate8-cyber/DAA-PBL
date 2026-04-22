#include <stdio.h>
#include <stdlib.h>
#include "hostel_data.h"
int find_nearest_vacant_bfs(int start_node) {
    if (start_node < TOTAL_ROOMS && nodes[start_node].status == 1) {
        return start_node; 
    }
    int visited[TOTAL_NODES] = {0};
    int queue[TOTAL_NODES];
    int front = 0, rear = 0;
    queue[rear++] = start_node;
    visited[start_node] = 1;
    while (front < rear) {
        int curr = queue[front++];
        if (curr < TOTAL_ROOMS && nodes[curr].status == 1) {
            return curr;
        }
        for (int i = 0; i < TOTAL_NODES; i++) {
            if (adj_matrix[curr][i] != INF && adj_matrix[curr][i] > 0 && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
    return -1; 
}
