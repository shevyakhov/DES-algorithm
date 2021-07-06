// DES.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <bitset>
#include <ctime>
using namespace std;

int IP[64] =
{
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9,  1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};
int PC_1[56] =
{
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};
/*Replacement selection PC-2*/
/*56bit key compressed into 48bit subkey*/
int PC_2[48] =
{
	14, 17, 11, 24,  1,  5,
	3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

/*Cyclic shift left*/
int shiftBits[16] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };
int E[48] =
{
	32,  1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};
/*S box instead of compression*/
/*Each S box is a 16x4 replacement table*/
/*48bit compressed to 32bit*/
int S_BOX[8][4][16] = {
	{
{ 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
{ 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
{ 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
{ 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 }
	},
{
	{ 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
{ 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
{ 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
{ 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 }
},
{
	{ 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
{ 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
{ 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
{ 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 }
},
{
	{ 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
{ 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
{ 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
{ 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 }
},
{
	{ 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
{ 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
{ 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
{ 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 }
},
{
	{ 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
{ 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
{ 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
{ 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 }
},
{
	{ 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
{ 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
{ 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
{ 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 }
},
{
	{ 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
{ 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
{ 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
{ 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 }
}
};

/*P box replacement*/
/*Similar to IP initialization*/
int P[32] =
{
	16,  7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25
};
/*Deinitialize the permutation table*/
int IP_1[64] =
{
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41,  9, 49, 17, 57, 25
};
void doSwap(bool* A, bool* B, int size) { // swapping arrays
	for (int i = 0; i < size; i++) {
		swap(A[i], B[i]);
	}
}
void ezPerm(bool* main, int* Table, int size) { // dinamic permutation (array , PermTable, size)
	bool* curText = new bool[size];
	for (int i = 0;i < size; i++) {
		curText[i] = main[Table[i]-1];
	}
	doSwap(main, curText,size);

}
void Debug(bool* A, int size) { // just printing out the arrays
	for (int i = 0; i < size; i++)
	{
		cout << A[i];
		if (i % 8 == 7) {
			cout << " ";
		}
	}
	cout << endl;
}
bool* make64bits(bool* key) //adding 8 8th additional bits making 64 bits key
{
	bool* key_64 = new bool[64];
	int sum = 0;
	int j = 0;

	for (int i = 0; i < 64;i++) { // make every 8th additional bit zero
		if (i % 8 == 7) {
			key_64[i] = 0;
		}
		else{
		key_64[i] = key[j];
		j++;
		}
		
	}
	for (int i = 0; i < 64;i += 8) { // make every 8th bit either 0 or 1 depending on sum
		sum = 0;
		for (int j = 0; j < 6; j++) {
			sum += key_64[j];
		}
		if (sum % 2 == 0) {
			key_64[i + 7] = 1;
		}
	}
	return key_64;
}
bool* keyPermC(bool* key64) {// generate D0 ignoring every 8th additional bit
	bool* C = new bool[28];
	int j = 0;
	for (int i = 0;i < 28; i++) {
		C[i] = key64[PC_1[i] - 1];
	}
	return C;
}
bool* keyPermD(bool* key64) { // generate D0 ignoring every 8th additional bit
	bool* D = new bool[28];
	for (int i = 28;i < 56; i++) {
		D[i-28] = key64[PC_1[i] - 1];
	}
	return D;
}

void moveBits(int i, bool* Mass) {// move bits to the left in incryption
	bool* D = new bool [28];
	for (int j = 0; j < 28;j++) {
		D[j] = Mass[(j + shiftBits[i])%28];
	}
	doSwap(Mass, D,28);
}
//void moveBits_1(int i, bool* Mass) { // move bits to the right in decryption
//	bool* D = new bool [28];
//	for (int j = 0; j < 28;j++) {
//		D[(j + shiftBits[i]) % 28] = Mass[j];
//	}
//	doSwap(Mass, D,28);
//}
bool* makeOne_56(bool* Ci, bool* Di) { // glue two parts of the round key
	bool* key = new bool[56];
	for (int i = 0; i < 28; i++) {
		key[i] = Ci[i];
		key[i+28] = Di[i];
	}
	return key;
}
bool* key_48(bool* key_56) {// from 56 bits to 48 bits key
	bool* key = new bool[48];
	for (int i = 0; i < 48;i++) {
		key[i] = key_56[PC_2[i] - 1];
	}
	return key;
}
void makeOneText(bool* L, bool* R, bool* text) { // glue two parts of the text
	for (int i = 0; i < 32; i++)
	{
		text[i] = L[i];
		text[i + 32] = R[i];
	}
}bool* makeOneD(bool* L, bool* R) { // glue two parts of the text
	bool* text = new bool[64];
	for (int i = 0; i < 32; i++)
	{
		text[i] = L[i];
		text[i + 32] = R[i];
	}
	return text;
}

void Generation(bool* vector, int N,bool* vector1, int N1) 
{   // random text\key generation
	srand((unsigned)time(NULL));
	for (int i = 0; i < N; i++)
	{
		vector[i] = rand() % 2;	 
	}
	for (int i = 0; i < N1; i++)
	{
		vector1[i] = rand() % 2;
		
	}
	/*for (int i = 0; i < 32; i++)
	{
		vector[i] = 1;
		vector[i + 32] = 0;
	}
	for (int i = 0; i < N1; i++)
	{
		vector1[i] = 0;

	}*/
}

void LnR(bool *L, bool *R, bool*text) {// divide text in two parts and keep it in (L) & (R)
	for (int i = 0; i < 32; i++)
	{
		L[i] = text[i];
		R[i] = text[i + 32];
	}
}
bool* ext48(bool* text32) { // extend the part of the text into 48 bits
	bool *ext = new bool[48];
	for (int i = 0; i < 48;i++) {
		ext[i] = text32[E[i] - 1];
	}
	return ext;
}
void xorF(bool* text, bool* ki, int size) { // xor the elements
	for (int i = 0; i < size; i++) {
		text[i] = text[i] xor ki[i];
	}
}
bool* sBlocks(bool* xored) { // make xored 48bits into 32 bits
	bool* iRound = new bool[32];
	/*cout << "______\n";
	Debug(xored, 48);*/
	for (int i = 0 , j = 0; i < 48; i+=6, j+=4 )
	{
		int row = xored[i] * 2 + xored[i + 5];
		int col = xored[i + 1] * 8 + xored[i + 2] * 4 + xored[i + 3] * 2 + xored[i + 4];
		int newNum = S_BOX[i/6][row][col];
		/*cout << i / 6 << " " << row << " " << col << " " << newNum << " \n";*/
		bitset<4> temp(newNum);
		iRound[j + 3] = temp[0];
		iRound[j+ 2] = temp[1];
		iRound[j + 1] = temp[2];
		iRound[j] = temp[3];
	}
	/*Debug(iRound, 32);*/
	return iRound;
}
void feistelRound(bool *L, bool* R, bool *keyC, bool * keyD, bool**save) { // name says for itself
	for (int i = 0; i < 16; i++) {
		bool* R48 = new bool[48];
		bool* key48 = new bool[48];
		bool* S = new bool[32];
		moveBits(i, keyC);
		moveBits(i, keyD);
		cout << "C"<<i+1<<" ";
		Debug(keyC, 28);
		cout << "D" << i + 1 << " ";
		Debug(keyD, 28);
		key48 = key_48(makeOne_56(keyC, keyD));
		cout << "key48 " << i + 1 << " ";
		Debug(key48, 48);
		save[i] = key48;
		cout << "extR" << i + 1 << " ";
		R48 = ext48(R);
		Debug(R48, 48);
		xorF(R48, key48, 48);
		cout << "xor1 " << i + 1 << " ";
		Debug(R48, 48);
		S = sBlocks(R48);
		cout << "sBlocks " << i + 1 << " ";
		Debug(S, 32);
		ezPerm(S, P, 32);
		cout << "sPerm " << i + 1 << " ";
		Debug(S, 32);
		xorF(L, S, 32);
		cout << "xor2 " << i + 1 << " ";
		Debug(L, 32);
		doSwap(L, R, 32);
	}
	doSwap(L, R, 32);
}


void desypher(bool* L, bool* R,  bool **save) { // feiselRound ^ -1
	
	for (int i = 15; i >= 0; i--) {
		bool* R48 = new bool[48];
		bool* key48 = new bool[48];
		bool* S = new bool[32];
		R48 = ext48(R);
		key48 = save[i];
		xorF(R48, key48 , 48);
		S = sBlocks(R48);
		ezPerm(S, P, 32);
		xorF(L, S , 32);
		doSwap(L, R, 32);
	}
	doSwap(L, R, 32);
}
int main()
{	
	bool text[64] = { 0,1,1,0,0,0,0,1, 0,1,1,0,0,0,0,1,
		0,1,1,0,0,0,0,1, 0,1,1,0,0,0,0,1, 0,1,1,0,0,0,0,1,
		0,1,1,0,0,0,0,1, 0,1,1,0,0,0,0,1, 0,1,1,0,0,0,0,1 };
bool key[64] = { 0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,
	0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,
	0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,0,1,0 };
	bool* key_64 = new bool[64];
	bool* C0 = new bool[28];
	bool* D0 = new bool[28];
	bool* C01 = new bool[28];
	bool* D01 = new bool[28];
	bool* L = new bool[32];
	bool* R = new bool[32];
	bool* L1 = new bool[32];
	bool* R1 = new bool[32];
	bool** save = new bool*[16];
	for (int i = 0; i < 16; i++)
	{
		save[i] = new bool[48];
	}
	/*Generation(text, 64, key, 64);*/
	cout << "text ";
	Debug(text, 64);
	cout << "key ";
	Debug(key, 64);
	ezPerm(text, IP, 64);/*first permutation*/
	C0 = keyPermC(key);
	D0 = keyPermD(key);
	cout << "C0 ";
	Debug(C0, 28);
	cout << "D0 ";
	Debug(D0, 28);
	LnR(L,R,text);
	cout << "left ";
	Debug(L, 32);
	cout << "right ";
	Debug(R, 32);
	feistelRound(L,R,C0,D0,save);
	makeOneText(L, R, text);
	ezPerm(text, IP_1, 64);
	cout << "DES ";
	Debug(text, 64);
	//decrypt
	C01 = keyPermC(key);
	D01 = keyPermD(key);
	ezPerm(text, IP, 64);
	LnR(L1, R1, text);
	desypher(L1, R1, save);
	makeOneText(L1, R1, text);
	ezPerm(text, IP_1, 64);
	Debug(text, 64);

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
