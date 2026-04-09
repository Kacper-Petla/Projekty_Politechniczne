#include<ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define B_FIRST_LINE 	"| 13 14 15 16 17 18 |==| 19 20 21 22 23 24 |==|"
#define B_BAR_LINE 		"|===================|00|===================|00|"
#define B_PAWNS_LINE 	"| .. .. .. .. .. .. |  | .. .. .. .. .. .. |  |"
#define B_NUMBER_LINE 	"| 00 00 00 00 00 00 |  | 00 00 00 00 00 00 |  |"
#define B_EMPTY_LINE 	"|                   |==|                   |==|"
#define B_LAST_LINE 	"| 12 11 10 09 08 07 |==| 06 05 04 03 02 01 |==|"
#define P_FILE 			"start.txt"
#define P_P1 "a"
#define P_P2 "b"
#define SPIKE "."

#define B_YP 1
#define B_Y0 B_YP+2
#define B_Y1 B_Y0+2
#define B_Y2 B_Y0+11
#define B_Y2D B_Y0+16
#define B_X0 3
#define B_X1 B_X0+2
#define B_X2 B_X0+25
#define B_X3 B_X0+49
#define H_B_X1 B_X0+21
#define H_B_X2 B_X0+44
#define H_B_Y1 B_Y0+1
#define H_B_Y2 B_Y0+17
#define NEXT 3
#define P_X1 12
#define P_X2 18
#define P_Y1 11
#define P_Y2 5
#define TEXT_Y 24
#define TEXT_X 3

#define PAWNS 15
#define FIELDS 24
#define PIECE_BOARD 6
#define STARTING_FIELD -1
#define PLAYERS 2
#define FAMOUS 3
#define NAMES 21
#define P1 0
#define P2 1
#define P3 2
#define P1M 1
#define P2M -1
#define ABORT 0
#define G_ABORT 10


struct board_t {
	int Cube[PLAYERS];
	int P_Pawns[PLAYERS][FIELDS];
	int Bar[PLAYERS];
	int Home[PLAYERS];
	int Points[PLAYERS];
	int M_Player;			//obecny gracz
	int S_Player;			//oczekujacy gracz
	int Turn;
	char F_Names[FAMOUS][NAMES];
	int F_Points[FAMOUS];
};

void C_Line(int i)
{
	move(TEXT_Y+i,TEXT_X);
	clrtoeol();
}

void Wait()
{
	move(0,0);
	refresh();
	getch();
}

void ERROR()
{
	mvprintw(B_YP+1,B_X0,"ERROR");
	Wait();
	move(B_YP+1,B_X0);
	clrtoeol();
	
}

void PrintPoints(int Points[])
{
	move(B_Y0-2,B_X0);
	clrtoeol();
	printw("Punkty gracza nr 1: %d, Punkty gracza nr 2: %d", Points[P1], Points[P2]);
}

int ExtraPawns(int Pawn, int U_D, int L_R)
{
	Pawn=Pawn-PIECE_BOARD;
	mvprintw(U_D,L_R,"%d",Pawn);
	return PIECE_BOARD;
}

void P1Pawns(int U_D, int L_R)
{
	mvprintw(U_D,L_R,P_P1);
	mvprintw(U_D,L_R+1,P_P1);
}

void P2Pawns(int U_D, int L_R)
{
	mvprintw(U_D,L_R,P_P2);
	mvprintw(U_D,L_R+1,P_P2);
}

void P1LoadPiecePawns(int Pawn, int U_D, int L_R, int Mode)
{
	if (Pawn>PIECE_BOARD)
	{
		Pawn=ExtraPawns(Pawn,U_D+(Mode*PIECE_BOARD),L_R+1);
	}
	for (int j=0; j<Pawn; j++)
	{
		P1Pawns(U_D+(Mode*j), L_R);
	}
}

void P2LoadPiecePawns(int Pawn, int U_D, int L_R, int Mode)
{
	if (Pawn>PIECE_BOARD)
	{
		Pawn=ExtraPawns(Pawn,U_D+(Mode*PIECE_BOARD),L_R+1);
	}
	for (int j=0; j<Pawn; j++)
	{
		P2Pawns(U_D+(Mode*j), L_R);
	}
}

void LoadPiecePawns(struct board_t *b, int L_R, int U_D, int Field, int Mode)
{
	for (int i=0; i<PIECE_BOARD; i++)
	{
		if ((*b).P_Pawns[P1][Field+(Mode*i)]!=0)
		{
			int Pawns=(*b).P_Pawns[P1][Field+(Mode*i)];
			P1LoadPiecePawns(Pawns, U_D, L_R+(i*NEXT), Mode);
		}
		else if ((*b).P_Pawns[P2][Field+(Mode*i)]!=0)
		{
			int Pawns=(*b).P_Pawns[P2][Field+(Mode*i)];
			P2LoadPiecePawns(Pawns, U_D, L_R+(i*NEXT), Mode);
		}
	}
}

void LoadOffBoard(int Value, int L_R, int U_D)
{
	if(Value>=10) //LICZBA DWUCYFROWA
	{
		mvprintw(U_D,L_R,"%d",Value);
	}
	else
	{
		mvprintw(U_D,L_R+1,"%d",Value);
	}
}

