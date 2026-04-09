#include <iostream>
#include <cstdio>
#include <algorithm>

#define MAX_DEPTH 4

struct graph {
    int** graf;
    int* degrees;
    int size;
};

struct degrees_plus {
    int degree;
    int index;
};

void Read_Graph(graph& g) //wczytanie jednego grafu
{
    int size;
    scanf("%d", &size);
    g.graf = new int* [size];
    g.degrees = new int[size];
    g.size = size;
    for (int i = 0; i < g.size; i++)
    {
        scanf("%d", &size);
        g.graf[i] = new int[size];
        g.degrees[i] = size;
        for (int j = 0; j < g.degrees[i]; j++)
        {
            scanf("%d", &g.graf[i][j]);
        }
    }
}

void Destroy_Graph(graph& g)
{
    for (int i = 0; i < g.size; i++)
    {
        delete[] g.graf[i];
    }
    delete[] g.graf;
    delete[] g.degrees;
}

void Merge(int* degrees, int left, int middle, int right)
{
    int n1 = middle - left + 1;
    int n2 = right - middle;

    int* LeftDegrees = new int[n1];
    int* RightDegrees = new int[n2];

    for (int i = 0; i < n1; ++i) // roździelanie na dwie tablice
    {
        LeftDegrees[i] = degrees[left + i];
    }
    for (int j = 0; j < n2; ++j)
    {
        RightDegrees[j] = degrees[middle + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) // scalanie tablic z sortowaniem
    {
        if (LeftDegrees[i] >= RightDegrees[j])
        {
            degrees[k] = LeftDegrees[i];
            i++;
        }
        else
        {
            degrees[k] = RightDegrees[j];
            j++;
        }
        k++;
    }

    while (i < n1) //scalanie reszty
    {
        degrees[k] = LeftDegrees[i];
        i++;
        k++;
    }

    while (j < n2) //scalanie reszty
    {
        degrees[k] = RightDegrees[j];
        j++;
        k++;
    }

    delete[] LeftDegrees;
    delete[] RightDegrees;
}

void MergeSort(int* degrees, int left, int right)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2;

        MergeSort(degrees, left, middle);
        MergeSort(degrees, middle + 1, right);

        Merge(degrees, left, middle, right);
    }
}


void The_Degree_Sequence(graph& g)
{
    int* degrees = new int[g.size];
    for (int i = 0; i < g.size; i++)
    {
        degrees[i] = g.degrees[i];
    }
    MergeSort(degrees, 0, g.size - 1);
    for (int j = 0; j < g.size; j++)
    {
        printf("%d ", degrees[j]);
    }
    delete[] degrees;
    printf("\n");
}

void DFS(int prev, graph& g, bool* visited)
{
    for (int i = 0; i < g.degrees[prev]; i++)
    {
        int curr = g.graf[prev][i] - 1;
        if (!visited[curr])
        {
            visited[curr] = true;
            DFS(curr, g, visited);
        }
    }
}

void The_Number_Of_Components(graph& g)
{
    bool* visited = new bool[g.size];
    for (int i = 0; i < g.size; i++)
    {
        visited[i] = false;
    }

    int components = 0;
    for (int j = 0; j < g.size; j++)    //każde puszczenie DFS -> +1 składowa spójności
    {
        if (!visited[j])
        {
            components++;
            visited[j] = true;
            DFS(j, g, visited);
        }
    }
    printf("%d\n", components);
    delete[] visited;
}

bool Bipartiteness_For_Components(int prev, graph& g, char* side) //przypisanie każdemu wierzchołkowi strony
{
    for (int i = 0; i < g.degrees[prev]; i++)
    {
        int curr = g.graf[prev][i] - 1;
        if (side[prev] == 'r' && side[curr] != 'r')
        {
            if (!side[curr])
            {
                side[curr] = 'l';
                if (!Bipartiteness_For_Components(curr, g, side))
                {
                    return false;
                }
            }
        }
        else if (side[prev] == 'l' && side[curr] != 'l')
        {
            if (!side[curr])
            {
                side[curr] = 'r';
                if (!Bipartiteness_For_Components(curr, g, side))
                {
                    return false;
                }
            }
        }
        else //jeśli sąsiednie wierzchołki mają tą samą stronę ,zakoncz
        {
            return false;
        }
    }
    return true;
}

void Bipartiteness(graph& g)
{
    bool* visited = new bool[g.size];
    char* side = new char[g.size];
    for (int i = 0; i < g.size; i++)
    {
        visited[i] = false;
        side[i] = false;
    }

    for (int j = 0; j < g.size; j++)
    {
        if (!visited[j])
        {
            side[j] = 'l';
            if (Bipartiteness_For_Components(j, g, side)) //po rozdzieleniu składowej spójności ,wyklucznie jej poprzez DFS
            {
                visited[j] = true;
                DFS(j, g, visited);
            }
            else
            {
                printf("F\n");
                delete[] visited;
                delete[] side;
                return;
            }
        }
    }
    printf("T\n");
    delete[] visited;
    delete[] side;
}

