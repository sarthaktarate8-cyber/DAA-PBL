#include <stdio.h>
#include "hostel_data.h"
void compute_floyd_warshall() {
    for (int i = 0; i < TOTAL_NODES; i++) {
        for (int j = 0; j < TOTAL_NODES; j++) {
            shortest_distance[i][j] = adj_matrix[i][j];
        }
    }
    for (int k = 0; k < TOTAL_NODES; k++) {
        for (int i = 0; i < TOTAL_NODES; i++) {
            for (int j = 0; j < TOTAL_NODES; j++) {
                if (shortest_distance[i][k] != INF && shortest_distance[k][j] != INF &&
                    shortest_distance[i][k] + shortest_distance[k][j] < shortest_distance[i][j]) {
                    shortest_distance[i][j] = shortest_distance[i][k] + shortest_distance[k][j];
                }
            }
        }
    }
}
int get_suitability_score(int room_node) {
    if (room_node < 0 || room_node >= TOTAL_ROOMS) return INF;
    int dist_to_mess = shortest_distance[room_node][MESS_ID];
    int dist_to_exit = shortest_distance[room_node][EXIT_ID];
    if (dist_to_mess == INF || dist_to_exit == INF) return INF;
    return dist_to_mess + dist_to_exit;
}
