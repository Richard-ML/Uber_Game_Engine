#include "cPathFindingManager.h"
#include "global.h"

cPathNode::cPathNode() : travelCostFactor(1.f) {}

cPathNode::~cPathNode() { connections.clear(); }

cPathConnection::cPathConnection(cPathNode *nodeA, cPathNode *nodeB)
    : nodeA(nodeA), nodeB(nodeB) {
  distance = glm::distance(nodeA->position, nodeB->position);
  distanceSquared = std::pow(distance, 2);
  difference = nodeA->position - nodeB->position;
}

cPathNode *cPathConnection::getOther(cPathNode *node) {
  return (node == nodeA) ? nodeB : nodeA;
}

cPathConnection::cPathConnection() {}

cPathConnection::~cPathConnection() {}

cPathNodeGrid::cPathNodeGrid(float radius, int nodesA, int nodesB,
                             const glm::vec3 init, const glm::vec3 edgeA,
                             const glm::vec3 edgeB) {
  clear();
  int rows = nodesA;
  int cols = nodesB;
  glm::vec3 sepA = edgeA / (float)(nodesA - 1);
  glm::vec3 sepB = edgeB / (float)(nodesB - 1);
  for (int idxRow = 0; idxRow < rows; idxRow++) {
    glm::vec3 posA = init + sepA * float(idxRow);
    std::vector<cPathNode *> row;
    for (int idxCol = 0; idxCol < cols; idxCol++) {
      cPathNode *node = new cPathNode();
      node->color = glm::vec4(g_vec_WorldTiles[idxRow][idxCol][0], 1.0f);
      node->radius = radius;
      node->position = posA + sepB * float(idxCol);
      row.push_back(node);
    }
    nodes.push_back(row);
  }
  for (int idxRow = 0; idxRow < rows - 1; idxRow++) {
    for (int idxCol = 0; idxCol < cols - 1; idxCol++) {
      // grab dem nodes
      cPathNode *node = nodes[idxRow][idxCol]; // node
      cPathNode *neighbourEast =
          nodes[idxRow][idxCol + 1]; // east (right) neighbor
      cPathNode *neighbourSouth =
          nodes[idxRow + 1][idxCol]; // south (down) neighbor
                                     // make the connections
      cPathConnection *connectEW = new cPathConnection(node, neighbourEast);
      cPathConnection *connectNS = new cPathConnection(node, neighbourSouth);
      // add the connections
      connections.push_back(connectEW);
      connections.push_back(connectNS);
      // let the nodes know about their connections
      node->connections.push_back(connectEW);
      neighbourEast->connections.push_back(connectEW);
      node->connections.push_back(connectNS);
      neighbourSouth->connections.push_back(connectNS);
    }
  }
}

cPathNodeGrid::cPathNodeGrid() {}

cPathNodeGrid::~cPathNodeGrid() { clear(); }

void cPathNodeGrid::clear() {
  std::vector<std::vector<cPathNode *>>::iterator itRow = nodes.begin();
  while (itRow != nodes.end()) {
    std::vector<cPathNode *>::iterator itNode = itRow->begin();
    while (itNode != itRow->end()) {
      delete (*itNode);
      itNode++;
    }
    itRow->clear();
    itRow++;
  }
  nodes.clear();
  std::vector<cPathConnection *>::iterator itConnection = connections.begin();
  while (itConnection != connections.end()) {
    delete (*itConnection);
    itConnection++;
  }
  connections.clear();
}

int cPathNodeGrid::Rows() { return nodes.size(); }

int cPathNodeGrid::Cols() { return nodes[0].size(); }

cPathNode *cPathNodeGrid::GetNodeAt(int row, int col) {
  return nodes[row][col];
}

glm::vec3 cPathNodeGrid::getRandomAdjacentTilePos(int row, int col) {
  cPathNode *node = g_pPathFindingManager->GetNodeAt(row, col);
  return node->connections
      .at(glm::linearRand(0, (int)node->connections.size() - 1))
      ->getOther(node)
      ->position;
}

