#include <bits/stdc++.h>
using namespace std;
int pc = 0;
string l;
int Clock, index1;
ifstream in;
int flag = 0;
int cacheindex = 0;
int globalLRU = 0;
stack<string> sstr;
stack<int> sint;
stack<string> labH;
stack<string> dataLab;
stack<int> dataNum;
stack<string> dataStr;

int a;
int R[32];
int mem[4096] = {0};
string instr;
// for instructions and its line number
struct inst
{
	string line;
	int no;
} ins;
// for storing destination,source registers
struct reg
{
	int dest = -1;
	int src1 = -1, src2 = -1;
	string line;
};
// for stalls
struct stal
{
	int lNo;
	int stalls;
};
class offset
{
public:
	int data;
};
class Block
{
public:
	offset *o;
	int blkSize;
	int tag;
	int dirty;
	int valid;
	int lru;
	Block()
		: blkSize(-1), tag(-1), dirty(0), valid(0), lru(0)
	{
	}
};

class Set
{
public:
	Block *blks;
	int no_of_blks;
};

class Cache
{
public:
	Set *s;
	int no_of_sets;
};
// corresponding vectors of above 3 structs
vector<inst> v;
vector<reg> v1, v2;
vector<stal> v3;
// checks if a word is a string
bool isString(string str)
{
	for (int i = 0; i < str.length(); i++)
		if (isdigit(str[i]) == false)
			return true;
	return false;
}
int power(int a, int b)
{
	int value = 1;
	for (int i = 0; i < b; i++)
	{
		value = value * a;
	}
	return value;
}
// converts decimal to hexdecimal and stores in datasegment/memory
void decToHexa(int n)
{
	mem[index1] = n;
	index1--;
}
// converts dec to hexadecimal and stores or re-write memory using initial index
void decToHexSW(int n, int i1)
{
	mem[i1] = n;
}
int BintoDec(int a[], int start, int end)
{
	int l = end + 1;
	int value = 0;
	for (int i = start; i <= end; i++)
	{
		value = (a[i]) * (power(2, l - 1 - i)) + value;
	}
	return value;
}
// stores maximum 5 instructions in order
void bal()
{
	if (v1.size() > 5)
	{
		for (int i = 1; i < v1.size(); i++)
		{
			reg r1;
			r1.line = v1[i].line;
			r1.dest = v1[i].dest;
			r1.src1 = v1[i].src1;
			r1.src2 = v1[i].src2;
			v2.push_back(r1);
		}
		v1.clear();
		v1.swap(v2);
		v2.clear();
	}
}
// stores max 3 instructions in order
void bal1()
{
	if (v1.size() > 3)
	{
		for (int i = 1; i < v1.size(); i++)
		{
			reg r1;
			r1.line = v1[i].line;
			r1.dest = v1[i].dest;
			r1.src1 = v1[i].src1;
			r1.src2 = v1[i].src2;
			v2.push_back(r1);
		}
		v1.clear();
		v1.swap(v2);
		v2.clear();
	}
}
// parses data segment
void parse_data(string fileN, string l1, string l2)
{
	string line = "";
	string line1 = "";
	string word = "";
	ifstream in2;
	in2.open(fileN);
	while (in2)
	{

		while (line1 != l1)
		{
			getline(in2, line1);
		}
		getline(in2, line);
		if (line == l2)
			in2.close();
		else
		{

			int l = line.length();
			char c1[l];

			for (int i = 0; i < l; i++)
			{
				c1[i] = line[i];
				// cout << c1[i];
			}
			for (int x = 0; x < l - 1; x++)
			{

				while ((c1[x] == ' ') || (c1[x] == ','))
				{
					if (x >= l)
					{
						break;
					}
					x++;
				}

				word = word + c1[x];

				if (c1[x + 1] == ' ' || c1[x + 1] == ',')
				{
					if (word != "")
					{
						// cout << word << endl;
						int len = word.length();
						if (word[len - 1] == ':')
						{
							string st = "";
							for (int i = 0; i < word.length() - 1; i++)
								st += word[i];
							dataLab.push(st);
							word = "";
						}
						else if (isString(word))
						{
							dataStr.push(word);
							word = "";
							// cout << "string stack : " << dataStr.top() << endl;
						}
						else
						{
							stringstream ss(word);
							int i = 0;
							ss >> i;
							// cout << i << endl;
							dataNum.push(i);
							// cout << "integer stack: " << dataNum.top() << endl;
							word = "";
						}
					}
				}
			}
			while (!dataNum.empty())
			{

				if (dataStr.top() == ".word")
				{
					if (flag == 0)
					{

						int size = dataNum.size();
						index1 = index1 + size - 1;
						flag++;
					}
					decToHexa(dataNum.top());
					dataNum.pop();
				}
			}
		}
	}
}
// parses the instructions
void parser(string s, int n)
{
	parse_data(s, ".data", ".text");
	string line;
	string l1;
	int i = 0;
	in.open(s);
	while (in.good())
	{
		while (l1 != "main:")
		{
			getline(in, l1);
		}
		getline(in, line);
		ins.line = line;
		ins.no = i;
		v.push_back(ins);
		i++;
	}
	in.close();
}

// converts hexadecimal to int/decimal
int convert(char a[])
{
	int len = strlen(a);
	int base = 1;
	int temp = 0;
	for (int i = len - 1; i >= 0; i--)
	{
		if (a[i] >= '0' && a[i] <= '9')
		{
			temp += (a[i] - 48) * base;
			base = base * 16;
		}
		else if (a[i] >= 'A' && a[i] <= 'F')
		{
			temp += (a[i] - 55) * base;
			base = base * 16;
		}
	}
	return temp;
}

// to search line number
int Search(string l)
{
	int t;
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i].line == l + ":")
		{
			t = i;
			break;
		}
	}
	return t;
}

