// Generate 10,000 test problems - Pure C++
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

int main() {
    srand(time(0));
    
    // CSV File
    ofstream csv("test_10000.csv");
    csv << "problem,answer,category\n";
    
    // TXT File  
    ofstream txt("test_10000.txt");
    txt << "# Test File - 10,000 Problems\n";
    txt << "# Generated for testing Progress Bar\n\n";
    
    cout << "Generating 10,000 problems...\n";
    
    for (int i = 0; i < 10000; i++) {
        int type = rand() % 10;
        string problem, category;
        
        if (type == 0) { // Addition
            int a = rand() % 500 + 1;
            int b = rand() % 500 + 1;
            problem = to_string(a) + "+" + to_string(b);
            category = "arithmetic";
        }
        else if (type == 1) { // Subtraction
            int a = rand() % 500 + 1;
            int b = rand() % 500 + 1;
            problem = to_string(a) + "-" + to_string(b);
            category = "arithmetic";
        }
        else if (type == 2) { // Multiplication
            int a = rand() % 50 + 1;
            int b = rand() % 50 + 1;
            problem = to_string(a) + "*" + to_string(b);
            category = "arithmetic";
        }
        else if (type == 3) { // Division
            int b = rand() % 20 + 1;
            int a = b * (rand() % 30 + 1);
            problem = to_string(a) + "/" + to_string(b);
            category = "arithmetic";
        }
        else if (type == 4) { // Power
            int base = rand() % 8 + 2;
            int exp = rand() % 4 + 2;
            problem = to_string(base) + "^" + to_string(exp);
            category = "power";
        }
        else if (type == 5) { // Sqrt
            int squares[] = {4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169, 196, 225, 256, 289, 324, 361, 400, 441, 484, 529, 576, 625};
            int num = squares[rand() % 24];
            problem = "sqrt(" + to_string(num) + ")";
            category = "power";
        }
        else if (type == 6) { // Linear equation
            int a = rand() % 9 + 1;
            int b = rand() % 20 - 10;
            int c = rand() % 100 - 50;
            problem = to_string(a) + "x";
            if (b >= 0) problem += "+" + to_string(b);
            else problem += to_string(b);
            problem += "=" + to_string(c);
            category = "linear";
        }
        else if (type == 7) { // Trigonometry
            int angles[] = {0, 30, 45, 60, 90, 120, 135, 150, 180};
            int angle = angles[rand() % 9];
            string funcs[] = {"sin", "cos", "tan"};
            string func = funcs[rand() % 3];
            if (func == "tan" && (angle == 90 || angle == 270)) angle = 45;
            problem = func + "(" + to_string(angle) + ")";
            category = "trigonometry";
        }
        else if (type == 8) { // Logarithm
            int nums[] = {10, 100, 1000, 2, 4, 8, 16, 32, 64, 128};
            int num = nums[rand() % 10];
            problem = "log(" + to_string(num) + ")";
            category = "logarithm";
        }
        else { // Complex expression
            int a = rand() % 20 + 1;
            int b = rand() % 20 + 1;
            int c = rand() % 20 + 1;
            problem = to_string(a) + "+" + to_string(b) + "*" + to_string(c);
            category = "arithmetic";
        }
        
        csv << problem << ",," << category << "\n";
        txt << problem << "\n";
        
        if ((i + 1) % 1000 == 0) {
            cout << "  Generated " << (i + 1) << " problems...\n";
        }
    }
    
    csv.close();
    txt.close();
    
    cout << "\nDone! Created:\n";
    cout << "  - test_10000.csv (10,000 problems)\n";
    cout << "  - test_10000.txt (10,000 problems)\n";
    
    return 0;
}