void LoadBar(struct board_t *b)
{
	LoadOffBoard((*b).Bar[P1], H_B_X1, H_B_Y2);
	LoadOffBoard((*b).Bar[P2], H_B_X1, H_B_Y1);
}

void LoadHome(struct board_t *b)
{
	LoadOffBoard((*b).Home[P1], H_B_X2, H_B_Y2);
	LoadOffBoard((*b).Home[P2], H_B_X2, H_B_Y1);
}

void LoadPawns(struct board_t *b)
{
	LoadPiecePawns(b,B_X1,B_Y1,P_X1,P1M);
	LoadPiecePawns(b,B_X2,B_Y1,P_X2,P1M);
	LoadPiecePawns(b,B_X1,B_Y2D,P_Y1,P2M);
	LoadPiecePawns(b,B_X2,B_Y2D,P_Y2,P2M);
	LoadBar(b);
	LoadHome(b);
}

void CB_Pawns(struct board_t *b, int U_D)
{
	for (int i=U_D; i<(U_D+PIECE_BOARD); i++)
	{
		mvprintw(i,B_X0,B_PAWNS_LINE);
	}
}

void CreateBoard(struct board_t *b)
{
	mvprintw(B_Y0,B_X0,B_FIRST_LINE);
	mvprintw(B_Y0+1,B_X0,B_BAR_LINE);
	CB_Pawns(b, B_Y1);
	mvprintw((B_Y1+PIECE_BOARD),B_X0,B_NUMBER_LINE);
	mvprintw((B_Y1+PIECE_BOARD+1),B_X0,B_EMPTY_LINE);
	mvprintw((B_Y2-1),B_X0,B_NUMBER_LINE);
	CB_Pawns(b, B_Y2);
	mvprintw((B_Y2+PIECE_BOARD),B_X0,B_BAR_LINE);
	mvprintw((B_Y2+PIECE_BOARD+1),B_X0,B_LAST_LINE);

	mvprintw(H_B_Y1, B_X3, "P2");
	mvprintw(H_B_Y2, B_X3, "P1");

	LoadPawns(b);
}

void SaveData(struct board_t *b, FILE *plik)
{
	for (int p=0; p<PLAYERS; p++)
	{
		for (int i=0; i<FIELDS; i++)
		{
			fprintf(plik,",%d,",(*b).P_Pawns[p][i]);
		}
	}
	for (int p=0; p<PLAYERS; p++)
	{
		fprintf(plik,",%d,",(*b).Bar[p]);
	}
	for (int p=0; p<PLAYERS; p++)
	{
		fprintf(plik,",%d,",(*b).Home[p]);
	}
}

void ReadData(struct board_t *b, FILE *plik)
{
	for (int p=0; p<PLAYERS; p++)
	{
		for (int i=0; i<FIELDS; i++)
		{
			fscanf(plik,",%d,",&(*b).P_Pawns[p][i]);
		}
	}
	for (int p=0; p<PLAYERS; p++)
	{
		fscanf(plik,",%d,",&(*b).Bar[p]);
	}
	for (int p=0; p<PLAYERS; p++)
	{
		fscanf(plik,",%d,",&(*b).Home[p]);
	}
}

void S_R_Pawns(struct board_t *b, FILE *plik)
{
	fprintf(plik,"_%d_",(*b).Turn);
	SaveData(b, plik);
	fprintf(plik,"\n");
}

void S_Replay(struct board_t *b)
{
	(*b).Turn++;
	FILE *plik;
	plik = fopen("Replay.txt", "a");
	if (plik == NULL)
	{
		ERROR();
		return;
	}
	S_R_Pawns(b, plik);
	fclose(plik);
}

void R_R_Pawns(struct board_t *b, FILE *plik, int looking)
{
	int Number;
	while (fscanf(plik,"_%d_",&Number)==1)
	{
		ReadData(b, plik);
		fscanf(plik,"\n");

		if(Number==looking)
		{
			return;
		}
	}
	ERROR();
}

void R_Replay(struct board_t *b, int looking)
{
	FILE *plik;
	plik = fopen("Replay.txt", "r");
	if (plik == NULL)
	{
		ERROR();
		return;
	}
	R_R_Pawns(b, plik, looking);
	fclose(plik);

	CreateBoard(b);
}

void R_Menu(struct board_t *b)
{
	while (1)
	{
		int looking;
		mvprintw (B_YP,B_X0,"Podaj liczbe tury od 1 do %d, lub 0 by zakonczyc ", (*b).Turn);
		refresh();
		scanw("%d",&looking);
		if (looking==ABORT)
		{
			erase();
			return;
		}
		R_Replay(b, looking);
	}
}

void R_New(struct board_t *b)
{
	FILE *plik;
	(*b).Turn=0;
	plik = fopen("Replay.txt", "w");
	fclose(plik);
	S_Replay(b);
}

void R_InGame(struct board_t *b)
{
	R_Menu(b);
	R_Replay(b, (*b).Turn);
	PrintPoints((*b).Points);
	CreateBoard(b);
}

