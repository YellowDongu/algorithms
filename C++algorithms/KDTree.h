#pragma once
#include <vector>
#include <stack>
#include <iostream>
#include <algorithm>
#include <tuple>

struct vec3
{
    float x, y, z;

    vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : x(x), y(y), z(z)
    {
    }

    // 좌표의 x와 z만 비교하는 연산자 (정렬 기준으로 사용)
    bool operator<(const vec3& other) const
    {
        if (x == other.x)
            return z < other.z;
        return x < other.x;
    }
};
// K-d 트리의 노드
struct KDTreeNode
{
    vec3 point;
    KDTreeNode* left;
    KDTreeNode* right;

    KDTreeNode(const vec3& pt)
        : point(pt), left(nullptr), right(nullptr)
    {
    }
};

class KDTree
{
public:
    KDTree() : root(nullptr) {}

    // 좌표가 저장된 벡터로 K-d 트리를 구축
    void build(const std::vector<vec3>& points)
    {
        points_ = points;
        root = buildIterative(points_);  // 반복문을 사용하여 트리 구축
    }

    // 트리의 루트 노드를 반환
    KDTreeNode* getRoot()
    {
        return root;
    }

    // 트리를 출력 (중위 순회)
    void printTree(KDTreeNode* node)
    {
        if (node == nullptr) return;
        printTree(node->left);
        std::cout << "(" << node->point.x << ", " << node->point.y << ", " << node->point.z << ")" << std::endl;
        printTree(node->right);
    }

    // 좌표가 변경된 경우, 전체 재구성
    void rebuild()
    {
        root = nullptr;
        build(points_);
    }

private:
    // 반복문을 사용하여 K-d 트리 구축
    KDTreeNode* buildIterative(std::vector<vec3>& points)
    {
        if (points.empty()) return nullptr;

        // 스택을 사용하여 트리를 구축합니다.
        std::stack<std::pair<std::vector<vec3>, int>> stack;
        stack.push({ points, 0 });

        KDTreeNode* root = nullptr;
        std::vector<KDTreeNode*> nodeStack;  // 생성된 노드들을 스택에 저장

        std::vector<vec3> pointList;
        int depth;

        while (!stack.empty())
        {
            std::tie(pointList, depth) = stack.top();
            stack.pop();

            // 현재 깊이에 해당하는 차원 선택 (x, z 차원으로만 분할)
            int axis = depth % 2;  // 0: x, 1: z

            // 좌표를 기준으로 정렬 (x와 z만 기준으로)
            std::sort(pointList.begin(), pointList.end(), [axis](const vec3& a, const vec3& b)
                {
                    if (axis == 0)
                        return a.x < b.x;  // x 기준 정렬
                    return a.z < b.z;      // z 기준 정렬
                });

            // 중간값을 루트로 설정
            int medianIndex = pointList.size() / 2;
            KDTreeNode* node = new KDTreeNode(pointList[medianIndex]);

            // 스택에 왼쪽과 오른쪽 부분을 추가
            std::vector<vec3> left(pointList.begin(), pointList.begin() + medianIndex);
            std::vector<vec3> right(pointList.begin() + medianIndex + 1, pointList.end());

            if (!left.empty()) stack.push({ left, depth + 1 });
            if (!right.empty()) stack.push({ right, depth + 1 });

            nodeStack.push_back(node);
        }

        // 생성된 노드들을 연결
        for (int i = 0; i < nodeStack.size(); ++i)
        {
            if (i * 2 + 1 < nodeStack.size())
            {
                nodeStack[i]->left = nodeStack[i * 2 + 1];
            }
            if (i * 2 + 2 < nodeStack.size())
            {
                nodeStack[i]->right = nodeStack[i * 2 + 2];
            }
        }

        return nodeStack[0];
    }

private:
    KDTreeNode* root;  // 트리의 루트
    std::vector<vec3> points_;  // 원본 데이터 (좌표 리스트)
};

// 예시 사용
int main()
{
    // 3D 좌표 리스트 (x, y, z)
    std::vector<vec3> points = {
        {2.0f, 3.0f, 1.0f},
        {5.0f, 4.0f, 2.0f},
        {9.0f, 6.0f, 3.0f},
        {4.0f, 7.0f, 4.0f},
        {8.0f, 1.0f, 5.0f},
        {7.0f, 2.0f, 6.0f}
    };

    // K-d 트리 생성 및 구축
    KDTree tree;
    tree.build(points);

    // 트리 출력
    std::cout << "Initial Tree:" << std::endl;
    tree.printTree(tree.getRoot());

    // 좌표 변경 후 전체 재구성
    points = {
        {10.0f, 10.0f, 10.0f},
        {1.0f, 2.0f, 1.0f},
        {4.0f, 5.0f, 3.0f},
        {6.0f, 7.0f, 2.0f},
        {3.0f, 4.0f, 7.0f}
    };

    std::cout << "\nRebuilding Tree..." << std::endl;
    tree.build(points);
    tree.rebuild();  // 전체 재구성

    // 재구성된 트리 출력
    std::cout << "Rebuilt Tree:" << std::endl;
    tree.printTree(tree.getRoot());

    return 0;
}