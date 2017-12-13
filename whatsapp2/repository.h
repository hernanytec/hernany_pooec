#ifndef POO_REPOSITORY_H
#define POO_REPOSITORY_H
#include <iostream>
#include <vector>
#include <map>

using namespace std;

template <class T>
class Repository{
    map<string, T> mapa;
    string tipoRepository;
public:
    Repository(string tipoRepository = ""){
        this->tipoRepository = tipoRepository;
    }

    T * add(string chave, T t){
        if(!mapa.insert(make_pair(chave, t)).second)
            throw tipoRepository + " " + chave + " já existe";
        return &mapa.find(chave)->second;
    }

    void rm(string chave){
        if(mapa.count(chave) == 0)
            throw tipoRepository + " " + chave + " não encontrado";
        mapa.erase(chave);
    }

    T * getValue(string chave){
        auto it = mapa.find(chave);
        if(it != mapa.end())
            return &it->second;
        throw tipoRepository + " " + chave + " não encontrado";
    }

    vector<T> getValues(){
        vector<T> vec;
        for(auto elem: mapa)
            vec.push_back(elem.second);
        return vec;
    }

};

#endif // POO_REPOSITORY_H
