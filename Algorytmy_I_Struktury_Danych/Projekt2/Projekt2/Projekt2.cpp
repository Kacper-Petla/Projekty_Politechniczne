#include <iostream>
#include <cstdio>

#define MAX_C 60
#define NEIGHBORS 7

enum Winner
{
    NONE, BLUE, RED
};

struct board
{
    char** fields;
    int size;
    int blue;
    int red;
};

struct copy
{
    char** tab;
    int size;
    bool end;
};

struct stos
{
    char field;
    stos* next;
};

void Push(stos*& head, char field)
{
    stos* nstos = new stos;
    nstos->field = field;
    nstos->next = head;
    head = nstos;
}

char Pop(stos*& head)
{
    if (head == nullptr)
    {
        return  NULL;
    }
    stos* tmp = head;
    char field = head->field;
    head = head->next;
    delete tmp;
    return field;
}

void PawnsType(stos*& head, board& b, char c) //zliczanie ilosci danego pionka
{
    if (c == 'r')
    {
        b.red++;
    }
    else if (c == 'b')
    {
        b.blue++;
    }
    Push(head, c);
}

bool IsCommand(char c) //litera rozpoczynajaca jakas komende
{
    return (c == 'B' || c == 'P' || c == 'I' || c == 'C');
}

void Input(stos*& head, board& b) //przedwarzanie inputu (plansza)
{
    int size = 0;
    while (true)
    {
        char c = getchar();
        if (IsCommand(c))
        {
            ungetc(c, stdin);
            break;
        }
        else if (c == '<') //pole
        {
            size++;
            getchar(); // ' '
            PawnsType(head, b, getchar()); //pionek
            getchar(); // ' '
            getchar(); // >
            getchar(); // -
        }
        else
        {
            if (size != 0)
            {
                if (size > b.size)
                {
                    b.size = size;
                }
                size = 0;
            }
        }
    }
}

void CreateBoard(stos*& head, board& b) //tworzenie reprezentacji planszy w tablicy 2d
{
    b.fields = new char* [b.size];
    for (int i = 0; i < b.size; i++)
    {
        b.fields[i] = new char[b.size];
    }

    for (int i = b.size - 1; i > -b.size; i--) //zapisywanie danych ze stosu do tablicy po skosie
    {
        if (i >= 0)
        {
            b.fields[i][b.size - 1] = Pop(head);
        }
        for (int j = 0; j < b.size - 1; j++)
        {
            int x = b.size - 2 + i - j;
            if (x >= 0 && x < b.size)
            {
                b.fields[x][j] = Pop(head);
            }
        }
    }
}

void DeleteTAB(char**& tab, int size)
{
    for (int i = 0; i < size; i++)
    {
        delete[] tab[i];
    }
    delete[] tab;
}

void CoppyTAB(char**& tab1, char**& tab2, int size)
{
    tab2 = new char* [size];
    for (int i = 0; i < size; i++)
    {
        tab2[i] = new char[size];
    }
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            tab2[i][j] = tab1[i][j];
        }
    }
}

bool IS_BOARD_CORRECT(board& b)
{
    return((b.red >= b.blue) && (b.red - b.blue <= 1)); //równe dla tury gracza czerwonego i wieksze o 1 dla tury gracza niebieskiego
}

void Neighbors_R(copy& c, int x, int y, int i, int j)
{
    for (int k = 0; k < NEIGHBORS; k++) //wyszukiwanie sasiada zaczynajac od pola najblizej pola poczatkowego
    {
        if (j == 1 && i > -1) {
            i--;
        }
        else if (i == 1 && j < 1) {
            j++;
        }
        else if (j == -1 && i < 1) {
            i++;
        }
        else if (i == -1 && j > -1) {
            j--;
        }

        if (c.end)
        {
            return;
        }
        if (i != -j)
        {
            if (x + i >= 0 && x + i < c.size && y + j >= 0 && y + j < c.size && c.tab[x + i][y + j] == 'r') // czy pole istnieje i jest r
            {
                c.tab[x + i][y + j] = ' ';
                if (x + i == c.size - 1)
                {
                    c.end = true;
                    return;
                }
                Neighbors_R(c, x + i, y + j, -i, -j);
            }
        }
    }
}

