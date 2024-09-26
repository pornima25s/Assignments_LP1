#include <bits/stdc++.h>

#include <string>
using namespace std;

class ICgenerator
{
    map<string, pair<string, string>> MOT;
    map<string, string> REG;
    map<string, string> CONDITIONALS;
    vector<pair<string, int>> symtab;
    vector<pair<string, int>> littab;
    ofstream output;
    int lc;
    int symbol_cnt = 0;
    int literal_cnt = 0;

    friend class MachineCodeGen;

    void makeMOT()
    {
        MOT["STOP"] = {"IS", "00"};
        MOT["ADD"] = {"IS", "01"};
        MOT["SUB"] = {"IS", "02"};
        MOT["MULT"] = {"IS", "03"};
        MOT["MOVER"] = {"IS", "04"};
        MOT["MOVEM"] = {"IS", "05"};
        MOT["COMP"] = {"IS", "06"};
        MOT["BC"] = {"IS", "07"};
        MOT["DIV"] = {"IS", "08"};
        MOT["READ"] = {"IS", "09"};
        MOT["PRINT"] = {"IS", "10"};
        MOT["START"] = {"AD", "01"};
        MOT["END"] = {"AD", "02"};
        MOT["ORIGIN"] = {"AD", "03"};
        MOT["EQU"] = {"AD", "04"};
        MOT["LTORG"] = {"AD", "05"};
        MOT["DC"] = {"DL", "01"};
        MOT["DS"] = {"DL", "02"};
    }
    void makeREG()
    {
        REG["AREG"] = "01";
        REG["BREG"] = "02";
        REG["CREG"] = "03";
        REG["DREG"] = "04";
    }
    void makeCONDITIONALS()
    {
        CONDITIONALS["LT"] = "01";
        CONDITIONALS["LE"] = "02";
        CONDITIONALS["EQ"] = "03";
        CONDITIONALS["GT"] = "04";
        CONDITIONALS["GE"] = "05";
        CONDITIONALS["ANY"] = "06";
    }
    
    vector<string> split(string &line, char del)
    {
        vector<string> ans;
        string temp = "";
        for (int i = 0; i < line.length(); i++)
        {
            char ch = line[i];
            if (ch == del)
            {
                ans.push_back(temp);
                temp = "";
                while (i < line.length() && line[i] == del)
                {
                    i++;
                }
                i--;
            }

            else
            {
                temp += (ch);
            }
        }
        if (!temp.empty())
        {
            ans.push_back(temp);
        }
        return ans;
    }

public:
    ICgenerator()
    {
        makeMOT();
        makeREG();
        makeCONDITIONALS();

        lc = 0;
    }
    void open_file(string file_name)
    {
        output.open("output.txt");
        ifstream obj(file_name);
        if (!obj.is_open())
        {
            cerr << "Failed to open the file." << endl;
            return;
        }
        process_code(obj);
    }