void NB_Pawns(struct board_t *b, FILE *plik)
{
	for (int p=0; p<PLAYERS; p++)
	{
		for (int i=0; i<FIELDS; i++)
		{
			fscanf(plik,",%d,",&(*b).P_Pawns[p][i]);
		}
		fscanf(plik,"\n");
	}
	for (int p=0; p<PLAYERS; p++)
	{
		fscanf(plik,",%d,",&(*b).Bar[p]);
	}
	fscanf(plik,"\n");
	for (int p=0; p<PLAYERS; p++)
	{
		fscanf(plik,",%d,",&(*b).Home[p]);
	}
	fscanf(plik,"\n");
}

void NewBoard(struct board_t *b)
{
	FILE *plik;
	plik = fopen(P_FILE, "r");
	if (plik == NULL)
	{
		ERROR();
		return;
	}
	NB_Pawns(b, plik);
	fclose(plik);
	
	R_New(b);
	PrintPoints((*b).Points);
	CreateBoard(b);
}

void P_FileName(char Name[])
{
	C_Line(0);
	printw("Podaj nazwe pliku txt (20 znakow): ");
	refresh();
	scanw("%s",Name);
}

void SaveBoard(struct board_t *b)
{
	char Name[NAMES];
	P_FileName(Name);
	FILE *plik;
	C_Line(0);
	plik = fopen(Name, "w");
	if (plik == NULL)
	{
		ERROR();
		return;
	}
	SaveData(b, plik);
	for (int p=0; p<PLAYERS; p++)
	{
		fprintf(plik,",%d,",(*b).Points[p]);
	}
	fprintf(plik,",%d,",(*b).M_Player);
	fprintf(plik,"\n");
	fclose(plik);
	printw("Zapisales stan gry");
}

int LoadBoard(struct board_t *b)
{
	char Name[NAMES];
	P_FileName(Name);
	FILE* plik;
	C_Line(0);
	plik = fopen(Name, "r");
	if (plik == NULL)
	{
		printw("Brak zapisu");
		return ABORT;
	}
	ReadData(b, plik);
	for (int p=0; p<PLAYERS; p++)
	{
		fscanf(plik,",%d,",&(*b).Points[p]);
	}
	fscanf(plik,",%d,",&(*b).M_Player);
	fscanf(plik,"\n");
	fclose(plik);

	R_New(b);
	PrintPoints((*b).Points);
	CreateBoard(b);
	C_Line(0);
	printw("Wczytales stan gry");
	return 1;
}

void FameLoad(struct board_t *b)
{
	FILE *plik;
	plik = fopen("fame.txt", "r");
	if (plik == NULL)
	{
		ERROR();
		return;
	}
	for(int i=0; i<FAMOUS; i++)
	{
		fscanf(plik,",%[^,],%d,\n", (*b).F_Names[i], &(*b).F_Points[i]);
	}
	fclose(plik);
}

void FameSave(struct board_t *b)
{
	FILE *plik;
	plik = fopen("fame.txt", "w");
	if (plik == NULL)
	{
		printw("ERROR");
		return;
	}
	for(int i=0; i<FAMOUS; i++)
	{
		fprintf(plik,",%s,%d,\n", (*b).F_Names[i], (*b).F_Points[i]);
	}
	fclose(plik);
}

void FamePrint(struct board_t *b)
{
	FameLoad(b);
	erase();
	mvprintw(B_Y0,B_X0,"HALL OF FAME:");
	for(int i=0; i<FAMOUS; i++)
	{
		mvprintw(H_B_Y1+i,B_X0,"%d: %s - %d P", i+1, (*b).F_Names[i], (*b).F_Points[i]);
	}
	Wait();
	erase();
}

void LoadName(char N_Name[])
{
	printw("Podaj swoja nazwe (max 20 znakow): ");
	refresh();
	scanw("%s",N_Name);
}

void CopyData(struct board_t *b, char N_Name[], int N_Points, int Player)
{
	strcpy((*b).F_Names[Player], N_Name);
	(*b).F_Points[Player]=N_Points;
}

void NewFame(struct board_t *b)
{
	int Points=(*b).Points[(*b).M_Player];
	char N_Name[NAMES];
	C_Line(0);
	FameLoad(b);
	if ((*b).F_Points[P1]<Points)
	{
		LoadName(N_Name);
		CopyData(b, (*b).F_Names[P2], (*b).F_Points[P2], P3);
		CopyData(b, (*b).F_Names[P1], (*b).F_Points[P1], P2);
		CopyData(b, N_Name, Points, P1);
	}
	else if((*b).F_Points[P2]<Points)
	{
		LoadName(N_Name);
		CopyData(b, (*b).F_Names[P2], (*b).F_Points[P2], P3);
		CopyData(b, N_Name, Points, P2);
	}
	else if((*b).F_Points[P3]<Points)
	{
		LoadName(N_Name);
		CopyData(b, N_Name, Points, P3);
	}
	FameSave(b);
}


void PiecePawnChangePlus(int U_D, int L_R, int Mode, int Pawn, int Player)
{
	if (Pawn>PIECE_BOARD)
	{
		ExtraPawns(Pawn, U_D+(Mode*PIECE_BOARD), L_R+1);
	}
	else
	{
		if (Player==P1)
		{
			P1Pawns(U_D+(Mode*Pawn)-Mode, L_R);
		}
		else
		{
			P2Pawns(U_D+(Mode*Pawn)-Mode, L_R);
		}
	}
}

