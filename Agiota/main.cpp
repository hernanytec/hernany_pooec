#include <iostream>
#include <vector>

using namespace std;

void listaDeComandos(){
    cout  <<  "  iniciar    _valor              \n"<<
              "  emprestar  _idCliente _valor   \n"<<
              "  saldoSis                       \n"<<
              "  saldoCli   _idCliente          \n"<<
              "  pagar      _idCliente          \n"<<
              "  show       _idCliente          \n"<<
              "  showall                        \n"<<
              "  apagar     _idTr               \n"<<
              "  contarcli                      \n"<<
              "  fim                            \n"<<
              "  help                           \n" << endl;
}

class Transacao{
public:
    int idTr;
    string idCliente;
    int valor;

    Transacao(int idTr = 0, string idCliente = "", int valor = 0){
        this->idTr = idTr;
        this->idCliente = idCliente;
        this->valor = valor;
    }
    string toString(){
        string s = "[ "+to_string(idTr)+" "+idCliente+" "+to_string(valor)+" ]";
        return s;
    }
};

class Sistema{
public:
    int nextTrId = 0;
    vector<Transacao> transacoes;
    vector<string> clientes;
    int saldo;

    Sistema(int saldoInicial = 0){
        this->saldo = saldoInicial;
    }

    bool emprestar(string idCli, int valor){
        if(saldo < valor)
            return false;
        transacoes.push_back(Transacao(nextTrId++, idCli, -valor));
        if(!hasCliente(idCli))
            clientes.push_back(idCli);
        saldo -= valor;
        return true;
    }

    bool hasCliente(string nome){
        for(string cli : clientes)
            if(cli == nome)
                return true;
        return false;
    }

    bool receber(string idCli, int valor){
        if(valor > -saldoCliente(idCli))
            return false;
        transacoes.push_back(Transacao(nextTrId++, idCli, valor));
        saldo += valor;
        return true;
    }

    bool rmTran(int idTran){
        for(int i = 0; i < transacoes.size(); i++){
            if(transacoes[i].idTr == idTran){
                string idCli = transacoes[i].idCliente;
                transacoes.erase(transacoes.begin()+i);
                rmListUser(idCli);
                return true;
            }
        }
        return false;
    }

    void rmListUser(string idCli){
        for(Transacao tr: transacoes){
            if(tr.idCliente == idCli)
                return;
            for(int j = 0; j < clientes.size(); j++){
                if(clientes[j] == idCli)
                    clientes.erase(clientes.begin()+j);
            }
        }

    }

    string showAll(){
        string s = "";
        for(Transacao tr : transacoes){
            s.append(tr.toString()+"\n");
        }
        return s;
    }
    string showCliente(string idCli){
        string s = "";
        for(Transacao tr : transacoes){
            if(tr.idCliente == idCli)
                s.append(tr.toString()+"\n");
        }
        return s;
    }
    int saldoCliente(string idCli){
        int ac =  0;
        for(Transacao tr : transacoes){
            if(tr.idCliente == idCli){
                ac+=tr.valor;
            }
        }
        return ac;
    }
    int contarClientes(){
        return clientes.size();
    }
};


void comandLine(){
    string op;
    cin >> op;
    Sistema sistema;
    while(op != "fim"){
        if(op == "iniciar"){
            int valor;
            cin >> valor;
            sistema = Sistema(valor);
            cout <<  "Sistema iniciado com " + to_string(valor) + " reais" << endl;
        }
        else if(op =="emprestar"){
            string id;
            cin >> id;
            int valor;
            cin >> valor;
            if(sistema.emprestar(id, valor))
                cout << "Sucesso" << endl;
            else
                cout <<  "Fundos insuficientes" << endl;
        }
        else if(op =="saldoSis"){
            cout <<  to_string(sistema.saldo) + " reais" << endl;
        }
        else if(op == "saldoCli"){
            string id = "";
            cin >> id;
            cout << id << " deve R$: " << -sistema.saldoCliente(id) << endl;
        }
        else if(op =="pagar"){
            string id = "";
            cin >> id;
            int valor;
            cin >> valor;
            if(sistema.receber(id, valor))
                cout << "Sucesso" << endl;
            else
                cout <<  "Valor maior do que deveria" << endl;
        }
        else if(op =="show"){
            string id = "";
            cin >> id;
            cout << sistema.showCliente(id);
        }
        else if(op =="showAll"){
            cout << sistema.showAll()   ;
        }
        else if(op =="apagar"){
            int idTran;
            cin >> idTran;
            if(sistema.rmTran(idTran))
                cout << "ok" << endl;
            else
                cout << "Transação não encontrada" << endl;

        }
        else if(op =="contarcli"){
            cout << "Existem " << sistema.contarClientes() << " clientes diferentes no sistema" << endl;
        }
        else if(op =="help"){
            listaDeComandos();
        }
        else{
            cout << "comando invalido [" << op << "]" << endl;
        }
        cin >> op;
    }
}

int main()
{
    comandLine();
    return 0;
}
