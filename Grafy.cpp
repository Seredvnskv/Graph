#include <iostream>
#include "Data_Structs.cpp"

using namespace std;

void heapify(int* array, int i, int size) {
    int L = 2 * i; // indeks lewgo dziecka 
    int R = 2 * i + 1; // indeks prawego dziecka 
    int maxps; // indeks najwiekszego elementu

    // czy L jest wieksze od rodzica
    if (L <= size && array[L - 1] > array[i - 1]) {
        maxps = L;
    }
    else {
        maxps = i;
    }

    // czy R jest wieksze od aktualnie najwiekszego elementu
    if (R <= size && array[R - 1] > array[maxps - 1]) {
        maxps = R;
    }

    // gdy najwiekszy element nie jest rodzicem zamien je miejscami 
    if (maxps != i) {
        int swap = array[i - 1];
        array[i - 1] = array[maxps - 1];
        array[maxps - 1] = swap;
        heapify(array, maxps, size);
    }
}

void build_heap(int* array, int size) {
    for (int i = size / 2; i > 0; i--) {
        heapify(array, i, size);
    }
}

void heap_sort(int* array, int size) {
    build_heap(array, size);

    for (int i = size; i > 1; i--) {
        int swap = array[i - 1];
        array[i - 1] = array[0];
        array[0] = swap;
        heapify(array, 1, i - 1);
    }
}

class Graph {
private:
    int number_of_vertices;
    int** adjacency_list; // lista sasiedztwa 
    int* size_of_vertex; // liczba sasiadow wierzcholka 
public: 
    Graph(int vertices) : number_of_vertices(vertices) {
        adjacency_list = new int* [number_of_vertices];
        size_of_vertex = new int[number_of_vertices];
        for (int i = 0; i < number_of_vertices; i++) {
            size_of_vertex[i] = 0; // inicjalizacja liczby sasiadow 
            adjacency_list[i] = nullptr; // inicjalizacja wskaznika na liste
        }
    }

    ~Graph() {
        for (int i = 0; i < number_of_vertices; i++) {
            delete[] adjacency_list[i];
        }
        delete[] adjacency_list;
        delete[] size_of_vertex;
    }

    struct vertexColor {
        int vertex; 
        int color;
    };

    // dodawanie sasiadow dla wierzcholka 
    void add_neighbours(int vertex, int* neighbours, int number_of_neighbours) {
        size_of_vertex[vertex] = number_of_neighbours;
        adjacency_list[vertex] = new int[number_of_neighbours];
        for (int i = 0; i < number_of_neighbours; ++i) {
            adjacency_list[vertex][i] = neighbours[i];
        }
    }

    void deggree_sequance() {
        int* deggree_sequence = new int[number_of_vertices];

        for (int i = 0; i < number_of_vertices; i++) {
            deggree_sequence[i] = size_of_vertex[i]; // przypisanie liczby wierzcholkow 
        }

        heap_sort(deggree_sequence, number_of_vertices);

        for (int i = 0; i < number_of_vertices / 2; i++) {
            int swap = deggree_sequence[i];
            deggree_sequence[i] = deggree_sequence[number_of_vertices - 1 - i];
            deggree_sequence[number_of_vertices - 1 - i] = swap;
        }

        for (int i = 0; i < number_of_vertices; i++) {
            cout << deggree_sequence[i] << " ";
        }

        cout << endl;

        delete[] deggree_sequence;
    }

    int number_of_components() {
        bool* visited = new bool[number_of_vertices]; // tablica odwiedzonych wierzcholkow 

        for (int i = 0; i < number_of_vertices; i++) {
            visited[i] = false; // inicjalizacja wszystkich jako nieodwiedzone 
        }

        int count = 0;
        for (int i = 0; i < number_of_vertices; i++) {
            if (!visited[i]) {
                Stack<int> stack;
                stack.push(i);

                while (!stack.isEmpty()) {
                    int vertex = stack.pop(); // pobranie wierzcholka ze stosu 

                    if (!visited[vertex]) {
                        visited[vertex] = true; // wierzcholek odwiedzony 

                        for (int j = 0; j < size_of_vertex[vertex]; j++) {
                            int neighbour = adjacency_list[vertex][j] - 1;
                            // dodanie sasiadow do stosu 
                            if (!visited[neighbour]) {
                                stack.push(neighbour);
                            }
                        }
                    }
                }
                count++;
            }
        }

        delete[] visited;
        return count;
    }