    void process_code(ifstream &obj)
    {
        string line;
        map<string, pair<string, string>>::iterator it;
        map<string, string>::iterator it2;

        while (getline(obj, line))
        {
            // output<<lc;
            int i = 0;
            vector<string> ans = split(line, ' ');
            int size = ans.size();

            it = MOT.find(ans[i]);
            if (it == MOT.end())
            { // label present

                process_label(ans[i]);
                i++;
                it = MOT.find(ans[i]);
            }

            lc++;
            if (ans[i] == "START")
            {
                lc = stoi(ans[1]);
            }

            // opcode
            // ltorg, equ and stop, origin require special handling
            if (ans[i] == "LTORG")
            {
                bool f = 0;

                for (auto &ele : littab)
                {
                    string str = ele.first;

                    if (ele.second == -1)
                    {
                        f = 1;
                        ele.second = lc;
                        output << "(DL,01) ";
                        process_constant(str);
                        output << endl;
                        lc++;
                    }
                }
                if (f)
                    lc--;
                continue;
            }
            else if (ans[i] == "STOP")
            {
                output << "(IS,00)";
            }
            else if (ans[i] == "EQU")
            {
                vector<pair<string, int>>::iterator sit;
                string key = ans[i + 1];
                for (int p = 0; p < symtab.size(); p++)
                {
                    sit = symtab.begin() + p;
                    if (sit->first == key)
                    {
                        break;
                    }
                }

                int pos = sit->second;
                key = ans[i - 1];
                for (int p = 0; p < symtab.size(); p++)
                {
                    sit = symtab.begin() + p;
                    if (sit->first == key)
                    {
                        break;
                    }
                }

                sit->second = pos;

                continue;
            }
            else if (ans[i] == "ORIGIN")
            {
                pair<string, string> x = it->second;
                output << "(" << x.first << "," << x.second << ")    ";
                string lab = "";
                i++;
                int j = 0;
                for (j; j < ans[i].length(); j++)
                {
                    if (ans[i][j] == '+')
                    {
                        j++;
                        break;
                    }
                    lab += ans[i][j];
                }
                process_symbol(lab);
                int cnt = 0;
                if (j < ans[i].length())
                {
                    cnt = stoi(ans[i].substr(j, ans[i].length()));
                }
                output << "+" << cnt;

                vector<pair<string, int>>::iterator mit;
                for (int p = 0; p < symtab.size(); p++)
                {
                    mit = symtab.begin() + p;
                    if (mit->first == lab)
                    {
                        break;
                    }
                }

                lc = (mit->second) + cnt;
            }
            else
            {
                pair<string, string> x = it->second;
                output << "(" << x.first << "," << x.second << ") ";
            }
            pair<string, string> x = it->second;
            i++;
            if (i == size)
            {
                output << endl;
                continue;
            }

            // operand1 : register/conditional/symbol/constant
            it2 = REG.find(ans[i]);
            if (it2 != REG.end()) // register
            {
                output << it2->second << " ";
                i++;
            }
            else
            {
                it2 = CONDITIONALS.find(ans[i]);
                if (it2 != CONDITIONALS.end()) // conditional
                {
                    output << it2->second << " ";
                }
                else
                {

                    if (x.first == "DL" && x.second == "01")
                    { // constant

                        process_constant(ans[i]);
                    }
                    else if (ans[i][0] >= '0' && ans[i][0] <= '9')
                    { // constant without ''
                        int a = stoi(ans[i]);
                        output << "(C," << a << ") ";
                    }
                    else
                    { // symbol
                        process_symbol(ans[i]);
                    }
                }
                i++;
            }
            if (i == size)
            {

                output << endl;
                continue;
            }

            // operand 2 : register,symbol,constant,literal
            it2 = REG.find(ans[i]);
            if (it2 != REG.end())
            { // register
                output << it2->second;
                i++;
            }
            else
            {
                if (ans[i][0] == '=')
                { // literal

                    process_literal(ans[i]);
                }
                else if (x.first == "DL" && x.second == "01")
                { // constant
                    process_constant(ans[i]);
                }
                else if (ans[i][0] >= '0' && ans[i][0] <= '9')
                { // constant without ''
                    int a = stoi(ans[i]);
                    output << "(C," << a << ") ";
                }
                else
                { // symbol
                    process_symbol(ans[i]);
                    //            			cout<<ans[i]<<endl;
                }
            }
            output << endl;
            //        		for(auto&i : symtab){
            //				cout<<i.first<<" "<<i.second<<endl;
            //			}
            //			cout<<"----------"<<endl;
        }

        // uninitialised literals are initialised after END
        bool f = 0;
        for (auto &ele : littab)
        {
            string str = ele.first;

            if (ele.second == -1)
            {
                f = 1;
                ele.second = lc;
                output << "(DL,01) ";

                process_constant(str);
                output << endl;
                lc++;
            }
        }
        if (f)
            lc--;
        for (auto x : littab)
        {
            cout << x.first << " " << x.second << endl;
        }
        cout << "----------\n";
        obj.close();
        output.close();
    }

    void process_label(string label)
    {

        // output<<"label     ";
        vector<pair<string, int>>::iterator sit;
        bool f = 0;
        for (int p = 0; p < symtab.size(); p++)
        {
            sit = symtab.begin() + p;
            if (sit->first == label)
            {
                sit->second = lc;

                f = 1;
                break;
            }
        }

        if (f == 0)
        {
            symbol_cnt++;
            symtab.push_back({label, lc});

            // p = symbol_cnt;
        }
        else
        {
            // p = symtab[ans[i]];
        }
    }

    void process_constant(string cons)
    {
        string sub = "";
        int s = 1;

        for (char ch : cons)
        {

            if (ch >= '0' && ch <= '9')
            {
                sub += ch;
            }
        }
        int a = stoi(sub);
        output << "(C," << a << ") ";
    }

    void process_symbol(string sym)
    {
        int p = 0;
        vector<pair<string, int>>::iterator sit;
        bool f = 0;
        for (int p = 0; p < symtab.size(); p++)
        {
            sit = symtab.begin() + p;
            if (sit->first == sym)
            {
                f = 1;
                break;
            }
        }
        if (f == 0)
        {
            symbol_cnt++;
            symtab.push_back({sym, -1});

            // p = symbol_cnt;
        }
        else
        {
            // p = symtab[ans[i]];
        }
        for (auto &c : symtab)
        {
            if (c.first == sym)
            {
                break;
            }
            p++;
        }

        output << "(S," << p + 1 << ") ";
    }

    void process_literal(string lit)
    {

        int p = 0;
        vector<pair<string, int>>::iterator sit;
        bool f = 0;
        for (int p = 0; p < littab.size(); p++)
        {
            sit = littab.begin() + p;
            if (sit->first == lit)
            {
                //					cout<<"literal found at"<<sit->first<<" : "<<sit->second<<"\n";
                f = 1;
                break;
            }
        }
        if (f == 0)
        {
            littab.push_back({lit, -1});

            literal_cnt++;
            // p=literal_cnt
        }
        else
        {
            // p=literal_cnt
        }

        for (auto &c : littab)
        {
            if (c.first == lit)
            {
                break;
            }
            p++;
        }

        output << "(L," << p + 1 << ")";
    }
};

