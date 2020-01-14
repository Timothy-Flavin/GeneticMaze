#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
using namespace std;
bool complete = false;
struct Goal {
	int x;
	int y;
};

struct Mouse {
	short* moves;
	short x;
	short y;
	short genomeLength;
	bool alive;
	Mouse() {

	}
	~Mouse() {
		//delete[] moves;
		//delete moves;
	}
};

void fillMap(bool[], const int, const int);
void initializeMouse(Mouse*, short, short, short, short);
void printMap(bool[], const int, const int, Mouse*, short);
void move(Mouse*, short, short, bool[], const int, const int, Goal);
bool miceAlive(Mouse*, short);
void makeFitness(double*,Mouse*, short, Goal);
double fitnessTotal(double*, short);
Mouse selectMouse(double*, double, short, Mouse*);
void crossOver(short, short*, short*, Mouse*, Mouse*);
void mutate(short*, short, short);
int getMostFit(double*, int);


int main() {
	srand(time(NULL));
	//useful constants to change the parameters of the algorithm
	const int WIDTH = 10, HEIGHT = 10, popSize = 30, startX = 1, startY = 8, numMoves = 15, mutationRate = 10, numRands=4;
	bool map[WIDTH*HEIGHT];
	double*fitnessArray = new double[popSize];
	double fitTot;
	//sets the goal location
	Goal goal;
	goal.x = 8;
	goal.y = 1;

	//mouse pointer initializing the population so it will be passed by reference into functions to edit it
	Mouse *population = new Mouse[popSize];
	for (int i = 0; i < popSize; i++) {
		initializeMouse(population, numMoves, startX, startY, i);
		//cout << population[i].moves[0];
	}
	//sets up the map bool array (only run one time)
	fillMap(map, WIDTH, HEIGHT);

	//prints the map without anything in it for use later
	//printMap(map, WIDTH, HEIGHT, goal);
	short generation = 0;

	//**************************************************************************************************************
	while (!complete) {

		for (short j = 0; j < numMoves; j++) {
			for (int i = 0; i < popSize; i++) {
				if (!complete)
					move(population, i, j, map, WIDTH, HEIGHT, goal);
			}
			if (!complete && miceAlive(population, popSize)) {
				//printMap(map, WIDTH, HEIGHT, population, popSize);     to see where they go
				//cin.get();
			}
		}
		makeFitness(fitnessArray, population, popSize, goal);
		cout << "x of most fit: " << population[getMostFit(fitnessArray, popSize)].x << ", y of most fit: " << population[getMostFit(fitnessArray, popSize)].y << endl;
		printMap(map, WIDTH, HEIGHT, population, popSize);
		fitTot = fitnessTotal(fitnessArray, popSize);
		Mouse*tempPop = new Mouse[popSize];
		for (int i = 0; i < (popSize); i += 2) {
			Mouse m1;
			Mouse m2;
			m1 = selectMouse(fitnessArray, fitTot, popSize, population);
			m2 = selectMouse(fitnessArray, fitTot, popSize, population);
			crossOver(numMoves, m1.moves, m2.moves,&tempPop[i],&tempPop[i+1]); //needs to copy arrays to temppop not pass reference
			mutate(tempPop[i].moves, numMoves, mutationRate);
			mutate(tempPop[i + 1].moves, numMoves, mutationRate);

			tempPop[i].x = startX;
			tempPop[i].y = startY;
			tempPop[i + 1].x = startX;
			tempPop[i + 1].y = startY;
			tempPop[i].alive = true;
			tempPop[i + 1].alive = true;
		}
		//for (int i = (popSize - numRands); i < popSize; i++) {
			//initializeMouse(population, numMoves, startX, startY, i);
		//}
		for (int i = 0; i < popSize; i++) {
			delete[] population[i].moves;
		}
		delete[](population);
		population = tempPop;
		//delete tempPop;
		generation++;
		cout <<endl<< "generation: " << generation<<endl;
		//cin.get();
	}
	//********************************************************************************************************************/
	//a pause so the program doesnt close when it ends

	/****************************************TEST CODE***************************************************************
	Mouse*testmice = new Mouse[5];
	Goal g;
	g.x = 5;
	g.y = 6;
	for (int i = 0; i < 5; i++) {
		initializeMouse(testmice, 5, 1, 1, i);
	}
	cout <<"testMice[2].x: "<< testmice[2].x<<endl;
	for (int i = 0; i < 5; i++) {
		initializeMouse(testmice, 5, 2, 1, i);
	}
	testmice[3].x = 5;
	cout << "testMice[2].x after redo: " << testmice[2].x << endl;
	double*fitTester = new double[5];
	makeFitness(fitTester, testmice, 5, g);
	for (int i = 0; i < 5; i++) {
		cout << "fitness array [" << i << "] is: " << fitTester[i]<<endl;
	}
	cout << "fitTester highest one: "<<getMostFit(fitTester, 5)<<endl;
	cout << "fitTotal: " << fitnessTotal(fitTester, 5)<<endl;
	cout << "mouse 1: ";
	for (int i = 0; i < 5; i++) {
		cout << testmice[0].moves[i]<<" ";
	}
	cout << endl;
	cout << "mouse 2: ";
	for (int i = 0; i < 5; i++) {
		cout << testmice[1].moves[i] << " ";
	}
	cout << endl; 
	crossOver(5, testmice[0].moves, testmice[1].moves, &testmice[2], &testmice[3]);

	cout << "mouse 3: ";
	for (int i = 0; i < 5; i++) {
		cout << testmice[2].moves[i] << " ";
	}
	cout << endl;
	cout << "mouse 4: ";
	for (int i = 0; i < 5; i++) {
		cout << testmice[3].moves[i] << " ";
	}
	cout << endl;
	mutate(testmice[2].moves, 5, 2);
	cout << "mouse 3: ";
	for (int i = 0; i < 5; i++) {
		cout << testmice[2].moves[i] << " ";
	}
	********************************************************************************************/
	cin.get();
	return 0;
}
int getMostFit(double*fitArray, int popSize) {
	int mostFit = 0;
	double current = 0;
	for (int i = 0; i < popSize;i++) {
		//cout << "in fit tester, fit [" << i << "] = "<< fitArray[i]<<endl;
		if (fitArray[i] > current) {
			mostFit = i;
			current = fitArray[i];
		}
	}
	return mostFit;
}
bool miceAlive(Mouse* pop, short popSize) {
	bool alive = false;
	for (int i = 0; i < popSize; i++) {
		if (pop[i].alive)
			alive = true;
	}
	if (alive == false)
		//cout << "they all died :(";
	return alive;
}
void initializeMouse(Mouse* mouse, short numMoves, short x, short y, short j) {
	mouse[j].moves = new short[numMoves];//the genome is called moves because it is a list of moves

	for (int i = 0; i < numMoves; i++) {
		mouse[j].moves[i] = rand() % 4;
	}
	mouse[j].x = x; //start x and y at first
	mouse[j].y = y;
	mouse[j].alive = true; // will be used later to break loops when mice run into walls
	mouse[j].genomeLength = numMoves; // stored so that later the genome length can be changed and it will not break
}
void fillMap(bool map[], const int WIDTH, const int HEIGHT) {
	for (int y = 0; y < HEIGHT; y++){
		for (int x = 0; x < WIDTH; x++) {
			if (y == 0 || x == 0 || x == WIDTH - 1 || y == HEIGHT - 1) { //if it is a border make it true (reprisented by a 1)
				map[y*WIDTH + x] = true;
			}
			else if (x == 4 && y < 5 || x>6&&y==5) { //2 walls I added
				map[y*WIDTH + x] = true;
			}
			else {
				map[y*WIDTH + x] = false;
			}
		}
	}
}
void printMap(bool map[], const int WIDTH, const int HEIGHT, Goal goal) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (x == goal.x&&y == goal.y) {
				cout << 2;
			}
			else
				cout << map[y*WIDTH + x];
		}
		cout << endl;
	}
}
void printMap(bool map[], const int WIDTH, const int HEIGHT, Mouse* mouse, short popSize) {
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) { //TODO re write to show all the mice
			bool mouseHere = false;
			for (int i = 0; i < popSize; i++) {
				if (x == mouse[i].x && y == mouse[i].y /*&& mouse[i].alive*/) {
					mouseHere = true;
				}
				//else
					//cout << map[y*WIDTH + x];
			}
			if (mouseHere)
				cout << 2;
			else
				cout << map[y*WIDTH + x];
		}
		cout << endl;
	}
}
//goes through the mouse's genome to figure out where it ended up (stored in it's local x and y)
void move(Mouse* mouse, short n, short i, bool map[], const int WIDTH, const int HEIGHT, Goal goal) { //n is the population number we are on

	if (mouse[n].alive) {
		//cout << "x " << mouse[n].x << ", y " << mouse[n].y << ", dir " << mouse[n].moves[i] << ", n " << n << endl;
		//cout << endl;

		switch (mouse[n].moves[i]) {
		case 0:
			if (map[(mouse[n].y - 1)*WIDTH + mouse[n].x] == 1) {
				mouse[n].alive = false;
			}
			else
				mouse[n].y--;
			break;
		case 1:
			if (map[mouse[n].y*WIDTH + mouse[n].x + 1] == 1) {
				mouse[n].alive = false;
			}
			else
				mouse[n].x++;
			break;
		case 2:
			if (map[(mouse[n].y + 1)*WIDTH + mouse[n].x] == 1) {
				mouse[n].alive = false;
			}
			else
				mouse[n].y++;
			break;
		case 3:
			if (map[mouse[n].y*WIDTH + mouse[n].x - 1] == 1) {
				mouse[n].alive = false;
			}
			else
				mouse[n].x--;
			break;
		default:
			break;
		}
	}
	if (mouse[n].x == goal.x && mouse[n].y == goal.y) {
		complete = true;
		cout << "MADE IT YEET!";
	}
}
void makeFitness(double* fitnessArray, Mouse* population, short popSize, Goal goal) {
	//fitnessArray = new  double[popSize];
	for (int i = 0; i < popSize; i++) {
		fitnessArray[i] = 1.0 / (abs(population[i].x - goal.x) + abs(population[i].y - goal.y));
		//fitnessArray[i] = 1.0 / (pow(pow(abs(population[i].x - goal.x),2) + pow(abs(population[i].y - goal.y),2),0.5));
		//will never be 1/0 because this funcion will not be called if a mouse made it
	}
	//return fitnessArray;
}
double fitnessTotal(double* fitnessArray, short popSize) {
	double total = 0;
	for (int i = 0; i < popSize; i++)
		total += fitnessArray[i];
	return total;
}
Mouse selectMouse(double* fitnessArray, double fitnessTotal, short popSize, Mouse* population) {
	double randSelect = static_cast <double> (rand()) / (static_cast <double> ((RAND_MAX) / fitnessTotal)); //makes a random number from 0 to total fitness

	short i = 0;
	while (randSelect >= fitnessArray[i]) {
		randSelect -= fitnessArray[i];
		i++;
	}
	return population[i];
}
void crossOver(short numMoves, short* parentGenome1, short* parentGenome2, Mouse* newMouse1, Mouse* newMouse2) {
	short*temp1 = new  short[numMoves];
	short*temp2 = new  short[numMoves];
	int crossPoint = rand() % numMoves;
	//cout << "cross point " << crossPoint<<endl;
	for (int i = 0; i < crossPoint; i++) {
		temp1[i] = parentGenome1[i];
		temp2[i] = parentGenome2[i];
	}
	for (int i = crossPoint; i < numMoves; i++) {
		temp1[i] = parentGenome2[i];
		temp2[i] = parentGenome1[i];
	}
	/*GenomePair gp;
	gp.gen1 = temp1;
	gp.gen2 = temp2;
	return gp;*/
	newMouse1->moves = temp1;
	newMouse2->moves = temp2;
	//delete temp1;
	//delete temp2;
}
void mutate(short* genome, short genLength, short mutationRate) {
	for (int i = 0; i < genLength; i++) {
		if (rand() % mutationRate == 0){
			//cout << "mutated " << i << endl;
			genome[i] = rand() % 4;
		}
	}
	//return genome;
}