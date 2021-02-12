/*
Machine Problem 7 File Name: mp7.cpp, Structures
Student Name: William Hubenschmidt
Professor’s Name: Hiyam Najim
Course: CPS 171 section 02
Due: November 30, 2020
Note:
* Run mp7.cpp file, which will consume mp7divers.txt
* and then prints output to the console.
*
* Objective: This assignment is to give you practice using structs and sorting.
*
* Problem: In competitive diving, each diver makes dives of varying degrees of difficulty. Nine judges score each dive from 0
* through 10 in steps of 0.5. The difficulty is a floating-point value between 1.0 and 3.0 that represents how complex the
* dive is to perform. The total score is obtained by discarding the lowest and highest of the judges’ scores, adding the
* remaining scores, and then multiplying that total by the degree of difficulty. Write a program to score each of the
* dives, using the following input and output specifications, and determine the winner of the competition.
*
* Additional File Required: mp7divers.txt, sample_output.txt, readme.rtf
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <numeric>

using namespace std;

const int NUMBER_OF_JUDGES = 9;

struct DiverType {
    string name;
    double difficulty;
    vector<double> rating{ NUMBER_OF_JUDGES };
    double points;
};

struct SortWinner {
    bool operator()(const DiverType& a, const DiverType& b) const
    {
        return a.points > b.points;
    }
};

/**
 * function prototypes
 */
void initialize(ifstream&, string, int&);
vector<string> getData(ifstream&, string, int);
void setName(vector<string>, vector<DiverType>&);
void setDifficulty(vector<string>, vector<DiverType>&);
void setRating(vector<string>, vector<DiverType>&);
void printResult(vector<DiverType>&);
void printHeader();
void sortRating(vector<DiverType>&);
void setPoints(vector<DiverType>&);
void sortWinner(vector<DiverType>&);
vector<string> tokenize(vector<string>, int);

/**
 * initialize diverType struct by getting first line of input file
 */
void initialize(ifstream& ifs, string inFile, int& numDivers)
{
    ifs.open(inFile);

    if(!ifs) {
        cerr << "Unable to open " << inFile << " in initialize()" << endl;
        exit(1);
    }

    ifs >> numDivers;
    ifs.close();
}

/**
 * get data from input file
 */
vector<string> getData(ifstream& ifs, string inFile, int numDivers)
{
    ifs.open(inFile);
    ifs.ignore(numeric_limits<streamsize>::max(), '\n'); // Get and drop a line
    string line;

    vector<string> lines;
    while(getline(ifs, line)) {
        if(line.size() > 0) {
            lines.push_back(line);
        }
    }
    ifs.close();
    return lines;
}

/**
 * assign contestant values to vector of diverTypes
 */
void setName(vector<string> lines, vector<DiverType>& diver)
{
    for(int i = 0; i < lines.size(); i++) {
        vector<string> tokenizeLine = tokenize(lines, i);
        for(int j = 0; j < diver.size(); j++) {
            diver[i].name = tokenizeLine[0];
        }
    }
}

void setDifficulty(vector<string> lines, vector<DiverType>& diver)
{
    for(int i = 0; i < lines.size(); i++) {
        vector<string> tokenizeLine = tokenize(lines, i);
        for(int j = 0; j < diver.size(); j++) {
            diver[i].difficulty = stod(tokenizeLine[1]);
        }
    }
}

void setRating(vector<string> lines, vector<DiverType>& diver)
{
    for(int i = 0; i < lines.size(); i++) {
        vector<string> tokenizeLine = tokenize(lines, i);
        for(int j = 2; j < tokenizeLine.size(); j++) {
            diver[i].rating.push_back(stod(tokenizeLine[j]));
        }
    }
}

/**
 * mutate rating vector
 */
void sortRating(vector<DiverType>& diver)
{
    for(int i = 0; i < diver.size(); i++) {
        sort(++diver[i].rating.begin(), diver[i].rating.end());
    }
}

/**
 * calculuate contestant's weighted score
 */
void setPoints(vector<DiverType>& diver)
{
    double lowest_score, highest_score;
    for(int i = 0; i < diver.size(); i++) {
        lowest_score = diver[i].rating[1];
        highest_score = diver[i].rating[9];
        diver[i].points = accumulate(++diver[i].rating.begin(), diver[i].rating.end(), 0.0);
        diver[i].points -= lowest_score + highest_score;
        diver[i].points *= diver[i].difficulty;
    }
}

/**
 * sort DiverType vector on points member
 */
void sortWinner(vector<DiverType>& diver)
{
    for(int i = 0; i < diver.size(); i++) {
        sort(diver.begin(), diver.end(), SortWinner());
    }
}

/**
 * display results of judging
 */
void printResult(vector<DiverType>& diver)
{
    for(int i = 0; i < diver.size(); i++) {
        cout << left << setw(15) << fixed << setprecision(1);
        cout << diver[i].name << setw(15) << diver[i].difficulty;

        for(int j = 1; j < diver[i].rating.size(); j++) { // offset beginning of vector to second element,
            cout << setw(5) << diver[i].rating[j];        // because first element contains vector size
        }

        cout << ".... " << diver[i].points;
        cout << "\n";
    }
}

void printHeader()
{
    cout << left << fixed << setprecision(1);
    cout << setw(15) << "DIVER" << setw(15) << "DIFFICULTY";
    cout << setw(50) << "SCORES"
         << "POINTS"
         << "\n";
    cout << "--------------------------------------------------------------------------------------\n";
}

/**
 * tokenize input file lines
 */
vector<string> tokenize(vector<string> vec, int iter)
{
    vector<string> tokens;
    stringstream ss(vec[iter]);
    string token;
    while(ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * driver
 */
int main()
{
    ifstream ifs;
    const string inFile = "./mp7divers.txt";
    int numDivers;
    initialize(ifs, inFile, numDivers);
    vector<DiverType> diver(numDivers);
    vector<string> lines = getData(ifs, inFile, numDivers);
    setName(lines, diver);
    setDifficulty(lines, diver);
    setRating(lines, diver);
    sortRating(diver);
    setPoints(diver);
    printHeader();
    printResult(diver);
    sortWinner(diver);

    cout << "\n";
    cout << "The winner is " + diver[0].name + " with " << diver[0].points << " points!" << endl;

    return 0;
}