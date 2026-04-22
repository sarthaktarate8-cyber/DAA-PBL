# Hostel Allocation Management System using Graph Algorithms

## Project Objective
The system automatically allocates vacant hostel rooms to students according to room availability, block/floor suitability, and shortest path/accessibility using graph algorithms. 

It handles multiple blocks (A, B), multiple floors (1 to 3), and multiple rooms (5 rooms per floor). The system supports a priority-based allocation logic leveraging graph traversal and minimum distance paths.

## Core Features
1. **Dynamic Room Allocation**: Allocates rooms based on student preference, suitability, and real-time vacancy.
2. **Shortest Distance Awareness**: Distances to key facilities like Mess and Exit are automatically computed.
3. **Vacancy and Occupancy Tracking**: Tracks empty, occupied, and under-maintenance rooms.
4. **Deep Graph Inspection**: Analyzes the status of entire blocks at once.

## Graph Algorithms Used

### 1. Breadth First Search (BFS)
**Use Case**: Used as a fallback mechanism to find the nearest vacant room starting from the student's preferred block/floor if their exact choice is occupied. Its level-by-level traversal ensures that the topologically closest room is picked first.
* **Time Complexity**: $O(V + E)$ where $V$ is the number of rooms and $E$ is the number of connections. Since $E \approx V$ in this grid-like hostel graph, it is $O(V)$. 
* **Space Complexity**: $O(V)$ for the queue.

### 2. Depth First Search (DFS)
**Use Case**: Used for deep inspection of a specific block. It recursively visits connected rooms within the same block, providing an exhaustive occupancy and maintenance report.
* **Time Complexity**: $O(V + E)$ for inspecting the block.
* **Space Complexity**: $O(V)$ for recursion stack.

### 3. Floyd-Warshall Algorithm
**Use Case**: Used to precompute the all-pairs shortest paths between every room and major facilities (Mess, Exit). This allows the system to instantly lookup distances in $O(1)$ time upon allocation and calculate a "suitability score."
* **Time Complexity**: $O(V^3)$. Given our small to moderate hostel size constraints, this static DP calculation during initialization is feasible and makes lookup ultra-fast.
* **Space Complexity**: $O(V^2)$ to store the distance matrix.

## Allocation Logic
Priority hierarchy during room allocation:
1. Student precisely prefers a room, and its vacancy is checked.
2. Filter the preferred block/floor for the highest suitability score (closest to Mess & Exit).
3. Fallback: Use BFS to locate the shortest path to an available room outside initial preferences.

## How to Compile and Run
Ensure you have GCC installed.

**Compile all files together:**
```bash
gcc main.c bfs.c dfs.c floyd_warshall.c -o hostel_allocation
```

**Run the program:**
```bash
./hostel_allocation
```
*(On Windows cmd/powershell run `hostel_allocation.exe`)*

## Input Example / Test Cases

### Test Case 1: Simple Allocation (Preferred floor has vacant capacity)
- **Action**: Register Student
- **Inputs**: ID=101, Name=Sarthak, Block=A, Floor=2
- **Expected Outcome**: Allocates a room (e.g., A-201 or A-203 with smallest suitability distance).

### Test Case 2: Deep Inspection 
- **Action**: Deep Inspection (DFS)
- **Inputs**: Block=B
- **Expected Outcome**: Recursively prints all rooms in Block B and their current status (Vacant, Occupied, Maintenance).

### Test Case 3: Display Vacant
- **Action**: Select Display All Vacant Rooms
- **Expected Outcome**: Lists all 100+ style rooms initially, excluding pre-occupied (A-102) and maintenance (A-304) demo data.
