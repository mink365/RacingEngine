#include "ControlPoints.h"

void re::ControlPoints::cacheVertex() {
    for (int i = 0; i < this->controlPoints.size(); ++i) {
      std::vector<int> vertexIndexList;

      for (int j = 0; j < this->vertexToControl.size(); ++j) {
        int &controlIndex = vertexToControl[j];

        if (controlIndex == i) {
          vertexIndexList.push_back(j);
        }
      }

      this->controlToVertex.push_back(vertexIndexList);
    }
}