void PiecePawnChangeMinus(int U_D, int L_R, int Mode, int Pawn)
{
	if (Pawn>=PIECE_BOARD)
	{
		ExtraPawns(Pawn, U_D+(Mode*PIECE_BOARD), L_R+1);
	}
	else
	{
		mvprintw(U_D+(Mode*Pawn),L_R,SPIKE);
		mvprintw(U_D+(Mode*Pawn),L_R+1,SPIKE);
	}
}

void PieceBordCords(int* U_D, int* L_R, int* Mode, int* field)
{
	if ((*field<=P_Y2))
	{
		*U_D=B_Y2D;
		*L_R=B_X2;
		*Mode=P2M;
		*field=PIECE_BOARD-1-(*field % PIECE_BOARD);
	}
	else if ((*field<=P_Y1)&&(*field>P_Y2))
	{
		*U_D=B_Y2D;
		*L_R=B_X1;
		*Mode=P2M;
		*field=PIECE_BOARD-1-(*field % PIECE_BOARD);
	}
	else if ((*field>=P_X1)&&(*field<P_X2))
	{
		*U_D=B_Y1;
		*L_R=B_X1;
		*Mode=P1M;
		*field=(*field % PIECE_BOARD);
	}
	else if ((*field>=P_X2))
	{
		*U_D=B_Y1;
		*L_R=B_X2;
		*Mode=P1M;
		*field=(*field % PIECE_BOARD);
	}
}

void PawnChange(int Pawn, int field , int Player, int Operation)
{
	int U_D;
	int L_R;
	int Mode; 
	PieceBordCords(&U_D, &L_R, &Mode, &field);
	L_R=L_R+(NEXT*field);
	if(Operation==P1M)
	{
		PiecePawnChangePlus(U_D, L_R, Mode, Pawn, Player);
	}
	else
	{
		PiecePawnChangeMinus(U_D, L_R, Mode, Pawn);
	}
}

void Throw(int Cube[])
{
	C_Line(0);
	Cube[P1] = (rand() % 6) + 1;
	Cube[P2] = (rand() % 6) + 1;
	printw("WYRZUCONE KOSCI: Nr 1: %d ,Nr 2: %d", Cube[P1], Cube[P2]);
}

void FirstThrow(struct board_t *b)
{
	while (1)
	{
		Throw((*b).Cube);
		C_Line(1);
		if ((*b).Cube[P1] != (*b).Cube[P2])
		{
			break;
		}
		printw("Resmis");
		Wait();
	}
	if ((*b).Cube[P1] > (*b).Cube[P2])
	{
		printw("Zaczyna gracz Nr 1");
		(*b).M_Player = P1;
	}
	else
	{
		printw("Zaczyna gracz Nr 2");
		(*b).M_Player = P2;
	}
	Wait();
	C_Line(1);
}

void CubeChange(struct board_t *b)
{
	if ((*b).M_Player == P2)
	{
		(*b).Cube[P1] = (*b).Cube[P1] * P2M;
		(*b).Cube[P2] = (*b).Cube[P2] * P2M;
	}
}

int PawnsInHome(struct board_t *b, int* HomeField, int* Direction)
{
	int Hom[PLAYERS] = { FIELDS,STARTING_FIELD };						//Pola domu odpowiednio gracza 1 i 2
	*HomeField = Hom[(*b).M_Player];
	int H=*HomeField;
	int Dir[PLAYERS] = { P1M,P2M };									//Kierunek ruchu odpowiednio gracza 1 i 2
	*Direction = Dir[(*b).M_Player];
	int D=*Direction;

	int FirstHome = (H - (D * PIECE_BOARD));
	int InHome = 0;
	
	for (int i = (D * FirstHome); i < (D * H); i++)		
	{
		InHome = InHome+(*b).P_Pawns[(*b).M_Player][(i * D)];
	}
	InHome = InHome + (*b).Home[(*b).M_Player];
	if (InHome == PAWNS)
	{
		return 1;
	}
	return ABORT;
}

int AnyPawnsInHome(struct board_t *b)
{
	int HomeField[PLAYERS] = { FIELDS,STARTING_FIELD };						//Pola domu odpowiednio gracza 1 i 2
	int H = HomeField[(*b).M_Player];
	int Direction[PLAYERS] = { P1M,P2M };									//Kierunek ruchu odpowiednio gracza 1 i 2
	int D = Direction[(*b).M_Player];

	int FirstHome = (H - (D * PIECE_BOARD));
	int InHome = 0;

	for (int i = (FirstHome*D); i < (D*H); i++)
	{
		if ((*b).P_Pawns[(*b).S_Player][(i*D)] != 0)
		{
			return 1;
		}
	}
	return ABORT;
}

int P_Add(struct board_t *b, int i)
{
	(*b).Points[(*b).M_Player] = (*b).Points[(*b).M_Player] + i;
}

void EndGame(struct board_t *b, int* Status)
{
	if ((*b).Home[(*b).M_Player] == PAWNS)
	{
		*Status=G_ABORT;
		C_Line(0);
		int InHome = AnyPawnsInHome(b);
		printw("Wygrywa gracz Nr %d ,zdobywajac ",(*b).M_Player + 1);
		if ((*b).Home[(*b).S_Player] != 0)
		{
			P_Add(b,1);
			printw("1 punkt");
		}
		else if (((*b).Bar[(*b).S_Player] != 0) || (InHome != 0))
		{
			P_Add(b,3);
			printw("3 punkty");
		} 
		else
		{
			P_Add(b,2);
			printw("2 punkty");
		}
		PrintPoints((*b).Points);
		S_Replay(b);
		Wait();
		NewFame(b);
	}
}


