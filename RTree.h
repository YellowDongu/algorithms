#pragma once
#include <algorithm>
#include <vector>
#include <queue>
#include <iostream>
#include <memory>
/*
 Simple R-Tree (Quadratic split) implementation in C++17.
 - Rect stores axis-aligned rectangles.
 - Entry represents either a child pointer (internal) or a data id (leaf).
 - Node holds entries and isLeaf flag.
 - RTree supports insert(Rect, id), search(range), nearest(query, k).
*/

// --- Rect (MBR) ---
struct Rectangle
{
    double minX, minY, maxX, maxY; // [x1,y1] lower-left, [x2,y2] upper-right

    Rectangle() : minX(0), minY(0), maxX(0), maxY(0) {}
    Rectangle(double x1, double y1, double x2, double y2)
    {
        minX = std::min(x1, x2); maxX = std::max(x1, x2);
        minY = std::min(y1, y2); maxY = std::max(y1, y2);
    }

    double area(void) const { return std::max(0.0, maxX - minX) * std::max(0.0, maxY - minY); }

    static Rectangle combine(const Rectangle& one, const Rectangle& other) { return Rectangle(std::min(one.minX, other.minX), std::min(one.minY, other.minY), std::max(one.maxX, other.maxX), std::max(one.maxY, other.maxY)); }

    bool intersects(const Rectangle& other) const
    {
        if (maxX < other.minX || other.maxX < minX) return false;
        if (maxY < other.minY || other.maxY < minY) return false;
        return true;
    }

    bool contains(const Rectangle& other) const { return minX <= other.minX && minY <= other.minY && maxX >= other.maxX && maxY >= other.maxY; }

    // distance from point (px,py) to this rect (0 if inside)
    double distanceToPoint(double px, double py) const
    {
        double dx = 0.0;
        if (px < minX) dx = minX - px;
        else if (px > maxX) dx = px - maxX;
        double dy = 0.0;
        if (py < minY) dy = minY - py;
        else if (py > maxY) dy = py - maxY;
        return sqrt(dx * dx + dy * dy);
    }

    // minimum possible distance from rect to point squared (for PQ key without sqrt)
    double minDist2(double px, double py) const
    {
        double dx = 0.0;
        if (px < minX) dx = minX - px;
        else if (px > maxX) dx = px - maxX;
        double dy = 0.0;
        if (py < minY) dy = minY - py;
        else if (py > maxY) dy = py - maxY;
        return dx * dx + dy * dy;
    }
};

// --- Forward declaration ---
struct Node;

// --- Entry inside a node ---
struct Entry
{
    Rectangle mbr;
    Node* child; // null if leaf entry (holds id)
    int id;      // valid only if child == nullptr (leaf entry)
    Entry() : child(nullptr), id(-1) {}
    Entry(const Rectangle& r, Node* c, int _id) : mbr(r), child(c), id(_id) {}
};

// --- Node ---
struct Node
{
    bool isLeaf;
    std::vector<Entry> entries;
    Node* parent;

    Node(bool leaf = true) : isLeaf(leaf), parent(nullptr) {}

    Rectangle nodeMBR() const
    {
        if (entries.empty()) return Rectangle(0, 0, 0, 0);
        Rectangle r = entries[0].mbr;
        for (size_t i = 1; i < entries.size(); ++i)
            r = Rectangle::combine(r, entries[i].mbr);
        return r;
    }
};

// --- RTree parameters ---
const int MAX_ENTRIES = 8; // adjust (e.g., 8, 16). bigger -> shallower tree.
const int MIN_ENTRIES = (MAX_ENTRIES + 1) / 2;

// --- RTree class ---
class RTree
{
public:
    RTree(void) { root = new Node(true); }
    ~RTree(void) { destroyNode(root); }

    // insert data rectangle with associated id
    void insert(const Rectangle& object, int id)
    {
        Entry entry(object, nullptr, id);
        Node* leaf = chooseLeaf(root, entry);
        leaf->entries.push_back(entry);
        // link parent of entry is implicit via node->parent
        if ((int)leaf->entries.size() > MAX_ENTRIES)
        {
            Node* n1 = leaf;
            Node* n2 = splitNode(n1);
            adjustTree(n1, n2);
        }
        else
        {
            adjustMBRUpwards(leaf);
        }
    }

