#include "jna.h"


/* initiallize Matrix */
// Matrix[i][j] = 0
int MatrixReset(float** Matrix, int m, int n)
{
	int ret = 0;
	int i, j;
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			Matrix[i][j] = 0;
		}
	}
	return ret;
}


float **MatrixCreate(int m, int n)
{
	int i;
	int bytes;
	float **Matrix;
	bytes = sizeof(float *)* m;
	//pBuf = (u8*)pvPortMalloc(BytesOfData);
	//mallocCount++;
#ifdef WIN32

	Matrix = (float **)malloc(bytes);
#else
	Matrix = (float **)malloc(bytes);
#endif

	//int size = sizeof(float);

	for (i = 0; i < m; i++)
	{

#ifdef WIN32

		Matrix[i] = (float *)malloc(sizeof(float)* n);

#else
		Matrix[i] = (float *)malloc(sizeof(float)* n);
#endif
	}
	return Matrix;
}

/* free */
int MatrixFree(float** Matrix, int m)
{
	int ret = 0;
	int i;
	for (i = 0; i < m; i++)
	{
#ifdef WIN32
		//qDebug() << "Matrix[i]:" << Matrix[i];
		if (Matrix != NULL)
			free(Matrix[i]);

#else
		if (Matrix != NULL)
			free(Matrix[i]);
#endif
	}

	//freeCount ++;
#ifdef WIN32
	//qDebug() << "Matrix:" << Matrix;
	if (Matrix != NULL)
		free(Matrix);

#else
	if (Matrix != NULL)
		free(Matrix[i]);
#endif
	return ret;
}

/* Matrix transpose
input a output b
*/

int  MatrixTranspose(float** a, float** b, int row_a, int col_a)
{
	int i, j;
	int ret = 0;
	for (i = 0; i < col_a; i++)
	{
		for (j = 0; j < row_a; j++)
		{

			b[i][j] = a[j][i];

		}
	}
	return ret;
}

//Matrix multiply
//para: c = a*b
//return:
//-------------------------------------------------------------------

int  MatrixMutiply(float** a, float** b, float** c, int row_a, int col_a, int col_b)
{
	int ret = 0;
	MatrixReset(c, row_a, col_b);

	int i, j, k;
	for (i = 0; i < row_a; i++)
	{
		for (j = 0; j < col_b; j++)
		{
			for (k = 0; k < col_a; k++)
			{
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}

	return ret;
}
/*
Matrix inversion
input a output b
*/
int  MatrixInv_Gauss(float** A, float** B, int n)
{
	int ret = 0;
	int i, j, k;
	float max, temp;
	float ** t;                //temporally Matrix
	t = MatrixCreate(n, n);
	if (t == 0)
		return 2;

	//stort A[] to t[n][n]
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			t[i][j] = A[i][j];
		}
	}
	//init B[]
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
		{
			B[i][j] = (i == j) ? (float)1 : 0;
		}
	}
	for (i = 0; i < n; i++)
	{
		//
		max = t[i][i];
		k = i;
		for (j = i + 1; j < n; j++)
		{
			if (fabs(t[j][i]) > fabs(max))
			{
				max = t[j][i];
				k = j;
			}
		}
		//
		if (k != i)
		{
			for (j = 0; j < n; j++)
			{
				temp = t[i][j];
				t[i][j] = t[k][j];
				t[k][j] = temp;
				//B
				temp = B[i][j];
				B[i][j] = B[k][j];
				B[k][j] = temp;
			}
		}
		//
		if (t[i][i] == 0)
		{
			//printf("Inv Matrix does not exist!!\n");
			//      qDebug()<<"Inv Matrix does not exist!!";
			//qDebug() << "t[i][i] == 0 ++ ";
			MatrixFree(t, n);
			//qDebug() << "t[i][i] == 0 --";
			return 2;
		}
		//
		temp = t[i][i];
		for (j = 0; j < n; j++)
		{
			t[i][j] = t[i][j] / temp;        //
			B[i][j] = B[i][j] / temp;        //
		}
		for (j = 0; j < n; j++)        //
		{
			if (j != i)                //
			{
				temp = t[j][i];
				for (k = 0; k < n; k++)        //
				{
					t[j][k] = t[j][k] - t[i][k] * temp;
					B[j][k] = B[j][k] - B[i][k] * temp;
				}
			}
		}
	}
	//qDebug() << "before ret |= Atom_MatrixFree(t, n) ++";
	ret |= MatrixFree(t, n);
	//qDebug() << "after ret |= Atom_MatrixFree(t, n) --";
	return ret;
}

uint8_t FindMinIndex(int* buffer, uint8_t len)
{
	int tmp = 0, i = 0, min = 0, index = 0;
	tmp = *buffer;
	min = 999999;
	for (i = 0; i<len; i++)
	{
		tmp = *(buffer + i);
		if (tmp<min)
		{
			min = tmp;
			index = i;
		}
	}
	return index;
}

#define filter_para 0.7

