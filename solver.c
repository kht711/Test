#include <stdio.h>
#include <stdlib.h>
#define ROW 5
#define COL 4
#define LIMIT 80

struct depth{
	int p[20];
};


int puzzle[ROW][COL] = {
	{0, -1, -1, -1},
	{1, 2, 3, 4},
	{5, 6, 7, 8},
	{9, 10, 11, 12},
	{-1, -1, -1, 0}
};

int dump[ROW][COL] = {
	{-1, -1, -1, -1},
	{-1, -1, -1, -1},
	{-1, -1, -1, -1},
	{-1, -1, -1, -1},
	{-1, -1, -1, -1},
};

int sol[ROW][COL] = {
	{0, -1, -1, -1},
	{12, 11, 10, 9},
	{8, 7, 6, 5},
	{4, 3, 2, 1},
	{-1, -1, -1, 0}
};

/*
int sol[ROW][COL] = {
	{1, -1, -1, -1},
	{2, 0, 4, 8},
	{5, 0, 12, 11},
	{9, 3, 6, 10},
	{-1, -1, -1, 7}
};

int f[ROW][COL] = {
	{1, -1, -1, -1},
	{5, 2, 3,8},
	{9, 6, 4, 0},
	{10, 11, 0, 7},
	{-1, -1, -1, 12}
};
*/

int f[ROW][COL] = {
	{1, -1, -1, -1},
	{2, 3, 4, 8},
	{5, 0, 12, 11},
	{9, 0, 6, 10},
	{-1, -1, -1, 7}
};

int seek = 0;
int num = -1;
int minDist = 50;
int maxCor = 0;
int intoFlag = 0;
const int seekLimit = LIMIT;

struct depth d[LIMIT+1];

void deepCopy(int x[][COL], int* p){
	int i, j;

	for (i = 0; i < ROW; i++){
		for (j = 0; j < COL; j++){
			p[i*COL + j] = x[i][j];
		}
	}
	
	return;
}

void printPuzzle(int* x){
	int i, j;
	FILE *fp = fopen("result.txt", "a");

	for (i = 0; i < ROW; i++){
		for (j = 0; j < COL; j++){
			printf("%d ", x[i*COL + j]);
			fprintf(fp, "%d ", x[i*COL + j]); 
		}
		printf("\n");
		fprintf(fp, "\n");
	}
	printf("\n");
	fprintf(fp, "\n");

	fclose(fp);
}

int isSol(int x[][COL]){
	int i, j;

	for (i = 0; i < ROW; i++){
		for (j = 0; j < COL; j++){
			if (x[i][j] != sol[i][j]){
				return 0;
			}
		}
	}
	return 1;
}

int isFind(int x[][COL]){
	int i, j;

	for (i = 0; i < ROW; i++){
		for (j = 0; j < COL; j++){
			if (x[i][j] != f[i][j]){
				return 0;
			}
		}
	}
	return 1;
}

int isNotInList(int x[][COL]){
	int i, j, k;
	int flag = 0;

	for (i = 0; i < seekLimit; i++){
		if (d[i].p[0] == -1){
			return 1;
		}

		//equal flag. 1 is equal.
		flag = 1;
		for (j = 0; j < ROW; j++){
			for (k = 0; k < COL; k++){
				if (d[i].p[j*COL + k] != x[j][k]){
					flag = 0;
					break;
				}
			}
			if (flag == 0){
				break;
			}
		}

		if (flag == 1){
			return 0;
		}
	}

	return 1;
}

int distance(int* x){
	int i, j, num;
	int dist = 0;
	int solxy[12][2] = {
		{3, 3}, {3, 2}, {3, 1}, {3, 0},
		{2, 3}, {2, 2}, {2, 1}, {2, 0},
		{1, 3}, {1, 2}, {1, 1}, {1, 0}
	};

	for (i = 0; i < ROW; i++){
		for (j = 0; j < COL; j++){
			num = x[i*COL + j];
			if (num > 0){
				dist += (abs(i-solxy[num-1][0]) + abs(j-solxy[num-1][1]));
			}
		}
	}
	return dist;
}

int correct(int* x){
	int i, j;
	int cor = 0;

	for (i = 0; i < ROW; i++){
		for (j = 0; j < COL; j++){
			if (x[i*COL + j] == sol[i][j]){
				cor++;
			}
		}
	}
	return cor;
}