    // range search: return ids whose rect intersects query
    inline std::vector<int> search(const Rectangle& range) const
    {
        std::vector<int> result;
        searchNode(root, range, result);
        return result;
    }

    // k nearest neighbor to point (qx,qy), returns pairs (id, distance)
    std::vector<std::pair<int, double>> nearest(double qx, double qy, int k) const
    {
        std::vector<std::pair<int, double>> result;
        // min-heap by distance (we want smallest dist first => use greater)
        using PQItem = std::pair<double, std::pair<Node*, int>>;
        // For nodes: second = (-1) for node; for leaf entry: second = id (>=0)
        auto cmp = [](const PQItem& a, const PQItem& b) { return a.first > b.first; };
        std::priority_queue<PQItem, std::vector<PQItem>, decltype(cmp)> pq(cmp);

        // push root with its minDist2
        pq.push({ root->nodeMBR().minDist2(qx, qy), {root, -1} });

        // temporary container for found leaves (id, dist2)
        std::vector<std::pair<int, double>> found;

        while (!pq.empty())
        {
            auto it = pq.top(); pq.pop();
            double dist2 = it.first;
            Node* node = it.second.first;
            int entId = it.second.second;

            if (node != nullptr && entId == -1)
            {
                // node popped: expand node's entries
                if (node->isLeaf)
                {
                    for (const Entry& e : node->entries)
                    {
                        double d2 = e.mbr.minDist2(qx, qy);
                        // push leaf entry as (d2, {nullptr, id})
                        pq.push({ d2, { (Node*)nullptr, e.id } });
                    }
                }
                else
                {
                    for (const Entry& e : node->entries)
                    {
                        double d2 = e.mbr.minDist2(qx, qy);
                        pq.push({ d2, { e.child, -1 } });
                    }
                }
            }
            else
            {
                // entId >= 0 : an actual data entry
                int id = entId;
                // We need the actual Rect for id to compute exact distance (center or rect->point distance).
                // For simplicity we stored only id; we need a map id->rect. To avoid global state, 
                // we instead store exact distance using entry.mbr.minDist2 earlier; that's okay: minDist2 is exact for distance to rect.
                // Here dist2 is the saved minDist2 value.
                found.push_back({ id, sqrt(dist2) });
                if ((int)found.size() >= k) break;
            }
        }

        // return up to k results (found contains k nearest by rect-dist)
        return found;
    }

private:
    Node* root;

    void destroyNode(Node* node)
    {
        if (!node) return;
        if (!node->isLeaf)
        {
            for (auto& entry : node->entries)
            {
                if (entry.child) destroyNode(entry.child);
            }
        }
        delete node;
    }

    // choose leaf using minimum area enlargement heuristic
    Node* chooseLeaf(Node* start, const Entry& e)
    {
        Node* node = start;
        while (!node->isLeaf)
        {
            // choose child that requires least enlargement, tie by smaller area
            double bestInc = std::numeric_limits<double>::infinity();
            double bestArea = std::numeric_limits<double>::infinity();
            int bestIdx = -1;
            for (int i = 0; i < (int)node->entries.size(); ++i)
            {
                const Rectangle& m = node->entries[i].mbr;
                double area = m.area();
                Rectangle comb = Rectangle::combine(m, e.mbr);
                double inc = comb.area() - area;
                if (inc < bestInc || (inc == bestInc && area < bestArea))
                {
                    bestInc = inc;
                    bestArea = area;
                    bestIdx = i;
                }
            }
            node = node->entries[bestIdx].child;
        }
        return node;
    }

    // adjust MBRs up to root (after non-splitting insertion)
    void adjustMBRUpwards(Node* node)
    {
        while (node != nullptr)
        {
            // update parent's entry.mbr that points to this node
            if (node->parent != nullptr)
            {
                Node* p = node->parent;
                for (auto& e : p->entries)
                {
                    if (e.child == node)
                    {
                        e.mbr = node->nodeMBR();
                        break;
                    }
                }
            }
            node = node->parent;
        }
    }

