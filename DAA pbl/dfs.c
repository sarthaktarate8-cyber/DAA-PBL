#include <stdio.h>
#include "hostel_data.h"
void deep_inspection_dfs(int current_node, int visited[], char target_block) {
    visited[current_node] = 1;
    if (current_node < TOTAL_ROOMS && nodes[current_node].block == target_block) {
        printf("Room %c-%03d: ", nodes[current_node].block, nodes[current_node].room_number);
        if (nodes[current_node].status == 1) printf("Vacant\n");
        else if (nodes[current_node].status == 0) printf("Occupied\n");
        else if (nodes[current_node].status == -1) printf("Under Maintenance\n");
    }
    for (int i = 0; i < TOTAL_NODES; i++) {
        if (adj_matrix[current_node][i] != INF && adj_matrix[current_node][i] > 0 && !visited[i]) {
            if (i < TOTAL_ROOMS && nodes[i].block == target_block) {
                deep_inspection_dfs(i, visited, target_block);
            }
        }
    }
}
