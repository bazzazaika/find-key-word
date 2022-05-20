/*

FIND KEY WORDS FROM .c FILE 
USING THE OPTIMAL SEARCH TREE

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define N 32

typedef struct tree {
	int count;
	char* word;
	struct tree* left, * right;
}TR;


char table[32][9];
int p[N+1] = { 
	1	,	//auto     
	21	,	//break	
	3	,	//case	
	58	,	//char	
	9	,	//const	
	11	,	//continue
	1 	,	//default	
	10 	,	//do		
	19 	,	//double	
	95 	,	//else	
	4 	,	//enum	
	1	,	//extern	
	16  ,	//float 	
	59  ,	//for	    
	21 	,	//goto	
	345 ,	//if		
	239 ,	//int		
	56  ,	//long	
	1 	,	//register
	116 ,	//return	
	10 	,	//short	
	1 	,	//signed	
	34 	,	//sizeof	
	21 	,	//static	
	115 ,	//struct	
	1	,	//switch	
	18 	,	//typedef	
	3 	,	//union 	
	84 	,	//unsigned
	53 	,	//void	
	1 	,	//volatile
	63  ,	//while
	0   ,
};				  

int W[N+1][N+1];
int R[N+1][N+1];
int C[N+1][N+1] = { 0 }; // C === P
int q[N+1] = { 0 };


//Auxiliary functions

void fill_key_word();

void process(TR* root, char* str);


// TREE

void optTree()
{
	int x, min;
	int i, j, k, h, m;
	
	//algoritm to build optimal search tree from Wirth 
	for (i = 0; i <= N;i++)
	{
		W[i][i] = q[i];
		for (j = i+1; j <= N;j++)
			W[i][j] = W[i][j - 1] + p[i];
	}

	for (i = 0;i < N;i++)
		C[i][i] = W[i][i];

	for (i = 0; i <= N - 1; i++)
	{
		j = i + 1;
		C[i][j] = W[i][j];
		R[i][j] = j;
	}
	for (h = 2; h <= N; h++)
	{
		for (i = 0; i <= N - h; i++)
		{
			j = i + h;
			m = R[i][j - 1];
			min = C[i][m - 1] + C[m][j];
			for (k = m + 1; k <= R[i + 1][j]; k++)
			{
				x = C[i][k - 1] + C[k][j];
				if (x < min)
				{
					m = k;
					min = x;
				}
			}
			C[i][j] = min + W[i][j];
			R[i][j] = m;
		}
	}
}

TR* crtTree(int i, int j)
{
	TR* p;
	if (i == j)
		p = NULL;
	else
	{
		p = (TR*)malloc(sizeof(TR));
		p->word = table[R[i][j] - 1];
		p->count = 0;
		p->left = crtTree(i, R[i][j] - 1);
		p->right = crtTree(R[i][j], j);
	}
	return p;
}

void PRINT(TR* root, int level)
{
	int i;
	if (root != NULL)
	{
		PRINT(root->left, level + 1);
		for (i = 0; i < level;i++)
		{
			printf("     ");
		}
		printf("%s\n", root->word);
		PRINT(root->right, level + 1);
	}
}

void PRINT_count(TR* root)
{
	if (root != NULL)
	{
		PRINT_count(root->left);
		if (root->count != 0)
		{
			printf("%s = ", root->word);
			printf("%d", root->count);
			printf("\n");
		}
		PRINT_count(root->right);
	}
}
void PRT_R()
{
	int i, j;
	for (i = 0; i <= N; i++)
	{
		for (j = 0; j <= N; j++)
			printf("%.2d ", R[i][j]);
		printf("\n");
	}
	printf("\n");
}



int main()
{
	system("chcp 1251 > nul");
	fill_key_word();
	optTree();
	TR* root;
	root = crtTree(0, N);
	//PRT_R();


	PRINT(root, 0);
	//file that will open
	FILE* f = fopen("2.c", "rb"); 

	int c;
	char str[100];
	int i = 0;
	int hs;
	int fl_kav = 1;
	int fl_com = 1;
	int fl_od_kav = 1;
	while ((c = fgetc(f)) != EOF)
	{
		if (fl_kav && fl_com && fl_od_kav)
		{
			if (c != '(' && c != ' ' && c != '\n' && c != '=' && c != '[' && c != ')' && c != ']'
				&& c != ';' && c != '\t' && c != ',' && c != '*' && c != '+' && c != '-' && c != '='
				&& c != '&' && c != '|' && c != '\"' &&  c != '/' && c != '{' && c != '}' && c != '\'' && c != '\r')
			{
				str[i] = c;
				i++;
			}
			else
			{
				str[i] = 0;
				if (str[0] != 0)
					process(root, str);
				str[0] = 0;
				i = 0;
				if (c == '\"')
					fl_kav = 0;
				if (c == '/')
					fl_com = 0;
				if (c == '\'')
					fl_od_kav = 0;
			}
		}
		else if (fl_kav == 0)
		{
			if (c == '\\')
			{
				c = fgetc(f);
				c = fgetc(f);
			}
			if (c == '\"')
				fl_kav = 1;
		}
		else if (fl_od_kav == 0)
		{
			c = fgetc(f);
			if (c == '\\')
				c = fgetc(f);
			c = fgetc(f);
			fl_od_kav = 1;
			
		}
		else
		{
			//c = fgetc(f);
			if (c == '/')
			{
				while (c != '\n' && c != EOF)
				{
					c = fgetc(f);
					if (c == '\\')
					{
						c == fgetc(f);
						if (c == '\n')
						{
							c = fgetc(f);
						}
						else
						{
							continue;
						}
					}
				}
				fl_com = 1;
			}
			else if (c == '*')
			{
				c = fgetc(f);
				while (c != '*' && fl_com == 0)
				{
					c = fgetc(f);
					while (c == '*')
					{
						c = fgetc(f);
					}
					if (c == '/')
					{
						fl_com = 1;
						break;
					}
				}
			}
		}
	}
	str[i] = 0;
	if (str[0] != 0)
		process(root, str);
	PRINT_count(root);

}

void fill_key_word()
{
	strcpy(table[0], "auto");
	strcpy(table[1], "break");
	strcpy(table[2], "case");
	strcpy(table[3], "char");
	strcpy(table[4], "const");
	strcpy(table[5], "continue");
	strcpy(table[6], "default");
	strcpy(table[7], "do");
	strcpy(table[8], "double");
	strcpy(table[9], "else");
	strcpy(table[10], "enum");
	strcpy(table[11], "extern");
	strcpy(table[12], "float"); 
	strcpy(table[13], "for");
	strcpy(table[14], "goto");
	strcpy(table[15], "if");
	strcpy(table[16], "int");
	strcpy(table[17], "long");
	strcpy(table[18], "register");
	strcpy(table[19], "return");
	strcpy(table[20], "short");
	strcpy(table[21], "signed");
	strcpy(table[22], "sizeof");
	strcpy(table[23], "static");
	strcpy(table[24], "struct");
	strcpy(table[25], "switch");
	strcpy(table[26], "typedef");
	strcpy(table[27], "union");
	strcpy(table[28], "unsigned");
	strcpy(table[29], "void");
	strcpy(table[30], "volatile");
	strcpy(table[31], "while");
}

void process(TR* root, char* str)
{
	int j = 0;
	int k = 0;
	TR* p = root;
	while (p != NULL)
	{
		if (strcmp(p->word, str) == 0)
		{
			p->count++;
			break;
		}
		else if (strcmp(p->word, str) < 0)
			p = p->right;
		else
			p = p->left;

	}
}