    // adjust tree after split: n1 and optionally n2 (new node)
    void adjustTree(Node* n1, Node* n2)
    {
        if (n1 == root)
        {
            if (n2)
            {
                // create new root
                Node* newRoot = new Node(false);
                Entry e1(n1->nodeMBR(), n1, -1);
                Entry e2(n2->nodeMBR(), n2, -1);
                newRoot->entries.push_back(e1);
                newRoot->entries.push_back(e2);
                n1->parent = newRoot;
                n2->parent = newRoot;
                root = newRoot;
            }
            return;
        }

        Node* parent = n1->parent;
        // find entry in parent that points to n1 and update its MBR
        bool updated = false;
        for (auto& e : parent->entries)
        {
            if (e.child == n1)
            {
                e.mbr = n1->nodeMBR();
                updated = true;
                break;
            }
        }
        if (!updated)
        {
            // should not happen
        }

        if (n2)
        {
            // create entry for n2 and insert into parent
            Entry e(n2->nodeMBR(), n2, -1);
            parent->entries.push_back(e);
            n2->parent = parent;
            if ((int)parent->entries.size() > MAX_ENTRIES)
            {
                Node* p2 = splitNode(parent);
                adjustTree(parent, p2);
            }
            else
            {
                adjustMBRUpwards(parent);
            }
        }
        else
        {
            adjustMBRUpwards(parent);
        }
    }

    // Quadratic split (Guttman)
    Node* splitNode(Node* node)
    {
        // Create two new groups G1,G2. We'll reuse 'node' as G1 and create new Node G2.
        Node* group1 = node;
        Node* group2 = new Node(node->isLeaf);
        // entries to distribute: copy current entries
        std::vector<Entry> entries = node->entries;
        group1->entries.clear();

        int N = entries.size();

        // 1) Pick first two seeds: choose pair with largest dead area (waste)
        double worstD = -1.0;
        int seed1 = -1, seed2 = -1;
        for (int i = 0; i < N; ++i)
        {
            for (int j = i + 1; j < N; ++j)
            {
                Rectangle comb = Rectangle::combine(entries[i].mbr, entries[j].mbr);
                double d = comb.area() - entries[i].mbr.area() - entries[j].mbr.area();
                if (d > worstD)
                {
                    worstD = d;
                    seed1 = i; seed2 = j;
                }
            }
        }

        // assign seeds
        group1->entries.push_back(entries[seed1]);
        group2->entries.push_back(entries[seed2]);

        // mark assigned
        std::vector<bool> assigned(N, false);
        assigned[seed1] = assigned[seed2] = true;

        // set parents for child pointers if internal nodes
        if (!group1->isLeaf)
        {
            for (auto& e : group1->entries) if (e.child) e.child->parent = group1;
            for (auto& e : group2->entries) if (e.child) e.child->parent = group2;
        }

        // distribute remaining
        int remaining = N - 2;
        while (remaining > 0)
        {
            // if one group needs to take all to satisfy MIN_ENTRIES, do it
            int need1 = std::max(0, MIN_ENTRIES - (int)group1->entries.size());
            int need2 = std::max(0, MIN_ENTRIES - (int)group2->entries.size());
            if (need1 == remaining)
            {
                for (int i = 0; i < N; ++i) if (!assigned[i])
                {
                    group1->entries.push_back(entries[i]);
                    if (!group1->isLeaf && entries[i].child) entries[i].child->parent = group1;
                    assigned[i] = true;
                    --remaining;
                }
                break;
            }
            if (need2 == remaining)
            {
                for (int i = 0; i < N; ++i) if (!assigned[i])
                {
                    group2->entries.push_back(entries[i]);
                    if (!group2->isLeaf && entries[i].child) entries[i].child->parent = group2;
                    assigned[i] = true;
                    --remaining;
                }
                break;
            }

            // choose next entry maximizing preference difference
            double bestDiff = -1.0;
            int bestIdx = -1;
            bool assignTo1 = true;

            Rectangle mbr1 = group1->nodeMBR();
            Rectangle mbr2 = group2->nodeMBR();

            for (int i = 0; i < N; ++i)
            {
                if (assigned[i]) continue;
                const Rectangle& r = entries[i].mbr;
                double area1 = mbr1.area();
                double area2 = mbr2.area();
                double inc1 = Rectangle::combine(mbr1, r).area() - area1;
                double inc2 = Rectangle::combine(mbr2, r).area() - area2;
                double diff = fabs(inc1 - inc2);
                if (diff > bestDiff)
                {
                    bestDiff = diff;
                    bestIdx = i;
                    assignTo1 = (inc1 < inc2) || (inc1 == inc2 && area1 < area2);
                }
            }
            // assign
            if (assignTo1)
            {
                group1->entries.push_back(entries[bestIdx]);
                if (!group1->isLeaf && entries[bestIdx].child) entries[bestIdx].child->parent = group1;
            }
            else
            {
                group2->entries.push_back(entries[bestIdx]);
                if (!group2->isLeaf && entries[bestIdx].child) entries[bestIdx].child->parent = group2;
            }
            assigned[bestIdx] = true;
            --remaining;
        }

        // set parent pointers for group2 children
        if (!group2->isLeaf)
        {
            for (auto& e : group2->entries) if (e.child) e.child->parent = group2;
        }

        // If node was root and had parent pointer null, keep group1's parent as before (may be null)
        group1->parent = node->parent;
        group2->parent = node->parent;

        return group2;
    }