class MachineCodeGen
{
    ofstream output1;
    int lc;
    ICgenerator icCode;

public:
    MachineCodeGen(ICgenerator &ic)
    {
        output1.open("machineCode.txt");
        lc = -1;
        icCode.symtab = ic.symtab;
        icCode.littab = ic.littab;
        for (auto x : icCode.littab)
        {
            cout << x.first << " " << x.second << endl;
        }
    }
    void open_file()
    {
        ifstream obj("output.txt");
        if (!obj.is_open())
        {
            cerr << "Failed to open the file. output" << endl;
            return;
        }
        process_ic(obj);
    }
    void process_ic(ifstream &obj)
    {
        string line;
        while (getline(obj, line))
        {
            output1 << endl;
            int i = 0;
            //        		cout<<line<<endl;
            vector<string> ans = icCode.split(line, ' ');
            for (string x : ans)
            {
                cout << x << " ";
            }
            cout << endl;
            int size = ans.size();
            if (lc == -1)
            { // first statement so initialise lc
                string s = ans[1];
                string x = "";
                for (int j = 3; j < s.size() - 1; j++)
                {
                    x += s[j];
                }
                lc = stoi(x);
                output1 << endl;
                continue;
            }
            output1 << lc << " ";
            lc++;
            if (ans[i][1] == 'I' && ans[i][2] == 'S')
            { // imperative statement  (opcode=IS)
                string x = "";
                for (int j = 4; j < ans[0].size() - 1; j++)
                {
                    x += ans[i][j];
                }
                output1 << x << "   ";
                i++;
                if (i == ans.size())
                {
                    output1 << endl;
                    continue;
                }

                // operand 1

                if (ans[i][0] != '(')
                {

                    output1 << ans[i] << "   ";
                }
                else
                {
                    if (ans[i][1] == 'L')
                    { // literal
                        string x = "";
                        for (int j = 3; j < ans[i].size() - 1; j++)
                        {
                            x += ans[i][j];
                        }
                        int ind = stoi(x);
                        ind -= 1;
                        int pos = icCode.littab[ind].second;
                        output1 << pos << "   ";
                    }
                    else if (ans[i][1] == 'S')
                    { // symbol

                        string x = "";
                        for (int j = 3; j < ans[i].size() - 1; j++)
                        {
                            x += ans[i][j];
                        }

                        int ind = stoi(x);
                        ind -= 1;
                        int pos = icCode.symtab[ind].second;
                        output1 << pos << "   ";
                    }
                }
                i++;
                if (i == ans.size())
                {

                    output1 << endl;
                    continue;
                }
                //					//operand1
                //					if(ans[i][0]!='('){
                //						output1<<ans[i]<<"   ";
                //					}
                else
                {
                    if (ans[i][1] == 'L')
                    { // literal

                        string x = "";
                        for (int j = 3; j < ans[i].size() - 1; j++)
                        {
                            x += ans[i][j];
                        }
                        cout << x;
                        int ind = stoi(x);

                        ind -= 1;
                        int pos = icCode.littab[ind].second;
                        output1 << pos << "   ";
                    }
                    else if (ans[i][1] == 'S')
                    { // symbol

                        string x = "";
                        for (int j = 3; j < ans[i].size() - 1; j++)
                        {
                            x += ans[i][j];
                        }
                        int ind = stoi(x);
                        ind -= 1;
                        int pos = icCode.symtab[ind].second;
                        output1 << pos << "   ";
                    }
                }
                i++;
                if (i == ans.size())
                {
                    output1 << endl;
                    continue;
                }
            }
            else if (ans[i][1] == 'D' && ans[i][2] == 'L')
            {
                //////---------------error
                output1 << "00 0 ";
                i++;

                if (ans[i][1] == 'C')
                {
                    string x = "";
                    int j = 3;
                    while (j != ')')
                    {
                        x += ans[i][j];
                        j++;
                    }

                    output1 << x;
                }
            }
            else if (ans[i][1] == 'A' && ans[i][5] == '3')
            {
                i++;
                string x = "";
                int j = 3;
                while (ans[i][j] != ')')
                {
                    x += ans[i][j];
                    j++;
                }
                int pos = stoi(x);
                int loc = icCode.symtab[pos].second;
                j++;
                if (ans[i][j] == '+')
                {
                    j++;
                    x = "";
                    while (j < ans[i].size())
                    {
                        x += ans[i][j];
                        j++;
                    }
                    loc += stoi(x);
                }
                lc = loc - 1;
                output1 << endl;
                continue;
            }
            else
            {
                output1 << endl; // no machine code for other statements
            }
        }
        output1.close();
        obj.close();
    }
};

int main()
{
    ICgenerator obj;
    obj.open_file("assembly2.txt");
    MachineCodeGen obj2(obj);
    obj2.open_file();

    return 0;
}