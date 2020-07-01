#pragma once

#include <vector>
#include <unordered_set>
#include <algorithm>

namespace SorasPathfindImplementation {
    struct Node {
        size_t x = 0;
        size_t y = 0;
        size_t costToEnd = 0;
        size_t costToStart = ~0;
        size_t cost() { return costToEnd + costToStart; }
        bool passable = false;
        Node* parent = nullptr;
        std::vector<Node*> neighbours;
    };

    template<size_t MAP_WIDTH = 2, size_t MAP_HEIGHT = 2>
    class AStar {
    public:
        typedef char(*MapPtr)[MAP_HEIGHT];

        AStar(const MapPtr map, const std::pair<size_t, size_t>& startPos, const std::pair<size_t, size_t>& endPos, bool(PassibleCheck)(char));

        bool Step();

        size_t ManhattanDistance(Node* startPos, Node* endPos);

        std::vector<std::pair<size_t, size_t>> GetPath() {
            if (path.empty()) {
                path.resize(mDestination->costToStart + 1);
                Node* curNode = mDestination;
                while (curNode) {
                    path[curNode->costToStart] = std::make_pair(curNode->x, curNode->y);
                    curNode = curNode->parent;
                }
            }

            return path;
        }

    private:

        std::vector<std::pair<size_t, size_t>> path;
        std::unordered_set<Node*> mActiveNodes;
        Node* mDestination = nullptr;

        Node mNodes[MAP_WIDTH][MAP_HEIGHT];
    };

    template<size_t MAP_WIDTH, size_t MAP_HEIGHT>
    inline AStar<MAP_WIDTH, MAP_HEIGHT>::AStar(const MapPtr map, const std::pair<size_t, size_t>& startPos, const std::pair<size_t, size_t>& endPos, bool(PassibleCheck)(char))
    {
        for (size_t y = 0; y < MAP_HEIGHT; ++y) {
            for (size_t x = 0; x < MAP_WIDTH; ++x) {
                mNodes[x][y].x = x;
                mNodes[x][y].y = y;
                mNodes[x][y].passable = !PassibleCheck(map[x][y]);
                if (x != 0)
                    mNodes[x][y].neighbours.push_back(&mNodes[x - 1][y]);
                if (x != MAP_WIDTH - 1)
                    mNodes[x][y].neighbours.push_back(&mNodes[x + 1][y]);
                if (y != 0)
                    mNodes[x][y].neighbours.push_back(&mNodes[x][y - 1]);
                if (y != MAP_HEIGHT - 1)
                    mNodes[x][y].neighbours.push_back(&mNodes[x][y + 1]);
            }
        }
        Node& startNode = mNodes[startPos.first][startPos.second];
        mDestination = &mNodes[endPos.first][endPos.second];
        mActiveNodes.insert(&startNode);
        startNode.costToEnd = ManhattanDistance(&startNode, mDestination);
        startNode.costToStart = 0;
    }

    template<size_t MAP_WIDTH, size_t MAP_HEIGHT>
    inline bool AStar<MAP_WIDTH, MAP_HEIGHT>::Step()
    {
        if (mActiveNodes.empty())
            return true;

        std::unordered_set<Node*>::iterator bestNodeItr = mActiveNodes.begin();
        for (std::unordered_set<Node*>::iterator it = std::next(bestNodeItr); it != mActiveNodes.end(); ++it) {
            if ((*it)->cost() < (*bestNodeItr)->cost()) {
                bestNodeItr = it;
            }
        }

        Node* bestNode = *bestNodeItr;
        for (size_t i = 0; i < bestNode->neighbours.size(); ++i) {
            Node* neighbour = bestNode->neighbours[i];
            if (!neighbour->passable) continue;
            if (neighbour == mDestination) {
                mActiveNodes.clear();
                mDestination->parent = bestNode;
                neighbour->costToStart = bestNode->costToStart + 1;
                return true;
            }

            if (neighbour->costToStart > bestNode->costToStart + 1) {
                neighbour->costToEnd = ManhattanDistance(neighbour, mDestination);
                neighbour->parent = bestNode;
                neighbour->costToStart = bestNode->costToStart + 1;

                if (!mActiveNodes.count(neighbour))
                    mActiveNodes.insert(neighbour);
            }
        }

        mActiveNodes.erase(bestNodeItr);

        return false;
    }

    template<size_t MAP_WIDTH, size_t MAP_HEIGHT>
    inline size_t AStar<MAP_WIDTH, MAP_HEIGHT>::ManhattanDistance(Node* start, Node* end)
    {
        long long x = (long long)end->x - start->x;
        long long y = (long long)end->y - start->y;
        return static_cast<size_t>(std::abs(x) + std::abs(y));
    }
}