void Neighbors_B(copy& c, int x, int y, int i, int j)
{
    for (int k = 0; k < NEIGHBORS; k++) //wyszukiwanie sasiada zaczynajac od pola najblizej pola poczatkowego
    {
        if (j == 1 && i < 1) {
            i++;
        }
        else if (i == 1 && j > -1) {
            j--;
        }
        else if (j == -1 && i > -1) {
            i--;
        }
        else if (i == -1 && j < 1) {
            j++;
        }

        if (c.end)
        {
            return;
        }
        if (i != -j)
        {
            if (x + i >= 0 && x + i < c.size && y + j >= 0 && y + j < c.size && c.tab[x + i][y + j] == 'b') // czy pole istnieje i jest b
            {
                c.tab[x + i][y + j] = ' ';
                if (y + j == c.size - 1)
                {
                    c.end = true;
                    return;
                }
                Neighbors_B(c, x + i, y + j, -i, -j);
            }
        }
    }
}

bool DFS_R(board& b)
{
    copy c;
    c.end = false;
    c.size = b.size;
    CoppyTAB(b.fields, c.tab, c.size);

    for (int i = 0; i < c.size; i++) //pola poczatkowe czerwone
    {
        if (c.tab[0][i] == 'r')
        {
            c.tab[0][i] = ' ';
            Neighbors_R(c, 0, i, -1, 0);
            if (c.end || c.size == 1)
            {
                DeleteTAB(c.tab, c.size);
                return true;
            }
        }
    }

    DeleteTAB(c.tab, c.size);
    return false;
}

bool DFS_B(board& b) 
{
    copy c;
    c.end = false;
    c.size = b.size;
    CoppyTAB(b.fields, c.tab, c.size);

    for (int i = 0; i < c.size; i++) //pola poczatkowe niebieskie
    {
        if (c.tab[i][0] == 'b')
        {
            c.tab[i][0] = ' ';
            Neighbors_B(c, i, 0, -1, 0);
            if (c.end || c.size == 1)
            {
                DeleteTAB(c.tab, c.size);
                return true;
            }
        }
    }
    DeleteTAB(c.tab, c.size);
    return false;
}

Winner IS_GAME_OVER(board& b)
{
    if (IS_BOARD_CORRECT(b))
    {
        if (b.red >= b.size) //czy wystarczajaca ilosc pionkow do wygranej (eliminacja prawie pustych plansz)
        {
            if (DFS_R(b))
            {
                return RED;
            }
            if (DFS_B(b))
            {
                return BLUE;
            }
        }
    }
    return NONE;
}

void IS_BOARD_POSSIBLE(board& b)
{
    if (IS_BOARD_CORRECT(b))
    {
        if (b.red >= b.size) //czy wystarczajaca ilosc pionkow do wygranej (eliminacja prawie pustych plansz)
        {
            copy c;
            bool finish;
            c.size = b.size;
            CoppyTAB(b.fields, c.tab, c.size);

            c.end = false;
            finish = false;
            for (int i = 0; i < c.size; i++)
            {
                if (c.tab[0][i] == 'r')
                {
                    c.tab[0][i] = ' ';
                    Neighbors_R(c, 0, i, -1, 0);
                    if (c.end)
                    {
                        if (b.red == b.blue) //wykonano ruch po wygranej
                        {
                            printf("NO \n");
                            DeleteTAB(c.tab, c.size);
                            return;
                        }
                        else if (finish) //wiecej niz jedna droga do wygranej
                        {
                            printf("NO \n");
                            DeleteTAB(c.tab, c.size);
                            return;
                        }
                        c.end = false;
                        finish = true;
                    }
                }
            }
            c.end = false;
            finish = false;
            for (int i = 0; i < c.size; i++)
            {
                if (c.tab[i][0] == 'b')
                {
                    c.tab[i][0] = ' ';
                    Neighbors_B(c, i, 0, -1, 0);
                    if (c.end)
                    {
                        if (b.red > b.blue) //wykonano ruch po wygranej
                        {
                            printf("NO \n");
                            DeleteTAB(c.tab, c.size);
                            return;
                        }
                        else if (finish) //wiecej niz jedna droga do wygranej
                        {
                            printf("NO \n");
                            DeleteTAB(c.tab, c.size);
                            return;
                        }
                        c.end = false;
                        finish = true;
                    }
                }
            }
            DeleteTAB(c.tab, c.size);
        }
        printf("YES \n");
        return;
    }
    printf("NO\n");
}

