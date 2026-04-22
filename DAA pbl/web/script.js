const NUM_BLOCKS = 2;
const NUM_FLOORS = 3;
const NUM_ROOMS_PER_FLOOR = 5;
const TOTAL_ROOMS = NUM_BLOCKS * NUM_FLOORS * NUM_ROOMS_PER_FLOOR;
const MESS_ID = TOTAL_ROOMS;
const EXIT_ID = TOTAL_ROOMS + 1;
const TOTAL_NODES = TOTAL_ROOMS + 2;
const INF = 99999;
let nodes = [];
let adjMatrix = Array(TOTAL_NODES).fill(null).map(() => Array(TOTAL_NODES).fill(INF));
let shortestDistance = Array(TOTAL_NODES).fill(null).map(() => Array(TOTAL_NODES).fill(INF));
let students = []; 
function initializeData() {
    let nodeIdx = 0;
    for (let b = 0; b < NUM_BLOCKS; b++) {
        let blockChar = String.fromCharCode(65 + b); 
        for (let f = 1; f <= NUM_FLOORS; f++) {
            for (let r = 1; r <= NUM_ROOMS_PER_FLOOR; r++) {
                let status = 1; 
                if (f === 1 && r === 2) status = 0;
                if (f === 3 && r === 4) status = -1;
                nodes.push({
                    id: nodeIdx,
                    block: blockChar,
                    floor: f,
                    roomNumber: (f * 100) + r,
                    status: status,
                    type: "Room"
                });
                nodeIdx++;
            }
        }
    }
    nodes.push({ id: MESS_ID, block: '-', floor: 0, roomNumber: 0, type: "Mess", status: 0 });
    nodes.push({ id: EXIT_ID, block: '-', floor: 0, roomNumber: 0, type: "Exit", status: 0 });
    for (let i = 0; i < TOTAL_NODES; i++) adjMatrix[i][i] = 0;
    for (let b = 0; b < NUM_BLOCKS; b++) {
        for (let f = 1; f <= NUM_FLOORS; f++) {
            for (let r = 1; r <= NUM_ROOMS_PER_FLOOR; r++) {
                let currIdx = (b * NUM_FLOORS * NUM_ROOMS_PER_FLOOR) + ((f - 1) * NUM_ROOMS_PER_FLOOR) + (r - 1);
                if (r < NUM_ROOMS_PER_FLOOR) {
                    let nextIdx = currIdx + 1;
                    adjMatrix[currIdx][nextIdx] = 1; 
                    adjMatrix[nextIdx][currIdx] = 1;
                }
                if (r === 1 && f < NUM_FLOORS) {
                    let upperIdx = currIdx + NUM_ROOMS_PER_FLOOR;
                    adjMatrix[currIdx][upperIdx] = 3;
                    adjMatrix[upperIdx][currIdx] = 3;
                }
                if (r === 1 && f === 1) {
                    let messDist = (b === 0) ? 5 : 15; 
                    adjMatrix[currIdx][MESS_ID] = messDist;
                    adjMatrix[MESS_ID][currIdx] = messDist;
                    let exitDist = (b === 0) ? 10 : 8; 
                    adjMatrix[currIdx][EXIT_ID] = exitDist;
                    adjMatrix[EXIT_ID][currIdx] = exitDist;
                }
            }
        }
    }
    computeFloydWarshall();
    renderTopology();
}
function computeFloydWarshall() {
    for (let i = 0; i < TOTAL_NODES; i++) {
        for (let j = 0; j < TOTAL_NODES; j++) {
            shortestDistance[i][j] = adjMatrix[i][j];
        }
    }
    for (let k = 0; k < TOTAL_NODES; k++) {
        for (let i = 0; i < TOTAL_NODES; i++) {
            for (let j = 0; j < TOTAL_NODES; j++) {
                if (shortestDistance[i][k] !== INF && shortestDistance[k][j] !== INF) {
                    if (shortestDistance[i][k] + shortestDistance[k][j] < shortestDistance[i][j]) {
                        shortestDistance[i][j] = shortestDistance[i][k] + shortestDistance[k][j];
                    }
                }
            }
        }
    }
}
function getSuitabilityScore(roomId) {
    if (roomId < 0 || roomId >= TOTAL_ROOMS) return INF;
    let distMess = shortestDistance[roomId][MESS_ID];
    let distExit = shortestDistance[roomId][EXIT_ID];
    return distMess + distExit;
}
function findNearestVacantBFS(startNode) {
    if (startNode < TOTAL_ROOMS && nodes[startNode].status === 1) return startNode;
    let visited = Array(TOTAL_NODES).fill(false);
    let queue = [startNode];
    visited[startNode] = true;
    while (queue.length > 0) {
        let curr = queue.shift();
        if (curr < TOTAL_ROOMS && nodes[curr].status === 1) return curr;
        for (let i = 0; i < TOTAL_NODES; i++) {
            if (adjMatrix[curr][i] !== INF && adjMatrix[curr][i] > 0 && !visited[i]) {
                visited[i] = true;
                queue.push(i);
            }
        }
    }
    return -1;
}
function deepInspectionDFS(currentNode, visited, targetBlock, logArray) {
    visited[currentNode] = true;
    if (currentNode < TOTAL_ROOMS && nodes[currentNode].block === targetBlock) {
        let statusStr = "Unknown";
        if (nodes[currentNode].status === 1) statusStr = "Vacant";
        if (nodes[currentNode].status === 0) statusStr = "Occupied";
        if (nodes[currentNode].status === -1) statusStr = "Maintenance";
        logArray.push(`Room ${nodes[currentNode].block}-${nodes[currentNode].roomNumber} : ${statusStr}`);
    }
    for (let i = 0; i < TOTAL_NODES; i++) {
        if (adjMatrix[currentNode][i] !== INF && adjMatrix[currentNode][i] > 0 && !visited[i]) {
            if (i < TOTAL_ROOMS && nodes[i].block === targetBlock) {
                deepInspectionDFS(i, visited, targetBlock, logArray);
            }
        }
    }
}
function getFirstNode(block, floor) {
    let target = nodes.find(n => n.block === block && n.floor == floor);
    return target ? target.id : 0;
}
function renderTopology() {
    const container = document.getElementById("graphVisualization");
    container.innerHTML = "";
    for (let b = 0; b < NUM_BLOCKS; b++) {
        let blockChar = String.fromCharCode(65 + b);
        let blockHTML = `<div class="block-container"><div class="block-title">Block ${blockChar}</div>`;
        for (let f = NUM_FLOORS; f >= 1; f--) {
            blockHTML += `<div class="floor"><div class="floor-label">Floor ${f}</div>`;
            for (let r = 1; r <= NUM_ROOMS_PER_FLOOR; r++) {
                let room = nodes.find(n => n.block === blockChar && n.floor === f && n.roomNumber % 100 === r);
                if (room) {
                    let cname = room.status === 1 ? 'vacant' : (room.status === 0 ? 'occupied' : 'maintenance');
                    blockHTML += `<div class="room ${cname}" id="room_${room.id}">${room.roomNumber}</div>`;
                }
            }
            blockHTML += `</div>`;
        }
        blockHTML += `</div>`;
        container.innerHTML += blockHTML;
    }
}
document.getElementById('allocateForm').addEventListener('submit', function(e) {
    e.preventDefault();
    let sid = parseInt(document.getElementById('studentId').value);
    let sname = document.getElementById('studentName').value;
    let pBlock = document.getElementById('prefBlock').value;
    let pFloor = parseInt(document.getElementById('prefFloor').value);
    if(students.find(s => s.id === sid)) {
        showOutputModal("Error", `Student ID ${sid} is already registered.`);
        return;
    }
    let logs = [];
    logs.push(`> Received Allocation Request for ${sname} [ID: ${sid}]`);
    logs.push(`> Preference: Block ${pBlock}, Floor ${pFloor}`);
    let bestNode = -1;
    let minSuitability = INF;
    logs.push(`> Step 1: Searching for best room in preferred location...`);
    for (let i = 0; i < TOTAL_ROOMS; i++) {
        if (nodes[i].status === 1 && nodes[i].block === pBlock && nodes[i].floor === pFloor) {
            let score = getSuitabilityScore(i);
            if (score < minSuitability) {
                minSuitability = score;
                bestNode = i;
            }
        }
    }
    if (bestNode === -1) {
        logs.push(`> Step 1 Failed: Preferred section is full.`);
        logs.push(`> Step 2: Executing BFS Graph Traversal to find nearest vacant room...`);
        let start = getFirstNode(pBlock, pFloor);
        bestNode = findNearestVacantBFS(start);
    }
    if (bestNode !== -1) {
        nodes[bestNode].status = 0; 
        students.push({ id: sid, name: sname, roomNode: bestNode });
        logs.push(`> Allocation SUCCESS!`);
        logs.push(`> Assigned Room: ${nodes[bestNode].block}-${nodes[bestNode].roomNumber}`);
        logs.push(`> Path to Mess: ${shortestDistance[bestNode][MESS_ID]} units`);
        logs.push(`> Path to Exit: ${shortestDistance[bestNode][EXIT_ID]} units`);
        renderTopology();
        showOutputModal("Allocation Complete", logs.join("\n"));
        let div = document.getElementById(`room_${bestNode}`);
        if(div) {
            div.classList.add("highlight");
            setTimeout(() => div.classList.remove("highlight"), 3000);
        }
    } else {
        logs.push(`> FAILURE: Error 404 - No vacant rooms found in the entire matrix.`);
        showOutputModal("Allocation Failed", logs.join("\n"));
    }
});
function runDFS() {
    document.getElementById('dfsModal').style.display = 'none';
    let block = document.getElementById('dfsBlock').value;
    let visited = Array(TOTAL_NODES).fill(false);
    let startNode = getFirstNode(block, 1);
    let logs = [];
    logs.push(`> Deep Inspection (DFS) trace triggered for Block ${block}...`);
    logs.push(`> Output order corresponds to network edge traversal:`);
    logs.push(``);
    deepInspectionDFS(startNode, visited, block, logs);
    showOutputModal(`DFS Results: Block ${block}`, logs.join("\n"));
}
function runSearch() {
    document.getElementById('searchModal').style.display = 'none';
    let sid = parseInt(document.getElementById('searchId').value);
    let logs = [`> Searching Database for ID [${sid}]...`];
    let student = students.find(s => s.id === sid);
    if (student) {
        let r = nodes[student.roomNode];
        logs.push(`> Match Found!`);
        logs.push(`> Name: ${student.name}`);
        logs.push(`> Allocated Room: ${r.block}-${r.roomNumber}`);
        let div = document.getElementById(`room_${r.id}`);
        if (div) {
            div.classList.add("highlight");
            setTimeout(() => div.classList.remove("highlight"), 3000);
        }
    } else {
        logs.push(`> ID not found in current session.`);
    }
    showOutputModal("Search Results", logs.join("\n"));
}
function showVacant() {
    let logs = [`> Querying Vacancy Status...`];
    let numVacant = nodes.filter(n => n.type === "Room" && n.status === 1).length;
    logs.push(`> Active Vacant Rooms: ${numVacant} out of ${TOTAL_ROOMS}`);
    logs.push(`> Visual Dashboard Updated.`);
    showOutputModal("Vacancy Query", logs.join("\n"));
}
function showOutputModal(title, bodyText) {
    document.getElementById('modalTitle').innerText = title;
    document.getElementById('modalBody').innerText = bodyText;
    document.getElementById('outputModal').style.display = 'flex';
}
function closeOutput() {
    document.getElementById('outputModal').style.display = 'none';
}
window.onload = initializeData;
