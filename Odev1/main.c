#include <stdio.h>
#include <stdlib.h>

struct Edge {
    int neighbor;
    int weight;
    struct Edge* next;
};

struct Graph {
    int numVertices;
    struct Edge** adjacencyList;
    int* visited;
};

struct Edge* createEdge(int neighbor, int weight) {
    struct Edge* newEdge = (struct Edge*)malloc(sizeof(struct Edge));
    newEdge->neighbor = neighbor;
    newEdge->weight = weight;
    newEdge->next = NULL;
    return newEdge;
}

struct Graph* createGraph(int numVertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = numVertices;

    graph->adjacencyList = (struct Edge**)malloc(numVertices * sizeof(struct Edge*));
    for (int i = 0; i < numVertices; ++i) {
        graph->adjacencyList[i] = NULL;
    }

    graph->visited = (int*)malloc(numVertices * sizeof(int));
    for (int i = 0; i < numVertices; ++i) {
        graph->visited[i] = 0;
    }

    return graph;
}

void printNeighbors(struct Graph* graph, int source) {
    struct Edge* current = graph->adjacencyList[source];
    while (current != NULL) {
        printf("Komþu: %d, Aðýrlýk: %d\n", current->neighbor, current->weight);
        current = current->next;
    }
}

void BFS(struct Graph* graph, int source) {
    int* queue = (int*)malloc(graph->numVertices * sizeof(int));
    int front = 0, rear = 0;

    queue[rear++] = source;
    graph->visited[source] = 1;

    printf("BFS sonucu:\n");

    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);

        struct Edge* temp = graph->adjacencyList[current];
        while (temp != NULL) {
            int neighbor = temp->neighbor;
            if (!graph->visited[neighbor]) {
                queue[rear++] = neighbor;
                graph->visited[neighbor] = 1;
            }
            temp = temp->next;
        }
    }

    printf("\n");

    free(queue);
}

void DFSUtil(struct Graph* graph, int source) {
    printf("%d ", source);
    graph->visited[source] = 1;

    struct Edge* temp = graph->adjacencyList[source];
    while (temp != NULL) {
        int neighbor = temp->neighbor;
        if (!graph->visited[neighbor]) {
            DFSUtil(graph, neighbor);
        }
        temp = temp->next;
    }
}

void DFS(struct Graph* graph, int source) {
    printf("DFS sonucu:\n");
    DFSUtil(graph, source);
    printf("\n");
}

int main() {
    FILE* file = fopen("graf.txt", "r");
    if (file == NULL) {
        printf("Dosya açýlamadý!\n");
        return 1;
    }

    int numVertices;
    fscanf(file, "%d", &numVertices);

    struct Graph* graph = createGraph(numVertices);

    int source;
    printf("Baslangic nodunu girin (0 - %d): ",numVertices-1);
    scanf("%d", &source);

    int node, neighbor, weight;
    while (fscanf(file, "%d %d %d", &node, &neighbor, &weight) != EOF) {
        struct Edge* newEdge = createEdge(neighbor, weight);
        newEdge->next = graph->adjacencyList[node];
        graph->adjacencyList[node] = newEdge;
    }

    fclose(file);

    BFS(graph, source);
    DFS(graph, source);

    for (int i = 0; i < numVertices; ++i) {
        struct Edge* current = graph->adjacencyList[i];
        while (current != NULL) {
            struct Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(graph->adjacencyList);
    free(graph->visited);
    free(graph);

    return 0;
}
