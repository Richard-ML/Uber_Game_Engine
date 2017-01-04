#ifndef _cPathFindingManager_HG_
#define _cPathFindingManager_HG_
#include "externals.h"
#include <vector>
/**
*       __  __ __                   ______                           ______               _
*      / / / // /_   ___   _____   / ____/____ _ ____ ___   ___     / ____/____   ____ _ (_)____   ___
*     / / / // __ \ / _ \ / ___/  / / __ / __ `// __ `__ \ / _ \   / __/  / __ \ / __ `// // __ \ / _ \
*    / /_/ // /_/ //  __// /     / /_/ // /_/ // / / / / //  __/  / /___ / / / // /_/ // // / / //  __/
*    \____//_.___/ \___//_/      \____/ \__,_//_/ /_/ /_/ \___/  /_____//_/ /_/ \__, //_//_/ /_/ \___/
*                                                                              /____/
//===-- cPathFindingManager.h - Manages path-finding -------------*- C++ -*-===//
Description: Analyzes tile based environments, and provides simple path-finding functionality.
//===----------------------------------------------------------------------===//
Author(s):
 Name: Richard Mills-Laursen
 Role(s): Student
 E-mail: R_MillsLaursen@FanshaweOnline.ca | Revolut1on1ze@Hotmail.ca
 
 Name: Michael Feeney
 E-mail: MFeeney@FanshaweC.ca
 Role(s): Coordinator & Professor
 Course(s):
  INFO-6044-01 - Game Engine Framework/Patterns
  INFO-6028-01 - Graphics 1
  INFO-6019-01 - Physics & Simulation 1
  INFO-6045-01 - Animation
  INFO-6023-01 - Game Algorithms & Gems
  INFO-6020-01 - Graphics 2
 
 Name: Ben McIlmoyle
 E-Mail: B_McIlmoyle@FanshaweOnline.ca
 Role: Professor
 Course: INFO-6016-01 - Network Programming
 
 Name: James Lucas
 E-Mail: J_Lucas@FanshaweOnline.ca
 Role: Professor
 Course(s):
  INFO-6017-01 - Artificial Intelligence
  INFO-6022-01 - Physics & Simulation 2
 
 Name: Oscar Lara
 E-Mail: O_Lara@FanshaweOnline.ca
 Role: Professor
 Course(s):
  INFO-6046-01 - Media Fundamentals
  INFO-6025-01 - Configuration & Deployment
 
 Name: L. Gustafson
 E-Mail: L_Gustafson@FanshaweOnline.ca
 Role: Professor
 Course: INFO-6024-01 - Game Component Integration
//===----------------------------------------------------------------------===//
Status: Version 1.8 Alpha
(c) Copyright(s): Fanshawe College
//===----------------------------------------------------------------------===//
*/
// Connection between two cPathNodes
class cPathConnection;
// Part of a 2d navigation grid used for path finding
class cPathNode {
public:
  float radius; // Used to determine which node an entity resides in
  glm::vec3 position;
  glm::vec4 color;

  float travelCostFactor;
  std::vector<cPathConnection *> connections;

  cPathNode();
  ~cPathNode();
};
class cPathConnection {
public:
  float distance;
  float distanceSquared;
  glm::vec3 difference;
  cPathNode *nodeA;
  cPathNode *nodeB;

  cPathConnection(cPathNode *nodeA, cPathNode *nodeB);
  cPathNode *getOther(cPathNode *node);
  cPathConnection();
  ~cPathConnection();
};
class cPathNodeGrid {
public:
  struct sAStarUtilities {
    int row;
    int col;
    cPathNode *node;
    sAStarUtilities *parent;
    float gScore;
    float fScore;
  };
  cPathNodeGrid(float radius, int nodesA, int nodesB, const glm::vec3 init,
                const glm::vec3 edgeA, const glm::vec3 edgeB);
  void clear();
  std::vector<cPathConnection *> connections;
  std::vector<std::vector<cPathNode *>> nodes;
  int Rows();
  int Cols();
  glm::vec3 getRandomAdjacentTilePos(int row, int col);
  cPathNode *GetNodeAt(int row, int col);

  float ManhattanDist(cPathNode *nodeA, cPathNode *nodeB);
  bool AStar(int rowSrc, int colSrc, int rowDst, int colDst,
             std::vector<cPathNode *> &path);

  cPathNodeGrid();
  ~cPathNodeGrid();
};
#endif