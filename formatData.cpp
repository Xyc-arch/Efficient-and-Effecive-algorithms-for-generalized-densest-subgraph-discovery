#include<fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstring>
#include <sstream>
#include <bits/stdc++.h>
#include <math.h>
#include "experiment.h"
#include "dinic.h"
using namespace std;

void t()
{
    ofstream ofs;
    ofs.open("hhh007.txt",ios::out );
    for ( int i=1; i<264347; i++) {    //加了个for循环去将第一列写成全部Q，第二列写成123...
    ofs << "Q" << "\t" << i << "\t" << endl;  //endl用于换行
    }

    ofs.close();
}


void reformatGraph(string input, string split, string output, int startLine, int lineNum){
    ifstream infile;
    ofstream outfile;
    string line;
    int n1;
    int n2;
    int currentLine=0;
    char c[20];
    char* num_str;
    stringstream s;

    infile.open(input, ios::in);
    outfile.open(output, ios::out);


    while (getline(infile, line)){
        currentLine += 1;
        if (currentLine > lineNum){
            break;
        }
        if (currentLine < startLine){
            continue;
        }

        strcpy(c, line.c_str());
        num_str = strtok(c, split.c_str());
        s << num_str;
        s >> n1;
        s.clear();
        s.str("");

        num_str = strtok(NULL, split.c_str());
        s << num_str;
        s >> n2;
        s.clear();
        s.str("");

        if (n1 < n2){
            outfile << n1 << " " << n2 << endl;
        }

        if (currentLine%10000000 == 0) cout << "Line: " << currentLine << endl;
        }
    cout << "reformat finish" << endl;
}


//int main(){
////    string input = "D:\\research\\2020-6-12\\data\\com-lj.ungraph.txt";
////    string output = "D:\\research\\2020-6-12\\data\\comljnet.txt";
////    string split = "	";
////    int startLine = 5;
////    int lineNum = 34681193;

////    string input = "D:\\research\\2020-6-12\\data\\test.txt";
////    string output = "D:\\research\\2020-6-12\\data\\testnet.txt";
////    string split = " ";
////    int startLine = 1;
////    int lineNum = 95188;

////    string input = "D:\\research\\2020-6-12\\data\\com-dblp.ungraph.txt";
////    string output = "D:\\research\\2020-6-12\\data\\dblpnet.txt";
////    string split = "	";
////    int startLine = 5;
////    int lineNum = 1049866;

////    string input = "D:\\research\\2020-6-12\\data\\com-youtube.ungraph.txt";
////    string output = "D:\\research\\2020-6-12\\data\\youtubenet.txt";
////    string split = "	";
////    int startLine = 5;
////    int lineNum = 2987628;

////    string input = "D:\\research\\2020-6-12\\data\\com-amazon.ungraph.txt";
////    string output = "D:\\research\\2020-6-12\\data\\amazonnet.txt";
////    string split = "	";
////    int startLine = 5;
////    int lineNum = 925876;

//    string input = "D:\\research\\2020-6-12\\data\\com-orkut.ungraph.txt";
//    string output = "D:\\research\\2020-6-12\\data\\orkutnet.txt";
//    string split = "	";
//    int startLine = 5;
//    int lineNum = 117185087;

//    reformatGraph(input, split, output, startLine, lineNum);
//    return 0;
//}




