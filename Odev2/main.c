#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF INT_MAX
//20010011066 BURCU GÜL
typedef struct NeighborNode {
    char destination;
    int weight;
    struct NeighborNode *next;
} NeighborNode;

typedef struct Graph {
    int num_vertices;
    NeighborNode **adj_list;
} Graph;

NeighborNode *createNeighborNode(char destination, int weight) {
    NeighborNode *new_node = (NeighborNode *)malloc(sizeof(NeighborNode));
    new_node->destination = destination;
    new_node->weight = weight;
    new_node->next = NULL;
    return new_node;
}

Graph *createGraph(int num_vertices) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->adj_list = (NeighborNode **)malloc(num_vertices * sizeof(NeighborNode *));
    for (int i = 0; i < num_vertices; ++i) {
        graph->adj_list[i] = NULL;
    }
    return graph;
}

void addEdge(Graph *graph, char source, char destination, int weight) {
    NeighborNode *new_node = createNeighborNode(destination, weight);
    new_node->next = graph->adj_list[source - 'a'];
    graph->adj_list[source - 'a'] = new_node;
}

Graph *readGraphFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya acilamadi.\n");
        exit(EXIT_FAILURE);
    }

    int num_vertices, num_edges;
    fscanf(file, "%d", &num_vertices);
    num_edges = num_vertices;
    Graph *graph = createGraph(num_vertices);

    for (int i = 0; i < num_edges; ++i) {
        char source, destination;
        int weight;
        fscanf(file, " %c %c %d", &source, &destination, &weight);
        addEdge(graph, source, destination, weight);
    }

    fclose(file);
    return graph;
}

void printGraph(Graph *graph) {
    printf("Kenarlar:\n");
    for (int i = 0; i < graph->num_vertices; ++i) {
        NeighborNode *current = graph->adj_list[i];
        while (current != NULL) {
            printf("(%c -> %c) agirlik: %d\n", i + 'a', current->destination, current->weight);
            current = current->next;
        }
    }
}
void bellmanFord(Graph *graph, char source) {
    int num_vertices = graph->num_vertices;
    int distances[num_vertices];
    int predecessors[num_vertices];

    for (int i = 0; i < num_vertices; ++i) {
        distances[i] = INF;
        predecessors[i] = -1;
    }
    distances[source - 'a'] = 0;

    printf("Adim 0:\n");
    printf("Dugum   Uzaklik   Oncu\n");
    for (int i = 0; i < num_vertices; ++i) {
        printf("%c \t\t %d \t\t %c\n", i + 'a', distances[i], (predecessors[i] == -1) ? '-' : (predecessors[i] + 'a'));
    }

    for (int i = 0; i < num_vertices - 1; ++i) {
        for (int u = 0; u < num_vertices; ++u) {
            NeighborNode *neighbor = graph->adj_list[u];
            while (neighbor != NULL) {
                int v = neighbor->destination - 'a';
                int weight = neighbor->weight;
                if (distances[u] != INF && distances[u] + weight < distances[v]) {
                    distances[v] = distances[u] + weight;
                    predecessors[v] = u;
                    printf("\nAdim %d (Bellman-Ford):\n", i + 1);
                    printf("Dugum   Uzaklik   Oncu\n");
                    for (int j = 0; j < num_vertices; ++j) {
                        printf("%c \t\t %d \t\t %c\n", j + 'a', distances[j], (predecessors[j] == -1) ? '-' : (predecessors[j] + 'a'));
                    }
                }
                neighbor = neighbor->next;
            }
        }
    }

    printf("\nBellman-Ford Algoritmasi Cikislari:\n");
    printf("Dugum   Uzaklik   Oncu\n");
    for (int i = 0; i < num_vertices; ++i) {
        printf("%c \t\t %d \t\t %c\n", i + 'a', distances[i], (predecessors[i] == -1) ? '-' : (predecessors[i] + 'a'));
    }
}

void dijkstra(Graph *graph, char source) {
    int num_vertices = graph->num_vertices;
    int distances[num_vertices];
    int visited[num_vertices];

    for (int i = 0; i < num_vertices; ++i) {
        distances[i] = INF;
        visited[i] = 0;
    }
    distances[source - 'a'] = 0;

    printf("Adim 0:\n");
    printf("Dugum   Uzaklik\n");
    for (int i = 0; i < num_vertices; ++i) {
        printf("%c \t\t %d\n", i + 'a', distances[i]);
    }

    for (int i = 0; i < num_vertices - 1; ++i) {
        int min_distance = INF;
        int min_index = -1;

        for (int j = 0; j < num_vertices; ++j) {
            if (visited[j] == 0 && distances[j] <= min_distance) {
                min_distance = distances[j];
                min_index = j;
            }
        }
        visited[min_index] = 1;

        NeighborNode *neighbor = graph->adj_list[min_index];
        while (neighbor != NULL) {
            int v = neighbor->destination - 'a';
            int weight = neighbor->weight;
            if (visited[v] == 0 && distances[min_index] != INF && distances[min_index] + weight < distances[v]) {
                distances[v] = distances[min_index] + weight;
            }
            neighbor = neighbor->next;
        }

        printf("\nAdim %d (Dijkstra):\n", i + 1);
        printf("Dugum   Uzaklik\n");
        for (int i = 0; i < num_vertices; ++i) {
            printf("%c \t\t %d\n", i + 'a', distances[i]);
        }
    }

    printf("\nDijkstra Algoritmasi Cikislari:\n");
    printf("Dugum   Uzaklik\n");
    for (int i = 0; i < num_vertices; ++i) {
        printf("%c \t\t %d\n", i + 'a', distances[i]);
    }
}

int main() {
    Graph *graph = readGraphFromFile("graf.txt");
    printf("\nDosyaya Girilen Graf\n");
    printf("Dugum sayisi: %d\n", graph->num_vertices);

    char source;
    printf("\nBaslangic dugumunu girin(a,b,c,d,e,f): ");
    scanf(" %c", &source);

    bellmanFord(graph, source);
    dijkstra(graph, source);

    return 0;
}