double LP(double tmp, uint8_t channel)
{
	static double ld[1024] = { 0 };
	static uint8_t first[1024] = { 1 };

	double data = 0;
	if (first[channel])
	{
		first[channel] = 0;
		ld[channel] = tmp;
	}
	data = filter_para*ld[channel] + (1 - filter_para)*tmp;
	ld[channel] = data;
	return data;
}
//00000000000000000000000000000000000000000000000000000
bool get_position(float *an_x, float *an_y, float *an_z, float *dis, int len, int id, float *x, float *y, float *z)
{
	int tmp[10] = { 0 }, min_index1 = 0, min_index2 = 0;
	if (len <2) return 0;

	if (len>2){
		for (int i = 2; i<len; i++){
			memcpy(tmp, dis, sizeof(tmp));
			min_index1 = FindMinIndex(tmp, len);
			tmp[min_index1] = 99999;
			min_index2 = FindMinIndex(tmp, len);
		}
	}
	else{
		min_index1 = 0;
		min_index2 = 1;
	}

	float distA = dis[min_index1] / 100;
	float distB = dis[min_index2] / 100;

	float tempF = fabs((*z) - an_z[min_index1]);

	if (distA >tempF) distA = sqrt(pow(distA, 2) - pow(tempF, 2));

	tempF = abs((*z) - an_z[min_index2]);
	if (distB>tempF) distB = sqrt(pow(distB, 2) - pow(tempF, 2));

	int indexA = min_index1;
	int indexB = min_index2;


	float **tempA = MatrixCreate(2, 3);//n*3
	float **tempB = MatrixCreate(2, 1);//n*1
	if ((tempB == NULL) || (tempA == NULL))
	{
		MatrixFree(tempA, 2);
		MatrixFree(tempB, 2);
		return 0;
	}

	if ((tempA == NULL) || (tempB == NULL))
	{
		return 0;
	}
	tempA[0][0] = 2 * (an_x[indexA] - an_x[indexB]);
	tempA[0][1] = 2 * (an_y[indexA] - an_y[indexB]);

	float line_a, line_b;
	if (fabs(an_y[indexB] - an_y[indexA]) < 0.01){
		line_a = 0;
		line_b = an_y[indexB];
		tempA[1][0] = -line_a;
		tempA[1][1] = 1;

		tempB[0][0] = pow(an_x[indexA], 2.0f) - pow(an_x[indexB], 2.0f) + pow(an_y[indexA], 2.0f) - pow(an_y[indexB], 2.0f) + pow(distB, 2.0f) - pow(distA, 2.0f);
		tempB[1][0] = line_b;
	}
	else if (fabs(an_x[indexB] - an_x[indexA]) <0.01){
		tempA[1][0] = 1;
		tempA[1][1] = 0;
		tempB[0][0] = pow(an_x[indexA], 2.0f) - pow(an_x[indexB], 2.0f) + pow(an_y[indexA], 2.0f) - pow(an_y[indexB], 2.0f) + pow(distB, 2.0f) - pow(distA, 2.0f);
		tempB[1][0] = an_x[indexB];
	}
	else{
		line_a = (an_y[indexB] - an_y[indexA]) / (an_x[indexB] - an_x[indexA]);
		line_b = an_y[indexA] - line_a*an_x[indexA];
		tempA[1][0] = -line_a;
		tempA[1][1] = 1;

		tempB[0][0] = pow(an_x[indexA], 2.0f) - pow(an_x[indexB], 2.0f) + pow(an_y[indexA], 2.0f) - pow(an_y[indexB], 2.0f) + pow(distB, 2.0f) - pow(distA, 2.0f);
		tempB[1][0] = line_b;
	}

	float** Temp1 = MatrixCreate(2, 2);//AµÄ×ªÖÃ¾ØÕó
	float** Temp2 = MatrixCreate(2, 2);//(AT*A)
	float** Temp3 = MatrixCreate(2, 2);//inv
	float** Temp4 = MatrixCreate(2, 2);//inv * AT
	float** Temp5 = MatrixCreate(2, 1);//inv

	if ((Temp1 == NULL) || (Temp2 == NULL) || (Temp3 == NULL) || (Temp4 == NULL) || (Temp5 == NULL))
	{
		MatrixFree(Temp1, 2);
		MatrixFree(Temp2, 2);
		MatrixFree(Temp3, 2);
		MatrixFree(Temp4, 2);
		MatrixFree(Temp5, 2);
		return 0;
	}
	MatrixTranspose(tempA, Temp1, 2, 2);//Temp1 = (A_M )T    (2*n-1 )
	MatrixMutiply(Temp1, tempA, Temp2, 2, 2, 2);//Temp2 = Temp1*A_M
	MatrixInv_Gauss(Temp2, Temp3, 2);//Temp3 = inv temp2
	MatrixMutiply(Temp3, Temp1, Temp4, 2, 2, 2);//inv * AT  Temp4 = Temp3*Temp1
	MatrixMutiply(Temp4, tempB, Temp5, 2, 2, 1);//Temp5 = Temp4*Temp5

	*x = Temp5[0][0];
	*y = Temp5[1][0];
	*x = LP(*x, id * 2);
	*y = LP(*x, id * 2 + 1);

	MatrixFree(Temp1, 2);
	MatrixFree(Temp2, 2);
	MatrixFree(Temp3, 2);
	MatrixFree(Temp4, 2);
	MatrixFree(Temp5, 2);
	return 1;
}