void AttackChange(int Attacks[], int Player)
{
	if (Player==P1)
	{
		Attacks[0]=1;
	}
	else
	{
		int attack=Attacks[0];
		Attacks[1]=Attacks[attack];
		Attacks[0]=1;
	}
}

int MovePosibility(struct board_t *b, int PosibleM[], int C_Value)
{
	int posible = ABORT; 
	int attack = ABORT;
	for (int i = 0; i < FIELDS; i++)
	{
		if ((*b).P_Pawns[(*b).M_Player][i] != 0)
		{
			if ((i + C_Value > STARTING_FIELD) && (i + C_Value < FIELDS))
			{
				if ((*b).P_Pawns[(*b).S_Player][i + C_Value] == 1)
				{
					attack++;
					PosibleM[attack] = i;
				}
				else if (((*b).P_Pawns[(*b).S_Player][i + C_Value] == 0) && (attack == ABORT))
				{
					posible++;
					PosibleM[posible] = i;
				}
			}
		}
	}
	if (attack != ABORT)
	{
		PosibleM[0] = attack;
		AttackChange(PosibleM, (*b).M_Player);
		return 1;
	}
	else
	{
		PosibleM[0] = posible;
		return 0;
	}
}

int BarMovePosibility(struct board_t *b, int PosibleBarMove[], int C_Value)
{
	int f[PLAYERS] = { STARTING_FIELD,FIELDS }; // pola startowe band odpowiednio gracza 1 i 2
	int field = f[(*b).M_Player];					// if
	if ((*b).P_Pawns[(*b).S_Player][field + C_Value] == 1)
	{
		PosibleBarMove[0] = 1;
		PosibleBarMove[1] = field;
		return 1;
	}
	else if ((*b).P_Pawns[(*b).S_Player][field + C_Value] == 0)
	{
		PosibleBarMove[0] = 1;
		PosibleBarMove[1] = field;
		return 0;
	}
	else
	{
		PosibleBarMove[0] = ABORT;
		return 0;
	}
}

int HomeMovePosibility(struct board_t *b, int H_Field, int Dir, int PosibleHomeMove[], int C_Value)
{
	int posible = ABORT;
	int attack = ABORT;

	int FirstHome = (H_Field - (Dir * PIECE_BOARD));

	for (int i = (Dir*FirstHome); i < (Dir*H_Field); i++)
	{
		int j=(i*Dir);
		if ((*b).P_Pawns[(*b).M_Player][j] != 0)
		{
			if ((*b).P_Pawns[(*b).S_Player][j + C_Value] == 1)
			{
				attack++;
				PosibleHomeMove[attack] = j;
			}
			else if (((Dir*(j + C_Value)) >= (Dir*H_Field)) && (attack == ABORT) && (posible == ABORT))
			{
				posible++;
				PosibleHomeMove[posible] = j;
			}
		}
	}
	if (attack != ABORT)
	{
		PosibleHomeMove[0] = attack;
		AttackChange(PosibleHomeMove, (*b).M_Player);
		return 1;
	}
	else
	{
		PosibleHomeMove[0] = posible;
		return 0;
	}
}

void AP_Two(struct board_t *b, int PosibleAttackP1[], int PosibleAttackP2[])
{
	int M[]={P1M, P2M}; // tryby operacji dla odpowiednio gracza 1 i 2
	int Mode=M[(*b).M_Player];

	int x=(PosibleAttackP1[1]+((*b).Cube[P1]))*Mode;
	int y=(PosibleAttackP2[1]+((*b).Cube[P2]))*Mode;

	if (x<y)
	{
		PosibleAttackP2[0]=ABORT;
	}
	else if(x>y)
	{
		PosibleAttackP1[0]=ABORT;
	}
}

void AttackPosibility(struct board_t *b, int PosibleAttackP1[], int PosibleAttackP2[], int attack[])
{
	if ((attack[P1] + attack[P2]) == 2)
	{
		AP_Two(b, PosibleAttackP1, PosibleAttackP2);
	}
	if ((attack[P1] + attack[P2]) == 1)
	{
		if (attack[P1] == 1)
		{
			PosibleAttackP2[0] = ABORT;
		}
		else if (attack[P2] == 1)
		{
			PosibleAttackP1[0] = ABORT;
		}
	}
}


void C_Move(struct board_t *b, int C_Value, int field)
{
	(*b).P_Pawns[(*b).M_Player][field]--;
	PawnChange((*b).P_Pawns[(*b).M_Player][field], field, (*b).M_Player, -1);
	(*b).P_Pawns[(*b).M_Player][field + C_Value]++;
	PawnChange((*b).P_Pawns[(*b).M_Player][field+C_Value], field+C_Value, (*b).M_Player, 1);
}

void C_Attack(struct board_t *b, int C_Value, int field)
{
	(*b).P_Pawns[(*b).S_Player][field + C_Value]--;
	PawnChange((*b).P_Pawns[(*b).S_Player][field + C_Value], field + C_Value, (*b).S_Player, -1);
	(*b).Bar[(*b).S_Player]++;
	LoadBar(b);
}