bool Moves_R(board& b)
{
    for (int x = 0; x < b.size; x++)  //sprwdzanie wszystkich mozliwych pol
    {
        for (int y = 0; y < b.size; y++)
        {
            if (b.fields[x][y] == ' ')
            {
                b.fields[x][y] = 'r';
                if (DFS_R(b))
                {
                    b.fields[x][y] = ' ';
                    return true;
                }
                b.fields[x][y] = ' ';
            }
        }
    }
    return false;
}

bool Moves_B(board& b)
{
    for (int x = 0; x < b.size; x++) //sprwdzanie wszystkich mozliwych pol
    {
        for (int y = 0; y < b.size; y++)
        {
            if (b.fields[x][y] == ' ')
            {
                b.fields[x][y] = 'b';
                if (DFS_B(b))
                {
                    b.fields[x][y] = ' ';
                    return true;
                }
                b.fields[x][y] = ' ';
            }
        }
    }
    return false;
}

void CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT(board& b)
{
    if (IS_BOARD_CORRECT(b) && IS_GAME_OVER(b) == NONE)
    {
        if ((b.red >= b.size-1) && ((b.red == b.blue) || ( b.blue + b.red <= ( b.size * b.size - 2 )))) //wystarczajaca ilosc pionkow | kolej gracza czerwonego lub przynajmniej 2 wolne pola na planszy
        {
            if (Moves_R(b))
            {
                printf("YES\n");
                return;
            }
        }
    }
    printf("NO\n");
}

void CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT(board& b)
{
    if (IS_BOARD_CORRECT(b) && IS_GAME_OVER(b) == NONE)
    {
        if ((b.red >= b.size - 2) && (((b.red == b.blue) && (b.blue + b.red <= (b.size * b.size - 3))) || ((b.red > b.blue) && (b.blue + b.red <= (b.size * b.size - 4))))) //wystarczajaca ilosc pionkow | kolej gracza czerwonego i przynajmniej 3 wolne pola na planszy lub przynajmniej 4 wolne pola na planszy
        {
            for (int x = 0; x < b.size; x++)  //sprwdzanie wszystkich mozliwych pol
            {
                for (int y = 0; y < b.size; y++)
                {
                    if (b.fields[x][y] == ' ')
                    {
                        b.fields[x][y] = 'r';
                        if (!DFS_R(b))  //nie wygrano
                        {
                            if (Moves_R(b))
                            {
                                printf("YES\n");
                                b.fields[x][y] = ' ';
                                return;
                            }
                        }
                        b.fields[x][y] = ' ';
                    }
                }
            }
        }
    }
    printf("NO\n");
}

void CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT(board& b)
{
    if (IS_BOARD_CORRECT(b) && IS_GAME_OVER(b) == NONE)
    {
        if ((b.blue >= b.size - 1) && ((b.red > b.blue) || (b.blue + b.red <= (b.size * b.size - 2 )))) //wystarczajaca ilosc pionkow | kolej gracza niebieskiego lub przynajmniej 2 wolne pola na planszy
        {
            if (Moves_B(b))
            {
                printf("YES\n");
                return;
            }
        }
    }
    printf("NO\n");
}

void CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT(board& b)
{
    if (IS_BOARD_CORRECT(b) && IS_GAME_OVER(b) == NONE)
    {
        if ((b.blue >= b.size - 2) && (((b.red > b.blue) && (b.blue + b.red <= (b.size * b.size - 3))) || ((b.red == b.blue) && (b.blue + b.red <= (b.size * b.size - 4))))) //wystarczajaca ilosc pionkow | kolej gracza niebieskiego i przynajmniej 3 wolne pola na planszy lub przynajmniej 4 wolne pola na planszy
        {
            for (int x = 0; x < b.size; x++) //sprwdzanie wszystkich mozliwych pol
            {
                for (int y = 0; y < b.size; y++)
                {
                    if (b.fields[x][y] == ' ')
                    {
                        b.fields[x][y] = 'b';
                        if (!DFS_B(b)) //nie wygrano
                        {
                            if (Moves_B(b))
                            {
                                printf("YES\n");
                                b.fields[x][y] = ' ';
                                return;
                            }
                        }
                        b.fields[x][y] = ' ';
                    }
                }
            }
        }
    }
    printf("NO\n");
}

bool Moves_R_plus(board& b) //przynajmniej 2 mozliwosci wygranej
{
    bool posibility = false;
    for (int x = 0; x < b.size; x++)  //sprwdzanie wszystkich mozliwych pol
    {
        for (int y = 0; y < b.size; y++)
        {
            if (b.fields[x][y] == ' ')
            {
                b.fields[x][y] = 'r';
                if (DFS_R(b))
                {
                    if (posibility)
                    {
                        b.fields[x][y] = ' ';
                        return true;
                    }
                    posibility = true;
                }
                b.fields[x][y] = ' ';
            }
        }
    }
    return false;
}

bool Moves_B_plus(board& b) //przynajmniej 2 mozliwosci wygranej
{
    bool posibility = false;
    for (int x = 0; x < b.size; x++)  //sprwdzanie wszystkich mozliwych pol
    {
        for (int y = 0; y < b.size; y++)
        {
            if (b.fields[x][y] == ' ')
            {
                b.fields[x][y] = 'b';
                if (DFS_B(b))
                {
                    if (posibility)
                    {
                        b.fields[x][y] = ' ';
                        return true;
                    }
                    posibility = true;
                }
                b.fields[x][y] = ' ';
            }
        }
    }
    return false;
}

void CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT(board& b)
{
    if (IS_BOARD_CORRECT(b) && IS_GAME_OVER(b) == NONE)
    {
        if (b.red >= b.size - 1) //wystarczajaca ilosc pionkow
        {
            if (b.red == b.blue) //kolej gracza czerwonego
            {
                if (Moves_R(b))
                {
                    printf("YES\n");
                    return;
                }
            }
            else
            {
                if (!Moves_B(b) && Moves_R_plus(b)) //niebieski nie wygra i przynajmniej 2 mozliwosci wygranej (zablokowanie jednej przez niebieskiego)
                {
                    printf("YES\n");
                    return;
                }
            }
        }
    }
    printf("NO\n");
}

void CAN_BLUE_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT(board& b)
{
    if (IS_BOARD_CORRECT(b) && IS_GAME_OVER(b) == NONE)
    {
        if ((b.blue >= b.size - 1) && ((b.red > b.blue) || (b.blue + b.red <= (b.size * b.size - 2)))) //wystarczajaca ilosc pionkow
        {
            if (b.red > b.blue) //kolej gracza niebieskiego
            {
                if (Moves_B(b))
                {
                    printf("YES\n");
                    return;
                }
            }
            else
            {
                if (!Moves_R(b) && Moves_B_plus(b)) //czerwony nie wygra i przynajmniej 2 mozliwosci wygranej (zablokowanie jednej przez czerwonego)
                {
                    printf("YES\n");
                    return;
                }
            }
        }
    }
    printf("NO\n");
}

