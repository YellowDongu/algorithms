#include <vector>
#include <functional>


struct TSP
{
	std::vector<std::vector<int>> travelValue;

	std::vector<int> function(int startIndex)
	{
        int length = static_cast<int>(travelValue.size()), minCost = std::numeric_limits<int>::max();
        std::vector<int> result, path{startIndex};
		std::vector<bool> visitable(length, false);

		std::function<void(int, int, std::vector<int>&)> recursive = [&](int index, int cost, std::vector<int>& path) -> void
			{
                if (cost >= minCost)
                    return;

                int i = 0;
                for (; i < length; i++)
                    if (!visitable[i])
                        break;

                if (i == length)
                {
                    if (travelValue[index][startIndex] != 0)
                    {
                        int totalCost = cost + travelValue[index][startIndex];
                        if (totalCost < minCost)
                        {
                            minCost = totalCost;
                            result = path;
                            result.push_back(startIndex);
                        }
                    }

                    return;
                }

                for (int next = 0; next < length; ++next)
                {
                    if (!visitable[next] && travelValue[index][next] != 0)
                    {
                        visitable[next] = true;
                        path.push_back(next);
                        recursive(next, cost + travelValue[index][next], path);
                        visitable[next] = false;
                        path.pop_back();
                    }
                }
			};

        path.reserve(length + 1);
        result.reserve(length + 1);
        recursive(startIndex, 0, path);
        return result;
	}


    int function2(int startIndex)
    {
        int length = static_cast<int>(travelValue.size()), minCost = std::numeric_limits<int>::max();
        std::vector<bool> visitable(length, false);

        std::function<void(int, int)> recursive = [&](int index, int cost) -> void
            {
                if (cost >= minCost)
                    return;

                int i = 0;
                for (; i < length; i++)
                    if (!visitable[i])
                        break;

                if (i == length)
                {
                    if (travelValue[index][startIndex] != 0)
                    {
                        int totalCost = cost + travelValue[index][startIndex];
                        if (totalCost < minCost)
                            minCost = totalCost;
                    }

                    return;
                }

                for (int next = 0; next < length; ++next)
                {
                    if (!visitable[next] && travelValue[index][next] != 0)
                    {
                        visitable[next] = true;
                        recursive(next, cost + travelValue[index][next]);
                        visitable[next] = false;
                    }
                }


            };

        recursive(startIndex, 0);
        return minCost;
    }



    std::vector<int> function(int startIndex)
    {
        int length = static_cast<int>(travelValue.size()), minCost = std::numeric_limits<int>::max();
        std::vector<int> result, path{ startIndex };
        //std::vector<bool> visitable(length, false);
        int visitable = 0;
        visitable |= (1 << startIndex);

        std::function<void(int, int, std::vector<int>&)> recursive = [&](int index, int cost, std::vector<int>& path) -> void
            {
                if (cost >= minCost)
                    return;

                int i = 0;
                for (; i < length; i++)
                    if (!(visitable & (1 << i)))
                        break;

                if (i == length)
                {
                    if (travelValue[index][startIndex] != 0)
                    {
                        int totalCost = cost + travelValue[index][startIndex];
                        if (totalCost < minCost)
                        {
                            minCost = totalCost;
                            result = path;
                            result.push_back(startIndex);
                        }
                    }

                    return;
                }

                for (int next = 0; next < length; ++next)
                {
                    if (!(visitable & (1 << next)) && travelValue[index][next] != 0)
                    {
                        visitable |= (1 << next);
                        path.push_back(next);
                        recursive(next, cost + travelValue[index][next], path);
                        visitable ^= (1 << next);
                        path.pop_back();
                    }
                }
            };

        path.reserve(length + 1);
        result.reserve(length + 1);
        recursive(startIndex, 0, path);
        return result;
    }

    std::vector<int> function(int startIndex)
    {
        int length = static_cast<int>(travelValue.size()), minCost = std::numeric_limits<int>::max();
        std::vector<int> result, path{ startIndex };
        std::vector<bool> visitable(length, false);

        std::function<void(int, int, std::vector<int>&)> recursive = [&](int index, int cost, std::vector<int>& path) -> void
            {
                if (cost >= minCost)
                    return;

                int i = 0;
                for (; i < length; i++)
                    if (!visitable[i])
                        break;

                if (i == length)
                {
                    if (travelValue[index][startIndex] != 0)
                    {
                        int totalCost = cost + travelValue[index][startIndex];
                        if (totalCost < minCost)
                        {
                            minCost = totalCost;
                            result = path;
                            result.push_back(startIndex);
                        }
                    }

                    return;
                }

                for (int next = 0; next < length; ++next)
                {
                    if (!visitable[next] && travelValue[index][next] != 0)
                    {
                        visitable[next] = true;
                        path.push_back(next);
                        recursive(next, cost + travelValue[index][next], path);
                        visitable[next] = false;
                        path.pop_back();
                    }
                }
            };

        path.reserve(length + 1);
        result.reserve(length + 1);
        recursive(startIndex, 0, path);
        return result;
    }



