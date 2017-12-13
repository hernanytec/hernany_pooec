#ifndef POO_AUX_H
#define POO_AUX_H
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

vector<string> split(string str, char delim = ' ') {
    stringstream ss(str);
    string tok;
    vector<string> vec;
    while (getline(ss, tok, delim)) {
        if (!tok.empty())
            vec.push_back(tok);
    }
    return vec;
}

string vetToStr(vector<string> vet){
    string str = "";
    for(auto elem: vet){
        str.append(elem + " ");
    }
    return str;
}

string slacing(int index, vector<string> vet){
    string s = "";
    for(int i = index; i< (int) vet.size(); i++){
        s.append(vet[i] + " ");
    }
    return s;
}


#endif // POO_AUX_H
