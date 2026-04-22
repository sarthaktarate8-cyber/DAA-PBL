#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hostel_data.h"
Node nodes[TOTAL_NODES];
int adj_matrix[TOTAL_NODES][TOTAL_NODES];
int shortest_distance[TOTAL_NODES][TOTAL_NODES];
typedef struct {
    int id;
    char name[50];
    int room_node;
} Student;
Student students[TOTAL_ROOMS];
int student_count = 0;
void initialize_data() {
    int node_idx = 0;
    for (int b = 0; b < NUM_BLOCKS; b++) {
        char block_name = (char)('A' + b);
        for (int f = 1; f <= NUM_FLOORS; f++) {
            for (int r = 1; r <= NUM_ROOMS_PER_FLOOR; r++) {
                nodes[node_idx].node_id = node_idx;
                nodes[node_idx].block = block_name;
                nodes[node_idx].floor = f;
                nodes[node_idx].room_number = f * 100 + r;
                nodes[node_idx].status = 1; 
                strcpy(nodes[node_idx].type, "Room");
                if (f == 1 && r == 2) nodes[node_idx].status = 0;  
                if (f == 3 && r == 4) nodes[node_idx].status = -1; 
                node_idx++;
            }
        }
    }
    nodes[MESS_ID].node_id = MESS_ID;
    nodes[MESS_ID].block = '-';
    nodes[MESS_ID].floor = 0;
    nodes[MESS_ID].room_number = 0;
    strcpy(nodes[MESS_ID].type, "Mess");
    nodes[MESS_ID].status = 0;
    nodes[EXIT_ID].node_id = EXIT_ID;
    nodes[EXIT_ID].block = '-';
    nodes[EXIT_ID].floor = 0;
    nodes[EXIT_ID].room_number = 0;
    strcpy(nodes[EXIT_ID].type, "Exit");
    nodes[EXIT_ID].status = 0;
    for (int i = 0; i < TOTAL_NODES; i++) {
        for (int j = 0; j < TOTAL_NODES; j++) {
            if (i == j) adj_matrix[i][j] = 0;
            else adj_matrix[i][j] = INF;
        }
    }
    for (int b = 0; b < NUM_BLOCKS; b++) {
        for (int f = 1; f <= NUM_FLOORS; f++) {
            for (int r = 1; r <= NUM_ROOMS_PER_FLOOR; r++) {
                int curr_idx = (b * NUM_FLOORS * NUM_ROOMS_PER_FLOOR) + ((f - 1) * NUM_ROOMS_PER_FLOOR) + (r - 1);
                if (r < NUM_ROOMS_PER_FLOOR) {
                    int next_idx = curr_idx + 1;
                    adj_matrix[curr_idx][next_idx] = 1; 
                    adj_matrix[next_idx][curr_idx] = 1;
                }
                if (r == 1 && f < NUM_FLOORS) {
                    int upper_idx = curr_idx + NUM_ROOMS_PER_FLOOR;
                    adj_matrix[curr_idx][upper_idx] = 3; 
                    adj_matrix[upper_idx][curr_idx] = 3;
                }
                if (r == 1 && f == 1) {
                    int mess_dist = (b == 0) ? 5 : 15; 
                    adj_matrix[curr_idx][MESS_ID] = mess_dist;
                    adj_matrix[MESS_ID][curr_idx] = mess_dist;
                    int exit_dist = (b == 0) ? 10 : 8; 
                    adj_matrix[curr_idx][EXIT_ID] = exit_dist;
                    adj_matrix[EXIT_ID][curr_idx] = exit_dist;
                }
            }
        }
    }
    compute_floyd_warshall();
}
int get_first_node_in_block_floor(char block, int floor) {
    for (int i = 0; i < TOTAL_ROOMS; i++) {
        if (nodes[i].block == block && nodes[i].floor == floor) {
            return i;
        }
    }
    return 0; 
}
void allocate_room_process() {
    char name[50], pref_block;
    int student_id, pref_floor;
    printf("\n--- Room Allocation ---\n");
    printf("Enter Student ID: ");
    scanf("%d", &student_id);
    printf("Enter Student Name: ");
    scanf("%s", name);
    printf("Preferred Block (A or B): ");
    scanf(" %c", &pref_block);
    printf("Preferred Floor (1 to 3): ");
    scanf("%d", &pref_floor);
    if (pref_block != 'A' && pref_block != 'B') pref_block = 'A';
    if (pref_floor < 1 || pref_floor > NUM_FLOORS) pref_floor = 1;
    int best_node = -1;
    int min_suitability = INF;
    for(int i = 0; i < TOTAL_ROOMS; i++) {
        if(nodes[i].status == 1 && nodes[i].block == pref_block && nodes[i].floor == pref_floor) {
             int score = get_suitability_score(i);
             if (score < min_suitability) {
                 min_suitability = score;
                 best_node = i;
             }
        }
    }
    if (best_node == -1) {
        printf("Preferred section full. Searching nearest vacant room using BFS...\n");
        int starting_node = get_first_node_in_block_floor(pref_block, pref_floor);
        best_node = find_nearest_vacant_bfs(starting_node);
    }
    if (best_node == -1) {
        printf("Searching for most suitable room overall...\n");
        min_suitability = INF;
        for (int i = 0; i < TOTAL_ROOMS; i++) {
            if (nodes[i].status == 1) {
                int score = get_suitability_score(i);
                if (score < min_suitability) {
                    min_suitability = score;
                    best_node = i;
                }
            }
        }
    }
    if (best_node != -1) {
        nodes[best_node].status = 0; 
        students[student_count].id = student_id;
        strcpy(students[student_count].name, name);
        students[student_count].room_node = best_node;
        student_count++;
        printf("\n=> Success! Allocated Room %c-%03d to %s\n", nodes[best_node].block, nodes[best_node].room_number, name);
        printf("   Shortest Distance to Mess: %d units\n", shortest_distance[best_node][MESS_ID]);
        printf("   Shortest Distance to Exit: %d units\n", shortest_distance[best_node][EXIT_ID]);
    } else {
        printf("\n=> Sorry, no vacant rooms available in the entire hostel.\n");
    }
}
void search_room_by_student() {
    int sid;
    printf("\nEnter Student ID to search: ");
    scanf("%d", &sid);
    for (int i = 0; i < student_count; i++) {
        if (students[i].id == sid) {
            int rn = students[i].room_node;
            printf("Student %s is allocated in Room %c-%03d\n", students[i].name, nodes[rn].block, nodes[rn].room_number);
            return;
        }
    }
    printf("Student ID not found.\n");
}
void display_all_vacant() {
    printf("\n--- Vacant Rooms ---\n");
    int count = 0;
    for (int i = 0; i < TOTAL_ROOMS; i++) {
        if (nodes[i].status == 1) {
            printf("%c-%03d ", nodes[i].block, nodes[i].room_number);
            count++;
            if (count % 10 == 0) printf("\n");
        }
    }
    printf("\nTotal Vacant: %d\n", count);
}
void trigger_deep_inspection() {
    char blk;
    printf("\nEnter Block to inspect deeply (A or B): ");
    scanf(" %c", &blk);
    if(blk != 'A' && blk != 'B') {
        printf("Invalid Block.\n"); return;
    }
    printf("\n--- Deep Inspection (DFS) for Block %c ---\n", blk);
    int visited[TOTAL_NODES] = {0};
    int start_n = get_first_node_in_block_floor(blk, 1);
    deep_inspection_dfs(start_n, visited, blk);
}
void main_menu() {
    int choice;
    do {
        printf("\n=========================================\n");
        printf("   Graph-based Hostel Allocation System  \n");
        printf("=========================================\n");
        printf("1. Register Student & Auto Allocate Room\n");
        printf("2. Display All Vacant Rooms\n");
        printf("3. Search Room by Student ID\n");
        printf("4. Deep Inspection of Block (DFS)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: allocate_room_process(); break;
            case 2: display_all_vacant(); break;
            case 3: search_room_by_student(); break;
            case 4: trigger_deep_inspection(); break;
            case 5: printf("Exiting system...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 5);
}
int main() {
    initialize_data();
    main_menu();
    return 0;
}
