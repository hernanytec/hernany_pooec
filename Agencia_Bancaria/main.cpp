#include <iostream>
#include <vector>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

template <class T> T read(){
    T t;
    cin >> t;
    return t;
}

struct Operacao{
    string descricao;
    float valor;
    float saldoParcial;

    Operacao(string descricao = "", float valor = 0.0f, float saldoParcial = 0.0f){
        this->descricao = descricao;
        this->valor= valor;
        this->saldoParcial = saldoParcial;
    }
};

struct Conta{
    float saldo;
    int numero;
    vector<Operacao> extrato;
    bool ativa;

    Conta(float saldo = 0.0f, int numero = 0, vector<Operacao> extrato = {}, bool ativa = false){
        this->saldo = saldo;
        this->numero = numero;
        this->extrato = extrato;
        this->ativa = ativa;
    }

    bool sacar(float valor){
        if((this->saldo < valor) || (valor < 0))
            return false;
        this->saldo -= valor;
        this->extrato.push_back(Operacao("Saque:    ", -valor, this->saldo));
        return true;
    }

    bool depositar(float valor){
        if(valor < 1)
            return false;
        this->saldo += valor;
        this->extrato.push_back(Operacao("Deposito: ", valor, this->saldo));
        return true;
    }

    bool transferir(Conta other, float value){
        //TODO
    }

    vector<Operacao> getExtrato(){
        return this->extrato;
    }
};

struct Cliente{
    string cpf;
    vector<Conta> contas;

    Cliente(string cpf = "", vector<Conta> contas = {}){
        this->cpf = cpf;
        this->contas = contas;
    }
};

struct Agencia{
    vector<Cliente> clientes;
    Agencia(vector<Cliente> clientes = {}){
        this->clientes = clientes;
    }

    bool verificaCPF(string cpf){
        for(auto cliente : clientes)
            if(cliente.cpf == cpf)
                return true;
        return false;
    }

    bool abrirConta(string cpf){
        for(int i = 0; i < (int) clientes.size(); i++){
            Conta c;
            c.ativa = true;
            srand(time(NULL));
            c.numero = rand()  % 10000;
            //Descobrir a quantidade de contas ativas
            int qtd = 0;
            for(int j =0; j < (int) clientes[i].contas.size(); j++)
                if(clientes[i].contas[j].ativa)
                    qtd++;

            if(clientes[i].cpf == cpf &&  qtd < 2){
                clientes[i].contas.push_back(c);
                return true;
            }
        }
        return false;
    }

    bool encerrarConta(int numeroConta){
        for(Cliente& cliente : clientes){
            for(Conta& c : cliente.contas)
                if(c.numero == numeroConta && c.saldo == 0){
                    c.ativa = false;
                    return true;
                }
        }
        return false;
    }


    stringstream showClientData(string cpf){
        stringstream ss;
        float saldoTotal = 0;
        for(Cliente cliente : clientes){
            if(cliente.cpf == cpf){
                ss << "Cliente: " << cpf << endl;
                for(Conta c : cliente.contas){
                    ss << "Conta: " << c.numero << ", Saldo: " << c.saldo << ", Status: " << c.ativa << endl;
                    saldoTotal += c.saldo;
                }
                ss << "Saldo Total: " << saldoTotal;
            }
        }
        return ss;
    }

};

stringstream help(){
    stringstream ss;
    ss << "addCli $cpf" << endl
       << "abrirConta $cpf" << endl
       << "encerrarConta $conta" << endl
       << "login $cpf" << endl
       << "show" << endl

       << "saldo" << endl
       << "saque $valor" << endl
       << "deposito $valor" << endl
       << "extrato" << endl

       << "logout $cpf" << endl;
    return ss;
}

//Como o cliente não possuui nome, será ordenano pelo cpf
bool ordenaPorCPF(Cliente ini, Cliente fim){
    return ini.cpf < fim.cpf;
}

stringstream getContas(Cliente cliente){
    stringstream ss;
    ss << "[ ";
    for(Conta conta : cliente.contas){
        if(conta.ativa)
            ss << conta.numero << " ";
    }
    ss << "]";
    return ss;
}