void C_Bar(struct board_t *b, int C_Value, int PosibleBarMove)
{
	(*b).Bar[(*b).M_Player]--;
	LoadBar(b);
	(*b).P_Pawns[(*b).M_Player][PosibleBarMove + C_Value]++;
	PawnChange((*b).P_Pawns[(*b).M_Player][PosibleBarMove + C_Value], PosibleBarMove + C_Value, (*b).M_Player, 1);
}

void C_Home(struct board_t *b, int field)
{
	(*b).P_Pawns[(*b).M_Player][field]--;
	PawnChange((*b).P_Pawns[(*b).M_Player][field], field, (*b).M_Player, -1);
	(*b).Home[(*b).M_Player]++;
	LoadHome(b);
}

int MoveCounting(struct board_t *b, int PosibleMove[], int C_Value, int field)
{
	C_Line(1);
	if (PosibleMove[0] == ABORT)
	{
		printw("Nie mozna wykonac ruchu ta kostka");
		return 1;
	}
	for (int i = 1; i <= PosibleMove[0]; i++)
	{
		if (PosibleMove[i] == field)
		{
			if ((*b).P_Pawns[(*b).S_Player][field + C_Value] == 1) //bicie
			{
				printw("wykonales bicie");
				C_Attack(b, C_Value, field);
				C_Move(b, C_Value, field);
				return ABORT;
			}
			else //ruch
			{
				printw("wykonales ruch");
				C_Move(b, C_Value, field);
				return ABORT;
			}
		}
	}
	printw("Niepoprawny ruch");
	return 1;
}

int BarMoveCounting(struct board_t *b, int PosibleBarMove[], int C_Value)
{
	C_Line(1);
	if (PosibleBarMove[0] == ABORT)
	{
		printw("Nie mozna wykonac ruchu ta kostka");
		return 1;
	}
	else if ((*b).P_Pawns[(*b).S_Player][PosibleBarMove[1] + C_Value] == 1) //bicie
	{
		printw("wyszedles z biciem");
		C_Attack(b, C_Value, PosibleBarMove[1]);
		C_Bar(b, C_Value, PosibleBarMove[1]);
		return ABORT;
	}
	else //ruch
	{
		printw("wyszedles z bandy");
		C_Bar(b, C_Value, PosibleBarMove[1]);
		return ABORT;
	}
}

int HomeMoveCounting(struct board_t *b, int PosibleHomeMove[], int C_Value, int field)
{
	C_Line(1);
	if (PosibleHomeMove[0] == ABORT)
	{
		printw("Nie mozna wykonac ruchu ta kostka");
		return 1;
	}
	else if (PosibleHomeMove[1] == field)
	{
		if ((*b).P_Pawns[(*b).S_Player][field + C_Value] == 1) //bicie
		{
			printw("wykonales bicie");
			C_Attack(b, C_Value, field);
			C_Move(b, C_Value, field);
			return ABORT;
		}
		else //w domu
		{
			printw("wszedles do domu");
			C_Home(b, field);
			return ABORT;
		}
	}
	printw("Niepoprawny ruch");
	return 1;
}


int PlayerMoveFirst(struct board_t *b, int PosibleMove[PLAYERS][(PAWNS + 1)])
{
	int number, nr, field, f;
	int done = 1;
	while (done)
	{
		C_Line(1);
		printw("Podaj nr kostki (1/2): ");
		refresh();
		scanw("%d",&nr);
		C_Line(1);
		printw(",i pole do ruchu(1-24): ");
		refresh();
		scanw("%d",&f);
		number = nr-1; field = f-1;

		done = MoveCounting(b, PosibleMove[number], (*b).Cube[number], field);
		Wait();
		C_Line(1);
	}
	return (nr % PLAYERS);
}

void PlayerMoveSecound(struct board_t *b, int PosibleMove[], int C_Value)
{
	int field, f;
	int done = 1;
	while (done)
	{
		C_Line(1);
		printw("Podaj pole do ruchu(1-24): ");
		refresh();
		scanw("%d",&f);
		field = f - 1;

		done=MoveCounting(b, PosibleMove, C_Value, field);
		Wait();
		C_Line(1);
	}
}

int PlayerBarMove(struct board_t *b, int PosibleBarMove[PLAYERS][PLAYERS])
{
	int number, nr;
	int done = 1;
	while (done)
	{
		C_Line(1);
		printw("Podaj nr kostki (1/2): ");
		refresh();
		scanw("%d",&nr);
		number = nr - 1;

		done = BarMoveCounting(b, PosibleBarMove[number], (*b).Cube[number]);
		Wait();
		C_Line(1);
	}
	return (nr % PLAYERS);
}

int PlayerHomeMoveFirst(struct board_t *b, int PosibleHomeMove[PLAYERS][(PIECE_BOARD + 1)])
{
	int number, nr, field, f;
	int done = 1;
	while (done)
	{
		C_Line(1);
		printw("Podaj nr kostki (1/2): ");
		refresh();
		scanw("%d",&nr);
		C_Line(1);
		printw(",i pole do ruchu(1-24): ");
		refresh();
		scanw("%d",&f);
		number = nr - 1; field = f - 1;

		done = HomeMoveCounting(b, PosibleHomeMove[number], (*b).Cube[number], field);
		Wait();
		C_Line(1);
	}
	return (nr % PLAYERS);
}

