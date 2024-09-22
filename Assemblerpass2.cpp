#include <bits/stdc++.h>
#include <string>
using namespace std;

class Pass2
{
public:
    ofstream output1;
    int lc;
    vector<pair<string, int>> symtab;
    vector<pair<string, int>> littab;

    Pass2(vector<pair<string, int>> &symtab_in, vector<pair<string, int>> &littab_in)
    {
        output1.open("machineCode.txt");
        lc = -1;
        symtab = symtab_in;
        littab = littab_in;
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
                temp += ch;
            }
        }
        if (!temp.empty())
        {
            ans.push_back(temp);
        }
        return ans;
    }

    void process_ic(ifstream &obj)
    {
        string line;
        while (getline(obj, line))
        {
            vector<string> ans = split(line, ' ');
            if (lc == -1)
            { // First line with LC initialization
                string s = ans[1];
                string x = "";
                for (int j = 3; j < s.size() - 1; j++)
                {
                    x += s[j];
                }
                lc = stoi(x);
                continue;
            }
            output1 << lc << " ";
            lc++;

            if (ans[0][1] == 'I' && ans[0][2] == 'S')
            { // Imperative statement (opcode=IS)
                string opcode = ans[0].substr(4, ans[0].size() - 5);
                output1 << opcode << "   ";
                int i = 1;

                if (ans[i][0] == '(')
                {
                    process_operand(ans[i], output1);
                    i++;
                }

                if (i < ans.size() && ans[i][0] == '(')
                {
                    process_operand(ans[i], output1);
                }
                output1 << endl;
            }
            else if (ans[0][1] == 'D' && ans[0][2] == 'L')
            { // Declarative statement (opcode=DL)
                output1 << "00 0 ";
                string constant = ans[1].substr(3, ans[1].size() - 4);
                output1 << constant << endl;
            }
            else if (ans[0][1] == 'A' && ans[0][5] == '3')
            { // Assembler Directive (AD, 03)
                process_origin(ans[1]);
            }
            else
            {
                output1 << endl; // For other cases like END, no machine code generated
            }
        }
        output1.close();
    }

    void process_operand(string &operand, ofstream &output)
    {
        if (operand[1] == 'L')
        { // Literal
            int index = stoi(operand.substr(3, operand.size() - 4)) - 1;
            output << littab[index].second << "   ";
        }
        else if (operand[1] == 'S')
        { // Symbol
            int index = stoi(operand.substr(3, operand.size() - 4)) - 1;
            output << symtab[index].second << "   ";
        }
    }

    void process_origin(string &operand)
    {
        string label = "";
        int i = 0;
        while (operand[i] != '+' && i < operand.size())
        {
            label += operand[i++];
        }
        int sym_index = stoi(label.substr(1, label.size() - 2)) - 1;
        int new_lc = symtab[sym_index].second;

        if (i < operand.size())
        {
            int offset = stoi(operand.substr(i + 1));
            new_lc += offset;
        }
        lc = new_lc - 1;
    }
};

int main()
{
    // Assuming symtab and littab were populated in Pass1 and saved in output.txt.
    vector<pair<string, int>> symtab = {{"S1", 100}, {"S2", 200}};  // Sample symbol table from Pass1
    vector<pair<string, int>> littab = {{"=5", 300}, {"=10", 400}}; // Sample literal table from Pass1

    ifstream obj2("output.txt");
    Pass2 pass2(symtab, littab);
    pass2.process_ic(obj2);

    return 0;
}


 