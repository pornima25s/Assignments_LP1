#include <bits/stdc++.h>
#include <string>
using namespace std;

class Pass1
{
public:
    map<string, pair<string, string>> MOT;
    map<string, string> REG;
    map<string, string> CONDITIONALS;
    vector<pair<string, int>> symtab;
    vector<pair<string, int>> littab;
    ofstream output;  // Output file stream
    int lc;
    int symbol_cnt = 0;
    int literal_cnt = 0;

    Pass1()
    {
        output.open("output.txt");  // Open "output.txt" file
        if (!output.is_open()) {
            cerr << "Error opening output.txt file!" << endl;
            exit(1);  // Exit if file can't be opened
        }
        
        makeMOT();
        makeREG();
        makeCONDITIONALS();
        lc = 0;
    }

    ~Pass1()
    {
        output.close();  // Ensure file is closed when done
    }

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

    void process_label(string label)
    {
        bool found = false;
        for (int p = 0; p < symtab.size(); p++)
        {
            if (symtab[p].first == label)
            {
                symtab[p].second = lc;
                found = true;
                break;
            }
        }

        if (!found)
        {
            symbol_cnt++;
            symtab.push_back({label, lc});
        }
    }

    void process_constant(string cons)
    {
        string sub = "";
        for (char ch : cons)
        {
            if (ch >= '0' && ch <= '9')
            {
                sub += ch;
            }
        }
        int a = stoi(sub);
        output << "(C," << a << ") ";  // Write to output file
    }

    void process_symbol(string sym)
    {
        bool found = false;
        for (int p = 0; p < symtab.size(); p++)
        {
            if (symtab[p].first == sym)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            symbol_cnt++;
            symtab.push_back({sym, -1});
        }

        for (int p = 0; p < symtab.size(); p++)
        {
            if (symtab[p].first == sym)
            {
                output << "(S," << p + 1 << ") ";  // Write to output file
                break;
            }
        }
    }

    void process_literal(string lit)
    {
        bool found = false;
        for (int p = 0; p < littab.size(); p++)
        {
            if (littab[p].first == lit)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            littab.push_back({lit, -1});
            literal_cnt++;
        }

        for (int p = 0; p < littab.size(); p++)
        {
            if (littab[p].first == lit)
            {
                output << "(L," << p + 1 << ")";  // Write to output file
                break;
            }
        }
    }

    void process_code(ifstream &obj)
    {
        string line;
        while (getline(obj, line))
        {
            int i = 0;
            vector<string> ans = split(line, ' ');
            int size = ans.size();

            // Check if first part is a label
            if (MOT.find(ans[i]) == MOT.end())
            {
                process_label(ans[i]);
                i++;
            }

            lc++;  // Increment location counter

            // Check for assembler directives or instructions
            if (ans[i] == "START")
            {
                lc = stoi(ans[1]);
                output << "(AD,01) (C," << ans[1] << ")\n";  // Write START to output file
            }
            else if (ans[i] == "END")
            {
                output << "(AD,02)\n";  // Write END to output file
            }
            else if (ans[i] == "DC")
            {
                process_constant(ans[1]);
                output << "\n";
            }
            else if (ans[i] == "DS")
            {
                process_symbol(ans[1]);
                output << "\n";
            }
            // More cases for LTORG, ORIGIN, EQU, etc., can be added here
        }
    }
};

int main()
{
    Pass1 pass1;
    ifstream obj("assembly2.txt");
    if (!obj.is_open())
    {
        cerr << "Error opening assembly2.txt" << endl;
        return 1;
    }

    pass1.process_code(obj);
    obj.close();
    
    return 0;
}