void CAN_RED_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT(board& b)
{
    if (IS_BOARD_CORRECT(b) && IS_GAME_OVER(b) == NONE)
    {
        if (b.red >= b.size - 2) //wystarczajaca ilosc pionkow
        {
            if (b.red == b.blue)
            {
                for (int x = 0; x < b.size; x++)  //sprwdzanie wszystkich mozliwych pol
                {
                    for (int y = 0; y < b.size; y++)
                    {
                        if (b.fields[x][y] == ' ')
                        {
                            b.fields[x][y] = 'r';
                            if (!DFS_R(b) && !Moves_B(b) && Moves_R_plus(b)) //nie wygrano i niebieski nie wygra i przynajmniej 2 mozliwosci wygranej (zablokowanie jednej przez niebieskiego)
                            {
                                printf("YES\n");
                                b.fields[x][y] = ' ';
                                return;
                            }
                            b.fields[x][y] = ' ';
                        }
                    }
                }
            }
            else
            {
                if (!Moves_B(b)) // niebieski nie wygra w 1 ruchu
                {
                    for (int x_b = 0; x_b < b.size; x_b++)  //sprwdzanie wszystkich mozliwych pol przeciwnika
                    {
                        for (int y_b = 0; y_b < b.size; y_b++)
                        {
                            if (b.fields[x_b][y_b] == ' ')
                            {
                                bool red = false;
                                b.fields[x_b][y_b] = 'b';
                                for (int x_r = 0; x_r < b.size; x_r++)  //sprwdzanie wszystkich mozliwych kontr
                                {
                                    for (int y_r = 0; y_r < b.size; y_r++)
                                    {
                                        if (b.fields[x_r][y_r] == ' ')
                                        {
                                            b.fields[x_r][y_r] = 'r';
                                            if (!DFS_R(b) && !Moves_B(b) && Moves_R_plus(b)) //nie wygrano i niebieski nie wygra i przynajmniej 2 mozliwosci wygranej (zablokowanie jednej przez niebieskiego)
                                            {
                                                red = true;
                                                b.fields[x_r][y_r] = ' ';
                                                break;
                                            }
                                            b.fields[x_r][y_r] = ' ';
                                        }
                                    }
                                    if (red) //znaleziono kontre dla ruchu
                                    {
                                        break;
                                    }
                                }
                                b.fields[x_b][y_b] = ' ';
                                if (!red) //nie znaleziono kontry dla ruchu
                                {
                                    printf("NO\n");
                                    return;
                                }
                            }
                        }
                    }
                    printf("YES\n");
                    return;
                }
            }
        }
    }
    printf("NO\n");
}