    // recursive range search
    void searchNode(Node* node, const Rectangle& range, std::vector<int>& out) const
    {
        if (!node) return;
        if (!node->nodeMBR().intersects(range)) return;
        if (node->isLeaf)
        {
            for (const Entry& e : node->entries)
            {
                if (e.mbr.intersects(range)) out.push_back(e.id);
            }
        }
        else
        {
            for (const Entry& e : node->entries)
            {
                if (e.mbr.intersects(range)) searchNode(e.child, range, out);
            }
        }
    }
};

// --- Example usage ---
int mainSample()
{
    RTree rtree;

    // Insert some sample rectangles with id
    rtree.insert(Rectangle(10, 10, 12, 12), 1);
    rtree.insert(Rectangle(50, 50, 50, 50), 2); // degenerate point rect
    rtree.insert(Rectangle(53, 52, 53, 52), 3);
    rtree.insert(Rectangle(90, 85, 90, 85), 4);
    rtree.insert(Rectangle(11, 11, 11, 11), 5);
    rtree.insert(Rectangle(200, 200, 201, 201), 6);

    // range search
    Rectangle q(9, 9, 54, 54);
    auto res = rtree.search(q);
    std::cout << "Range search results for [9,9]-[54,54]: ";
    for (int id : res)
        std::cout << id << " ";
    std::cout << "\size";

    // nearest k (simple)
    auto near = rtree.nearest(52, 51, 3);
    std::cout << "Nearest results to (52,51):\size";
    for (auto& p : near)
    {
        std::cout << " id=" << p.first << " dist=" << p.second << "\size";
    }

    return 0;
}














// --------------------- Rect ---------------------
//struct Rectangle
//{
//    double x1, y1, x2, y2;
//    Rectangle() : x1(0), y1(0), x2(0), y2(0) {}
//    Rectangle(double a, double b, double c, double d)
//    {
//        x1 = std::min(a, c); x2 = std::max(a, c);
//        y1 = std::min(b, d); y2 = std::max(b, d);
//    }
//
//    double area() const { return std::max(0.0, x2 - x1) * std::max(0.0, y2 - y1); }
//
//    static Rectangle combine(const Rectangle& a, const Rectangle& b)
//    {
//        return Rectangle(std::min(a.x1, b.x1), std::min(a.y1, b.y1), std::max(a.x2, b.x2), std::max(a.y2, b.y2));
//    }
//
//    bool intersects(const Rectangle& o) const
//    {
//        if (x2 < o.x1 || o.x2 < x1) return false;
//        if (y2 < o.y1 || o.y2 < y1) return false;
//        return true;
//    }
//
//    bool contains(const Rectangle& o) const
//    {
//        return x1 <= o.x1 && y1 <= o.y1 && x2 >= o.x2 && y2 >= o.y2;
//    }
//
//    double distanceToPoint(double px, double py) const
//    {
//        double dx = 0.0, dy = 0.0;
//        if (px < x1) dx = x1 - px; else if (px > x2) dx = px - x2;
//        if (py < y1) dy = y1 - py; else if (py > y2) dy = py - y2;
//        return sqrt(dx * dx + dy * dy);
//    }
//
//    double minDist2(double px, double py) const
//    {
//        double dx = 0.0, dy = 0.0;
//        if (px < x1) dx = x1 - px; else if (px > x2) dx = px - x2;
//        if (py < y1) dy = y1 - py; else if (py > y2) dy = py - y2;
//        return dx * dx + dy * dy;
//    }
//};