    int function_DP(int startIndex)
    {
        int length = static_cast<int>(travelValue.size()), minCost = std::numeric_limits<int>::max();
        std::vector<std::vector<int>> visitable(length, std::vector<int>(1 << length, std::numeric_limits<int>::max()));

        visitable[startIndex][1 << startIndex] = 0;
        std::vector<std::pair<int, int>> stack{ {startIndex,1 << startIndex} };
        int lastBit = 0;

        for (int i = 0; i < length; i++)
            lastBit |= 1 << i;

        while (!stack.empty())
        {
            int bit = stack.back().second;
            int index = stack.back().first;
            stack.pop_back();

            if (bit == lastBit)
            {
                if (travelValue[index][startIndex] != 0)
                    minCost = std::min(minCost, visitable[index][bit] + travelValue[index][startIndex]);
                continue;
            }

            for (int i = 0; i < length; i++)
            {
                if (bit & (1 << i) || travelValue[index][i] == 0)
                    continue;

                int nextBit = bit | (1 << i);

                if (visitable[i][nextBit] > visitable[index][bit] + travelValue[index][i])
                {
                    visitable[i][nextBit] = visitable[index][bit] + travelValue[index][i];
                    stack.push_back({ i, nextBit });
                }
            }
        }

        return minCost;
    }

    std::vector<int> function_DP_Track(int startIndex)
    {
        int length = static_cast<int>(travelValue.size()), minCost = std::numeric_limits<int>::max(), minCostIndex = -1;
        std::vector<std::vector<int>> visitable(length, std::vector<int>(1 << length, std::numeric_limits<int>::max()));
        std::vector<int> result{startIndex};

        visitable[startIndex][1 << startIndex] = 0;
        result.reserve(length + 1);
        std::vector<std::pair<int, int>> stack{ {startIndex,1 << startIndex} };
        int lastBit = 0;

        for (int i = 0; i < length; i++)
            lastBit |= 1 << i;

        while (!stack.empty())
        {
            int bit = stack.back().second;
            int index = stack.back().first;
            stack.pop_back();

            if (bit == lastBit)
            {
                if (travelValue[index][startIndex] != 0)
                {
                    minCost = std::min(minCost, visitable[index][bit] + travelValue[index][startIndex]);
                    minCostIndex = index;
                }
                continue;
            }

            for (int i = 0; i < length; i++)
            {
                if (bit & (1 << i) || travelValue[index][i] == 0)
                    continue;

                int nextBit = bit | (1 << i);

                if (visitable[i][nextBit] > visitable[index][bit] + travelValue[index][i])
                {
                    visitable[i][nextBit] = visitable[index][bit] + travelValue[index][i];
                    stack.push_back({ i, nextBit });
                }
            }
        }

        if (minCostIndex == -1)
            return std::vector<int>();

        stack.push_back({ minCostIndex , lastBit });


        while (!stack.empty())
        {
            int bit = stack.back().second;
            int index = stack.back().first;
            stack.pop_back();

            result.push_back(index);

            int nextBit = bit ^ (1 << index);
            if (nextBit == 0)
                continue;
            int beforeValue = visitable[index][bit];

            for (int i = 0; i < length; i++)
            {
                if (!(nextBit & (1 << i)) || travelValue[i][index] == 0)
                    continue;
                
                if (visitable[i][nextBit] == beforeValue - travelValue[i][index])
                {
                    stack.push_back({ i, nextBit });
                    break;
                }
            }
        }

        std::reverse(result.begin(), result.end());
        return result;
    }
};

#include <iostream>
#pragma GCC optimize ("O3,unroll-loops")
#pragma GCC target ("avx,avx2,fma")

using namespace std;

using ll = long long;
using vl = std::vector<ll>;
using vvl = std::vector<vl>;
using pll = pair<ll, ll>;
using ld = long double;
using vd = std::vector<ld>;
using ull = unsigned long long;
using vp = std::vector<pll>;
using vvp = std::vector<vp>;
using tlll = array<ll, 3>;

#define endl '\n'
void setup()
{
#ifdef KIMHS
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#else
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
#endif
}
template <typename T> istream& operator>>(istream &is, std::vector<T> &arr)
{ for (auto &x: arr) is>>x; return is; }
template <typename T> ostream& operator<<(ostream& os, std::vector<T> arr)
{
    if (arr.size() == 0)
        return os << "()";
    os << '(';
    for (long long i = 0; i < arr.size() - 1; i++)
        os << arr[i] << ' ';
    if (arr.size())
        os << arr.back();
    return os << ')';
}

void preprocess() { ll i, j, k; }
void solve(long long testcase)
{
    long long n, ans = std::numeric_limits<long long>::max();

    cin >> n;
    std::vector<std::vector<long long>> adj(n, std::vector<long long>(n));

    cin >> adj;
    long long total = (1LL << n) - 1;
    std::vector<std::vector<long long>> dp(total + 1, std::vector<long long>(n, std::numeric_limits<long long>::max()));

    dp[1][0] = 0;

    for (long long mask = 0; mask < (1LL << n); mask++)
    {
        for (long long u = 0; u < n; u++)
        {
            if ((mask >> u) & 1)
            {
                for (long long v = 0; v < n; v++)
                {
                    if ((mask >> v) & 1)
                        continue;
                    if (u == v)
                        continue;
                    if (adj[u][v] == 0)
                        continue;

                    dp[mask | (1LL << v)][v] = std::min(dp[mask | (1LL << v)][v], dp[mask][u] + adj[u][v]);
                }
            }
        }
    }

    for (long long i = 0; i < n; i++)
    {
        cout << i << ": " << dp[total][i] << '\n';
        if (adj[i][0])
            ans = min(ans, dp[total][i] + adj[i][0]);
    }
    cout << ans << '\n';
}