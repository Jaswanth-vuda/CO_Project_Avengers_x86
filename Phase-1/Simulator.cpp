#include <bits/stdc++.h>
// #include "testStk.h"
using namespace std;

int R[32];
char mem[4096] = {'0'};
string fName;
int cnt = 0;
int index1 = 0;
int flag = 0;

ifstream in1;
ifstream in;

class Instruction_Exec
{
public:
    stack<string> sstr;
    stack<int> sint;
    stack<string> labH;
    stack<string> dataLab;
    stack<int> dataNum;
    stack<string> dataStr;
    
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
// checks if a number is a multiple of 4
    bool isMult(int i)
    {
        if (i % 4 == 0)
            return true;
        else
            return false;
    }
// check if a word is string or not
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
        // int size = dataNum.size();
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
    void decToHexSW(int n,int i1)
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
            mem[i1+1] = '0';
            mem[i1+2] = '0';
            mem[i1+3] = hexaDeciNum[i - 1];
        }
        else if (i == 2)
        {
            mem[i1+3] = hexaDeciNum[i - 2];
            mem[i1+2] = hexaDeciNum[i - 1];
            mem[i1+1] = '0';
            mem[i1] = '0';
            
        }
        else if (i == 3)
        {
            mem[i1+3] = hexaDeciNum[i - 3];
            mem[i1+2] = hexaDeciNum[i - 2];
            mem[i1+1] = hexaDeciNum[i - 1];
            mem[i1] = '0';
            
        }
        else if (i == 4)
        {
            mem[i1+3] = hexaDeciNum[i - 3];
            mem[i1+2] = hexaDeciNum[i - 2];
            mem[i1+1] = hexaDeciNum[i - 1];
            mem[i1] = hexaDeciNum[i];
        }
    }
// traverses the region between data and text segments and stores in memory
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
            // cout << line << endl;
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
                                for(int i = 0;i<word.length() - 1;i++)
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
// handles all labels using stack
    void parse_label(string fileName, string lab)
    {

        string line1;
        string l11 = "";
        in1.open(fileName);
        while (in1)
        {
            while (l11 != (lab + ":"))
            {
                getline(in1, l11);
            }
            getline(in1, line1);
            // cout << line1;
            Inst(line1);
        }

        in1.close();
    }
