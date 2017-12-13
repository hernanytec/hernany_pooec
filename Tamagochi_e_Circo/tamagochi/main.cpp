#include <iostream>
#include <cstdio>

using namespace std;

string catalogoPokemom(int nPokemon){
    switch (nPokemon) {
    case 1:
        return "Bulbasaur";
        break;
    case 2:
        return "Ivysaur";
        break;
    case 3:
        return "Venusaur";
        break;
    case 4:
        return "Charmander";
        break;
    case 5:
        return "Charmeleon";
        break;
    case 6:
        return "Charizard";
        break;
    case 7:
        return "Squirtle";
        break;
    case 8:
        return "Wartortlhe";
        break;
    case 9:
        return "Blastoise";
        break;
    case 10:
        return "Pichu";
        break;
    case 11:
        return "Pikachu";
        break;
    case 12:
        return "Raichu";
        break;
    default:
        return "Pokemom desconhecido";
        break;
    }
}

string escolherPokemom(int *numPokemom){
    cout << "Escolha seu pokemom: \n"
         << "1 - Bulbasaur \n"
         << "4 - Charmander \n"
         << "7 - Squirtle \n"
         << "10 - Pichu \n" << endl;

    cin >> *numPokemom;

    return catalogoPokemom(*numPokemom);
}


struct Pokemom{
    int experiencia;
    string nome;
    bool vivo;
    int forca;
    int vida;
    int numeroPokemon;
    int pontoDeEvolucao;

    Pokemom(string nome, int numero){
        this->nome = nome;
        this->numeroPokemon = numero;
        experiencia = 0;
        vivo = true;
        forca = 3 + rand() % 3;
        vida = 16 + rand() % 7;
        pontoDeEvolucao = 5 + rand() % 5;
    }

    void levarDano(int value){
        this->vida -= value;
        if(vida < 0)
            vivo = false;
    }

    void atacar(Pokemom &other){
        cout << "Meu " << nome << "(" << vida << ")"
             << " ataca "
             << other.nome << "(" << other.vida << ")"
             << endl;
        if(!vivo){
            cout << nome << "esta morto" << endl;
            return;
        }
        int dano = 1 + rand () % this->forca;
        other.levarDano(dano);
        cout << "golpe " << dano;
        if(other.vivo){
            int golpe = rand () % other.forca;
            this->levarDano(golpe);
            cout << " , contragolpe " << golpe << endl;
        }
        evoluir(other);
    }

    void evoluir(Pokemom &p2){

        if(this->vivo){
            this->experiencia ++;
            if(this->experiencia > this->pontoDeEvolucao){
                if(this->numeroPokemon % 3 != 0){
                    this->experiencia=0;
                    this->vida += 10 + rand() % 5;
                    this->forca = 1 + rand() % 2;
                    cout << "\n" << this->nome << " Evoluiu para "<< catalogoPokemom(this->numeroPokemon+1) << endl;
                    this->nome = catalogoPokemom(this->numeroPokemon+1);
                    this->numeroPokemon++;
                }
            }
        }

        if(p2.vivo){
            this->experiencia ++;
            if(p2.experiencia >= p2.pontoDeEvolucao){
                if(p2.numeroPokemon % 3 != 0){
                    p2.experiencia=0;
                    p2.vida += 10 + rand() % 5;
                    p2.forca = 1 + rand() % 2;
                    cout << "\n" << p2.nome << " Evoluiu para "<< catalogoPokemom(p2.numeroPokemon+1) << endl;
                    p2.nome = catalogoPokemom(p2.numeroPokemon+1);
                    p2.numeroPokemon++;
                }
            }
        }
    }

    void show(){
        cout << "Nome: " << this->nome
             << "\nForca: " << this->forca
             << "\nVida: " << this->vida << "\n" << endl;
    }

};


#include <cstdlib>
#include <ctime>

int main()
{
    srand(time(NULL));
    int numPokemom;
    Pokemom * p1 = new Pokemom(escolherPokemom(&numPokemom), numPokemom);
    Pokemom * p2 = new Pokemom(catalogoPokemom((numPokemom = 1+rand()%12)), numPokemom);

    //p2->vida += 30;

    system("clear");
    cout << p1->nome << " VS " << p2->nome << "\n" << endl;

    p1->show();
    p2->show();

    while(p1->vivo && p2->vivo){
        p1->atacar(*p2);
        getchar();
    }
    if(p1->vivo)
        cout << p1->nome << " venceu!" << endl;
    else
        cout << p2->nome << " venceu!" << endl;

    return 0;
}