void BFS(graph& g, int start, int* distance, int size) //szukanie dłgości dla danego wierzchołka
{
    int* queue = new int[size];
    int front = 0;
    int back = 0;

    for (int i = 0; i < g.size; i++)
    {
        distance[i] = -1; //-1 -> brak ustalonej długości
    }

    distance[start] = 0;
    queue[back++] = start;

    while (true)
    {
        int vertice = queue[front++];
        for (int j = 0; j < g.degrees[vertice]; j++)
        {
            int curr = g.graf[vertice][j] - 1;
            if (distance[curr] == -1)
            {
                distance[curr] = distance[vertice] + 1;
                queue[back++] = curr;
            }
            if (back == size)
            {
                printf("%d ", distance[curr]);
                delete[] queue;
                return;
            }
        }
    }
}

void Eccentricity_DFS_Plus(int prev, graph& g, bool* visited, int& size) //szukanie ilości wierzchołków w składowej spójności
{
    visited[prev] = true;
    size++;
    for (int i = 0; i < g.degrees[prev]; i++)
    {
        int curr = g.graf[prev][i] - 1;
        if (!visited[curr])
        {
            Eccentricity_DFS_Plus(curr, g, visited, size);
        }
    }
}

void Eccentricity_DFS_Minus(int prev, graph& g, bool* visited, int& size, int* vertices) //przypisanie znalezionej ilości dla każdego wierzchołka w składowej spójności 
{
    visited[prev] = false;
    vertices[prev] = size;
    for (int i = 0; i < g.degrees[prev]; i++)
    {
        int curr = g.graf[prev][i] - 1;
        if (visited[curr])
        {
            Eccentricity_DFS_Minus(curr, g, visited, size, vertices);
        }
    }
}

void The_Eccentricity_Of_Vertices(graph& g)
{
    int* distance = new int[g.size];
    bool* visited = new bool[g.size];
    int* vertices = new int[g.size];
    for (int i = 0; i < g.size; i++)
    {
        visited[i] = false;
        vertices[i] = 0;
    }

    for (int j = 0; j < g.size; j++)
    {
        if (g.degrees[j] == 0)
        {
            printf("0 ");
        }
        else
        {
            if (!vertices[j])
            {
                int size = 0;
                Eccentricity_DFS_Plus(j, g, visited, size);
                Eccentricity_DFS_Minus(j, g, visited, size, vertices);
            }
            BFS(g, j, distance, vertices[j]);
        }
    }
    printf("\n");
    delete[] distance;
    delete[] visited;
    delete[] vertices;
}

void Greedy(graph& g)
{
    bool* available = new bool[g.size];
    int* color = new int[g.size];
    for (int i = 0; i < g.size; i++)
    {
        available[i] = true;
        color[i] = -1; //-1 -> brak ustawonego koloru
    }

    color[0] = 0;

    for (int j = 1; j < g.size; j++) //kolorowanie wierzchołków
    {
        for (int n = 0; n < g.degrees[j]; n++)
        {
            int curr = g.graf[j][n] - 1;
            if (color[curr] != -1) //sprawdzanie niedostępnych kolorów
            {
                available[color[curr]] = false;
            }
        }
        int c;
        for (c = 0; c < g.size; c++) //wybieranie najmniejszego dostępnego koloru
        {
            if (available[c])
            {
                break;
            }
        }
        color[j] = c;
        for (int n = 0; n < g.degrees[j]; n++)
        {
            int curr = g.graf[j][n] - 1;
            if (color[curr] != -1) //resetowanie niedostępnych kolorów
            {
                available[color[curr]] = true;
            }
        }
    }

    for (int i = 0; i < g.size; i++)
    {
        printf("%d ", color[i] + 1);
    }
    printf("\n");
    delete[] available;
    delete[] color;
}