void PlayerHomeMoveSecound(struct board_t *b, int PosibleHomeMove[], int C_Value)
{
	int field, f;
	int done = 1;
	while (done)
	{
		C_Line(1);
		printw("Podaj pole do ruchu(1-24): ");
		refresh();
		scanw("%d",&f);
		field = f - 1;

		done = HomeMoveCounting(b, PosibleHomeMove, C_Value, field);
		Wait();
		C_Line(1);
	}
}


void EndTurn()
{
	printw("BRAK MOZLIWYCH RUCHOW (koniec kolejki)");
	Wait();
	C_Line(1);
}

int SecoundMove(struct board_t *b, int C_Value)
{
	C_Line(1);
	int PosibleMove[(PAWNS+1)];
	MovePosibility(b, PosibleMove, C_Value);
	if (PosibleMove[0] == ABORT)
	{
		EndTurn();
		return ABORT;
	}
	PlayerMoveSecound(b, PosibleMove, C_Value);
	return 1;
}

int FirstMove(struct board_t *b, int* SecoundCube)
{
	C_Line(1);
	int PosibleMove[PLAYERS][(PAWNS + 1)];
	int attack[PLAYERS];

	attack[P1] = MovePosibility(b, PosibleMove[P1], (*b).Cube[P1]);
	attack[P2] = MovePosibility(b, PosibleMove[P2], (*b).Cube[P2]);

	if ((PosibleMove[P1][0] == ABORT) && (PosibleMove[P2][0] == ABORT))
	{
		EndTurn();
		return ABORT;
	}
	AttackPosibility(b, PosibleMove[P1], PosibleMove[P2], attack);
	*SecoundCube=PlayerMoveFirst(b, PosibleMove);
	return 1;
}

int SecoundBarMove(struct board_t *b, int C_Value)
{
	C_Line(1);
	int PosibleBarMove[PLAYERS];
	BarMovePosibility(b, PosibleBarMove, C_Value);
	if (PosibleBarMove[0] == ABORT)
	{
		EndTurn();
		return ABORT;
	}
	BarMoveCounting(b, PosibleBarMove, C_Value);
	Wait();
	C_Line(1);
	return 1;
}

int FirstBarMove(struct board_t *b, int* SecoundCube)
{
	C_Line(1);
	int PosibleBarMove[PLAYERS][PLAYERS];
	int attack[PLAYERS];

	attack[P1] = BarMovePosibility(b, PosibleBarMove[P1], (*b).Cube[P1]);
	attack[P2] = BarMovePosibility(b, PosibleBarMove[P2], (*b).Cube[P2]);

	if ((PosibleBarMove[P1][0] == ABORT) && (PosibleBarMove[P2][0] == ABORT))
	{
		EndTurn();
		return ABORT;
	}
	AttackPosibility(b, PosibleBarMove[P1], PosibleBarMove[P2], attack);
	*SecoundCube = PlayerBarMove(b, PosibleBarMove);
	return 1;
}

void SecoundHomeMove(struct board_t *b, int HomeField, int Direction, int C_Value, int* Status)
{
	int PosibleHomeMove[(PIECE_BOARD + 1)];
	HomeMovePosibility(b, HomeField, Direction, PosibleHomeMove, C_Value);
	if (PosibleHomeMove[0] == ABORT)
	{
		*Status = SecoundMove(b, C_Value);
		return;
	}
	PlayerHomeMoveSecound(b, PosibleHomeMove , C_Value);
}

void FirstHomeMove(struct board_t *b, int HomeField, int Direction, int* SecoundCube, int* Status)
{
	int PosibleHomeMove[PLAYERS][(PIECE_BOARD+1)];
	int attack[PLAYERS];

	attack[P1] = HomeMovePosibility(b, HomeField, Direction, PosibleHomeMove[P1], (*b).Cube[P1]);
	attack[P2] = HomeMovePosibility(b, HomeField, Direction, PosibleHomeMove[P2], (*b).Cube[P2]);

	if ((PosibleHomeMove[P1][0] == ABORT) && (PosibleHomeMove[P2][0] == ABORT))
	{
		*Status = FirstMove(b, SecoundCube);
		return;
	}
	AttackPosibility(b, PosibleHomeMove[P1], PosibleHomeMove[P2], attack);
	*SecoundCube = PlayerHomeMoveFirst(b, PosibleHomeMove);
}


void TwoCubes(struct board_t *b, int* SecoundCube, int* Status)
{
	int InHome;
	int HomeField; 
	int Direction;
	InHome = PawnsInHome(b, &HomeField, &Direction);
	if ((*b).Bar[(*b).M_Player] != 0)
	{
		*Status = FirstBarMove(b, SecoundCube);
	}
	else if (InHome == 1)
	{
		FirstHomeMove(b, HomeField, Direction, SecoundCube, Status);
	}
	else
	{
		*Status = FirstMove(b, SecoundCube);
	}
	EndGame(b, Status);
}

