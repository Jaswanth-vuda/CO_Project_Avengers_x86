#include <bits/stdc++.h>
using namespace std;
int pc = 0;
string l;
int Clock, index1;
ifstream in;
int flag = 0;

stack<string> sstr;
stack<int> sint;
stack<string> labH;
stack<string> dataLab;
stack<int> dataNum;
stack<string> dataStr;

int a;
int R[32];
char mem[4096];
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

// converts decimal to hexdecimal and stores in datasegment/memory
void decToHexa(int n)
{
    char hexaDeciNum[4];
    int i = 0;
    while (n != 0)
    {
        int temp = 0;
        temp = n % 16;
        if (temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }

        n = n / 16;
    }

    if (i == 1)
    {
        mem[index1] = hexaDeciNum[i - 1];
        index1 = index1 - 4;
    }
    else if (i == 2)
    {
        mem[index1 - 1] = hexaDeciNum[i - 1];
        mem[index1] = hexaDeciNum[i - 2];
        index1 = index1 - 4;
    }
    else if (i == 3)
    {
        mem[index1 - 2] = hexaDeciNum[i - 1];
        mem[index1 - 1] = hexaDeciNum[i - 2];
        mem[index1] = hexaDeciNum[i - 3];
        index1 = index1 - 4;
    }
    else if (i == 4)
    {
        mem[index1 - 3] = hexaDeciNum[i - 1];
        mem[index1 - 2] = hexaDeciNum[i - 2];
        mem[index1 - 1] = hexaDeciNum[i - 3];
        mem[index1] = hexaDeciNum[i - 4];
        index1 = index1 - 4;
    }
}
// converts dec to hexadecimal and stores or re-write memory using initial index
void decToHexSW(int n, int i1)
{
    char hexaDeciNum[4];
    int i = 0;
    while (n != 0)
    {
        int temp = 0;
        temp = n % 16;
        if (temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }

        n = n / 16;
    }
    if (i == 1)
    {
        mem[i1] = '0';
        mem[i1 + 1] = '0';
        mem[i1 + 2] = '0';
        mem[i1 + 3] = hexaDeciNum[i - 1];
    }
    else if (i == 2)
    {
        mem[i1 + 3] = hexaDeciNum[i - 2];
        mem[i1 + 2] = hexaDeciNum[i - 1];
        mem[i1 + 1] = '0';
        mem[i1] = '0';
    }
    else if (i == 3)
    {
        mem[i1 + 3] = hexaDeciNum[i - 3];
        mem[i1 + 2] = hexaDeciNum[i - 2];
        mem[i1 + 1] = hexaDeciNum[i - 1];
        mem[i1] = '0';
    }
    else if (i == 4)
    {
        mem[i1 + 3] = hexaDeciNum[i - 3];
        mem[i1 + 2] = hexaDeciNum[i - 2];
        mem[i1 + 1] = hexaDeciNum[i - 1];
        mem[i1] = hexaDeciNum[i];
    }
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
                        index1 = index1 + 4 * size - 1;
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
        int res;
        if (temp_addr >= 0 && temp_addr < 4096)
        {
            char a1[4];
            for (int i = 0; i < 4; i++)
            {
                a1[i] = mem[temp_addr + i];
            }
            res = convert(a1);
        }
        sstr.pop();
        d = res;
        cout << "exe: " << d << endl;
    }

    else if (sstr.top() == "SW")
    {
        int temp_addr = b;
        if (temp_addr >= 0 && temp_addr < 4096)
        {
            decToHexSW(R[a], temp_addr);
        }

        // sstr.pop();
        d = -2;
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
        int z;
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
    cout<<"REGISTERS : "<<endl;
    for (int i = 0; i < 32; i++)
    {
        if (i >= 0 && i <= 9)
            cout << "R" << i << "    = " << R[i] << endl;
        else
            cout << "R" << (i) << "   = " << R[i] << endl;
    }
    // // Memory
    cout<<"DATA-SEGMENT : "<<endl;
    for (int j = 0; j < 100; j = j + 4)
    {
        cout << "mem[ " << j << " ]  =  0x" << mem[j] << mem[j + 1] << mem[j + 2] << mem[j + 3] << "\t\t ";
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
                        stall = 3;
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
                        if (v1[i].line == "LW")
                            stall = 2;
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
    for (int i = 0; i < 4096; i++)
    {
        mem[i] = '0';
    }
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
    cout<<"Instructions with Stalls : "<<endl;

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

                if (sstr.top() == "ADD" || sstr.top() == "SUB" || sstr.top() == "ADDI" || sstr.top() == "SUBI" || sstr.top() == "MULI" || sstr.top() == "BNE" || sstr.top() == "BEQ" || sstr.top() == "jump" || sstr.top() == "LW" || sstr.top() == "SW")
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
                        if (sstr.top() != "jump" || sstr.top() != "SW")
                        {
                            int a1 = MEM(res);
                            // cout<<"mem: "<<a1<<endl;
                            WB(a1);
                        }
                        else
                            sstr.pop();
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

                if (sstr.top() == "ADD" || sstr.top() == "SUB" || sstr.top() == "ADDI" || sstr.top() == "SUBI" || sstr.top() == "MULI" || sstr.top() == "BNE" || sstr.top() == "BEQ" || sstr.top() == "jump" || sstr.top() == "LW" || sstr.top() == "SW")
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
                        if (sstr.top() != "jump" || sstr.top() != "SW")
                        {
                            int a1 = MEM(res);
                            // cout<<"mem: "<<a1<<endl;
                            WB(a1);
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
    print();
    cout<<endl;
    float IPC = (float)ins/Clock;
    cout << "Total Number of Instructions : " << ins << endl;
    cout<< "Instructions per clock cycles : "<< IPC<< endl;
    cout << "Number of clock cycles : " << Clock << endl;
    cout << "Total number of stalls : " << TotalStalls << endl;
    return 0;
}