float cPathNodeGrid::ManhattanDist(cPathNode *nodeA, cPathNode *nodeB) {
  glm::vec3 sep = nodeA->position - nodeB->position;
  return abs(sep.x) + abs(sep.y) + abs(sep.z);
}
// give this source row and column, and a destination row and column
// it fills out a vector of path nodes from that source to that destination
// bool return value is for success/failure
bool cPathNodeGrid::AStar(int rowSrc, int colSrc, int rowDst, int colDst,
                          std::vector<cPathNode *> &path) {
  path.resize(0);

  std::vector<std::vector<sAStarUtilities *>> helpers;

  unsigned int numRows = Rows();
  unsigned int numCols = Cols();
  // just initializing the helpers
  for (unsigned int idxRow = 0; idxRow < numRows; idxRow++) {
    helpers.push_back(std::vector<sAStarUtilities *>());
    for (unsigned int idxCol = 0; idxCol < numCols; idxCol++) {
      sAStarUtilities *helper = new sAStarUtilities();
      helper->row = idxRow;
      helper->col = idxCol;
      helper->node = nodes[idxRow][idxCol];
      helper->parent = 0;
      helper->fScore = std::numeric_limits<float>::max();
      helper->gScore = std::numeric_limits<float>::max();
      helpers[idxRow].push_back(helper);
    }
  }
  // get the actual nodes for the start and end
  cPathNode *startNode = nodes[rowSrc][colSrc];
  cPathNode *endNode = nodes[rowDst][colDst];

  std::vector<sAStarUtilities *> closedSet;
  std::vector<sAStarUtilities *> openSet;

  // set up the open set to begin
  openSet.push_back(helpers[rowSrc][colSrc]);
  helpers[rowSrc][colSrc]->gScore = 0.f;
  helpers[rowSrc][colSrc]->fScore =
      helpers[rowSrc][colSrc]->gScore + ManhattanDist(startNode, endNode);

  sAStarUtilities *current;

  while (!openSet.empty()) {
    // get the node in openSet with the lowest fScoreValue
    current = 0;
    float bestF = std::numeric_limits<float>::max();
    unsigned int openSetSize = openSet.size();
    unsigned int idxOpenBestF = 0;
    for (unsigned int idx = 0; idx < openSetSize; idx++) {
      if (openSet[idx]->fScore < bestF) {
        bestF = openSet[idx]->fScore;
        current = openSet[idx];
        idxOpenBestF = idx;
      }
    }

    if (current == 0) // shouldn't happen
    {
      bool sexybreakpoint = true;
    }

    if (current->node == endNode) // completion condition, we're there
    {
      std::cout << "FINAL PATH: " << std::endl;
      std::cout << current->row << " " << current->col << std::endl;
      // reconstruct the path form cameFrom
      path.push_back(current->node);
      sAStarUtilities *parent = current->parent;
      while (parent != 0) {
        std::cout << parent->row << " " << parent->col << std::endl;
        path.push_back(parent->node);
        parent = parent->parent;
      }
      openSet.clear();
      break;
    } else {
      // add it to the closed list
      closedSet.push_back(current);
      // remove it from the open list
      openSet.erase(openSet.begin() + idxOpenBestF);

      std::vector<cPathConnection *>::iterator itPath =
          current->node->connections.begin();
      while (itPath !=
             current->node->connections.end()) // for each neighbor of current
      {
        cPathNode *neighbourNode = (*itPath)->getOther(current->node);
        sAStarUtilities *neighbourHelper = 0;
        for (unsigned int idxRow = 0; idxRow < numRows && !neighbourHelper;
             idxRow++) {
          for (unsigned int idxCol = 0; idxCol < numCols && !neighbourHelper;
               idxCol++) {
            if (neighbourNode == helpers[idxRow][idxCol]->node) {
              neighbourHelper = helpers[idxRow][idxCol];
            }
          }
        }

        // are you in the closed set?
        if (std::find(closedSet.begin(), closedSet.end(), neighbourHelper) !=
            closedSet.end()) {
          itPath++;
          continue; // ignore the neighbor which is already evaluated
        }

        float tentativeGScore =
            current->gScore +
            (*itPath)->distance * neighbourNode->travelCostFactor;

        // if you're not already in the open set
        if (std::find(openSet.begin(), openSet.end(), neighbourHelper) ==
            openSet.end()) {
          openSet.push_back(neighbourHelper); // discover a new node!
        } else if (tentativeGScore >= neighbourHelper->gScore) {
          itPath++;
          continue; // this is not a better path
        }

        // this path is the best until now, record it!
        neighbourHelper->parent = current;
        neighbourHelper->gScore = tentativeGScore;
        neighbourHelper->fScore =
            neighbourHelper->gScore + ManhattanDist(neighbourNode, endNode);

        itPath++;
      }
    }
  }

  for (unsigned int idxRow = 0; idxRow < numRows; idxRow++) {
    for (unsigned int idxCol = 0; idxCol < numCols; idxCol++) {
      delete helpers[idxRow][idxCol];
    }
  }

  return !path.empty();
}