void LF_Method_Merge(degrees_plus* d, int left, int middle, int right) //sortowanie stopni z zachowaniem ich pierwotnego indexu
{
    int n1 = middle - left + 1;
    int n2 = right - middle;

    degrees_plus* LeftDegrees = new degrees_plus[n1];
    degrees_plus* RightDegrees = new degrees_plus[n2];

    for (int i = 0; i < n1; i++)
    {
        LeftDegrees[i] = d[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        RightDegrees[j] = d[middle + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if ((LeftDegrees[i].degree > RightDegrees[j].degree) || (LeftDegrees[i].degree == RightDegrees[j].degree && LeftDegrees[i].index < RightDegrees[j].index))
        {
            d[k] = LeftDegrees[i];
            i++;
        }
        else
        {
            d[k] = RightDegrees[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        d[k] = LeftDegrees[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        d[k] = RightDegrees[j];
        j++;
        k++;
    }

    delete[] LeftDegrees;
    delete[] RightDegrees;
}

void LF_Method_MergeSort(degrees_plus* d, int left, int right)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2;

        LF_Method_MergeSort(d, left, middle);
        LF_Method_MergeSort(d, middle + 1, right);

        LF_Method_Merge(d, left, middle, right);
    }
}

void LF_Method(graph& g)
{
    degrees_plus* d = new degrees_plus[g.size];
    for (int i = 0; i < g.size; i++)
    {
        d[i].degree = g.degrees[i];
        d[i].index = i;
    }
    LF_Method_MergeSort(d, 0, g.size - 1);

    bool* available = new bool[g.size];
    int* color = new int[g.size];
    for (int i = 0; i < g.size; i++)
    {
        available[i] = true;
        color[i] = -1; //-1 -> brak ustawonego koloru
    }

    color[d[0].index] = 0;
    for (int j = 1; j < g.size; j++) //kolorowanie wierzchołków
    {
        int index = d[j].index;
        for (int n = 0; n < g.degrees[index]; n++)
        {
            int w = g.graf[index][n] - 1;
            if (color[w] != -1) //sprawdzanie niedostępnych kolorów
            {
                available[color[w]] = false;
            }
        }
        int c;
        for (c = 0; c < g.size; c++) //wybieranie najmniejszego dostępnego koloru
        {
            if (available[c])
            {
                break;
            }
        }
        color[index] = c;
        for (int n = 0; n < g.degrees[index]; n++)
        {
            int w = g.graf[index][n] - 1;
            if (color[w] != -1) //resetowanie niedostępnych kolorów
            {
                available[color[w]] = true;
            }
        }
    }

    for (int i = 0; i < g.size; i++)
    {
        printf("%d ", color[i] + 1);
    }
    printf("\n");
    delete[] available;
    delete[] color;
    delete[] d;
}

void C4_DFS(int start, int w, int w2, graph& g, int depth, int* c_four)
{
    for (int i = 0; i < g.degrees[w2]; i++)
    {
        int w3 = g.graf[w2][i] - 1;
        if (depth == MAX_DEPTH)
        {
            if (w3 == start)
            {
                (*c_four)++;
            }
        }
        else
        {
            if (w3 != w)
            {
                C4_DFS(start, w2, w3, g, depth + 1, c_four);
            }
        }
    }
}

void The_Number_Of_Different_C4_Subgraphs(graph& g)
{
    int count = 0;

    for (int i = 0; i < g.size; ++i) {
        // Iteracja po sąsiadach i-tego wierzchołka
        for (int j = 0; j < g.degrees[i]; ++j) {
            int neighbor1 = g.graf[i][j] - 1;
            // Sprawdzenie wszystkich par sąsiadów i
            for (int k = j + 1; k < g.degrees[i]; ++k) {
                int neighbor2 = g.graf[i][k] - 1;
                int common_neighbors = 0;
                for (int l = 0, m = 0; l < g.degrees[neighbor1] && m < g.degrees[neighbor2];) {
                    int n1 = g.graf[neighbor1][l] - 1;
                    int n2 = g.graf[neighbor2][m] - 1;

                    if (n1 < n2) {
                        ++l;
                    }
                    else if (n1 > n2) {
                        ++m;
                    }
                    else {
                        // Znaleziono wspólnego sąsiada
                        ++common_neighbors;
                        ++l;
                        ++m;
                    }
                }

                // Jeśli są co najmniej 2 wspólne sąsiady, to znaleziono cykl C4
                if (common_neighbors >= 2) {
                    count += common_neighbors * (common_neighbors - 1) / 2;
                }
            }
        }
    }

    // Każdy cykl C4 został policzony cztery razy, więc dzielimy przez 4

    printf("%d\n", count);
}

void The_Number_Of_The_Graph_Complements_Edges(graph& g)
{
    long long size = g.size;
    long long max_edges = (size * (size - 1)) / 2;
    long long edges = 0;
    for (int i = 0; i < size; i++)
    {
        edges += g.degrees[i];
    }
    long long result = max_edges - edges / 2;
    printf("%lld\n", result);
}

void Do_Graph()
{
    graph g;
    Read_Graph(g);

    The_Degree_Sequence(g);
    The_Number_Of_Components(g);
    Bipartiteness(g);
    The_Eccentricity_Of_Vertices(g);
    printf("?\n");
    Greedy(g);
    LF_Method(g);
    printf("?\n");
    The_Number_Of_Different_C4_Subgraphs(g);
    The_Number_Of_The_Graph_Complements_Edges(g);

    Destroy_Graph(g);
}

int main()
{
    int number_of_graphs;
    scanf("%d ", &number_of_graphs);
    for (int i = 0; i < number_of_graphs; i++)
    {
        Do_Graph();
    }

    return 0;
}
