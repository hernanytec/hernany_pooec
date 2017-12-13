#include <iostream>
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

struct Operacao{
    string descricao;
    float valor;
    float saldoParcial;
    Operacao(string descricao = "", float valor = 0, float saldoParcial = 0){
        this->descricao = descricao;
        this->valor = valor;
        this->saldoParcial = saldoParcial;
    }
};

class Conta{
private:
    int id;
    float saldo{0};
    vector<Operacao> extrato;
public:
    Conta(int id = 0){
        this->id = id;
    }
    float getSaldo(){
        return this->saldo;
    }
    int getId(){
        return this->id;
    }
    void setId(int id){
        this->id = id;
    }

    bool saque(float valor){
        if((this->saldo < valor) || (valor < 0))
            return false;

        this->saldo -= valor;
        this->extrato.push_back(Operacao("Saque:    ", -valor, this->saldo));

        return true;
    }

    bool deposito(float valor){
        if(valor < 1)
            return false;

        this->saldo += valor;
        this->extrato.push_back(Operacao("Deposito: ", valor, this->saldo));
        return true;
    }
    vector<Operacao> getExtrato(){
        return this->extrato;
    }

};

void inicializar(Conta& conta){
    conta = Conta(5);
    conta.deposito(50);
    conta.saque(10);
    conta.deposito(200);
    conta.saque(1000);//nao deve aparecer esse saque
}

void interacaoUsuario(Conta& conta){
    string op;

    while(op != "sair"){
        cout << "Digite comando ou ajuda: ";
        cin >> op;

        if(op == "ajuda"){
            cout << "---------" << endl
                 << "iniciar $id" << endl
                 << "saldo" << endl
                 << "saque $valor" << endl
                 << "deposito $valor" << endl
                 << "extrato" << endl
                 << "extratoN $qtdd" << endl
                 << "sair" << endl;
        }

        if(op == "iniciar"){
            limparTela();
            conta = Conta(read<int>());
            cout << "Conta criada!" << endl;
        }
        if(op == "saldo"){
            limparTela();
            cout << "Saldo atual: R$ " << conta.getSaldo() << endl;
        }
        if(op == "deposito"){
            limparTela();
            cout << (conta.deposito(read<float>())? "Deposito efetuado com sucesso!" : "erro") << endl;
        }
        if(op == "saque"){
            limparTela();
            cout << (conta.saque(read<float>())? "Saque efetuado com sucesso!" : "erro") << endl;
        }
        if(op == "extrato"){
            limparTela();
            for(Operacao operacao : conta.getExtrato()){
                cout << operacao.descricao << operacao.valor << " saldo: R$ " << operacao.saldoParcial<< endl;
            }
        }
    }
}

int main(){
    Conta conta;
    inicializar(conta);
    interacaoUsuario(conta);
}
