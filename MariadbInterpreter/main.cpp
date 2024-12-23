#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main() {

    vector<string> inputFiles = {"InputFile1.txt", "InputFile2.txt", "InputFile3.txt"};
    string outputFileName = "output.csv";

    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cout << "Error: Could not create output file: " << outputFileName << endl;
        return 1;
    }

    for (const string& inputFileName : inputFiles) {
        ifstream inputFile(inputFileName);
        if (!inputFile.is_open()) {
            cout << "Error: Could not open input file: " << inputFileName << endl;
            continue;
        }

        cout << "Processing file: " << inputFileName << endl;

        string line;
        while (getline(inputFile, line)) {
            cout << line << endl;

            outputFile << line << endl;
        }

        inputFile.close();
    }

    outputFile.close();

    cout << "All files processed. Results written to " << outputFileName << endl;
    return 0;
}