    bool is_bipartite() {
        int* vertex_colors = new int[number_of_vertices]; // tablica kolorow

        for (int i = 0; i < number_of_vertices; i++) {
            vertex_colors[i] = -1; // inicjalizacja -1 jako niepokolorowane 
        }

        Queue<vertexColor> queue; 

        for (int i = 0; i < number_of_vertices; i++) {
            if (vertex_colors[i] == -1) {
                queue.enqueue({ i, 0 }); // dodanie wierzcholka i jego koloru do kolejki 
                vertex_colors[i] = 0; // kolrowanie wierzcholka

                while (!queue.isEmpty()) {
                    vertexColor vertexColor = queue.dequeue(); 

                    int vertex = vertexColor.vertex;
                    int color = vertexColor.color;

                    for (int j = 0; j < size_of_vertex[vertex]; j++) {
                        int neighbour = adjacency_list[vertex][j] - 1;

                        if (vertex_colors[neighbour] == color) {
                            delete[] vertex_colors;
                            return false; // gdy sasiad ma ten sam kolor graf nie jest dwudzielny
                        }

                        if (vertex_colors[neighbour] == -1) {
                            if (color) { // kolorowanie na kolor przeciwny 
                                vertex_colors[neighbour] = 0;
                            }
                            else {
                                vertex_colors[neighbour] = 1; 
                            }
                            queue.enqueue({ neighbour, vertex_colors[neighbour] }); // dodanie sasiada do kolejki z nowym kolorem
                        }
                    }
                }
            }
        }

        delete[] vertex_colors;
        return true;
    }

    int count_edges_of_graph() {
        int edges = 0;
        for (int i = 0; i < number_of_vertices; i++) {
            edges += size_of_vertex[i]; 
        }
        int number_of_graph_edges = edges / 2; // kazda krawedz jest liczona dwukrotnie
        return number_of_graph_edges;
    }

    long long int graph_complements_edges() {
        long long int n = number_of_vertices; 
        long long int graph_edges = count_edges_of_graph(); 
        long long int max_edges = (n * (n - 1)) / 2; // maksymalna ilosc krawedzi w pelnym grafie 
        long long int complements_edges = max_edges - graph_edges; // liczba krawedzi dopelnienia 
        return complements_edges; 
    }

    void greedy_coloring() {
        int* vertex_colors = new int[number_of_vertices]; // tablica kolorow wierzcholkow 
        bool* used = new bool[number_of_vertices]; // dostepne kolory 

        for (int i = 0; i < number_of_vertices; i++) { // inicjalizacja
            vertex_colors[i] = -1;
            used[i] = false;
        }

        vertex_colors[0] = 1; // kolorowanie pierwszego wierzcholka 

        for (int i = 1; i < number_of_vertices; i++) {
            // sprawdzanie kolorow sasiadow
            for (int j = 0; j < size_of_vertex[i]; j++) {
                int neighbour = adjacency_list[i][j] - 1;

                if (vertex_colors[neighbour] != -1) {
                    used[vertex_colors[neighbour]] = true; // oznaczenie koloru sasiada jako uzytego
                }
            }

            int color;

            for (color = 1; color < number_of_vertices; color++) {
                if (used[color] == false) { 
                    break; // szukanie pierwszego dostepnego koloru
                }
            }

            vertex_colors[i] = color; // kolorowanie wierzcholka 

            for (int j = 0; j < size_of_vertex[i]; j++) {
                int neighbour = adjacency_list[i][j] - 1;

                if (vertex_colors[neighbour] != -1) {
                    used[vertex_colors[neighbour]] = false; // resetowanie uzywanego koloru
                }
            }
        }

        for (int i = 0; i < number_of_vertices; i++) {
            cout << vertex_colors[i] << " "; 
        }

        delete[] vertex_colors;
        delete[] used;
    }
};

int main() {
    int number_of_graphs;
    cin >> number_of_graphs;

    for (int i = 0; i < number_of_graphs; i++) {
        int number_of_vertices;
        cin >> number_of_vertices;

        Graph graph(number_of_vertices);

        for (int v = 0; v < number_of_vertices; v++) {
            int size;
            cin >> size;
            int* neighbours = new int[size];
            for (int n = 0; n < size; n++) {
                cin >> neighbours[n];
            }
            graph.add_neighbours(v, neighbours, size);
            delete[] neighbours;
        }

        graph.deggree_sequance();
        cout << graph.number_of_components() << endl; 
        if (graph.is_bipartite()) {
            cout << "T" << endl;
        }
        else {
            cout << "F" << endl;
        }
        cout << "?" << endl;
        cout << "?" << endl;
        graph.greedy_coloring();
        cout << endl;
        cout << "?" << endl;
        cout << "?" << endl;
        cout << "?" << endl;
        cout << graph.graph_complements_edges() << endl;
    }

    return 0;
}