// fetches the instructions
string IF()
{

	instr = v[pc].line;
	// cout<<l<<endl;
	return instr;
}
// decodes the instructions after IF
void ID(string str)
{
	string word = "";
	int cnt = 0;
	sstr.push("sample");
	int l = str.length();
	char c1[l];
	reg I;

	for (int i = 0; i < l; i++)
	{
		c1[i] = str[i];
	}

	for (int x = 0; x < l; x++)
	{

		while ((c1[x] == ' ') || (c1[x] == ','))
		{
			if (x >= l)
			{
				break;
			}
			x++;
		}

		word = word + c1[x];
		if (c1[x + 1] == ' ' || c1[x + 1] == ',')
		{
			if (word != "")
			{

				if (word == "ADD" || word == "SUB" || word == "ADDI" || word == "SUBI" || word == "LW" || word == "SW" || word == "BNE" || word == "BEQ" || word == "jump" || word == "SGT" || word == "MULI")
				{

					sstr.push(word);
					I.line = word;
					// cout << word << endl;
					word = "";
				}
				else
				{
					int brac = -1;
					for (int i = 0; i < word.length(); i++)
					{
						if (word[i] == '(')
							brac = i;
					}
					if (brac != -1)
					{
						string t = "";
						string temp = "";
						for (int i = 0; i < brac; i++)
						{
							t += word[i];
						}
						if (isString(t))
						{
							int i = atoi(&t[1]);
							sint.push(R[i]);
							// cout << R[i] << endl;
							word = "";
						}
						else
						{
							stringstream ss(word);
							int i = 0;
							ss >> i;
							sint.push(i);
							// cout << i << endl;

							for (int i = brac + 1; i < word.length() - 1; i++)
							{
								temp += word[i];
							}
							labH.push(temp);
							word = "";
						}
					}
					else
					{
						if (word[0] == 'R')
						{

							int i = atoi(&word[1]);
							sint.push(i);
							if (I.line == "ADD" || I.line == "SUB")
							{

								if (cnt == 0)
								{
									I.dest = i;
								}
								else if (cnt == 1)
									I.src1 = i;
								else if (cnt == 2)
									I.src2 = i;
							}
							else if (I.line == "ADDI" || I.line == "SUBI" || I.line == "MULI")
							{
								if (cnt == 0)
									I.dest = i;
								else if (cnt == 1)
									I.src1 = i;
							}
							else if (I.line == "BNE" || I.line == "BEQ")
							{
								if (cnt == 0)
									I.src1 = i;
								else if (cnt == 1)
									I.src2 = i;
							}
							else if (I.line == "LW")
							{
								if (cnt == 0)
									I.dest = i;
							}
							else if (I.line == "SW")
							{
								if (cnt == 0)
									I.src1 = i;
							}

							word = "";
							cnt++;
						}

						else if (isString(word))
						{
							if (word != "exit")
							{
								labH.push(word);
								// cout << "word in isString : " << labH.top() << "\n";
								word = "";
							}
						}
						else
						{
							// cout << "word in else: " << word << endl;
							stringstream ss(word);
							int i = 0;
							ss >> i;
							sint.push(i);

							// cout << i << endl;
							word = "";
						}
					}
				}
			}
		}
	}
	v1.push_back(I);
}
// executes the instructions after decoding it(ID)
int EXE()
{
	int b, c, d = 0;
	if (sint.size() == 3)
	{
		c = sint.top();
		sint.pop();
		b = sint.top();
		sint.pop();
		a = sint.top();
		sint.pop();
		// cout << a << " ," << b << " ," << c << endl;
	}
	else if (sint.size() == 2)
	{
		b = sint.top();
		sint.pop();
		a = sint.top();
		sint.pop();
	}
	//.........................................................................................................
	// executing each instruction
	if (sstr.top() == "ADD")
	{
		sstr.pop();
		d = R[b] + R[c];

		// cout << "top element after pop : " << sstr.top() << endl;
	}
	else if (sstr.top() == "SUB")
	{
		sstr.pop();
		d = R[b] - R[c];

		// cout << "top element after pop : " << sstr.top() << endl;
	}
	else if (sstr.top() == "ADDI")
	{
		sstr.pop();
		d = R[b] + c;

		// cout << "top element after pop : " << sstr.top() << endl;
	}

	else if (sstr.top() == "SUBI")
	{
		sstr.pop();
		d = R[b] - c;

		// cout << "top element after pop : " << sstr.top() << endl;
	}
	else if (sstr.top() == "MULI")
	{
		sstr.pop();
		d = R[b] * c;
		// cout << d << endl;
		// cout << "top element after pop : " << sstr.top() << endl;
	}
	else if (sstr.top() == "LW")
	{
		int temp_addr = b;
		cacheindex = b;
		int res;
		if (temp_addr >= 0 && temp_addr < 4096)
		{
			res = mem[temp_addr];
		}
		// sstr.pop();
		d = res;
		// cout << "exe: " << d << endl;
	}

	else if (sstr.top() == "SW")
	{
		int temp_addr = b;
		cacheindex = b;
		// if (temp_addr >= 0 && temp_addr < 4096)
		// {
		// 	decToHexSW(R[a], temp_addr);
		// }

		// sstr.pop();
		d = R[a];
	}
	else if (sstr.top() == "SGT")
	{
		sstr.pop();
		if (R[b] >= R[c])
		{
			R[c] = R[b];
			R[a] = R[16];
		}
		d = R[a];
	}
	else if (sstr.top() == "BNE")
	{
		int z = -1;
		cout << "Values of R2 and R3: " << R[2] << "   " << R[3] << endl;
		if (R[a] != R[b])
		{
			string s1 = labH.top();

			z = Search(s1);
		}
		d = z;
		// cout << "top element after pop : " << sstr.top() << endl;
	}
	else if (sstr.top() == "BEQ")
	{
		int z = -1;
		// cout << "Values of R2 and R3 (BEQ): " << R[2] << "   " << R[3] << endl;
		if (R[a] == R[b])
		{
			string s1 = labH.top();
			z = Search(s1);
		}
		// sstr.pop();
		d = z;
		// cout << "top element after pop : " << sstr.top() << endl;
	}
	else if (sstr.top() == "jump")
	{
		// in.close();
		// in1.close();
		int z = 0;
		string s2 = labH.top();
		z = Search(s2);
		// cout << "after jump: " << s2 << endl;
		// parse_label(fName, s2);
		d = z;
	}
	return d;
}
// memory segment
int MEM(int val)
{
	return val;
}
// write back stage
void WB(int x)
{
	R[a] = x;
}
// printing the contents of registers and memory
void print()
{
	// registers
	for (int i = 0; i < 32; i++)
	{
		if (i >= 0 && i <= 9)
			cout << "R" << i << "    = " << R[i] << endl;
		else
			cout << "R" << (i) << "   = " << R[i] << endl;
	}
	// // Memory

	for (int j = 0; j < 100; j++)
	{
		cout << "mem[ " << j << " ]  =" << mem[j] << "\t\t ";
	}
}
// checks if current instruction depends on previous 4 instructions
int checkDep()
{
	int size = v1.size();
	int stall = 0;
	if (size == 1)
	{
		stall = 0;
	}
	else if (size > 1)
	{
		int s1 = v1[size - 1].src1, s2 = v1[size - 1].src2;
		for (int i = 0; i < size; i++)
		{
			if (v1[i].dest != -1)
			{
				if (v1[i].dest == s1 || v1[i].dest == s2)
				{
					int diff = (size - 1) - i;

					if (diff == 1)
					{
						if (v1[i - 1].line == "Lw")
						{
							stall = 1 + v3[v3.size() - 1].stalls;
						}
						else
						{
							stall = 3;
						}
					}
					if (diff == 2)
						stall = 2;
					if (diff == 3)
						stall = 1;
					if (diff == 0)
						stall = 0;
				}
			}
		}
	}
	return stall;
}
// checks if current instruction depends on previous 3 instructions
int checkDep1()
{
	int size = v1.size();
	int stall = 0;
	if (size == 1)
	{
		stall = 0;
	}
	else if (size > 1)
	{
		int s1 = v1[size - 1].src1, s2 = v1[size - 1].src2;
		for (int i = 0; i < size; i++)
		{
			if (v1[i].dest != -1)
			{
				if (v1[i].dest == s1 || v1[i].dest == s2)
				{
					int diff = (size - 1) - i;

					if (diff == 1)
					{
						if (v1[i - 1].line == "Lw")
						{
							stall = stall + v3[v3.size() - 1].stalls;
						}
						if (v1[i].line == "LW")
							stall = stall + 2;
						else
							stall = 1;
					}

					if (diff == 2)
					{
						if (v1[i].line == "LW")
							stall = 1;
						else
							stall = 0;
					}
					if (diff == 0)
						stall = 0;
				}
			}
		}
	}
	return stall;
}
// calculates the difference between the two instructions
int Diff()
{
	int size = v1.size();
	int diff = 0;
	int stall = 0;
	if (size == 1)
	{
		stall = 0;
	}
	else if (size > 1)
	{
		int s1 = v1[size - 1].src1, s2 = v1[size - 1].src2;

		for (int i = 0; i < size; i++)
		{
			if (v1[i].dest != -1)
			{
				if (v1[i].dest == s1 || v1[i].dest == s2)
				{
					diff = (size - 1) - i;
				}
			}
		}
	}
	return diff;
}