int main()
{
    Agencia agencia;
    Cliente clienteTemp;
    string op = "";
    bool logado = false;
    string cpfClienteLogado;

    while (op != "sair") {
        if(op == "help"){
            cout << help().str() << endl;
            op = read<string>();
            system("clear");
        }
        if(op == "addCli"){
            clienteTemp.cpf = read<string>();
            if(agencia.verificaCPF(clienteTemp.cpf)){
                system("clear");
                cout << "Erro | cpf duplicado" << endl;
            }else{
                agencia.clientes.push_back(clienteTemp);
                agencia.abrirConta(clienteTemp.cpf);
                system("clear");
                cout << "Cliente adicionado"  << endl;
            }
        }
        if(op == "abrirConta"){
            string cpf = read<string>();
            int nConta;
            if(agencia.abrirConta(cpf)){
                for(Cliente& cliente : agencia.clientes){
                    for(Conta& c : cliente.contas)
                        if(cliente.cpf == cpf && c.saldo ==0){
                            nConta = c.numero;
                            system("clear");
                            cout <<  "Conta " << nConta << " aberta" << endl;
                        }
                }
            } else{
                system("clear");
                cout << "Erro" << endl;
            }
        }
        if(op == "encerrarConta"){
            int nConta = read<int>();
            if(agencia.encerrarConta(nConta)){
                system("clear");
                cout << "ok | conta " << nConta << " encerrada" << endl;
            }else{
                system("clear");
                cout << "Erro" << endl;
            }
        }
        if(op == "showAllCli"){
            std::sort(agencia.clientes.begin(),agencia.clientes.end(), ordenaPorCPF);
            for(Cliente x: agencia.clientes){
                system("clear");
                cout << x.cpf << " " << getContas(x).str() << endl;
            }
        }
        if(op == "login"){
            if(!logado){
                string cpfCompare = read<string>();
                string s = "cpf inválido";
                for(auto x : agencia.clientes)
                    if(x.cpf == cpfCompare){
                        logado = true;
                        cpfClienteLogado = cpfCompare;
                        s = "Bem-Vindo "+cpfClienteLogado;
                    }
                system("clear");
                cout << s << endl;
            }else{
                system("clear");
                cout << "Já existe um cliente logado" << endl;
            }
        }
        if(op == "show"){
            if(logado){
                system("clear");
                cout << agencia.showClientData(cpfClienteLogado).str() << endl;
            }
            else{
                system("clear");
                cout << "Cliente não logado" << endl;
            }
        }

        if(op == "saldo"){
            system("clear");
            if(logado){
                int nConta = read<int>();
                for(Cliente cliente : agencia.clientes)
                    if(cliente.cpf == cpfClienteLogado)
                        for(Conta& c : cliente.contas)
                            if(c.numero == nConta)
                                cout << "Saldo atual: R$ " << c.saldo << endl;
            }
            else {
                system("clear");
                cout << "Cliente não logado" << endl;
            }
        }
        if(op == "deposito"){
            system("clear");
            if(logado){
                int nConta = read<float>();
                for(Cliente& cliente : agencia.clientes)
                    if(cliente.cpf == cpfClienteLogado)
                        for(Conta& c : cliente.contas)
                            if(c.numero == nConta)
                                cout << (c.depositar(read<float>())? "Deposito efetuado com sucesso!" : "erro") << endl;
            }
            else {
                system("clear");
                cout << "Cliente não logado" << endl;
            }
        }
        if(op == "saque"){
            system("clear");
            if(logado){
                int nConta = read<float>();
                float value = read<float>();
                for(Cliente& cliente : agencia.clientes)
                    if(cliente.cpf == cpfClienteLogado)
                        for(Conta& c : cliente.contas)
                            if(c.numero == nConta){
                                cout << (c.sacar(value)? "Saque efetuado com sucesso!" : "erro") << endl;
                            }
            }
            else {
                system("clear");
                cout << "Cliente não logado" << endl;
            }

        }
        if(op == "extrato"){
            system("clear");
            if(logado){
                int nConta = read<float>();
                for(Cliente& cliente : agencia.clientes)
                    if(cliente.cpf == cpfClienteLogado)
                        for(Conta& c : cliente.contas)
                            if(c.numero == nConta)
                                for(Operacao operacao : c.getExtrato()){
                                    cout << operacao.descricao << operacao.valor << " saldo: R$ " << operacao.saldoParcial<< endl;
                                }
                            else {
                                system("clear");
                                cout << "Cliente não logado" << endl;
                            }
            }
            else {
                system("clear");
                cout << "Cliente não logado" << endl;
            }
        }


        if(op == "logout"){
            if(logado){
                logado = false;
                cpfClienteLogado = "";
                system("clear");
                cout << "Sessão encerrada" << endl;
            }else{
                system("clear");
                cout << "Nenhum cliente logado" << endl;
            }  }

        cout << "Digite um comando ou help: ";
        cin >> op;
    }

    return 0;
}



