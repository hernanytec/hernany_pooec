#include <iostream>
#include <vector>
#include <map>
using namespace std;

enum Direction{IN = 1, OUT = 0};

string HELP = "inicar _maxVagoes    \n"
              "addVagao _capacidade \n"
              "show                 \n"
              "entrar _idPass       \n"
              "sair _idPass         \n"
              "cadastro             \n"
              "movimentacao         \n";

class Passageiro{
    string idPass;
public:
    Passageiro(string idPass = ""){
        this->idPass = idPass;
    }

    string getIdPass(){
        return this->idPass;
    }

    string toString(){
        return idPass;
    }
};

class Vagao{
public:
    vector<Passageiro*> cadeiras;
    int capacidade;

    Vagao(int capacidade = 0):
        cadeiras(capacidade)
    {
        this->capacidade = capacidade;
    }

    bool embarcar();
    Passageiro desembarcar();
    Passageiro * search(string idPass){
        for(Passageiro * passageiro : cadeiras){
            if(passageiro != nullptr && passageiro->getIdPass() == idPass)
                return passageiro;
        }
        return nullptr;
    }

    string toString(){
        string s = "[";
        for(Passageiro * passageiro : cadeiras){
            if(passageiro == nullptr)
                s.append(" - ");
            else
                s.append(" "+passageiro->toString()+" ");
        }
        return s.append("]");
    }
};

class Trem{
    int maxVagoes;
    vector<Vagao> vagoes = {};
public:
    Trem(int maxVagoes = 0){
        this->maxVagoes = maxVagoes;
    }

    bool addVagao(Vagao vagao){
        if(!((int)vagoes.size() < maxVagoes))
            return false;
        vagoes.push_back(vagao);
        return true;
    }

    bool embarcar(Passageiro * passageiro){
        if(search(passageiro->getIdPass()) == nullptr)
            for(Vagao& vagao : vagoes)
                for(int i = 0; i < vagao.capacidade; i++){
                    if(vagao.cadeiras[i] == nullptr){
                        vagao.cadeiras[i] = passageiro;
                        return true;
                    }
                }
        return false;
    }

    bool desembarcar(string idPass){
        if(search(idPass) != nullptr)
            for(Vagao& vagao : vagoes)
                for(int i = 0; i < vagao.capacidade; i++){
                    if(vagao.cadeiras[i]->getIdPass() == idPass){
                        vagao.cadeiras[i] = nullptr;
                        return true;
                    }
                }
        return false;
    }

    Passageiro * search(string idPass){
        Passageiro * pass;
        for(Vagao vagao : vagoes){
            pass = vagao.search(idPass);
            if(pass != nullptr)
                return pass;
        }
        return nullptr;
    }

    string toString(){
        string s = "Trem ";
        for(Vagao vagao : vagoes)
            s.append(vagao.toString());
        s.append("\n");
        return s;
    }

};

class Movimento{
    string idPass;
    Direction direcao;
public:
    Movimento(string idPass, Direction direcao){
        this->idPass = idPass;
        this->direcao = direcao;
    }

    string toString(){
        return idPass+" "+ (direcao ? "in" : "out") ;
    }
};

class Registro{
public:
    map<string, Passageiro> cadastro;
    vector<Movimento> movimentacao;

    Registro(map<string, Passageiro> cadastro = {}, vector<Movimento> movimentacao = {}){
        this->cadastro = cadastro;
        this->movimentacao = movimentacao;
    }

    void cadastrar(Passageiro pass){
        cadastro[pass.getIdPass()] = pass;
    }

    void addMov(Movimento mov){
        movimentacao.push_back(mov);
    }

    Passageiro * getPass(string idPass){
        auto it = cadastro.find(idPass);
        if(it != cadastro.end())
            return &it->second;
        return nullptr;
    }
};

class Manager{
    Trem trem;
    Registro registro;
public:
    void iniciarTrem(int maxVagoes){
        trem = Trem(maxVagoes);
    }
    string addVagao(Vagao vagao){
        return trem.addVagao(vagao) ? "done\n" : "limite de vagões atingido\n";
    }
    string showTrem(){
        return trem.toString();
    }
    string embarcar(string idPass){
        registro.cadastrar(Passageiro(idPass));
        if(trem.embarcar(registro.getPass(idPass))){
            registro.addMov(Movimento(idPass,IN));
            return "done\n";
        }
        return "error\n";
    }
    string desembarcar(string idPass){
        if(trem.desembarcar(idPass)){
            registro.addMov(Movimento(idPass,OUT));
            return "done\n";
        }
        return "error\n";
    }
    string showCadastro(){
        string s;
        for(auto elem : registro.cadastro)
            s.append(elem.second.toString()+"\n");
        return s;
    }
    string showMovimentacao(){
        string s;
        for(auto elem : registro.movimentacao)
            s.append(elem.toString()+"\n");
        return s;
    }
};

int main()
{ 
    Manager manager;
    string cmd;
    string strValue;
    int intValue;
    cin >> cmd;
    while(cmd != "fim"){
        if(cmd == "iniciar"){
            cin >> intValue;
            manager.iniciarTrem(intValue);
        }
        else if(cmd == "addVagao"){
            cin >> intValue;
            cout << manager.addVagao(intValue);
        }
        else if(cmd == "entrar"){
            cin >> strValue;
            cout << manager.embarcar(strValue);
        }
        else if(cmd == "sair"){
            cin >> strValue;
            cout << manager.desembarcar(strValue);
        }
        else if(cmd == "cadastro"){
            cout << manager.showCadastro();
        }
        else if(cmd == "movimentacao"){

            cout << manager.showMovimentacao();
        }
        else if(cmd == "show"){

            cout << manager.showTrem();
        }
        else if(cmd == "help"){
            cout << HELP;
        }
        else{
            cout << "comando invalido [" << cmd << "]" << endl;
        }
        cin >> cmd;
    }
    return 0;
}
