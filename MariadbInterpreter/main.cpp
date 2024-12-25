#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

// Variables
string outputFileName = "output.txt"; // Default output file
string tableName = "";               // Current table name
vector<string> tableColumns;         // Table column names
vector<vector<string>> database;     // Table rows
string output = "";                  // Output log

// Function prototypes
void read_input_file(const string &fileName, vector<string> &queries);
void process_queries(const vector<string> &queries);
void handle_create_output_file(const string &query);
void create_table(const string &query);
void insert_into_table(const string &query);
void select_from_table(const string &query);
void write_output_to_file();
string trim(const string &str);

int main() {
    vector<string> queries;                     // List of commands
    read_input_file("InputFile1.txt", queries); // Read commands from file
    process_queries(queries);                   // Process commands
    write_output_to_file();                     // Save results to file
    return 0;
}

// Function to read commands from input file
void read_input_file(const string &fileName, vector<string> &queries) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return;
    }

    string line, command = "";
    while (getline(inputFile, line)) {
        line = trim(line);
        if (!line.empty()) {
            command += line + " ";       // Append line
            if (line.back() == ';') {    // End of command
                queries.push_back(trim(command));
                command = "";            // Reset for next command
            }
        }
    }
    inputFile.close();
}

// Function to process each query
void process_queries(const vector<string> &queries) {
    for (const string &query : queries) {
        if (query.find("CREATE") == 0 && query.find("fileOutput") != string::npos) {
            handle_create_output_file(query);
        } else if (query.find("CREATE TABLE") == 0) {
            create_table(query);
        } else if (query.find("INSERT INTO") == 0) {
            insert_into_table(query);
        } else if (query.find("SELECT * FROM") == 0) {
            select_from_table(query);
        } else {
            output += "Error: Unsupported query - " + query + "\n";
        }
    }
}

// Function to handle `CREATE fileOutput1.txt;`
void handle_create_output_file(const string &query) {
    size_t start = query.find("CREATE") + 7;
    outputFileName = trim(query.substr(start, query.find(';') - start));
    output += "Output file set to: " + outputFileName + "\n";
}

// Function to create a table
void create_table(const string &query) {
    size_t start = query.find("CREATE TABLE") + 13;
    size_t end = query.find('(');
    tableName = trim(query.substr(start, end - start));

    string schema = query.substr(end + 1, query.find(')') - end - 1);
    stringstream ss(schema);
    string column;

    tableColumns.clear();
    while (getline(ss, column, ',')) {
        tableColumns.push_back(trim(column));
    }

    database.clear(); // Clear existing rows
    output += "Table '" + tableName + "' created with columns: ";
    for (const string &col : tableColumns) {
        output += col + ", ";
    }
    output = output.substr(0, output.size() - 2) + ".\n"; // Remove trailing comma
}

// Function to insert data into a table
void insert_into_table(const string &query) {
    if (tableName.empty()) {
        output += "Error: No table created. Use CREATE TABLE first.\n";
        return;
    }

    size_t start = query.find("INSERT INTO") + 12;
    size_t end = query.find("VALUES");
    string table = trim(query.substr(start, end - start));

    if (table != tableName) {
        output += "Error: Table '" + table + "' does not exist.\n";
        return;
    }

    start = query.find('(') + 1;
    end = query.find_last_of(')');
    string values = query.substr(start, end - start);
    stringstream ss(values);
    string value;
    vector<string> row;

    while (getline(ss, value, ',')) {
        value = trim(value);
        if (value.front() == '\'' && value.back() == '\'') {
            value = value.substr(1, value.size() - 2); // Remove quotes
        }
        row.push_back(value);
    }

    if (row.size() != tableColumns.size()) {
        output += "Error: Number of values does not match the table schema.\n";
        return;
    }

    database.push_back(row);
    output += "Row inserted into table '" + table + "'.\n";
}

// Function to select data from the table
void select_from_table(const string &query) {
    if (tableName.empty()) {
        output += "Error: No table created.\n";
        return;
    }

    size_t start = query.find("SELECT * FROM") + 13;
    string table = trim(query.substr(start, query.find(';') - start));

    if (table != tableName) {
        output += "Error: Table '" + table + "' does not exist.\n";
        return;
    }

    output += "Table '" + table + "':\n";
    for (const string &col : tableColumns) {
        output += col + "\t";
    }
    output += "\n";

    for (const vector<string> &row : database) {
        for (const string &value : row) {
            output += value + "\t";
        }
        output += "\n";
    }
}

// Function to write output to a file
void write_output_to_file() {
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "Error: Could not open " << outputFileName << " for writing.\n";
        return;
    }

    outputFile << output;
    outputFile.close();
    cout << "Output written to " << outputFileName << " successfully.\n";
}

// Function to trim whitespace from a string
string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    return (first == string::npos || last == string::npos) ? "" : str.substr(first, (last - first + 1));
}