void OneCube(struct board_t *b, int C_Value, int* Status)
{
	int InHome;
	int HomeField;
	int Direction;
	InHome = PawnsInHome(b, &HomeField, &Direction);
	if ((*b).Bar[(*b).M_Player]!=0)
	{
		*Status = SecoundBarMove(b, C_Value);
	}
	else if (InHome==1)
	{
		SecoundHomeMove(b, HomeField, Direction, C_Value, Status);
	}
	else
	{
		*Status = SecoundMove(b, C_Value);
	}
	EndGame(b, Status);
}

int N_Move(struct board_t *b)
{
	int Status=1;
	int SecoundCube = P1;
	TwoCubes(b, &SecoundCube, &Status);
	if (Status == G_ABORT)
	{
		return G_ABORT;
	}
	if (Status == ABORT)
	{
		return (*b).S_Player;
	}
	S_Replay(b);

	OneCube(b, (*b).Cube[SecoundCube], &Status);
	if (Status == G_ABORT)
	{
		return G_ABORT;
	}
	if (Status == ABORT)
	{
		return (*b).S_Player;
	}
	S_Replay(b);

	return (*b).S_Player;
}

int D_Move(struct board_t *b)
{
	int Status=1;
	int SecoundCube = P1;
	int d=4;				//liczba ruchow kostka
	if ((*b).Bar[(*b).M_Player]!=0)
	{
		d=2;
	}
	for (int i = 1; i <= d; i++)
	{
		OneCube(b, (*b).Cube[SecoundCube], &Status);
		if (Status == G_ABORT)
		{
			return G_ABORT;
		}
		if (Status == ABORT)
		{
			return (*b).S_Player;
		}
		S_Replay(b);
	}
	return (*b).S_Player;
}

int Move(struct board_t *b)
{
	int Status;
	int SecoundCube = P1;
	if ((*b).Cube[P1] == (*b).Cube[P2])
	{
		Status=D_Move(b);
		return Status;
	}
	else
	{
		Status=N_Move(b);
		return Status;
	}
	return (*b).S_Player;
}

void C_Load(char* Command)
{
	printw("Podaj komende: ");
	refresh();
	scanw("%c",Command);
}

int M_Description()
{
	int i=B_Y0;
	mvprintw(i,B_X0,"Backamon by Kacper Petla 198186");
	i=i+2;
	mvprintw(i,B_X0,"Dostepne komendy:");
	i++;
	mvprintw(i,B_X0," N - Stworz nowa gre");
	i++;
	mvprintw(i,B_X0," L - Zaladuj gre");
	i++;
	mvprintw(i,B_X0," S - Zapisz gre");
	i++;
	mvprintw(i,B_X0," T - Wykonaj rzut kostka");
	i++;
	mvprintw(i,B_X0," R - Powtorki");
	i++;
	mvprintw(i,B_X0," F - Hall of Fame");
	i++;
	mvprintw(i,B_X0," E - EXIT");
	i=i+2;
	mvprintw(i,B_X0,"'S' i 'T' dostepne po zaladowaniu planszy");
	i=i+2;
	return i;
}

void Menu(struct board_t *b)
{
	while(1)
	{
		int i;
		i=M_Description();
		char Command;
		move(i,B_X0);
		clrtoeol();
		C_Load(&Command);
		erase();
		switch(Command)
		{
			case 'N':
				NewBoard(b);
				FirstThrow(b);
				return;
			break;
			case 'L':
				int Status;
				Status=LoadBoard(b);
				Wait();
				if (Status!=ABORT)
				{
					return;
				}
			break;
			case 'R':
				R_Menu(b);
			break;
			case 'F':
				FamePrint(b);
			break;
			case 'E':
				endwin();
				exit(0);
			break;
			default:
				move(i,B_X0);
				clrtoeol();
				printw("Niepoprawna komenda");
				Wait();
			break;
		}
	}
}

int G_Throw(struct board_t *b)
{
	Throw((*b).Cube);
	printw("	Kolej gracza Nr %d", (*b).M_Player+1);
	CubeChange(b);
	(*b).S_Player = (((*b).M_Player + 1) % PLAYERS);
	(*b).M_Player=Move(b);
	if ((*b).M_Player==G_ABORT)
	{
		return ABORT;
	}
	return 1;
}

void Game(struct board_t *b)
{
	while(1)
	{
		char Command;
		C_Line(0);
		C_Load(&Command);
		switch(Command)
		{
			case 'N':
				(*b).Points[P1]=0;
				(*b).Points[P2]=0;
				NewBoard(b);
				FirstThrow(b);
			break;
			case 'L':
				LoadBoard(b);
				Wait();
			break;
			case 'S':
				SaveBoard(b);
				Wait();
			break;
			case 'T':
				int Status;
				Status=G_Throw(b);
				if (Status == ABORT)
				{
					return;
				}
			break;
			case 'R':
				R_InGame(b);
			break;
			case 'F':
				FamePrint(b);
				PrintPoints((*b).Points);
				CreateBoard(b);
			break;
			case 'E':
				endwin();
				exit(0);
			break;
			default:
				C_Line(0);
				printw("Niepoprawna komenda");
				Wait();
			break;
		}

	}
}

int main()
{
	struct board_t b;
	srand(time(NULL));
	initscr();
	b.Points[P1]=0;
	b.Points[P2]=0;
	b.Turn=0;

	while (1)
	{
		Menu(&b);
		Game(&b);
		erase();
	}
	return 0;
}