void CAN_BLUE_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT(board& b)
{
    if (IS_BOARD_CORRECT(b) && IS_GAME_OVER(b) == NONE)
    {
        if (b.blue >= b.size - 2) //wystarczajaca ilosc pionkow
        {
            if (b.red > b.blue) //kolej gracza niebieskiego
            {
                for (int x = 0; x < b.size; x++)  //sprwdzanie wszystkich mozliwych pol
                {
                    for (int y = 0; y < b.size; y++)
                    {
                        if (b.fields[x][y] == ' ')
                        {
                            b.fields[x][y] = 'b';
                            if (!DFS_B(b) && !Moves_R(b) && Moves_B_plus(b)) //nie wygrano i czerwony nie wygra i przynajmniej 2 mozliwosci wygranej (zablokowanie jednej przez czerwonego)
                            {
                                printf("YES\n");
                                b.fields[x][y] = ' ';
                                return;
                            }
                            b.fields[x][y] = ' ';
                        }
                    }
                }
            }
            else
            {
                if (!Moves_R(b)) // czerwony nie wygra w 1 ruchu
                {
                    for (int x_r = 0; x_r < b.size; x_r++)  //sprwdzanie wszystkich mozliwych pol przeciwnika
                    {
                        for (int y_r = 0; y_r < b.size; y_r++)
                        {
                            if (b.fields[x_r][y_r] == ' ')
                            {
                                bool blue = false;
                                b.fields[x_r][y_r] = 'r';
                                for (int x_b = 0; x_b < b.size; x_b++)  //sprwdzanie wszystkich mozliwych kontr
                                {
                                    for (int y_b = 0; y_b < b.size; y_b++)
                                    {
                                        if (b.fields[x_b][y_b] == ' ')
                                        {
                                            b.fields[x_b][y_b] = 'b';
                                            if (!DFS_B(b) && !Moves_R(b) && Moves_B_plus(b)) //nie wygrano i czerwony nie wygra i przynajmniej 2 mozliwosci wygranej (zablokowanie jednej przez czerwonego)
                                            {
                                                blue = true;
                                                b.fields[x_b][y_b] = ' ';
                                                break;
                                            }
                                            b.fields[x_b][y_b] = ' ';
                                        }
                                    }
                                    if (blue) //znaleziono kontre dla ruchu
                                    {
                                        break;
                                    }
                                }
                                b.fields[x_r][y_r] = ' ';
                                if (!blue) //nie znaleziono kontry dla ruchu
                                {
                                    printf("NO\n");
                                    return;
                                }
                            }
                        }
                    }
                    printf("YES\n");
                    return;
                }
            }
        }
    }
    printf("NO\n");
}

bool Command(board& b) //przetwarzanie inputu (komenda)
{
    char s[MAX_C];
    int error = scanf("%s", s);
    while (error == 1)
    {
        if (!strcmp(s, "---")) //poczatek nowej planszy
        {
            return false;
        }
        else
        {
            if (!strcmp(s, "BOARD_SIZE"))
            {
                printf("%d \n", b.size);
            }
            else if (!strcmp(s, "PAWNS_NUMBER"))
            {
                printf("%d \n", b.red + b.blue);
            }
            else if (!strcmp(s, "IS_BOARD_CORRECT"))
            {
                if (IS_BOARD_CORRECT(b))
                {
                    printf("YES\n");
                }
                else
                {
                    printf("NO\n");
                }
            }
            else if (!strcmp(s, "IS_GAME_OVER"))
            {
                switch (IS_GAME_OVER(b))
                {
                case RED:
                    printf("YES RED\n");
                    break;
                case BLUE:
                    printf("YES BLUE\n");
                    break;
                default:
                    printf("NO\n");
                    break;
                }
                
            }
            else if (!strcmp(s, "IS_BOARD_POSSIBLE"))
            {
                IS_BOARD_POSSIBLE(b);
            }
            else if (!strcmp(s, "CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT"))
            {
                CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT(b);
            }
            else if (!strcmp(s, "CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT"))
            {
                CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT(b);
            }
            else if (!strcmp(s, "CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT"))
            {
                CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT(b);
            }
            else if (!strcmp(s, "CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT"))
            {
                CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT(b);
            }
            else if (!strcmp(s, "CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT"))
            {
                CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT(b);
            }
            else if (!strcmp(s, "CAN_BLUE_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT"))
            {
                CAN_BLUE_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT(b);
            }
            else if (!strcmp(s, "CAN_RED_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT"))
            {
                CAN_RED_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT(b);
            }
            else if (!strcmp(s, "CAN_BLUE_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT"))
            {
                CAN_BLUE_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT(b);
            }
        }
        error = scanf("%s", s);
    }
    return true;
}

bool ReadInput() //przetwarzanie inputu
{
    stos* head = nullptr;
    board b;
    bool end;
    b.size = 0; b.red = 0; b.blue = 0;
    Input(head, b);
    CreateBoard(head, b);
    end = Command(b);
    printf("\n");
    DeleteTAB(b.fields, b.size);
    return end;
}

int main()
{
    bool end = false;
    while (!end)
    {
        end = ReadInput();
    }
    return 0;
}