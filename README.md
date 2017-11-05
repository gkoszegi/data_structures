## Indexed heap:
A priority queue implementation that allows to change priority of elements already in the queue.

### Example use:

```cpp
/// Disjkstra's shortest path algorithm
uint32_t dijkstra(
        unsigned short nodeCount,
        const std::unordered_multimap<unsigned short, EdgeTo>& adjacencyList,
        unsigned short startNode,
        unsigned short destinationNode)
{
    indexed_heap<unsigned short, uint32_t> queue(nodeCount);
    queue.push(startNode, 0);

    const auto unreachable = std::numeric_limits<uint32_t>::max();
    std::vector<uint32_t> distance(nodeCount, unreachable);
    distance[startNode] = 0;

    while (!queue.empty() && distance[destinationNode] == unreachable)
    {
        unsigned short currentNode = queue.top();
        queue.pop();

        auto outEdges = adjacencyList.equal_range(currentNode);

        while (outEdges.first != outEdges.second)
        {
            const EdgeTo& edge = outEdges.first->second;
            auto distanceViaCurrentNode = distance[currentNode] + edge.weight;

            if (distanceViaCurrentNode < distance[edge.to])
            {
                queue.set_priority(edge.to, distanceViaCurrentNode);
                distance[edge.to] = distanceViaCurrentNode;
            }

            ++outEdges.first;
        }
    }

    return distance[destinationNode];
}

```

## Union-Find:
A data structure that keeps track of a set of elements partitioned into a number of disjoint (non-overlapping) subsets.
[See it on wikipedia](https://en.wikipedia.org/wiki/Disjoint-set_data_structure)

### Example use:

```cpp
/// Kruskal's minimum weight spanning tree algorithm
size_t kruskal(unsigned short nodeCount, const std::vector<Edge>& edgesSortedByWeight)
{
    size_t minSpaningTreeWeight = 0;

    union_find<unsigned short> unionFind(nodeCount);

    for (const Edge& edge : edgesSortedByWeight)
    {
        if (unionFind.join(edge.from, edge.to))
            minSpaningTreeWeight += edge.weight;
    }

    return minSpaningTreeWeight;
}
```
