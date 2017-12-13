#include <iostream>
#include<map>
#include<vector>

using namespace std;

class Fone{
public:
    string foneId;
    string number;

    Fone(string foneId = "", string number = ""){
        this->foneId = foneId;
        this->number = number;
    }

    static bool validate(string number);

    string toString(){
        string result;
        result = foneId + " " + number;
        return result;
    }
};

class Contato{
public:
    string id;
    vector<Fone> fones;
    bool isFavorited;

    Contato(string id = "", vector<Fone> fones = {}, bool isFavorited = false){
        this->id = id;
        this->fones = fones;
        this->isFavorited = isFavorited;
    }

    bool addFone(Fone telefone){
        for(auto fone: fones)
            if(fone.foneId == telefone.foneId)
                return false;
        fones.push_back(telefone);
        return true;
    }
    bool rmFone(string foneId){
        bool ret = false;
        for(int i = 0; i < (int) fones.size(); i++)
            if(fones.at(i).foneId == foneId){
                fones.erase(fones.begin() + i);
                ret = true;
            }
        return ret;
    }

    bool match(string pattern);

    string toString(){
        string result = "";
        (isFavorited ? result.append("@ "): result.append("- "));
        result.append(id+" ");
        for(auto fone: fones)
            result.append("[" + fone.toString() +"]");
        return result.append("\n");
    }

};

class Agenda{
public:
    map<string, Contato> contatos;
    map<string, Contato> favoritos;


    bool addContato(Contato contato){
        return contatos.insert(make_pair(contato.id,contato)).second;
    }

    bool rmContato(string id){
        if(contatos.erase(id)){
            favoritos.erase(id);
            return true;
        }
        return false;
    }

    vector<Contato> search(string pattern){
        vector<Contato> result;
        //não implementado
    }

    bool favoritar(string id){
        Contato * cont = getContato(id);
        if(!cont || cont->isFavorited)
            return false;
        cont->isFavorited = true;
        favoritos[id] = *cont;
        return true;
    }

    bool desfavoritar(string id){
        Contato * cont = getContato(id);
        if(!cont || !cont->isFavorited)
            return false;
        cont->isFavorited = false;
        favoritos.erase(id);
        return true;
    }

    Contato * getContato(string id){
        auto it = contatos.find(id);
        if(it == contatos.end())
            return nullptr;
        return &it->second;
    }

    vector<Contato> getAllCContatos(){
        vector<Contato> ret;
        for(auto cont: contatos)
            ret.push_back(cont.second);
        return ret;
    }
};

class Manager{
public:
    Agenda agenda;


    void listaDeComandos(){
        cout  <<  "  addContato _idContato                      \n"<<
                  "  rmContato  _idContato                      \n" <<
                  "  addFone    _idContato _idFone _number      \n"<<
                  "  rmFone     _idContato _idFone              \n"<<
                  "  show       _idContato                      \n"<<
                  "  showAll                                    \n"<<
                  "  search     _pattern                        \n"<<
                  "  fav        _idContato                      \n"<<
                  "  desfav     _idContato                      \n"<<
                  "  showFav                                    \n"<<
                  "  fim                                        \n"<<
                  "  help                                       \n" << endl;
    }

    void comandLine(){
        listaDeComandos();
        string op;
        cin >> op;
        while(op != "fim"){

            if(op == "addContato"){
                string id;
                cin >> id;
                if(agenda.addContato(Contato(id)))
                    cout << "Sucesso" << endl;
                else
                    cout <<  "Id duplicada" << endl;
            }
            if(op =="rmContato"){
                string id;
                cin >> id;
                if(agenda.rmContato(id))
                    cout << "Sucesso" << endl;
                else
                    cout <<  "Contato nao existe" << endl;
            }
            if(op =="addFone"){
                string id, numero, foneId;
                cin >> id >> foneId >> numero;
                Contato * contato = agenda.getContato(id);
                if(!contato){
                    cout <<  "Contato nao existe" << endl;
                    continue;
                }
                if(!contato->addFone(Fone(foneId, numero))){
                    cout <<  "Id cuplicada" << endl;
                    continue;
                }
                cout << "Sucesso" << endl;
            }
            if(op =="rmFone"){
                string id, foneId;
                cin >> id >> foneId;
                Contato * contato = agenda.getContato(id);
                if(!contato){
                    cout <<  "Contato nao existe" << endl;
                    continue;
                }
                if(!contato->rmFone(foneId)){
                    cout <<  "Id cuplicada" << endl;
                    continue;
                }
                cout << "Sucesso!";
            }
            if(op =="show"){
                string id;
                cin >> id;
                cout << agenda.getContato(id)->toString();
            }
            if(op =="showAll"){
                auto contatos = agenda.getAllCContatos();
                if(!contatos.size()){
                    cout << "Nenhum contato adicionado\n";
                }
                else
                    for(auto cont : contatos)
                        cout << cont.toString();
            }
            if(op =="search"){
                //não implementado
            }
            if(op =="fav"){
                string id;
                cin >> id;
                if(!agenda.favoritar(id))
                    cout <<  "Contato nao existe ou já está favoritado" << endl;
                else
                    cout << "Sucesso" << endl;
            }
            if(op =="desfav"){
                string id;
                cin >> id;
                if(!agenda.desfavoritar(id))
                    cout <<   "Contato não existe ou nao eh favorito"  << endl;
                else
                    cout << "Sucesso" << endl;
            }
            if(op =="showFav"){
                for(auto contato: agenda.favoritos)
                    cout << contato.second.toString();
            }
            if(op =="help"){
                listaDeComandos();
            }
            cin >> op;
        }
    }
};

int main()
{
    Manager manager;
    manager.comandLine();

}