// --------------------- Forward ---------------------
//struct Node;

// --------------------- Entry ---------------------
//struct Entry
//{
//    Rect mbr;
//    Node* child; // null if leaf entry
//    int id;      // only valid if child==nullptr
//    Entry() : child(nullptr), id(-1) {}
//    Entry(const Rect& r, Node* c, int _id) : mbr(r), child(c), id(_id) {}
//};

// --------------------- Node ---------------------
//struct Node
//{
//    bool isLeaf;
//    vector<Entry> entries;
//    Node* parent;
//    Node(bool leaf = true) : isLeaf(leaf), parent(nullptr) {}
//    Rect nodeMBR() const
//    {
//        if (entries.empty()) return Rect(0, 0, 0, 0);
//        Rect r = entries[0].mbr;
//        for (size_t i = 1; i < entries.size(); ++i)
//            r = Rect::combine(r, entries[i].mbr);
//        return r;
//    }
//};

// --------------------- R*-Tree Parameters ---------------------
//const int MAX_ENTRIES = 8;
//const int MIN_ENTRIES = (MAX_ENTRIES + 1) / 2;

// --------------------- R*-Tree Class ---------------------
class RStarTree
{
public:
    RStarTree(void) { root = new Node(true); }
    ~RStarTree(void) { destroyNode(root); }

    void insert(const Rectangle& r, int id)
    {
        Entry e(r, nullptr, id);
        Node* leaf = chooseLeaf(root, e);
        leaf->entries.push_back(e);

        if ((int)leaf->entries.size() > MAX_ENTRIES)
        {
            // R*-tree 재삽입
            if (!leaf->isLeaf && leaf != root)
            {
                reinsert(leaf);
            }
            else
            {
                Node* n2 = splitNode(leaf);
                adjustTree(leaf, n2);
            }
        }
        else adjustMBRUpwards(leaf);
    }

    std::vector<int> search(const Rectangle& range) const
    {
        std::vector<int> result;
        searchNode(root, range, result);
        return result;
    }

private:
    Node* root;

    void destroyNode(Node* node)
    {
        if (node == nullptr)
            return;

        if (!node->isLeaf)
            for (auto& e : node->entries) if (e.child) destroyNode(e.child);

        delete node;
    }

    Node* chooseLeaf(Node* node, const Entry& e)
    {
        while (!node->isLeaf)
        {
            double bestInc = 1e18;
            double bestArea = 1e18;
            int bestIdx = -1;
            for (int i = 0; i < (int)node->entries.size(); ++i)
            {
                const Rectangle& m = node->entries[i].mbr;
                double area = m.area();
                double inc = Rectangle::combine(m, e.mbr).area() - area;
                if (inc < bestInc || (inc == bestInc && area < bestArea))
                {
                    bestInc = inc; bestArea = area; bestIdx = i;
                }
            }
            node = node->entries[bestIdx].child;
        }
        return node;
    }

    void adjustMBRUpwards(Node* node)
    {
        while (node)
        {
            if (node->parent)
            {
                Node* p = node->parent;
                for (auto& e : p->entries)
                    if (e.child == node) { e.mbr = node->nodeMBR(); break; }
            }
            node = node->parent;
        }
    }

    void adjustTree(Node* n1, Node* n2)
    {
        if (n1 == root)
        {
            if (n2)
            {
                Node* newRoot = new Node(false);
                Entry e1(n1->nodeMBR(), n1, -1);
                Entry e2(n2->nodeMBR(), n2, -1);
                newRoot->entries.push_back(e1);
                newRoot->entries.push_back(e2);
                n1->parent = newRoot;
                n2->parent = newRoot;
                root = newRoot;
            }
            return;
        }

        Node* parent = n1->parent;
        for (auto& e : parent->entries)
            if (e.child == n1) { e.mbr = n1->nodeMBR(); break; }

        if (n2)
        {
            Entry e(n2->nodeMBR(), n2, -1);
            parent->entries.push_back(e);
            n2->parent = parent;
            if ((int)parent->entries.size() > MAX_ENTRIES)
            {
                Node* p2 = splitNode(parent);
                adjustTree(parent, p2);
            }
            else adjustMBRUpwards(parent);
        }
        else adjustMBRUpwards(parent);
    }

