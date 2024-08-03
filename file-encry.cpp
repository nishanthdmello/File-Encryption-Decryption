#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <vector>

using namespace std;

const string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

string base64_encode(const string &input)
{
    string output;
    int val = 0, valb = -6;
    for (unsigned char c : input)
    {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0)
        {
            output.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        output.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (output.size() % 4)
        output.push_back('=');
    return output;
}

string base64_decode(const string &input)
{
    vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;

    string output;
    int val = 0, valb = -8;
    for (unsigned char c : input)
    {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0)
        {
            output.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return output;
}

class File
{
    string inputFile;
    string tempFile;

    public:
    File(string inputFile)
    {
        this->inputFile = inputFile;
        this->tempFile = inputFile + ".tmp";
    }

    File operator++(int)
    {
        File temp = *this;

        ifstream input(inputFile, ios::binary);
        ofstream output(tempFile, ios::binary);

        if (!input.is_open() || !output.is_open())
        {
            cerr << "Error opening file!" << endl;
            return temp;
        }

        string fileContent((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
        string encodedContent = base64_encode(fileContent);
        output << encodedContent;

        input.close();
        output.close();

        remove(inputFile.c_str());
        rename(tempFile.c_str(), inputFile.c_str());

        return temp;
    }

    File operator--(int)
    {
        File temp = *this;

        ifstream input(inputFile, ios::binary);
        ofstream output(tempFile, ios::binary);

        if (!input.is_open() || !output.is_open())
        {
            cerr << "Error opening file!" << endl;
            return temp;
        }

        string fileContent((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
        string decodedContent = base64_decode(fileContent);
        output << decodedContent;

        input.close();
        output.close();

        remove(inputFile.c_str());
        rename(tempFile.c_str(), inputFile.c_str());

        return temp;
    }

    ostream& operator<<(ostream& os)
    {
        ifstream file(inputFile);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                os << line << endl;
            }
            file.close();
        } else {
            os << "Error: Could not open the file.\n";
        }
        return os;
    }

    ~File(){}
};

int main()
{
    string inputFile;
    cout << "Enter the name of the file : ";
    cin >> inputFile;
    File f1(inputFile);
    char choice;

    while (true)
    {
        cout << "Do you want to encode (e) or decode (d) or print (p) or break (b)? ";
        cin >> choice;

        if (choice == 'e')
        {
            f1++;
            cout << "The file has been encoded." << endl;
        }
        else if (choice == 'd')
        {
            f1--;
            cout << "The file has been decoded." << endl;
        }
        else if (choice == 'p')
        {
            cout << "\n\n\n";
            f1 << cout;
            cout << "\n\n\n";
        }
        else if (choice == 'b')
        {
            break;
        }
        else
        {
            cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}