void minWrite(int dist, int cor, int mode){
	int j, k;
	FILE *fp;
	if (mode == 0){
		fp = fopen("min.txt", "w");
	}else{
		fp = fopen("min.txt", "a");
	}
	fprintf(fp, "%d, %d\n", dist, cor);

	for (j = 0; j < ROW; j++){
		for (k = 0; k < COL; k++){
			fprintf(fp, "%d ", d[LIMIT].p[j*COL + k]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");

	fclose(fp);
}

void moveTile(){
	int i, j, k;
	int dist, cor;
	int flag = 0;

	if (intoFlag == 0){
		//first seek
		for(i = 0; i < ROW; i++){
			for (j = 0; j < COL; j++){
				if(puzzle[i][j] > 0){
					num = puzzle[i][j];
					/////
					if (i-1 >= 0 && puzzle[i-1][j] == 0){
						intoFlag = 1;
						puzzle[i-1][j] = puzzle[i][j];
						puzzle[i][j] = 0;
						i = i-1;

						if (isNotInList(puzzle)){
							seek++;
							deepCopy(puzzle, d[seek].p);

							if (isSol(puzzle)){
								printf("Find!\n");
								for (k = 0; k <= seekLimit; k++){
									printPuzzle(d[k].p);
								}
								exit(0);
							}

							if (seek >= seekLimit){
								//
								dist = distance(d[seek].p);
								cor = correct(d[seek].p);

								if (dist < minDist){
									minWrite(dist, cor, 0);
									minDist = dist;
								}else if (dist == minDist){
									if (cor > maxCor){
										minWrite(dist, cor, 0);
										maxCor = cor;
									}else if (cor == maxCor){
										minWrite(dist, cor, 1);
									}
								}
								//

								deepCopy(dump, d[seek].p);

								seek--;
								puzzle[i+1][j] = puzzle[i][j];
								puzzle[i][j] = 0;
								i = i+1;
								intoFlag = 0;
							}else{
								///
								///
								while(intoFlag){
									moveTile();
								}
								moveTile();
								deepCopy(dump, d[seek].p);

								seek--;
								puzzle[i+1][j] = puzzle[i][j];
								puzzle[i][j] = 0;
								i = i+1;
								intoFlag = 0;
							}
						}else{
							puzzle[i+1][j] = puzzle[i][j];
							puzzle[i][j] = 0;
							i = i+1;
							intoFlag = 0;
						}
					}


					if (j-1 >= 0 && puzzle[i][j-1] == 0){
						intoFlag = 1;
						puzzle[i][j-1] = puzzle[i][j];
						puzzle[i][j] = 0;
						j = j-1;

						if (isNotInList(puzzle)){
							seek++;
							deepCopy(puzzle, d[seek].p);

							if (isSol(puzzle)){
								printf("Find!\n");
								for (k = 0; k <= seekLimit; k++){
									printPuzzle(d[k].p);
								}
								exit(0);
							}

							if (seek >= seekLimit){
								//
								dist = distance(d[seek].p);
								cor = correct(d[seek].p);

								if (dist < minDist){
									minWrite(dist, cor, 0);
									minDist = dist;
								}else if (dist == minDist){
									if (cor > maxCor){
										minWrite(dist, cor, 0);
										maxCor = cor;
									}else if (cor == maxCor){
										minWrite(dist, cor, 1);
									}
								}
								//

								deepCopy(dump, d[seek].p);

								seek--;
								puzzle[i][j+1] = puzzle[i][j];
								puzzle[i][j] = 0;
								j = j+1;
								intoFlag = 0;
							}else{
								///
								///

								while(intoFlag){
									moveTile();
								}
								moveTile();
								deepCopy(dump, d[seek].p);

								seek--;
								puzzle[i][j+1] = puzzle[i][j];
								puzzle[i][j] = 0;
								j = j+1;
								intoFlag = 0;
							}
						}else{
							puzzle[i][j+1] = puzzle[i][j];
							puzzle[i][j] = 0;
							j = j+1;
							intoFlag = 0;
						}
					}


					if (i+1 <= ROW-1 && puzzle[i+1][j] == 0){
						intoFlag = 1;
						puzzle[i+1][j] = puzzle[i][j];
						puzzle[i][j] = 0;
						i = i+1;

						if (isNotInList(puzzle)){
							seek++;
							deepCopy(puzzle, d[seek].p);

							if (isSol(puzzle)){
								printf("Find!\n");
								for (k = 0; k <= seekLimit; k++){
									printPuzzle(d[k].p);
								}
								exit(0);
							}

							if (seek >= seekLimit){
								//
								dist = distance(d[seek].p);
								cor = correct(d[seek].p);

								if (dist < minDist){
									minWrite(dist, cor, 0);
									minDist = dist;
								}else if (dist == minDist){
									if (cor > maxCor){
										minWrite(dist, cor, 0);
										maxCor = cor;
									}else if (cor == maxCor){
										minWrite(dist, cor, 1);
									}
								}
								//
								deepCopy(dump, d[seek].p);

								seek--;
								puzzle[i-1][j] = puzzle[i][j];
								puzzle[i][j] = 0;
								i = i-1;
								intoFlag = 0;
							}else{
								///
								///

								while(intoFlag){
									moveTile();
								}
								moveTile();
								deepCopy(dump, d[seek].p);

								seek--;
								puzzle[i-1][j] = puzzle[i][j];
								puzzle[i][j] = 0;
								i = i-1;
								intoFlag = 0;
							}
						}else{
							puzzle[i-1][j] = puzzle[i][j];
							puzzle[i][j] = 0;
							i = i-1;
							intoFlag = 0;
						}
					}


					if (j+1 <= COL-1 && puzzle[i][j+1] == 0){
						intoFlag = 1;
						puzzle[i][j+1] = puzzle[i][j];
						puzzle[i][j] = 0;
						j = j+1;

						if (isNotInList(puzzle)){
							seek++;
							deepCopy(puzzle, d[seek].p);

							if (isSol(puzzle)){
								printf("Find!\n");
								for (k = 0; k <= seekLimit; k++){
									printPuzzle(d[k].p);
								}
								exit(0);
							}

							if (seek >= seekLimit){
								//
								dist = distance(d[seek].p);
								cor = correct(d[seek].p);

								if (dist < minDist){
									minWrite(dist, cor, 0);
									minDist = dist;
								}else if (dist == minDist){
									if (cor > maxCor){
										minWrite(dist, cor, 0);
										maxCor = cor;
									}else if (cor == maxCor){
										minWrite(dist, cor, 1);
									}
								}
								//
								deepCopy(dump, d[seek].p);

								seek--;
								puzzle[i][j-1] = puzzle[i][j];
								puzzle[i][j] = 0;
								j = j-1;
								intoFlag = 0;
							}else{
								///
								///

								while(intoFlag){
									moveTile();
								}
								moveTile();
								deepCopy(dump, d[seek].p);

								seek--;
								puzzle[i][j-1] = puzzle[i][j];
								puzzle[i][j] = 0;
								j = j-1;
								intoFlag = 0;
							}
						}else{
							puzzle[i][j-1] = puzzle[i][j];
							puzzle[i][j] = 0;
							j = j-1;
							intoFlag = 0;
						}
					}
					/////
				}
			}
		}
	}else{
		intoFlag = 0;
		//num -> i, j
		for(i = 0; i < ROW; i++){
			for (j = 0; j < COL; j++){
				if(puzzle[i][j] == num){
					flag = 1;
					num = puzzle[i][j];
					break;
				}
			}
			if (flag == 1){
				break;
			}
		}

		/////
		if (i-1 >= 0 && puzzle[i-1][j] == 0){
			intoFlag = 1;
			puzzle[i-1][j] = puzzle[i][j];
			puzzle[i][j] = 0;
			i = i-1;

			if (isNotInList(puzzle)){
				seek++;
				deepCopy(puzzle, d[seek].p);

				if (isSol(puzzle)){
					printf("Find!\n");
					for (k = 0; k <= seekLimit; k++){
						printPuzzle(d[k].p);
					}
					exit(0);
				}

				if (seek >= seekLimit){
					//
					dist = distance(d[seek].p);
					cor = correct(d[seek].p);

					if (dist < minDist){
						minWrite(dist, cor, 0);
						minDist = dist;
					}else if (dist == minDist){
						if (cor > maxCor){
							minWrite(dist, cor, 0);
							maxCor = cor;
						}else if (cor == maxCor){
							minWrite(dist, cor, 1);
						}
					}
					//
					deepCopy(dump, d[seek].p);

					seek--;
					puzzle[i+1][j] = puzzle[i][j];
					puzzle[i][j] = 0;
					i = i+1;
					intoFlag = 0;
				}else{
					///
					///

					moveTile();
					deepCopy(dump, d[seek].p);

					seek--;
					puzzle[i+1][j] = puzzle[i][j];
					puzzle[i][j] = 0;
					i = i+1;
					intoFlag = 0;
				}
			}else{
				puzzle[i+1][j] = puzzle[i][j];
				puzzle[i][j] = 0;
				i = i+1;
				intoFlag = 0;
			}
		}


		if (j-1 >= 0 && puzzle[i][j-1] == 0){
			intoFlag = 1;
			puzzle[i][j-1] = puzzle[i][j];
			puzzle[i][j] = 0;
			j = j-1;

			if (isNotInList(puzzle)){
				seek++;
				deepCopy(puzzle, d[seek].p);

				if (isSol(puzzle)){
					printf("Find!\n");
					for (k = 0; k <= seekLimit; k++){
						printPuzzle(d[k].p);
					}
					exit(0);
				}

				if (seek >= seekLimit){
					//
					dist = distance(d[seek].p);
					cor = correct(d[seek].p);

					if (dist < minDist){
						minWrite(dist, cor, 0);
						minDist = dist;
					}else if (dist == minDist){
						if (cor > maxCor){
							minWrite(dist, cor, 0);
							maxCor = cor;
						}else if (cor == maxCor){
							minWrite(dist, cor, 1);
						}
					}
					//
					deepCopy(dump, d[seek].p);

					seek--;
					puzzle[i][j+1] = puzzle[i][j];
					puzzle[i][j] = 0;
					j = j+1;
					intoFlag = 0;
				}else{
					///
					///

					moveTile();
					deepCopy(dump, d[seek].p);

					seek--;
					puzzle[i][j+1] = puzzle[i][j];
					puzzle[i][j] = 0;
					j = j+1;
					intoFlag = 0;
				}
			}else{
				puzzle[i][j+1] = puzzle[i][j];
				puzzle[i][j] = 0;
				j = j+1;
				intoFlag = 0;
			}
		}


		if (i+1 <= ROW-1 && puzzle[i+1][j] == 0){
			intoFlag = 1;
			puzzle[i+1][j] = puzzle[i][j];
			puzzle[i][j] = 0;
			i = i+1;

			if (isNotInList(puzzle)){
				seek++;
				deepCopy(puzzle, d[seek].p);

				if (isSol(puzzle)){
					printf("Find!\n");
					for (k = 0; k <= seekLimit; k++){
						printPuzzle(d[k].p);
					}
					exit(0);
				}

				if (seek >= seekLimit){
					//
					dist = distance(d[seek].p);
					cor = correct(d[seek].p);

					if (dist < minDist){
						minWrite(dist, cor, 0);
						minDist = dist;
					}else if (dist == minDist){
						if (cor > maxCor){
							minWrite(dist, cor, 0);
							maxCor = cor;
						}else if (cor == maxCor){
							minWrite(dist, cor, 1);
						}
					}
					//
					deepCopy(dump, d[seek].p);

					seek--;
					puzzle[i-1][j] = puzzle[i][j];
					puzzle[i][j] = 0;
					i = i-1;
					intoFlag = 0;
				}else{
					///
					///

					moveTile();
					deepCopy(dump, d[seek].p);

					seek--;
					puzzle[i-1][j] = puzzle[i][j];
					puzzle[i][j] = 0;
					i = i-1;
					intoFlag = 0;
				}
			}else{
				puzzle[i-1][j] = puzzle[i][j];
				puzzle[i][j] = 0;
				i = i-1;
				intoFlag = 0;
			}
		}


		if (j+1 <= COL-1 && puzzle[i][j+1] == 0){
			intoFlag = 1;
			puzzle[i][j+1] = puzzle[i][j];
			puzzle[i][j] = 0;
			j = j+1;

			if (isNotInList(puzzle)){
				seek++;
				deepCopy(puzzle, d[seek].p);

				if (isSol(puzzle)){
					printf("Find!\n");
					for (k = 0; k <= seekLimit; k++){
						printPuzzle(d[k].p);
					}
					exit(0);
				}

				if (seek >= seekLimit){
					//
					dist = distance(d[seek].p);
					cor = correct(d[seek].p);

					if (dist < minDist){
						minWrite(dist, cor, 0);
						minDist = dist;
					}else if (dist == minDist){
						if (cor > maxCor){
							minWrite(dist, cor, 0);
							maxCor = cor;
						}else if (cor == maxCor){
							minWrite(dist, cor, 1);
						}
					}
					//

					deepCopy(dump, d[seek].p);

					seek--;
					puzzle[i][j-1] = puzzle[i][j];
					puzzle[i][j] = 0;
					j = j-1;
					intoFlag = 0;
				}else{
					///
					///

					moveTile();
					deepCopy(dump, d[seek].p);

					seek--;
					puzzle[i][j-1] = puzzle[i][j];
					puzzle[i][j] = 0;
					j = j-1;
					intoFlag = 0;
				}
			}else{
				puzzle[i][j-1] = puzzle[i][j];
				puzzle[i][j] = 0;
				j = j-1;
				intoFlag = 0;
			}
		}
		/////
	}
}

void main(){
	int i;
	FILE *rfp, *rfp2;

	rfp = fopen("result2.txt", "a");
	fprintf(rfp, "Start!\n");
	fclose(rfp);

	deepCopy(puzzle, d[0].p);
	for (i = 1; i <= seekLimit; i++){
		deepCopy(dump, d[i].p);
	}
	moveTile();

	rfp2 = fopen("result2.txt", "a");
	fprintf(rfp2, "Not Found...\n");
	fclose(rfp2);
}