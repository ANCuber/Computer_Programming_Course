#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Function to read fragments from a file
vector<string> readFragments(const string& filename) {
    ifstream infile(filename);
    vector<string> fragments;
    string fragment;
    
    while (infile >> fragment) {
        fragments.push_back(fragment);
    }
    
    infile.close();
    return fragments;
}

// Function to reconstruct the original DNA sequence
string reconstructDNA(const vector<string>& fragments) {
    if (fragments.empty()) return "";

    string dna = fragments[0];  // Start with the first fragment
    
    for (size_t i = 1; i < fragments.size(); ++i) {
        // Append only the last character of each fragment to the DNA string
        dna += fragments[i].back();
    }
    
    return dna;
}

// Function to write the result to a file
void writeResult(const string& result, const string& filename) {
    ofstream outfile(filename);
    outfile << result;
    outfile.close();
}

int main() {
    string inputFile = "fragments.txt";
    string outputFile = "result.txt";
    
    // Read fragments from file
    vector<string> fragments = readFragments(inputFile);
    
    // Reconstruct the original DNA sequence
    string originalDNA = reconstructDNA(fragments);
    
    // Write the reconstructed DNA sequence to result.txt
    writeResult(originalDNA, outputFile);
    
    cout << "Reconstruction complete. Output written to " << outputFile << endl;
    
    return 0;
}
