#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

template <class T>
T read(){
    T t;
    cin >> t;
    return t;
}

void limparTela(){
    system("clear");
}

struct Espiral{
    string nome;
    int quantidade;
    float valor;

    Espiral(string nome = "", int quantidade = 0, float valor = 0.0f){
        this->nome = nome;
        this->quantidade = quantidade;
        this->valor = valor;
    }
};

struct Maquina{
    vector<Espiral> espirais;
    int maxProdutosPorEspiral;
    float saldoCliente;
    float lucro;

    Maquina(int qtd = 0, int maxProdutosPorEspiral = 0):
        espirais(qtd), maxProdutosPorEspiral(maxProdutosPorEspiral), saldoCliente(0),lucro(0)
    {}

    Maquina iniciar(int qtdEspirais, int maxProdutosPorEspiral){
       return Maquina(qtdEspirais, maxProdutosPorEspiral);
    }
    // metodo de alteração não funcional, BUGADO
    stringstream alterarEspiral(int indice, string nome, int quantidade, float valor){

        stringstream ss;
        if((indice <= (int)espirais.size()) && (indice >= 0)){
            espirais[indice].nome = nome;
            espirais[indice].quantidade = quantidade;
            espirais[indice].valor = valor;
            ss << "Espiral de indice " << indice << " alterada com sucesso!" << endl;
        }else{
            ss << "erro | indice invalido" << endl;
        }
        return ss;
    }

    stringstream vender(int indice){
        stringstream ss;
        if(indice < (int) espirais.size()){
            Espiral produtos =  espirais[indice];
            if(produtos.valor <= saldoCliente){
                produtos.quantidade--;
                this->saldoCliente -= produtos.valor;
                this->lucro = produtos.valor;
                ss << "Ok | você comprou um " << produtos.nome << endl;
            }
            else
                ss << "erro | Saldo insuficiente" << endl;
        }
        else
            ss << "erro | produto nao existe" << endl;
        return ss;
    }

    stringstream status(){
        stringstream ss;
        ss << "Saldo do cliente = R$" << saldoCliente << endl
           << "Lucro total = R$" << lucro << endl;

        return ss;
    }

    bool receberDinheiro(float dinheiro = 0){
        if(dinheiro > 0)
           return this->saldoCliente += dinheiro;
        else
            return false;
    }

    float getSaldo(){
        return saldoCliente;
    }

    float entregarTroco(){
        float troco = saldoCliente;
        this->saldoCliente = 0;
        return troco;
    }
};

void inicializar(){
    //BUGADO não funciona
    Maquina maquina;
    maquina.iniciar(5, 20);
    maquina.alterarEspiral(0, "Chocolate", 20, 3);
    maquina.alterarEspiral(1, "Cheetos", 20, 2);
    maquina.alterarEspiral(2, "Batata", 20, 4);
    maquina.alterarEspiral(3, "Refrigerante", 20, 3);
    maquina.alterarEspiral(4, "Doce", 20, 1);
}

void interacaoUsuario(Maquina& maquina){
    string op;
    while(op != "sair"){
        cout << "Digite comando ou ajuda: ";
        cin >> op;

        if(op == "ajuda"){
            cout << "---------" << endl
                 << "iniciar $qtdEspirais $maxProdutos" << endl
                 << "alterarEspiral $ind $nome $qtd $valor" << endl
                 << "inserirDin $valor" << endl
                 << "comprar $ind" << endl
                 << "pedirTroco" << endl
                 << "saldo" << endl
                 << "status" << endl
                 << "inicializar" << endl
                 << "sair" << endl;
        }

        if(op == "iniciar"){
            limparTela();
            maquina = Maquina(read<int>(), read<int>());
            cout << "Maquina iniciada!" << endl;
        }
        if(op == "inicializar"){
            limparTela();
            inicializar();
            cout << "Maquina inicializada!" << endl;
        }
        if(op == "alterarEspiral"){
            limparTela();
            cout << (maquina.alterarEspiral(read<int>(),read<string>(),read<int>(),read<float>())).str() << endl;
        }
        if(op == "inserirDin"){
            limparTela();
            if(maquina.receberDinheiro(read<float>()))
                cout << "Saldo disponível = R$" <<  maquina.getSaldo() << endl;
            else
                cout << "Erro | Valor inválido" << endl;
        }
        if(op == "comprar"){
            limparTela();
            cout << maquina.vender(read<int>()).str() << endl;
        }if(op == "pedirTroco"){
            limparTela();
            maquina.entregarTroco();
        }
        if(op == "saldo"){
            limparTela();
            cout << "Seu saldo é de R$" << maquina.getSaldo() << endl;
        }
        if(op == "status"){
            limparTela();
           cout << maquina.status().str() << endl;
        }
    }
}

int main()
{
    Maquina maquina;
    interacaoUsuario(maquina);
    return 0;
}
