#include <iostream>
#include <cstdio>


using namespace std;

enum DataType{
	INT , CHAR , IF , MIN , MAX
};

struct onp {
	DataType type;
	int value;
	onp* next;
};

struct kolejka {
	onp* head;
	onp* tail;
};

void Put(kolejka& k, DataType type, int value)
{
	onp* nkolejka = new onp;
	nkolejka->type = type;
	nkolejka->value = value;
	nkolejka->next = nullptr;
	if (k.head == nullptr)
	{
		k.head = nkolejka;
		k.tail = nkolejka;
	}
	else
	{
		k.tail->next = nkolejka;
		k.tail = nkolejka;
	}
}

bool Get(kolejka& k, DataType* type, int* value)
{
	if (k.head != nullptr)
	{
		onp* tmp = k.head;
		*type = k.head->type;
		*value = k.head->value;
		k.head = k.head->next;
		delete tmp;
		if (k.head == nullptr)
		{
			k.tail = nullptr;
		}
		return true;
	}
	else 
	{
		return false;
	}
}

void Delete_k(kolejka& k)
{
	while (k.head != nullptr)
	{
		onp* tmp = k.head;
		k.head = k.head->next;
		delete tmp;
	}
	k.tail = nullptr;
}

struct stos_t
{
	char token;
	stos_t* next;
};

void Push_t(stos_t*& head_t, char token)
{
	stos_t* nstos = new stos_t;
	nstos->token = token;
	nstos->next = head_t;
	head_t = nstos;
}

char Pop_t(stos_t*& head_t)
{
	if (head_t == nullptr)
	{
		return NULL;
	}
	stos_t* tmp = head_t;
	char token = head_t->token;
	head_t = head_t->next;
	delete tmp;
	return token;
}

struct stos_v
{
	int value;
	stos_v* next;
};

void Push_v(stos_v*& head_v, int value)
{
	stos_v* nstos = new stos_v;
	nstos->value = value;
	nstos->next = head_v;
	head_v = nstos;
}

int Pop_v(stos_v*& head_v)
{
	if (head_v == nullptr)
	{
		return NULL;
	}
	stos_v* tmp = head_v;
	int value = head_v->value;
	head_v = head_v->next;
	delete tmp;
	return value;
}

void Print_s_v(stos_v*& head_v)
{
	stos_v* tmp = head_v;
	while (tmp != nullptr)
	{
		printf("%d ", tmp->value);
		tmp = tmp->next;
	}
}

void Delete_s_v(stos_v*& head_v)
{
	while (head_v != nullptr)
	{
		stos_v* tmp = head_v;
		head_v = head_v->next;
		delete tmp;
	}
}

int CreateInt(stos_t*& head, int* number)
{
	char token;
	int value=0;
	int order=1;
	for (*number; *number > 0; *number = *number - 1)
	{
		token=Pop_t(head);
		value = value + (atoi(&token) * order);
		order = order * 10;
	}
	return value;
}

int Priority(char token)
{
	if (token == '+' || token == '-')
	{
		return 1;
	}
	else if (token == '*' || token == '/')
	{
		return 2;
	}
	else
	{
		return 0;
	}
}

bool IsNumber(char token)
{
	return (token >= '0' && token <= '9');
}

bool IsOperator(char token)
{
	return (token == '+' || token == '-' || token == '*' || token == '/');
}

void IsNegation(kolejka& k, stos_t*& head_t)
{
	char token = head_t->token;
	while (token == 'N')
	{
		Put(k, CHAR, token);
		printf("%c ", token);
		Pop_t(head_t);
		if (head_t == nullptr)
		{
			break;
		}
		token = head_t->token;
	}
}

