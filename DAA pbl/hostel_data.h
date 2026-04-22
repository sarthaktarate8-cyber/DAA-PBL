#ifndef HOSTEL_DATA_H
#define HOSTEL_DATA_H
#define NUM_BLOCKS 2
#define NUM_FLOORS 3
#define NUM_ROOMS_PER_FLOOR 5
#define TOTAL_ROOMS (NUM_BLOCKS * NUM_FLOORS * NUM_ROOMS_PER_FLOOR)
#define MESS_ID TOTAL_ROOMS
#define EXIT_ID (TOTAL_ROOMS + 1)
#define NUM_FACILITIES 2
#define TOTAL_NODES (TOTAL_ROOMS + NUM_FACILITIES)
#define INF 99999
typedef struct {
    int node_id;       
    char block;        
    int floor;         
    int room_number;   
    int status;        
    char type[20];     
} Node;
extern Node nodes[TOTAL_NODES];
extern int adj_matrix[TOTAL_NODES][TOTAL_NODES];
extern int shortest_distance[TOTAL_NODES][TOTAL_NODES];
void initialize_data();
int allocate_room(char pref_block, int pref_floor, int student_id, const char* name);
void deallocate_room(int room_number);
void update_vacancy(int room_number, int status);
void display_all_vacant();
void check_occupancy_report();
int find_nearest_vacant_bfs(int start_node);
void deep_inspection_dfs(int current_node, int visited[], char target_block);
void compute_floyd_warshall();
int get_suitability_score(int room_node);
#endif 