int main()
{
	string fname, s1;

	cout << "enter file name to be opened: ";
	cin >> fname;
	int n, res, r1, stall, TotalStalls = 0;
	int ins = 0;
	n = fname.length();
	parser(fname, n);
	int vsize = v.size();
	int flag1 = 0;
	int opt;
	cout << "select your option (1 or 2): " << endl;
	cout << "1.Disable data forwarding\n2.Enable data forwarding" << endl;
	cin >> opt;
	int cacheaccess = 0, h1, h2, memacc, miss1 = 0, miss2 = 0;
	int cs, bs, asc;
	cout << "enter cache size in bytes for level 1:" << endl;
	cin >> cs;
	cout << "enter block size in bytes for l1:" << endl;
	cin >> bs;
	cout << "enter associativity:" << endl;
	cin >> asc;
	cout << "enter hit time of l1 cache:" << endl;
	cin >> h1;
	int cs2, bs2, asc2;
	cout << "enter cache size in bytes for level 2:" << endl;
	cin >> cs2;
	cout << "enter block size in bytes for l2:" << endl;
	cin >> bs2;
	cout << "enter associativity:" << endl;
	cin >> asc2;
	cout << "enter hit time of l2 cache" << endl;
	cin >> h2;
	cout << "enter memory access time" << endl;
	cin >> memacc;

	Cache c;
	int nb = cs / bs, ofs = bs;
	c.no_of_sets = nb / asc;
	int offsetbits = log2(ofs);
	int setbits = log2(c.no_of_sets);
	int tagbits = 12 - offsetbits - setbits;

	c.s = new Set[c.no_of_sets];
	for (int i = 0; i < c.no_of_sets; i++)
	{
		c.s[i].no_of_blks = nb;
	}

	for (int i = 0; i < c.no_of_sets; i++)
	{
		c.s[i].blks = new Block[nb];
		for (int j = 0; j < nb; j++)
		{
			c.s[i].blks[j].blkSize = ofs;
		}
	}
	for (int i = 0; i < c.no_of_sets; i++)
	{

		for (int j = 0; j < nb; j++)
		{
			c.s[i].blks[j].o = new offset[ofs];
			for (int k = 0; k < ofs; k++)
			{
				c.s[i].blks[j].o[k].data = 0;
			}
		}
	}
	//.................level 2 cavhe
	Cache c2;
	int nb2 = cs2 / bs2, ofs2 = bs2;
	c2.no_of_sets = nb2 / asc2;
	int offsetbits2 = log2(ofs2);
	int setbits2 = log2(c2.no_of_sets);
	int tagbits2 = 12 - offsetbits2 - setbits2;

	c2.s = new Set[c2.no_of_sets];
	for (int i = 0; i < c2.no_of_sets; i++)
	{
		c2.s[i].no_of_blks = nb2;
	}

	for (int i = 0; i < c2.no_of_sets; i++)
	{
		c2.s[i].blks = new Block[nb2];
		for (int j = 0; j < nb2; j++)
		{
			c2.s[i].blks[j].blkSize = ofs2;
		}
	}
	for (int i = 0; i < c2.no_of_sets; i++)
	{

		for (int j = 0; j < nb2; j++)
		{
			c2.s[i].blks[j].o = new offset[ofs2];
			for (int k = 0; k < ofs2; k++)
			{
				c2.s[i].blks[j].o[k].data = 0;
			}
		}
	}
	while (pc < vsize)
	{
		s1 = IF();
		if (s1 != "exit,")
		{
			ID(s1);
			if (opt == 1)
			{
				bal();
				stal New;
				stall = checkDep();

				if (sstr.top() == "ADD" || sstr.top() == "SUB" || sstr.top() == "ADDI" || sstr.top() == "SUBI" || sstr.top() == "MULI" || sstr.top() == "BNE" || sstr.top() == "BEQ" || sstr.top() == "jump" || sstr.top() == "LW" || sstr.top() == "SW" || sstr.top() == "SGT")
				{
					ins++;
					res = EXE();
					if (sstr.top() == "BNE" || sstr.top() == "BEQ")
					{
						if (flag1 == 0)
						{

							Clock = Clock + stall + 6;
							// cout << "inc: " << Clock << endl;
							flag1 = 1;
						}
						else
						{
							if (v3[v3.size() - 1].stalls != 0)
							{
								int d = Diff();
								if (d == 1)
								{
									Clock = Clock + stall + 2;
								}
								else if (d > 1)
								{
									stall = 0;
									Clock = Clock + 2;
								}
								else
								{
									stall = 0;
									Clock = Clock + 2;
								}
								// cout << "inc: " << Clock << endl;
							}
							else
								Clock = Clock + stall + 2;
						}
						sstr.pop();
						if (res != -1)
						{
							pc = res;
						}
					}
					else
					{
						if (flag1 == 0)
						{
							Clock = Clock + 5;
							// cout << "inc: " << Clock << endl;
							flag1 = 1;
						}
						else
						{
							if (v3[v3.size() - 1].stalls != 0)
							{
								int d = Diff();
								if (d == 1)
									Clock = Clock + stall + 1;

								else if (d > 1)
								{
									stall = 0;
									Clock = Clock + 1;
								}
								else
								{
									stall = 0;
									Clock = Clock + 2;
								}

								// cout << "inc: " << Clock << endl;
							}
							else if (v3[v3.size() - 1].stalls == 0)
							{
								Clock = Clock + stall + 1;
								// cout << "inc: " << Clock << endl;
							}
						}
						if (sstr.top() != "jump" && sstr.top() != "SW")
						{

							if (sstr.top() == "LW")
							{
								cacheaccess++;
								// cout << "this is lw" << endl;
								int address[12] = {0};
								globalLRU++;
								int n = cacheindex;
								for (int i = 11; i >= 0; i--)
								{
									address[i] = n % 2;
									n = n / 2;
								}

								int setindex = BintoDec(address, 12 - setbits - offsetbits, 12 - offsetbits - 1);
								int offsetindex = BintoDec(address, 12 - offsetbits, 11);
								int addr = BintoDec(address, 0, tagbits - 1);
								int setindex2 = BintoDec(address, 12 - setbits2 - offsetbits2, 12 - offsetbits2 - 1);
								int offsetindex2 = BintoDec(address, 12 - offsetbits2, 11);
								int addr2 = BintoDec(address, 0, tagbits2 - 1);
								int flagz = 0, flagz2 = 0, flagi = -1, flagi2 = -1;
								for (int i = 0; i < c.s[0].no_of_blks; i++)
								{
									if (c.s[setindex].blks[i].tag == addr)
									{
										flagz = 1; //returns hit or miss
										flagi = i; // returns hit index
									}
								}
								for (int i = 0; i < c2.s[0].no_of_blks; i++)
								{
									if (c2.s[setindex2].blks[i].tag == addr2)
									{
										flagz2 = 1; //returns hit or miss
										flagi2 = i; // returns hit index
									}
								}
								if (flagz == 0) //miss in l1 cache
								{
									miss1++;
									if (flagz2 == 0) // miss in l2 cache
									{
										miss2++;
										for (int i = 0; i < c.s[0].no_of_blks; i++)
										{
											if (c.s[setindex].blks[i].tag == -1)
											{
												flagi = i; // returns where to insert index
												break;
											}
										}
										for (int i = 0; i < c2.s[0].no_of_blks; i++)
										{
											if (c2.s[setindex2].blks[i].tag == -1)
											{
												flagi2 = i; // returns where to insert index
												break;
											}
										}
										if (flagi != -1) // space in cache l1
										{
											int r = cacheindex / bs;
											c.s[setindex].blks[flagi].tag = addr;
											for (int i = r * bs; i < (r + 1) * bs; i++)
											{
												c.s[setindex].blks[flagi].o[i - ((r)*bs)].data = mem[i];
											}
											c.s[setindex].blks[flagi].lru = globalLRU;
											// cout << " miss and empty" << endl;
											if (flagi2 != -1) // also space in l2 cache
											{
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												// cout << " miss and empty in both caches" << endl;
											}
											else //not empty in l2 cache
											{
												int temp = c2.s[setindex2].blks[0].lru;
												flagi2 = 0;
												for (int i = 1; i < c2.s[0].no_of_blks; i++)
												{
													if (temp < c2.s[setindex2].blks[i].lru)
													{
														temp = c2.s[setindex2].blks[i].lru;
														flagi2 = i;
													}
												}
												int l2 = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
												for (int i = 0; i < c.no_of_sets; i++)
												{
													for (int j = 0; j < nb; j++)
													{
														int l1 = c.s[i].blks[j].tag * power(2, offsetbits + setbits) + i * power(2, offsetbits);
														if (l1 == l2)
														{
															if (c.s[i].blks[j].dirty == 1)
															{
																c2.s[setindex2].blks[flagi2].dirty = 1;
																c.s[i].blks[j].tag = -1;
																c.s[i].blks[j].lru = 0;
																for (int k = 0; k < ofs; k++)
																{
																	c2.s[setindex2].blks[flagi2].o[k].data = c.s[i].blks[j].o[k].data;
																}
															}
														}
													}
												}
												if (c2.s[setindex2].blks[flagi2].dirty == 1)
												{
													int mind = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
													for (int k = 0; k < ofs2; k++)
													{
														mem[mind + k] = c2.s[setindex2].blks[flagi2].o[k].data;
													}
												}
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												// cout << " miss and empty in l1 not empty in l2" << endl;
											}
										}
										else if (flagi == -1) // no space in l1 cache
										{
											int temp = c.s[setindex].blks[0].lru;
											flagi = 0;
											for (int i = 1; i < c.s[0].no_of_blks; i++)
											{
												if (temp < c.s[setindex].blks[i].lru)
												{
													temp = c.s[setindex].blks[i].lru;
													flagi = i;
												}
											}

											if (c.s[setindex].blks[flagi].dirty == 1)
											{
												int l1 = c.s[setindex].blks[flagi].tag * power(2, offsetbits + setbits) + setindex * power(2, offsetbits);
												for (int i = 0; i < c2.no_of_sets; i++)
												{
													for (int j = 0; j < nb2; j++)
													{
														int l2 = c2.s[i].blks[j].tag * power(2, offsetbits2 + setbits2) + i * power(2, offsetbits2);
														if (l1 == l2)
														{
															c2.s[setindex2].blks[flagi2].dirty = 1;
															c.s[i].blks[j].tag = -1;
															for (int k = 0; k < ofs; k++)
															{
																c2.s[i].blks[j].o[k].data = c.s[setindex].blks[flagi].o[k].data;
															}
															c2.s[i].blks[j].lru = c.s[setindex].blks[flagi].lru;
														}
													}
												}
											}
											int r = cacheindex / bs;
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = 0;
											for (int i = r * bs; i < (r + 1) * bs; i++)
											{
												c.s[setindex].blks[flagi].o[i - ((r)*bs)].data = mem[i];
											}
											c.s[setindex].blks[flagi].lru = globalLRU;
											// cout << "miss and not empty" << endl;
											if (flagi2 != -1) //  space in l2 cache
											{
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												// cout << " miss and empty in l2 not empty in l1" << endl;
											}
											else //not empty in l2 cache
											{
												int temp = c2.s[setindex2].blks[0].lru;
												flagi2 = 0;
												for (int i = 1; i < c2.s[0].no_of_blks; i++)
												{
													if (temp < c2.s[setindex2].blks[i].lru)
													{
														temp = c2.s[setindex2].blks[i].lru;
														flagi2 = i;
													}
												}
												int l2 = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
												for (int i = 0; i < c.no_of_sets; i++)
												{
													for (int j = 0; j < nb; j++)
													{
														int l1 = c.s[i].blks[j].tag * power(2, offsetbits + setbits) + i * power(2, offsetbits);
														if (l1 == l2)
														{
															if (c.s[i].blks[j].dirty == 1)
															{
																c2.s[setindex2].blks[flagi2].dirty = 1;
																c.s[i].blks[j].tag = -1;
																for (int k = 0; k < ofs; k++)
																{
																	c2.s[setindex2].blks[flagi2].o[k].data = c.s[i].blks[j].o[k].data;
																}
															}
															c.s[i].blks[j].lru = 0;
														}
													}
												}
												if (c2.s[setindex2].blks[flagi2].dirty == 1)
												{
													int mind = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
													for (int k = 0; k < ofs2; k++)
													{
														mem[mind + k] = c2.s[setindex2].blks[flagi2].o[k].data;
													}
												}
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												c2.s[setindex2].blks[flagi2].dirty = 0;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												// cout << " miss and not empty in both caches" << endl;
											}
										}
										Clock = Clock + h1 + h2 + memacc;
										stall = stall + h1 + h2 + memacc;
									}
									else if (flagz2 == 1) // hit in l2 cache and miss in l1 cache
									{
										for (int i = 0; i < c.s[0].no_of_blks; i++)
										{
											if (c.s[setindex].blks[i].tag == -1)
											{
												flagi = i; // returns where to insert index
												break;
											}
										}
										if (flagi != -1) //space in l1
										{
											for (int k = 0; k < ofs; k++)
											{
												c.s[setindex].blks[flagi].o[k].data = c2.s[setindex2].blks[flagi2].o[k].data;
											}
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = c2.s[setindex2].blks[flagi2].dirty;
											c.s[setindex].blks[flagi].lru = globalLRU;
											c2.s[setindex2].blks[flagi2].lru = globalLRU;
											// cout << "hit in l2 and miss in l1 and empty" << endl;
										}
										else //no space in l1
										{
											int temp = c.s[setindex].blks[0].lru;
											flagi = 0;
											for (int i = 1; i < c.s[0].no_of_blks; i++)
											{
												if (temp < c.s[setindex].blks[i].lru)
												{
													temp = c.s[setindex].blks[i].lru;
													flagi = i;
												}
											}
											if (c.s[setindex].blks[flagi].dirty == 1)
											{
												int l1 = c.s[setindex].blks[flagi].tag * power(2, offsetbits + setbits) + setindex * power(2, offsetbits);
												for (int i = 0; i < c2.no_of_sets; i++)
												{
													for (int j = 0; j < nb2; j++)
													{
														int l2 = c2.s[i].blks[j].tag * power(2, offsetbits2 + setbits2) + i * power(2, offsetbits2);
														if (l1 == l2)
														{
															c2.s[setindex2].blks[flagi2].dirty = 1;
															c.s[i].blks[j].tag = -1;
															c.s[i].blks[j].dirty = 0;
															for (int k = 0; k < ofs; k++)
															{
																c2.s[i].blks[j].o[k].data = c.s[setindex].blks[flagi].o[k].data;
															}
															c2.s[i].blks[j].lru = c.s[setindex].blks[flagi].lru;
														}
													}
												}
											}
											for (int k = 0; k < ofs; k++)
											{
												c.s[setindex].blks[flagi].o[k].data = c2.s[setindex2].blks[flagi2].o[k].data;
											}
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = c2.s[setindex2].blks[flagi2].dirty;
											c.s[setindex].blks[flagi].lru = globalLRU;
											c2.s[setindex2].blks[flagi2].lru = globalLRU;
										}
										Clock = Clock + h1 + h2;
										stall = stall + h1 + h2;
										// cout << "hit in l2 and miss in l1 and not empty" << endl;
									}
								}
								else //hit in l1 cache
								{
									//wb(c.s[setindex].blks[flagi].o[i].data);
									c.s[setindex].blks[flagi].lru = globalLRU;
									// cout << "hit in l1 cache" << endl;
									Clock = Clock + h1;
									stall = stall + h1;
								}
								sstr.pop();
								int a1 = MEM(c.s[setindex].blks[flagi].o[offsetindex].data);
								// cout << "mem: " << a1 << endl;
								WB(a1);
							}
							else
							{
								int a1 = MEM(res);
								// cout<<"mem: "<<a1<<endl;
								WB(a1);
							}
						}
						else
						{
							if (sstr.top() == "SW")
							{
								cacheaccess++;
								// cout << "this is sw" << endl;
								int address[12] = {0};
								globalLRU++;
								int n = cacheindex;
								for (int i = 11; i >= 0; i--)
								{
									address[i] = n % 2;
									n = n / 2;
								}

								int setindex = BintoDec(address, 12 - setbits - offsetbits, 12 - offsetbits - 1);
								int offsetindex = BintoDec(address, 12 - offsetbits, 11);
								int addr = BintoDec(address, 0, tagbits - 1);
								int setindex2 = BintoDec(address, 12 - setbits2 - offsetbits2, 12 - offsetbits2 - 1);
								int offsetindex2 = BintoDec(address, 12 - offsetbits2, 11);
								int addr2 = BintoDec(address, 0, tagbits2 - 1);
								int flagz = 0, flagz2 = 0, flagi = -1, flagi2 = -1;
								for (int i = 0; i < c.s[0].no_of_blks; i++)
								{
									if (c.s[setindex].blks[i].tag == addr)
									{
										flagz = 1; //returns hit or miss
										flagi = i; // returns hit index
									}
								}
								for (int i = 0; i < c2.s[0].no_of_blks; i++)
								{
									if (c2.s[setindex2].blks[i].tag == addr2)
									{
										flagz2 = 1; //returns hit or miss
										flagi2 = i; // returns hit index
									}
								}
								if (flagz == 0) //miss in l1 cache
								{
									miss1++;
									if (flagz2 == 0) // miss in l2 cache
									{
										miss2++;
										for (int i = 0; i < c.s[0].no_of_blks; i++)
										{
											if (c.s[setindex].blks[i].tag == -1)
											{
												flagi = i; // returns where to insert index
												break;
											}
										}
										for (int i = 0; i < c2.s[0].no_of_blks; i++)
										{
											if (c2.s[setindex2].blks[i].tag == -1)
											{
												flagi2 = i; // returns where to insert index
												break;
											}
										}
										if (flagi != -1) // space in cache l1
										{
											int r = cacheindex / bs;
											c.s[setindex].blks[flagi].tag = addr;
											for (int i = r * bs; i < (r + 1) * bs; i++)
											{
												c.s[setindex].blks[flagi].o[i - ((r)*bs)].data = mem[i];
											}
											c.s[setindex].blks[flagi].lru = globalLRU;
											if (R[a] != c.s[setindex].blks[flagi].o[offsetindex].data)
											{
												c.s[setindex].blks[flagi].dirty = 1;
												c.s[setindex].blks[flagi].o[offsetindex].data = R[a];
											}
											// cout << " miss and empty" << endl;
											if (flagi2 != -1) // also space in l2 cache
											{
												if (cacheindex >= 0 && cacheindex < 4096)
												{
													int r2 = cacheindex / bs2;
													c2.s[setindex2].blks[flagi2].tag = addr2;
													for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
													{
														c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
													}
													c2.s[setindex2].blks[flagi2].lru = globalLRU;
													if (R[a] != c2.s[setindex2].blks[flagi2].o[offsetindex2].data)
													{
														c2.s[setindex2].blks[flagi2].dirty = 1;
														c2.s[setindex2].blks[flagi2].o[offsetindex2].data = R[a];
													}
												}
												// cout << " miss and empty in both caches" << endl;
											}
											else //not empty in l2 cache
											{
												int temp = c2.s[setindex2].blks[0].lru;
												flagi2 = 0;
												for (int i = 1; i < c2.s[0].no_of_blks; i++)
												{
													if (temp < c2.s[setindex2].blks[i].lru)
													{
														temp = c2.s[setindex2].blks[i].lru;
														flagi2 = i;
													}
												}
												int l2 = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
												for (int i = 0; i < c.no_of_sets; i++)
												{
													for (int j = 0; j < nb; j++)
													{
														int l1 = c.s[i].blks[j].tag * power(2, offsetbits + setbits) + i * power(2, offsetbits);
														if (l1 == l2)
														{
															if (c.s[i].blks[j].dirty == 1)
															{
																c2.s[setindex2].blks[flagi2].dirty = 1;
																c.s[i].blks[j].tag = -1;
																c.s[i].blks[j].lru = 0;
																for (int k = 0; k < ofs; k++)
																{
																	c2.s[setindex2].blks[flagi2].o[k].data = c.s[i].blks[j].o[k].data;
																}
															}
														}
													}
												}
												if (c2.s[setindex2].blks[flagi2].dirty == 1)
												{
													int mind = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
													for (int k = 0; k < ofs2; k++)
													{
														mem[mind + k] = c2.s[setindex2].blks[flagi2].o[k].data;
													}
												}

												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												if (R[a] != c2.s[setindex2].blks[flagi2].o[offsetindex2].data)
												{
													c2.s[setindex2].blks[flagi2].dirty = 1;
													c2.s[setindex2].blks[flagi2].o[offsetindex2].data = R[a];
												}
												// cout << " miss and empty in l1 not empty in l2" << endl;
											}
										}
										else if (flagi == -1) // no space in l1 cache
										{
											int temp = c.s[setindex].blks[0].lru;
											flagi = 0;
											for (int i = 1; i < c.s[0].no_of_blks; i++)
											{
												if (temp < c.s[setindex].blks[i].lru)
												{
													temp = c.s[setindex].blks[i].lru;
													flagi = i;
												}
											}

											if (c.s[setindex].blks[flagi].dirty == 1)
											{
												int l1 = c.s[setindex].blks[flagi].tag * power(2, offsetbits + setbits) + setindex * power(2, offsetbits);
												for (int i = 0; i < c2.no_of_sets; i++)
												{
													for (int j = 0; j < nb2; j++)
													{
														int l2 = c2.s[i].blks[j].tag * power(2, offsetbits2 + setbits2) + i * power(2, offsetbits2);
														if (l1 == l2)
														{
															c2.s[setindex2].blks[flagi2].dirty = 1;
															c.s[i].blks[j].tag = -1;
															for (int k = 0; k < ofs; k++)
															{
																c2.s[i].blks[j].o[k].data = c.s[setindex].blks[flagi].o[k].data;
															}
															c2.s[i].blks[j].lru = c.s[setindex].blks[flagi].lru;
														}
													}
												}
											}
											int r = cacheindex / bs;
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = 0;
											for (int i = r * bs; i < (r + 1) * bs; i++)
											{
												c.s[setindex].blks[flagi].o[i - ((r)*bs)].data = mem[i];
											}
											if (R[a] != c.s[setindex].blks[flagi].o[offsetindex].data)
											{
												c.s[setindex].blks[flagi].dirty = 1;
												c.s[setindex].blks[flagi].o[offsetindex].data = R[a];
											}
											c.s[setindex].blks[flagi].lru = globalLRU;
											// cout << "miss and not empty" << endl;
											if (flagi2 != -1) //  space in l2 cache
											{
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												if (R[a] != c2.s[setindex2].blks[flagi2].o[offsetindex2].data)
												{
													c2.s[setindex2].blks[flagi2].dirty = 1;
													c2.s[setindex2].blks[flagi2].o[offsetindex2].data = R[a];
												}
												// cout << " miss and empty in l2 not empty in l1" << endl;
											}
											else //not empty in l2 cache
											{
												int temp = c2.s[setindex2].blks[0].lru;
												flagi2 = 0;
												for (int i = 1; i < c2.s[0].no_of_blks; i++)
												{
													if (temp < c2.s[setindex2].blks[i].lru)
													{
														temp = c2.s[setindex2].blks[i].lru;
														flagi2 = i;
													}
												}
												int l2 = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
												for (int i = 0; i < c.no_of_sets; i++)
												{
													for (int j = 0; j < nb; j++)
													{
														int l1 = c.s[i].blks[j].tag * power(2, offsetbits + setbits) + i * power(2, offsetbits);
														if (l1 == l2)
														{
															if (c.s[i].blks[j].dirty == 1)
															{
																c2.s[setindex2].blks[flagi2].dirty = 1;
																c.s[i].blks[j].tag = -1;
																for (int k = 0; k < ofs; k++)
																{
																	c2.s[setindex2].blks[flagi2].o[k].data = c.s[i].blks[j].o[k].data;
																}
															}
															c.s[i].blks[j].lru = 0;
														}
													}
												}
												if (c2.s[setindex2].blks[flagi2].dirty == 1)
												{
													int mind = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
													for (int k = 0; k < ofs2; k++)
													{
														mem[mind + k] = c2.s[setindex2].blks[flagi2].o[k].data;
													}
												}
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												c2.s[setindex2].blks[flagi2].dirty = 0;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												if (R[a] != c2.s[setindex2].blks[flagi2].o[offsetindex2].data)
												{
													c2.s[setindex2].blks[flagi2].dirty = 1;
													c2.s[setindex2].blks[flagi2].o[offsetindex2].data = R[a];
												}
												// cout << " miss and not empty in both caches" << endl;
											}
										}
										Clock = Clock + h1 + h2 + memacc;
										stall = stall + h1 + h2 + memacc;
									}
									else if (flagz2 == 1) // hit in l2 cache and miss in l1 cache
									{
										for (int i = 0; i < c.s[0].no_of_blks; i++)
										{
											if (c.s[setindex].blks[i].tag == -1)
											{
												flagi = i; // returns where to insert index
												break;
											}
										}
										if (flagi != -1) //space in l1
										{
											for (int k = 0; k < ofs; k++)
											{
												c.s[setindex].blks[flagi].o[k].data = c2.s[setindex2].blks[flagi2].o[k].data;
											}
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = c2.s[setindex2].blks[flagi2].dirty;
											c.s[setindex].blks[flagi].lru = globalLRU;
											c2.s[setindex2].blks[flagi2].lru = globalLRU;
											// cout << "hit in l2 and miss in l1 and empty" << endl;
										}
										else //no space in l1
										{
											int temp = c.s[setindex].blks[0].lru;
											flagi = 0;
											for (int i = 1; i < c.s[0].no_of_blks; i++)
											{
												if (temp < c.s[setindex].blks[i].lru)
												{
													temp = c.s[setindex].blks[i].lru;
													flagi = i;
												}
											}
											if (c.s[setindex].blks[flagi].dirty == 1)
											{
												int l1 = c.s[setindex].blks[flagi].tag * power(2, offsetbits + setbits) + setindex * power(2, offsetbits);
												for (int i = 0; i < c2.no_of_sets; i++)
												{
													for (int j = 0; j < nb2; j++)
													{
														int l2 = c2.s[i].blks[j].tag * power(2, offsetbits2 + setbits2) + i * power(2, offsetbits2);
														if (l1 == l2)
														{
															c2.s[setindex2].blks[flagi2].dirty = 1;
															c.s[i].blks[j].tag = -1;
															c.s[i].blks[j].dirty = 0;
															for (int k = 0; k < ofs; k++)
															{
																c2.s[i].blks[j].o[k].data = c.s[setindex].blks[flagi].o[k].data;
															}
															c2.s[i].blks[j].lru = c.s[setindex].blks[flagi].lru;
														}
													}
												}
											}
											for (int k = 0; k < ofs; k++)
											{
												c.s[setindex].blks[flagi].o[k].data = c2.s[setindex2].blks[flagi2].o[k].data;
											}
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = c2.s[setindex2].blks[flagi2].dirty;
											c.s[setindex].blks[flagi].lru = globalLRU;
											c2.s[setindex2].blks[flagi2].lru = globalLRU;
											if (R[a] != c.s[setindex].blks[flagi].o[offsetindex].data)
											{
												c.s[setindex].blks[flagi].dirty = 1;
												c.s[setindex].blks[flagi].o[offsetindex].data = R[a];
											}
										}
										// cout << "hit in l2 and miss in l1 and not empty" << endl;
										Clock = Clock + h1 + h2;
										stall = stall + h1 + h2;
									}
								}
								else //hit in l1 cache
								{
									//wb(c.s[setindex].blks[flagi].o[i].data);
									c.s[setindex].blks[flagi].lru = globalLRU;
									if (R[a] != c.s[setindex].blks[flagi].o[offsetindex].data)
									{
										c.s[setindex].blks[flagi].dirty = 1;
										c.s[setindex].blks[flagi].o[offsetindex].data = R[a];
									}
									// cout << "hit in l1 cache" << endl;
									Clock = Clock + h1;
									stall = stall + h1;
								}

								// cout << "mem: " << c.s[setindex].blks[flagi].o[offsetindex].data << endl;
							}
							else
							{
								pc = res;
							}
							sstr.pop();
						}
					}
					New.stalls = stall;
					New.lNo = pc;
					v3.push_back(New);
					if (stall != 0)
					{
						cout << s1 << "   ";
						cout << "stalls : " << stall << endl;
					}
				}
				pc++;
			}
			else if (opt == 2)
			{
				bal1();
				stal New;
				stall = checkDep1();
				New.stalls = stall;
				New.lNo = pc;
				v3.push_back(New);

				if (sstr.top() == "ADD" || sstr.top() == "SUB" || sstr.top() == "ADDI" || sstr.top() == "SUBI" || sstr.top() == "MULI" || sstr.top() == "BNE" || sstr.top() == "BEQ" || sstr.top() == "jump" || sstr.top() == "LW" || sstr.top() == "SW" || sstr.top() == "SGT")
				{
					ins++;
					res = EXE();
					if (sstr.top() == "BNE" || sstr.top() == "BEQ")
					{
						if (flag1 == 0)
						{
							Clock = Clock + stall + 6;
							// cout << "inc: " << Clock << endl;
							flag1 = 1;
						}
						else
						{
							if (v3[v3.size() - 1].stalls != 0)
							{
								int d = Diff();
								if (d == 1)
									Clock = Clock + stall + 2;

								else if (d > 1)
								{
									stall = 0;
									Clock = Clock + 2;
								}
								else
								{
									stall = 0;
									Clock = Clock + 2;
								}
								// cout << "inc: " << Clock << endl;
							}
							else
								Clock = Clock + stall + 2;
						}
						sstr.pop();
						if (res != -1)
						{
							pc = res;
						}
					}

					else
					{
						if (flag1 == 0)
						{
							Clock = Clock + 5;
							// cout << "inc: " << Clock << endl;
							flag1 = 1;
						}
						else
						{
							if (v3[v3.size() - 1].stalls != 0)
							{
								int d = Diff();
								if (d == 1)
									Clock = Clock + stall + 1;
								else if (d > 1)
									Clock = Clock + 1;
								// cout << "inc: " << Clock << endl;
							}
							else if (v3[v3.size() - 1].stalls == 0)
							{
								Clock = Clock + stall + 1;
								// cout << "inc: " << Clock << endl;
							}
							else
							{
								stall = 0;
								Clock = Clock + 2;
							}
						}
						if (sstr.top() != "jump" && sstr.top() != "SW")
						{
							if (sstr.top() == "LW")
							{
								cacheaccess++;
								// cout << "this is lw" << endl;
								int address[12] = {0};
								globalLRU++;
								int n = cacheindex;
								for (int i = 11; i >= 0; i--)
								{
									address[i] = n % 2;
									n = n / 2;
								}

								int setindex = BintoDec(address, 12 - setbits - offsetbits, 12 - offsetbits - 1);
								int offsetindex = BintoDec(address, 12 - offsetbits, 11);
								int addr = BintoDec(address, 0, tagbits - 1);
								int setindex2 = BintoDec(address, 12 - setbits2 - offsetbits2, 12 - offsetbits2 - 1);
								int offsetindex2 = BintoDec(address, 12 - offsetbits2, 11);
								int addr2 = BintoDec(address, 0, tagbits2 - 1);
								int flagz = 0, flagz2 = 0, flagi = -1, flagi2 = -1;
								for (int i = 0; i < c.s[0].no_of_blks; i++)
								{
									if (c.s[setindex].blks[i].tag == addr)
									{
										flagz = 1; //returns hit or miss
										flagi = i; // returns hit index
									}
								}
								for (int i = 0; i < c2.s[0].no_of_blks; i++)
								{
									if (c2.s[setindex2].blks[i].tag == addr2)
									{
										flagz2 = 1; //returns hit or miss
										flagi2 = i; // returns hit index
									}
								}
								if (flagz == 0) //miss in l1 cache
								{
									miss1++;
									if (flagz2 == 0) // miss in l2 cache
									{
										miss2++;
										for (int i = 0; i < c.s[0].no_of_blks; i++)
										{
											if (c.s[setindex].blks[i].tag == -1)
											{
												flagi = i; // returns where to insert index
												break;
											}
										}
										for (int i = 0; i < c2.s[0].no_of_blks; i++)
										{
											if (c2.s[setindex2].blks[i].tag == -1)
											{
												flagi2 = i; // returns where to insert index
												break;
											}
										}
										if (flagi != -1) // space in cache l1
										{
											int r = cacheindex / bs;
											c.s[setindex].blks[flagi].tag = addr;
											for (int i = r * bs; i < (r + 1) * bs; i++)
											{
												c.s[setindex].blks[flagi].o[i - ((r)*bs)].data = mem[i];
											}
											c.s[setindex].blks[flagi].lru = globalLRU;
											// cout << " miss and empty" << endl;
											if (flagi2 != -1) // also space in l2 cache
											{
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												// cout << " miss and empty in both caches" << endl;
											}
											else //not empty in l2 cache
											{
												int temp = c2.s[setindex2].blks[0].lru;
												flagi2 = 0;
												for (int i = 1; i < c2.s[0].no_of_blks; i++)
												{
													if (temp < c2.s[setindex2].blks[i].lru)
													{
														temp = c2.s[setindex2].blks[i].lru;
														flagi2 = i;
													}
												}
												int l2 = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
												for (int i = 0; i < c.no_of_sets; i++)
												{
													for (int j = 0; j < nb; j++)
													{
														int l1 = c.s[i].blks[j].tag * power(2, offsetbits + setbits) + i * power(2, offsetbits);
														if (l1 == l2)
														{
															if (c.s[i].blks[j].dirty == 1)
															{
																c2.s[setindex2].blks[flagi2].dirty = 1;
																c.s[i].blks[j].tag = -1;
																c.s[i].blks[j].lru = 0;
																for (int k = 0; k < ofs; k++)
																{
																	c2.s[setindex2].blks[flagi2].o[k].data = c.s[i].blks[j].o[k].data;
																}
															}
														}
													}
												}
												if (c2.s[setindex2].blks[flagi2].dirty == 1)
												{
													int mind = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
													for (int k = 0; k < ofs2; k++)
													{
														mem[mind + k] = c2.s[setindex2].blks[flagi2].o[k].data;
													}
												}
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												// cout << " miss and empty in l1 not empty in l2" << endl;
											}
										}
										else if (flagi == -1) // no space in l1 cache
										{
											int temp = c.s[setindex].blks[0].lru;
											flagi = 0;
											for (int i = 1; i < c.s[0].no_of_blks; i++)
											{
												if (temp < c.s[setindex].blks[i].lru)
												{
													temp = c.s[setindex].blks[i].lru;
													flagi = i;
												}
											}

											if (c.s[setindex].blks[flagi].dirty == 1)
											{
												int l1 = c.s[setindex].blks[flagi].tag * power(2, offsetbits + setbits) + setindex * power(2, offsetbits);
												for (int i = 0; i < c2.no_of_sets; i++)
												{
													for (int j = 0; j < nb2; j++)
													{
														int l2 = c2.s[i].blks[j].tag * power(2, offsetbits2 + setbits2) + i * power(2, offsetbits2);
														if (l1 == l2)
														{
															c2.s[setindex2].blks[flagi2].dirty = 1;
															c.s[i].blks[j].tag = -1;
															for (int k = 0; k < ofs; k++)
															{
																c2.s[i].blks[j].o[k].data = c.s[setindex].blks[flagi].o[k].data;
															}
															c2.s[i].blks[j].lru = c.s[setindex].blks[flagi].lru;
														}
													}
												}
											}
											int r = cacheindex / bs;
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = 0;
											for (int i = r * bs; i < (r + 1) * bs; i++)
											{
												c.s[setindex].blks[flagi].o[i - ((r)*bs)].data = mem[i];
											}
											c.s[setindex].blks[flagi].lru = globalLRU;
											// cout << "miss and not empty" << endl;
											if (flagi2 != -1) //  space in l2 cache
											{
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												// cout << " miss and empty in l2 not empty in l1" << endl;
											}
											else //not empty in l2 cache
											{
												int temp = c2.s[setindex2].blks[0].lru;
												flagi2 = 0;
												for (int i = 1; i < c2.s[0].no_of_blks; i++)
												{
													if (temp < c2.s[setindex2].blks[i].lru)
													{
														temp = c2.s[setindex2].blks[i].lru;
														flagi2 = i;
													}
												}
												int l2 = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
												for (int i = 0; i < c.no_of_sets; i++)
												{
													for (int j = 0; j < nb; j++)
													{
														int l1 = c.s[i].blks[j].tag * power(2, offsetbits + setbits) + i * power(2, offsetbits);
														if (l1 == l2)
														{
															if (c.s[i].blks[j].dirty == 1)
															{
																c2.s[setindex2].blks[flagi2].dirty = 1;
																c.s[i].blks[j].tag = -1;
																for (int k = 0; k < ofs; k++)
																{
																	c2.s[setindex2].blks[flagi2].o[k].data = c.s[i].blks[j].o[k].data;
																}
															}
															c.s[i].blks[j].lru = 0;
														}
													}
												}
												if (c2.s[setindex2].blks[flagi2].dirty == 1)
												{
													int mind = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
													for (int k = 0; k < ofs2; k++)
													{
														mem[mind + k] = c2.s[setindex2].blks[flagi2].o[k].data;
													}
												}
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												c2.s[setindex2].blks[flagi2].dirty = 0;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												// cout << " miss and not empty in both caches" << endl;
											}
										}
										Clock = Clock + h1 + h2 + memacc;
										stall = stall + h1 + h2 + memacc;
									}
									else if (flagz2 == 1) // hit in l2 cache and miss in l1 cache
									{
										for (int i = 0; i < c.s[0].no_of_blks; i++)
										{
											if (c.s[setindex].blks[i].tag == -1)
											{
												flagi = i; // returns where to insert index
												break;
											}
										}
										if (flagi != -1) //space in l1
										{
											for (int k = 0; k < ofs; k++)
											{
												c.s[setindex].blks[flagi].o[k].data = c2.s[setindex2].blks[flagi2].o[k].data;
											}
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = c2.s[setindex2].blks[flagi2].dirty;
											c.s[setindex].blks[flagi].lru = globalLRU;
											c2.s[setindex2].blks[flagi2].lru = globalLRU;
											// cout << "hit in l2 and miss in l1 and empty" << endl;
										}
										else //no space in l1
										{
											int temp = c.s[setindex].blks[0].lru;
											flagi = 0;
											for (int i = 1; i < c.s[0].no_of_blks; i++)
											{
												if (temp < c.s[setindex].blks[i].lru)
												{
													temp = c.s[setindex].blks[i].lru;
													flagi = i;
												}
											}
											if (c.s[setindex].blks[flagi].dirty == 1)
											{
												int l1 = c.s[setindex].blks[flagi].tag * power(2, offsetbits + setbits) + setindex * power(2, offsetbits);
												for (int i = 0; i < c2.no_of_sets; i++)
												{
													for (int j = 0; j < nb2; j++)
													{
														int l2 = c2.s[i].blks[j].tag * power(2, offsetbits2 + setbits2) + i * power(2, offsetbits2);
														if (l1 == l2)
														{
															c2.s[setindex2].blks[flagi2].dirty = 1;
															c.s[i].blks[j].tag = -1;
															c.s[i].blks[j].dirty = 0;
															for (int k = 0; k < ofs; k++)
															{
																c2.s[i].blks[j].o[k].data = c.s[setindex].blks[flagi].o[k].data;
															}
															c2.s[i].blks[j].lru = c.s[setindex].blks[flagi].lru;
														}
													}
												}
											}
											for (int k = 0; k < ofs; k++)
											{
												c.s[setindex].blks[flagi].o[k].data = c2.s[setindex2].blks[flagi2].o[k].data;
											}
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = c2.s[setindex2].blks[flagi2].dirty;
											c.s[setindex].blks[flagi].lru = globalLRU;
											c2.s[setindex2].blks[flagi2].lru = globalLRU;
										}
										Clock = Clock + h1 + h2;
										stall = stall + h1 + h2;
										// cout << "hit in l2 and miss in l1 and not empty" << endl;
									}
								}
								else //hit in l1 cache
								{
									//wb(c.s[setindex].blks[flagi].o[i].data);
									c.s[setindex].blks[flagi].lru = globalLRU;
									// cout << "hit in l1 cache" << endl;
									Clock = Clock + h1;
									stall = stall + h1;
								}
								sstr.pop();
								int a1 = MEM(c.s[setindex].blks[flagi].o[offsetindex].data);
								// cout << "mem: " << a1 << endl;
								WB(a1);
							}
							else
							{
								int a1 = MEM(res);
								// cout<<"mem: "<<a1<<endl;
								WB(a1);
							}
						}
						else
						{
							if (sstr.top() == "jump")
							{
								pc = res;
							}
							else if (sstr.top() == "SW")
							{
								cacheaccess++;
								// cout << "this is sw" << endl;
								int address[12] = {0};
								globalLRU++;
								int n = cacheindex;
								for (int i = 11; i >= 0; i--)
								{
									address[i] = n % 2;
									n = n / 2;
								}

								int setindex = BintoDec(address, 12 - setbits - offsetbits, 12 - offsetbits - 1);
								int offsetindex = BintoDec(address, 12 - offsetbits, 11);
								int addr = BintoDec(address, 0, tagbits - 1);
								int setindex2 = BintoDec(address, 12 - setbits2 - offsetbits2, 12 - offsetbits2 - 1);
								int offsetindex2 = BintoDec(address, 12 - offsetbits2, 11);
								int addr2 = BintoDec(address, 0, tagbits2 - 1);
								int flagz = 0, flagz2 = 0, flagi = -1, flagi2 = -1;
								for (int i = 0; i < c.s[0].no_of_blks; i++)
								{
									if (c.s[setindex].blks[i].tag == addr)
									{
										flagz = 1; //returns hit or miss
										flagi = i; // returns hit index
									}
								}
								for (int i = 0; i < c2.s[0].no_of_blks; i++)
								{
									if (c2.s[setindex2].blks[i].tag == addr2)
									{
										flagz2 = 1; //returns hit or miss
										flagi2 = i; // returns hit index
									}
								}
								if (flagz == 0) //miss in l1 cache
								{
									miss1++;
									if (flagz2 == 0) // miss in l2 cache
									{
										miss2++;
										for (int i = 0; i < c.s[0].no_of_blks; i++)
										{
											if (c.s[setindex].blks[i].tag == -1)
											{
												flagi = i; // returns where to insert index
												break;
											}
										}
										for (int i = 0; i < c2.s[0].no_of_blks; i++)
										{
											if (c2.s[setindex2].blks[i].tag == -1)
											{
												flagi2 = i; // returns where to insert index
												break;
											}
										}
										if (flagi != -1) // space in cache l1
										{
											int r = cacheindex / bs;
											c.s[setindex].blks[flagi].tag = addr;
											for (int i = r * bs; i < (r + 1) * bs; i++)
											{
												c.s[setindex].blks[flagi].o[i - ((r)*bs)].data = mem[i];
											}
											c.s[setindex].blks[flagi].lru = globalLRU;
											if (R[a] != c.s[setindex].blks[flagi].o[offsetindex].data)
											{
												c.s[setindex].blks[flagi].dirty = 1;
												c.s[setindex].blks[flagi].o[offsetindex].data = R[a];
											}
											// cout << " miss and empty" << endl;
											if (flagi2 != -1) // also space in l2 cache
											{
												if (cacheindex >= 0 && cacheindex < 4096)
												{
													int r2 = cacheindex / bs2;
													c2.s[setindex2].blks[flagi2].tag = addr2;
													for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
													{
														c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
													}
													c2.s[setindex2].blks[flagi2].lru = globalLRU;
													if (R[a] != c2.s[setindex2].blks[flagi2].o[offsetindex2].data)
													{
														c2.s[setindex2].blks[flagi2].dirty = 1;
														c2.s[setindex2].blks[flagi2].o[offsetindex2].data = R[a];
													}
												}
												// cout << " miss and empty in both caches" << endl;
											}
											else //not empty in l2 cache
											{
												int temp = c2.s[setindex2].blks[0].lru;
												flagi2 = 0;
												for (int i = 1; i < c2.s[0].no_of_blks; i++)
												{
													if (temp < c2.s[setindex2].blks[i].lru)
													{
														temp = c2.s[setindex2].blks[i].lru;
														flagi2 = i;
													}
												}
												int l2 = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
												for (int i = 0; i < c.no_of_sets; i++)
												{
													for (int j = 0; j < nb; j++)
													{
														int l1 = c.s[i].blks[j].tag * power(2, offsetbits + setbits) + i * power(2, offsetbits);
														if (l1 == l2)
														{
															if (c.s[i].blks[j].dirty == 1)
															{
																c2.s[setindex2].blks[flagi2].dirty = 1;
																c.s[i].blks[j].tag = -1;
																c.s[i].blks[j].lru = 0;
																for (int k = 0; k < ofs; k++)
																{
																	c2.s[setindex2].blks[flagi2].o[k].data = c.s[i].blks[j].o[k].data;
																}
															}
														}
													}
												}
												if (c2.s[setindex2].blks[flagi2].dirty == 1)
												{
													int mind = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
													for (int k = 0; k < ofs2; k++)
													{
														mem[mind + k] = c2.s[setindex2].blks[flagi2].o[k].data;
													}
												}

												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												if (R[a] != c2.s[setindex2].blks[flagi2].o[offsetindex2].data)
												{
													c2.s[setindex2].blks[flagi2].dirty = 1;
													c2.s[setindex2].blks[flagi2].o[offsetindex2].data = R[a];
												}
												// cout << " miss and empty in l1 not empty in l2" << endl;
											}
										}
										else if (flagi == -1) // no space in l1 cache
										{
											int temp = c.s[setindex].blks[0].lru;
											flagi = 0;
											for (int i = 1; i < c.s[0].no_of_blks; i++)
											{
												if (temp < c.s[setindex].blks[i].lru)
												{
													temp = c.s[setindex].blks[i].lru;
													flagi = i;
												}
											}

											if (c.s[setindex].blks[flagi].dirty == 1)
											{
												int l1 = c.s[setindex].blks[flagi].tag * power(2, offsetbits + setbits) + setindex * power(2, offsetbits);
												for (int i = 0; i < c2.no_of_sets; i++)
												{
													for (int j = 0; j < nb2; j++)
													{
														int l2 = c2.s[i].blks[j].tag * power(2, offsetbits2 + setbits2) + i * power(2, offsetbits2);
														if (l1 == l2)
														{
															c2.s[setindex2].blks[flagi2].dirty = 1;
															c.s[i].blks[j].tag = -1;
															for (int k = 0; k < ofs; k++)
															{
																c2.s[i].blks[j].o[k].data = c.s[setindex].blks[flagi].o[k].data;
															}
															c2.s[i].blks[j].lru = c.s[setindex].blks[flagi].lru;
														}
													}
												}
											}
											int r = cacheindex / bs;
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = 0;
											for (int i = r * bs; i < (r + 1) * bs; i++)
											{
												c.s[setindex].blks[flagi].o[i - ((r)*bs)].data = mem[i];
											}
											if (R[a] != c.s[setindex].blks[flagi].o[offsetindex].data)
											{
												c.s[setindex].blks[flagi].dirty = 1;
												c.s[setindex].blks[flagi].o[offsetindex].data = R[a];
											}
											c.s[setindex].blks[flagi].lru = globalLRU;
											// cout << "miss and not empty" << endl;
											if (flagi2 != -1) //  space in l2 cache
											{
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												if (R[a] != c2.s[setindex2].blks[flagi2].o[offsetindex2].data)
												{
													c2.s[setindex2].blks[flagi2].dirty = 1;
													c2.s[setindex2].blks[flagi2].o[offsetindex2].data = R[a];
												}
												// cout << " miss and empty in l2 not empty in l1" << endl;
											}
											else //not empty in l2 cache
											{
												int temp = c2.s[setindex2].blks[0].lru;
												flagi2 = 0;
												for (int i = 1; i < c2.s[0].no_of_blks; i++)
												{
													if (temp < c2.s[setindex2].blks[i].lru)
													{
														temp = c2.s[setindex2].blks[i].lru;
														flagi2 = i;
													}
												}
												int l2 = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
												for (int i = 0; i < c.no_of_sets; i++)
												{
													for (int j = 0; j < nb; j++)
													{
														int l1 = c.s[i].blks[j].tag * power(2, offsetbits + setbits) + i * power(2, offsetbits);
														if (l1 == l2)
														{
															if (c.s[i].blks[j].dirty == 1)
															{
																c2.s[setindex2].blks[flagi2].dirty = 1;
																c.s[i].blks[j].tag = -1;
																for (int k = 0; k < ofs; k++)
																{
																	c2.s[setindex2].blks[flagi2].o[k].data = c.s[i].blks[j].o[k].data;
																}
															}
															c.s[i].blks[j].lru = 0;
														}
													}
												}
												if (c2.s[setindex2].blks[flagi2].dirty == 1)
												{
													int mind = c2.s[setindex2].blks[flagi2].tag * power(2, offsetbits2 + setbits2) + setindex2 * power(2, offsetbits2);
													for (int k = 0; k < ofs2; k++)
													{
														mem[mind + k] = c2.s[setindex2].blks[flagi2].o[k].data;
													}
												}
												int r2 = cacheindex / bs2;
												c2.s[setindex2].blks[flagi2].tag = addr2;
												c2.s[setindex2].blks[flagi2].dirty = 0;
												for (int i = r2 * bs2; i < (r2 + 1) * bs2; i++)
												{
													c2.s[setindex2].blks[flagi2].o[i - ((r2)*bs2)].data = mem[i];
												}
												c.s[setindex].blks[flagi].lru = globalLRU;
												c2.s[setindex2].blks[flagi2].lru = globalLRU;
												if (R[a] != c2.s[setindex2].blks[flagi2].o[offsetindex2].data)
												{
													c2.s[setindex2].blks[flagi2].dirty = 1;
													c2.s[setindex2].blks[flagi2].o[offsetindex2].data = R[a];
												}
												// cout << " miss and not empty in both caches" << endl;
											}
										}
										Clock = Clock + h1 + h2 + memacc;
										stall = stall + h1 + h2 + memacc;
									}
									else if (flagz2 == 1) // hit in l2 cache and miss in l1 cache
									{
										for (int i = 0; i < c.s[0].no_of_blks; i++)
										{
											if (c.s[setindex].blks[i].tag == -1)
											{
												flagi = i; // returns where to insert index
												break;
											}
										}
										if (flagi != -1) //space in l1
										{
											for (int k = 0; k < ofs; k++)
											{
												c.s[setindex].blks[flagi].o[k].data = c2.s[setindex2].blks[flagi2].o[k].data;
											}
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = c2.s[setindex2].blks[flagi2].dirty;
											c.s[setindex].blks[flagi].lru = globalLRU;
											c2.s[setindex2].blks[flagi2].lru = globalLRU;
											// cout << "hit in l2 and miss in l1 and empty" << endl;
										}
										else //no space in l1
										{
											int temp = c.s[setindex].blks[0].lru;
											flagi = 0;
											for (int i = 1; i < c.s[0].no_of_blks; i++)
											{
												if (temp < c.s[setindex].blks[i].lru)
												{
													temp = c.s[setindex].blks[i].lru;
													flagi = i;
												}
											}
											if (c.s[setindex].blks[flagi].dirty == 1)
											{
												int l1 = c.s[setindex].blks[flagi].tag * power(2, offsetbits + setbits) + setindex * power(2, offsetbits);
												for (int i = 0; i < c2.no_of_sets; i++)
												{
													for (int j = 0; j < nb2; j++)
													{
														int l2 = c2.s[i].blks[j].tag * power(2, offsetbits2 + setbits2) + i * power(2, offsetbits2);
														if (l1 == l2)
														{
															c2.s[setindex2].blks[flagi2].dirty = 1;
															c.s[i].blks[j].tag = -1;
															c.s[i].blks[j].dirty = 0;
															for (int k = 0; k < ofs; k++)
															{
																c2.s[i].blks[j].o[k].data = c.s[setindex].blks[flagi].o[k].data;
															}
															c2.s[i].blks[j].lru = c.s[setindex].blks[flagi].lru;
														}
													}
												}
											}
											for (int k = 0; k < ofs; k++)
											{
												c.s[setindex].blks[flagi].o[k].data = c2.s[setindex2].blks[flagi2].o[k].data;
											}
											c.s[setindex].blks[flagi].tag = addr;
											c.s[setindex].blks[flagi].dirty = c2.s[setindex2].blks[flagi2].dirty;
											c.s[setindex].blks[flagi].lru = globalLRU;
											c2.s[setindex2].blks[flagi2].lru = globalLRU;
											if (R[a] != c.s[setindex].blks[flagi].o[offsetindex].data)
											{
												c.s[setindex].blks[flagi].dirty = 1;
												c.s[setindex].blks[flagi].o[offsetindex].data = R[a];
											}
										}
										// cout << "hit in l2 and miss in l1 and not empty" << endl;
										Clock = Clock + h1 + h2;
										stall = stall + h1 + h2;
									}
								}
								else //hit in l1 cache
								{
									//wb(c.s[setindex].blks[flagi].o[i].data);
									c.s[setindex].blks[flagi].lru = globalLRU;
									if (R[a] != c.s[setindex].blks[flagi].o[offsetindex].data)
									{
										c.s[setindex].blks[flagi].dirty = 1;
										c.s[setindex].blks[flagi].o[offsetindex].data = R[a];
									}
									// cout << "hit in l1 cache" << endl;
									Clock = Clock + h1;
									stall = stall + h1;
								}
							}
							sstr.pop();
						}
					}
					New.stalls = stall;
					New.lNo = pc;
					v3.push_back(New);
					if (stall != 0)
					{
						cout << s1 << "   ";
						cout << "stall : " << stall << endl;
					}
				}
				pc++;
			}
			TotalStalls += stall;
		}
		else
		{
			pc = v.size() + 1;
		}
	}
	for (int i = 0; i < c.no_of_sets; i++)
	{

		for (int j = 0; j < nb; j++)
		{
			if (c.s[i].blks[j].dirty == 1)
			{
				int l1 = i * power(2, offsetbits) + (c.s[i].blks[j].tag) * power(2, offsetbits + setbits);
				int a1[12 - offsetbits] = {0};
				for (int i = 12 - offsetbits - 1; i >= 0; i--)
				{
					a1[i] = l1 % 2;
					l1 = l1 / 2;
				}
				int s1 = BintoDec(a1, 12 - setbits2 - offsetbits2 - 1, 11 - offsetbits2);
				for (int k = 0; k < c2.s[0].no_of_blks; k++)
				{
					if (c2.s[s1].blks[k].tag == BintoDec(a1, 0, tagbits2 - 1))
					{
						c2.s[s1].blks[k].dirty == 0;
						break;
					}
				}

				for (int k = 0; k < ofs; k++)
				{

					int mind = k + i * power(2, offsetbits) + (c.s[i].blks[j].tag) * power(2, offsetbits + setbits);
					mem[mind] = c.s[i].blks[j].o[k].data;
				}
			}
		}
	}
	for (int i = 0; i < c2.no_of_sets; i++)
	{

		for (int j = 0; j < nb; j++)
		{
			if (c2.s[i].blks[j].dirty == 1)
			{
				for (int k = 0; k < ofs; k++)
				{

					int mind = k + i * power(2, offsetbits2) + (c.s[i].blks[j].tag) * power(2, offsetbits2 + setbits2);
					mem[mind] = c2.s[i].blks[j].o[k].data;
				}
			}
		}
	}
	print();
	cout<<endl;
	float IPC = (float)ins / Clock;
	cout << "Total Number of Instructions : " << ins << endl;
	cout << "Instructions per clock cycles : " << IPC << endl;
	cout << "Number of clock cycles : " << Clock << endl;
	cout << "Total number of stalls : " << TotalStalls << endl;
	float miss = miss2;
	float access = cacheaccess;
	float missrate = (miss / access) * 100;
	cout << "cacheaccesses =" << cacheaccess << endl;
	cout <<  " total misses =" << miss2 << endl;
	cout << "miss rate=" << missrate << endl;
	return 0;
}