    Node* splitNode(Node* node)
    {
        Node* group1 = node;
        Node* group2 = new Node(node->isLeaf);
        std::vector<Entry> entries = node->entries;
        group1->entries.clear();

        int N = entries.size();
        double worstD = -1;
        int seed1 = -1, seed2 = -1;
        for (int i = 0; i < N; i++)
        {
            for (int j = i + 1; j < N; j++)
            {
                double d = Rectangle::combine(entries[i].mbr, entries[j].mbr).area() - entries[i].mbr.area() - entries[j].mbr.area();
                if (d > worstD) { worstD = d; seed1 = i; seed2 = j; }
            }
        }

        group1->entries.push_back(entries[seed1]);
        group2->entries.push_back(entries[seed2]);
        std::vector<bool> assigned(N, false);
        assigned[seed1] = assigned[seed2] = true;
        int remaining = N - 2;

        while (remaining > 0)
        {
            int need1 = std::max(0, MIN_ENTRIES - (int)group1->entries.size());
            int need2 = std::max(0, MIN_ENTRIES - (int)group2->entries.size());
            if (need1 == remaining) { for (int i = 0; i < N; i++) if (!assigned[i]) { group1->entries.push_back(entries[i]); assigned[i] = true; remaining--; } break; }
            if (need2 == remaining) { for (int i = 0; i < N; i++) if (!assigned[i]) { group2->entries.push_back(entries[i]); assigned[i] = true; remaining--; } break; }

            double bestDiff = -1; int bestIdx = -1; bool assignTo1 = true;
            Rectangle mbr1 = group1->nodeMBR();
            Rectangle mbr2 = group2->nodeMBR();

            for (int i = 0; i < N; i++)
            {
                if (assigned[i]) continue;
                double inc1 = Rectangle::combine(mbr1, entries[i].mbr).area() - mbr1.area();
                double inc2 = Rectangle::combine(mbr2, entries[i].mbr).area() - mbr2.area();
                double diff = fabs(inc1 - inc2);
                if (diff > bestDiff) { bestDiff = diff; bestIdx = i; assignTo1 = (inc1 < inc2) || (inc1 == inc2 && mbr1.area() < mbr2.area()); }
            }

            if (assignTo1) group1->entries.push_back(entries[bestIdx]);
            else group2->entries.push_back(entries[bestIdx]);
            assigned[bestIdx] = true;
            remaining--;
        }

        group1->parent = node->parent;
        group2->parent = node->parent;
        return group2;
    }

    void reinsert(Node* node)
    {
        // 노드 중심에서 멀리 떨어진 항목 30% 정도를 상위 트리에 재삽입
        std::vector<std::pair<double, Entry>> distList;
        Rectangle mbr = node->nodeMBR();
        double cx = (mbr.minX + mbr.maxX) / 2, cy = (mbr.minY + mbr.maxY) / 2;
        for (auto& e : node->entries)
        {
            double ex = (e.mbr.minX + e.mbr.maxX) / 2, ey = (e.mbr.minY + e.mbr.maxY) / 2;
            double d2 = (cx - ex) * (cx - ex) + (cy - ey) * (cy - ey);
            distList.push_back({ d2,e });
        }
        sort(distList.rbegin(), distList.rend()); // 멀리 있는 순
        int nReinsert = distList.size() * 0.3;
        node->entries.clear();
        for (int i = 0; i < (int)distList.size(); ++i)
        {
            if (i < nReinsert)
            {
                insert(distList[i].second.mbr, distList[i].second.id); // 재삽입
            }
            else node->entries.push_back(distList[i].second);
        }
    }