// handles all instructions in main of a file
    void Inst(string str)
    {

        string word = "";
        sstr.push("sample");
        int l = str.length();
        char c1[l];

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

                    if (word == "ADD" || word == "SUB" || word == "ADDI" || word == "SUBI" || word == "LW" || word == "SW" || word == "BNE" || word == "BEQ" || word == "jump"||word=="SGT"||word=="MULI")
                    {

                        sstr.push(word);
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
                            if(isString(t))
                            {
                            //    cout << "word in else: " << t << endl;
                                int i = atoi(&t[1]);
                                sint.push(R[i]);
                                // cout << R[i] << endl;
                                word= "";
                            }
                            else{
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
                                // cout << i << endl;
                                word = "";
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
        int a, b, c;
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
        // if (sstr.size() == 2)
        // {
// executing each instruction
        if (sstr.top() == "ADD")
        {
            R[a] = R[b] + R[c];
            sstr.pop();
            // cout << "top element after pop : " << sstr.top() << endl;
        }
        else if (sstr.top() == "SUB")
        {
            R[a] = R[b] - R[c];
            sstr.pop();
            // cout << "top element after pop : " << sstr.top() << endl;
        }
        else if (sstr.top() == "ADDI")
        {
            R[a] = R[b] + c;
            sstr.pop();
            // cout << "top element after pop : " << sstr.top() << endl;
        }

        else if (sstr.top() == "SUBI")
        {
            R[a] = R[b] - c;
            sstr.pop();
            // cout << "top element after pop : " << sstr.top() << endl;
        }
        else if(sstr.top() == "MULI")
        {
           R[a]=R[b]*c;
           sstr.pop();
            // cout << "top element after pop : " << sstr.top() << endl;
        }
        else if (sstr.top() == "LW")
        {
            int temp_addr = b;
            if (temp_addr >= 0 && temp_addr < 4096)
            {
                char a1[4];
                for(int i=0;i<4;i++)
                {
                    a1[i] = mem[temp_addr+i];
                }
                int res = convert(a1);
                R[a] = res;
            }
            sstr.pop();
        }

        else if (sstr.top() == "SW")
        {
            int temp_addr = b;
            if (temp_addr >= 0 && temp_addr < 4096){
                decToHexSW(R[a],temp_addr);
            }
                
            sstr.pop();
        }
        else if (sstr.top() == "BNE")
        {
            cout << "Values of R2 and R3: " << R[2] << "   " << R[3] << endl;
            if (R[a] != R[b])
            {
                in.close();
                in1.close();
                string s1 = labH.top();
                parse_label(fName, s1);
            }
            sstr.pop();
            // cout << "top element after pop : " << sstr.top() << endl;
        }
        else if (sstr.top() == "BEQ")
        {
            // cout << "Values of R2 and R3 (BEQ): " << R[2] << "   " << R[3] << endl;
            if (R[a] == R[b])
            {
                in.close();
                in1.close();
                string s1 = labH.top();
                // cout << "label : " << labH.top() << endl;
                parse_label(fName, s1);
            }
            sstr.pop();
            // cout << "top element after pop : " << sstr.top() << endl;
        }
        else if(sstr.top()=="SGT")
        {
          if(R[b]>=R[c])
          {
              R[c]=R[b];
              R[a]=R[16];
          }
        }
        else if (sstr.top() == "jump")
        {
           // in.close();
            in1.close();
            string s2 = labH.top();
            // cout << "after jump: " << s2 << endl;
            parse_label(fName, s2);
        }
    }
};

class Parser
{
    Instruction_Exec obj;

public:
// reads the file line by line
    void parser(string s, int n)
    {
        obj.parse_data(fName, ".data", ".text");
        string line;
        string l1;

        if (n >= 5)
        {
            if ((s[n - 4] == '.' && s[n - 3] == 'a' && s[n - 2] == 's' && s[n - 1] == 'm'))
            {
                in.open(s);

                while (in)
                {
                    while (l1 != "main:")
                    {
                        getline(in, l1);
                    }
                    getline(in, line);
                    if (line == "exit,")
                        in.close();
                    else
                        obj.Inst(line);
                }
                print();
                // in.close();
            }
            else
                cout << "Not an assembly language !!" << endl;
        }
        else if (n >= 3)
        {
            if ((s[n - 2] == '.' && s[n - 1] == 'a') || (s[n - 2] == '.' && s[n - 1] == 's'))
            {
                in.open(s);

                while (in)
                {
                    while (l1 != "main:")
                    {
                        getline(in, l1);
                    }
                    getline(in, line);
                    if (line == "exit,")
                    {
         
                        in.close();
                    }
                  
                    else
                    {
                        obj.Inst(line);
                    }
                }
                print();
              
            }
            else
                cout << "Not an assembly language !!" << endl;
        }
        else
            cout << "Invalid file :(" << endl;
    }
// prints the contents of registers and memory
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
        cout<<"Data Segment : "<<endl;
        for (int j = 0; j < 4096; j = j + 4)
        {
            cout << "mem[ " << j << " ]  =  0x" << mem[j] << mem[j + 1] << mem[j + 2] << mem[j + 3] << "\t\t ";
        }
    }
};

int main()
{
    for (int i = 0; i < 4096; i++)
    {
        mem[i] = '0';
    }
    Parser p;
    cout << "Enter the file name to be opened : ";
    cin >> fName;
    int n = fName.length();

    p.parser(fName, n);

    return 0;
}