bool ONP(kolejka& k)
{
	stos_t* head_t = nullptr;
	char token;
	int number = 0;
	int bracket = 0;
	int order = 0;
	int value;
	bool end = true;
	while (true)
	{
		cin >> token;
		if (IsNumber(token))
		{
			Push_t(head_t, token);
			number++;
		}
		else if (number != 0)
		{
			value = CreateInt(head_t, &number);
			Put(k, INT, value);
			printf("%d ", value);
			order++;
		}

		if (token == '(')
		{
			bracket++;
			Push_t(head_t, token);
		}
		else if (token == ')' && bracket > 0)
		{
			bracket--;
			while (true)
			{
				token = Pop_t(head_t);
				if (token == '(')
				{
					break;
				}
				else
				{
					Put(k, CHAR, token);
					printf("%c ", token);
				}
			}
		}
		else if (token == 'N')
		{
			Push_t(head_t, token);
		}
		else if (token == 'I')
		{
			order++;
			cin >> token; cin >> token; // F (
			for (int i = 0; i < 3; i++)
			{
				ONP(k);
			}
			Put(k, IF, 'I');
			printf("IF ");
		}
		else if (token == 'M')
		{
			order++;
			char type;
			int size = 0;
			bool end_in = false;
			cin >> token; cin >> type; cin >> token ; // I/A N/X (
			while (!end_in)
			{
				end_in=ONP(k);
				size++;
			}
			if (type == 'N')
			{
				Put(k, MIN, size);
				printf("MIN%d ", size);
			}
			else
			{
				Put(k, MAX, size);
				printf("MAX%d ", size);
			}
		}
		else if (IsOperator(token))
		{
			if (head_t != nullptr && head_t->token == 'N')
			{
				IsNegation(k, head_t);
			}

			while (true)
			{
				if (order > 1 && head_t != nullptr)
				{
					char token2 = head_t->token;
					if (Priority(token2) >= Priority(token))
					{
						Put(k, CHAR, token2);
						printf("%c ", token2);
						Pop_t(head_t);
					}
					else
					{
						Push_t(head_t, token);
						break;
					}
				}
				else
				{
					Push_t(head_t, token);
					break;
				}
			}
		}
		else if (token == '.' || (token == ')' && bracket == 0))
		{
			break;
		}
		else if (token == ',')
		{
			end = false;
			break;
		}

	}
	while (head_t != nullptr)
	{
		token = Pop_t(head_t);
		Put(k, CHAR, token);
		printf("%c ", token);
	}
	if (end)
	{
		return true;
	}
	return false;
}

void C_MAX(stos_v*& head_v, int value)
{
	printf("MAX%d ", value);
	Print_s_v(head_v);
	printf("\n");
	int v1 = Pop_v(head_v);
	for (int i = 1; i < value; i++)
	{
		int v2 = Pop_v(head_v);
		if (v2 > v1)
		{
			v1 = v2;
		}
	}
	Push_v(head_v, v1);
}

void C_MIN(stos_v*& head_v, int value)
{
	printf("MIN%d ", value);
	Print_s_v(head_v);
	printf("\n");
	int v1 = Pop_v(head_v);
	for (int i = 1; i < value; i++)
	{
		int v2 = Pop_v(head_v);;
		if (v2 < v1)
		{
			v1 = v2;
		}
	}
	Push_v(head_v, v1);
}

void C_IF(stos_v*& head_v)
{
	int v1, v2, v3;
	v3 = Pop_v(head_v);
	v2 = Pop_v(head_v);
	v1 = Pop_v(head_v);
	printf("IF %d %d %d ", v3, v2, v1);
	Print_s_v(head_v);
	printf("\n");
	if (v1 > 0)
	{
		Push_v(head_v, v2);
	}
	else
	{
		Push_v(head_v, v3);
	}
}

bool Operation(char o, int v1, int v2, int* v3)
{
	if (o == '+')
	{
		*v3 = v1 + v2;
	}
	else if (o == '-')
	{
		*v3 = v1 - v2;
	}
	else if (o == '*')
	{
		*v3 = v1 * v2;
	}
	else if (o == '/')
	{
		if (v2 == 0)
		{
			return false;
		}
		*v3 = v1 / v2;
	}
	return true;;
}

bool C_CHAR(stos_v*& head_v, kolejka& k, char oper)
{
	if (oper == 'N')
	{
		int v1;
		v1 = Pop_v(head_v);
		printf("N %d ", v1);
		Print_s_v(head_v);
		printf("\n");
		Push_v(head_v, -v1);
	}
	else
	{
		int v1, v2, v3;
		v2 = Pop_v(head_v);
		v1 = Pop_v(head_v);
		printf("%c %d %d ", oper, v2, v1);
		Print_s_v(head_v);
		printf("\n");
		if (!Operation(oper, v1, v2, &v3))
		{
			printf("ERROR\n");
			Delete_k(k);
			Delete_s_v(head_v);
			return false;
		}
		Push_v(head_v, v3);
	}
	return true;
}

void Calculate(kolejka& k)
{
	int value;
	stos_v* head_v = nullptr;
	DataType type;
	while (Get(k, &type, &value))
	{
		if (type == INT)
		{
			Push_v(head_v, value);
		}
		else if (type == MAX)
		{
			C_MAX(head_v, value);
		}
		else if (type == MIN)
		{
			C_MIN(head_v, value);
		}
		else if (type == IF)
		{
			C_IF(head_v);
		}
		else if (type == CHAR)
		{
			if (!C_CHAR(head_v, k, value))
			{
				return;
			}
		}
	}
	value = Pop_v(head_v);
	printf("%d\n", value);
}

int main()
{
	//ios_base::sync_with_stdio(false);
	//cin.tie(NULL);

	kolejka k;
	int i;
	k.head = nullptr;
	k.tail = nullptr;
	cin >> i;
	for (i; i > 0; i--)
	{
		ONP(k);
		printf("\n");
		Calculate(k);
	}
	return 0;
}