    void searchNode(Node* node, const Rectangle& range, std::vector<int>& out) const
    {
        if (!node) return;
        if (!node->nodeMBR().intersects(range)) return;
        if (node->isLeaf)
        {
            for (const Entry& e : node->entries)
                if (e.mbr.intersects(range)) out.push_back(e.id);
        }
        else
        {
            for (const Entry& e : node->entries)
                if (e.mbr.intersects(range)) searchNode(e.child, range, out);
        }
    }

    bool remove(int id, const Rectangle& r)
    {
        return deleteFromNode(root, id, r);
    }

    bool deleteFromNode(Node* node, int id, const Rectangle& r)
    {
        if (!node->nodeMBR().intersects(r)) return false;

        if (node->isLeaf)
        {
            for (auto it = node->entries.begin(); it != node->entries.end(); ++it)
            {
                if (it->id == id && it->mbr.contains(r))
                {
                    node->entries.erase(it);
                    condenseTree(node);
                    return true;
                }
            }
            return false;
        }
        else
        {
            for (auto& e : node->entries)
            {
                if (e.mbr.intersects(r) && e.child)
                {
                    if (deleteFromNode(e.child, id, r)) return true;
                }
            }
            return false;
        }
    }

    void condenseTree(Node* node)
    {
        std::vector<Node*> Q; // 재삽입할 노드 리스트

        while (node != root)
        {
            if ((int)node->entries.size() < MIN_ENTRIES)
            {
                // 노드 언더플로우: 부모에서 제거
                Node* parent = node->parent;
                auto it = find_if(parent->entries.begin(), parent->entries.end(),
                                  [&](const Entry& e) { return e.child == node; });
                if (it != parent->entries.end()) parent->entries.erase(it);

                // 재삽입할 항목 모음
                if (!node->isLeaf)
                {
                    for (auto& e : node->entries) Q.push_back(e.child);
                }
                else
                {
                    for (auto& e : node->entries) insert(e.mbr, e.id);
                }

                delete node;
                node = parent;
            }
            else
            {
                // 노드가 정상적이면 MBR 갱신
                for (auto& e : node->parent->entries)
                    if (e.child == node) e.mbr = node->nodeMBR();
                node = node->parent;
            }
        }

        // 재삽입
        for (Node* size : Q)
        {
            for (auto& e : size->entries)
                insert(e.mbr, e.id);
            delete size;
        }

        // 루트 특수 처리
        if (!root->isLeaf && root->entries.size() == 1)
        {
            Node* oldRoot = root;
            root = root->entries[0].child;
            root->parent = nullptr;
            delete oldRoot;
        }
    }
};


// --------------------- 예제 ---------------------
int main()
{
    std::ios::sync_with_stdio(false); std::cin.tie(nullptr);
    RStarTree rtree;

    rtree.insert(Rectangle(10, 10, 12, 12), 1);
    rtree.insert(Rectangle(50, 50, 50, 50), 2);
    rtree.insert(Rectangle(53, 52, 53, 52), 3);
    rtree.insert(Rectangle(90, 85, 90, 85), 4);
    rtree.insert(Rectangle(11, 11, 11, 11), 5);
    rtree.insert(Rectangle(60, 60, 62, 62), 6);

    Rectangle query(10, 10, 55, 55);
    std::vector<int> res = rtree.search(query);
    std::cout << "Search results: ";
    for (int id : res)
        std::cout << id << " ";
    std::cout << "\size";

    return 0;
}

//#include <boost/geometry.hpp>
//#include <boost/geometry/index/rtree.hpp>
//#include <iostream>
//using namespace std;
//namespace bg = boost::geometry;
//namespace bgi = boost::geometry::index;
//
//int main()
//{
//    using point = bg::model::point<double, 2, bg::cs::cartesian>;
//    using value = std::pair<point, int>;
//
//    bgi::rtree<value, bgi::quadratic<16>> rtree;
//
//    // 점 추가
//    rtree.insert({ point(10, 10), 1 });
//    rtree.insert({ point(50, 50), 2 });
//    rtree.insert({ point(51, 51), 3 });
//
//    // 가까운 점 탐색
//    point query(52, 52);
//    vector<value> result;
//    rtree.query(bgi::nearest(query, 1), back_inserter(result));
//
//    cout << "가장 가까운 점 ID: " << result[0].second << "\n";
//}