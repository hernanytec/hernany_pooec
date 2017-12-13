#include <iostream>
#include <vector>

using namespace std;

class Circense{
public:
    string nome;
    string especialidade;
    Circense(string nome = "Tanguinha", string especialidade = "palhaco"){
        this->nome = nome;
        this->especialidade = especialidade;
    }
};

class Pessoa{
public:
    string nome;
    vector<string> preferencias;
    int idade;
    int alegria{0};

    Pessoa(string nome = "", int idade = 18, vector<string> preferencias = vector<string>()){
        this->nome = nome;
        this->idade = idade;
        this->alegria = 0;
        this->preferencias = preferencias;
    }

    void verEspetaculo(vector<Circense> vetor){

        for (Circense& circense : vetor)
            for(string pref : preferencias)
                if(circense.especialidade == pref )
                    this->alegria += 1;

        /* MODO INDEXADO
         * for (int i = 0; i < vetor.size(); i++)
            for(int j = 0; j < preferencias.size(); j++)
                if(vetor[i].especialidade == preferencias[j])
                    this->alegria += 1;
        */

    }

    void addPreferencias(string pref){
        preferencias.push_back(pref);
    }


};

double lucroDaNoite(vector<Pessoa> plateia, double valorDoIngresso){
    double lucro = 0;
    for(Pessoa pessoa : plateia){
        if(pessoa.idade < 13){
            lucro += valorDoIngresso/2;
        }else{
            lucro += valorDoIngresso;
        }
    }
}

#include <vector>
#include <sstream>
int main(){
    vector<Pessoa> plateia;
    vector<string> especialidades = {"palhaco", "magico", "domador", "equilibrista", "contorcionista"};
    vector<string> nomes = {"Ana", "Maria", "Francisco","João","Marcelo","Pedro","Sandra","Neide",   "Junior","Elionai","Ronaldo","Jesse","Renato","Ediberto","Denilson","Lisandra","Fernanda","Lívia"};

    for(int i = 0; i < 5 + rand()%13; i++){
        plateia.push_back(Pessoa(nomes[rand()%17], 5 + rand() % 60, {especialidades[rand()%4], especialidades[rand()%4], especialidades[rand()%4]}));
    }

    vector<Circense> atracoes = {Circense("Tanguinha", "palhaco"),
                                 Circense("Shazam", "magico"),
                                 Circense("Juvenal", "domador"),
                                 Circense("Valentina", "contorcionista")};


    for(Pessoa &pessoa : plateia)
        pessoa.verEspetaculo(atracoes);

    puts("Feedback da plateia:  ");
    for(Pessoa &pessoa : plateia)
        cout << pessoa.nome << " " << pessoa.idade << " anos." << " Felicidade = " << pessoa.alegria << endl;

    double valorIngresso;
    puts("\nInforme o valor do ingresso para ver o lucro da noite: ");
    cin >> valorIngresso;
    cout << "O lucro da bilheteria foi de: R$" << lucroDaNoite(plateia,valorIngresso) << "." << endl;

}
