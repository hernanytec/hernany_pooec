#include <iostream>
#include <map>
#include <list>
#include "poo_aux.h"
#include "repository.h"

using namespace std;
string HELP = "addUser _user                         \n"
              "allUsers                              \n"
              "newChat _user _chatName               \n"
              "newTalk _user_a _user_b               \n"
              "chats _user                           \n"
              "invite _user_a _user_b _chatName      \n"
              "users _chatName                       \n"
              "leave _user _chatName                 \n"
              "zap _user _chatName __msg             \n"
              "notify _user                          \n"
              "help                                  \n";


class User;
class Zap{
public:
    string userId;
    string msg;

    Zap(string userId = "", string msg = ""){
        this->userId = userId;
        this->msg = msg;
    }
};


/*  Quando deixei os métodos add e leave como virtuais puros
    dava erro ao definir o repositório para a classe Chat
    então implementei os métodos, mas deixei-os como virtuais
    assim o talk, que herda de chat, pode alterar seu comportamento.
    por isso não tenho a classe Group, poi o próprio chat já faz a implementação dos metódos.
*/
class Chat{
public:
    string idChat;
    list<Zap>  listZap;
    vector<User*> listUsers;

    Chat(string idChat = ""){
        this->idChat = idChat;
    }
    void addFirstUser(User * convidado);
    virtual void addUser(string admId, User * convidado);
    virtual void leave(User * user);
    string getUsers();
    void deliverZap(Zap zap);
    string getUnread(User * user);
    int getUnreadCount(User * user);

    bool hashUser(string userId);
};

class User{
public:
    string id;
    map<string, int> naoLidas;
    map<string, Chat*> listaChat;
    vector<string> chats;

    User(string id = ""){
        this->id = id;
    }
    string myChats(){
        string ret = "[ ";
        for(auto elem :  chats){
            ret.append(elem + " ");
        }
        return ret.append("]");
    }

    string notifications(){
        string ret = "[ ";
        for(auto elem :  listaChat){
            ret.append(elem.second->idChat + "(" + to_string(elem.second->getUnreadCount(this)) + ") ");
        }
        return ret.append("]");
    }
};


void Chat::addFirstUser(User * convidado){
    listUsers.push_back(convidado);
    convidado->listaChat.insert(make_pair(this->idChat,this));
    convidado->chats.push_back(this->idChat);
}

void Chat::addUser(string admId, User * convidado){
    if(!hashUser(admId))
        throw admId + " não está no grupo";
    if(hashUser(convidado->id))
        throw convidado->id +" já está no grupo";
    listUsers.push_back(convidado);
    convidado->listaChat.insert(make_pair(this->idChat,this));
    convidado->chats.push_back(this->idChat);
}

void Chat::leave(User * user) {
    if(!hashUser(user->id))
        throw user->id + " não está no grupo";
    for(int i = 0; i < (int)listUsers.size(); i++){
        if(listUsers[i]->id == user->id){
            listUsers.erase(listUsers.begin()+i);
        }
    }
    user->listaChat.erase(idChat);
    for(int i = 0; i < (int)user->chats.size(); i++){
        user->chats.erase(user->chats.begin()+i);
    }
}

string Chat::getUsers(){
    string ret = "[ ";
    for(auto elem :  listUsers){
        ret.append(elem->id + " ");
    }
    return ret.append("]");
}

void Chat::deliverZap(Zap zap){
    //testa se usuário existe
    if(!hashUser(zap.userId))
        throw "Usuário " + zap.userId + " não está no grupo";
    //se existir adiciona o zap na lista do grupo
    listZap.push_front(zap);
    //incrementa o contador de não lidas desse grupo para cada usuário diferente do que enviou o zap
    for(User * user: listUsers){
        if(user->id != zap.userId){
            user->naoLidas[idChat] = getUnreadCount(user)+1;
        }
    }
}

string Chat::getUnread(User * user){
    if(!hashUser(user->id))
        throw "Usuário " + user->id + " não está no grupo";
    vector<Zap> vet;
    for(Zap msg : listZap){
        if(msg.userId != user->id && getUnreadCount(user) > 0){
            vet.push_back(msg);
            user->naoLidas[idChat] = getUnreadCount(user)-1;
            if(getUnreadCount(user) == 0)
                break;
        }
    }
    if(vet.size() == 0)
        return "Não há novas mensagens\n";
    string s = "";
    for(Zap zap : vet)
        s.append("["+zap.userId + ": " + zap.msg + "]\n");
    return s;
}

int Chat::getUnreadCount(User * user){
    auto it = user->naoLidas.find(idChat);
    if(it != user->naoLidas.end())
        return it->second;
    return 0;
}

bool Chat::hashUser(string userId){
    for(User * user : listUsers){
        if(user->id == userId)
            return true;
    }
    return false;
}

class Talk: public Chat
{
    vector<string> idTalks;

public:
    Talk(User * user1, User * user2){
        idChat = defineId(user1->id, user2->id);
        add(user1);
        add(user2);
    }

    string defineId(string str1, string str2){
        if(str1 > str2)
            swap(str1, str2);
        string newId = (str1+"-"+str2);
        for(auto id: idTalks)
            if(id == newId)
                throw "talk " + newId + " já existe";
        idTalks.push_back(newId);
        return newId;
    }

    void add(User * user){
        listUsers.push_back(user);
        user->listaChat.insert(make_pair(this->idChat, this));
        user->chats.push_back(this->idChat);
    }

    void addUser(string s, User * user){
        throw (string) "fail: operacao de acionar usuarios nao suportada";
    }

    void leave(User * user){
        throw (string) "fail: operacao de sair do talk nao suportada";
    }
};

class Whatsapp{
    Repository<Chat*> r_chat;
    Repository<User> r_user;

public:
    Whatsapp():
        r_chat("chat"),r_user("user")
    {

    }
    string process(string line, char token){
        auto ui = split(line, token);
        auto cmd = ui[0];
        if(cmd == "help")
            return HELP;
        else if(cmd == "addUser"){
            r_user.add(ui[1], User(ui[1]));
        }
        else if(cmd == "newChat"){
            (*r_chat.add(ui[2], new Chat(ui[2])))->addFirstUser(r_user.getValue(ui[1]));
        }
        else if(cmd == "newTalk"){
            Talk * t = new Talk(r_user.getValue(ui[1]), r_user.getValue(ui[2]));
            r_chat.add(t->idChat, t);
        }
        else if(cmd == "invite"){
            (*r_chat.getValue(ui[3]))->addUser(ui[1], r_user.getValue(ui[2]));;
        }
        else if(cmd == "leave"){
            (*r_chat.getValue(ui[2]))->leave(r_user.getValue(ui[1]));
        }
        else if(cmd == "zap"){//_user _chat _my_msg
            (*r_chat.getValue(ui[2]))->deliverZap(Zap(ui[1], slacing(3, ui)));
        }
        else if(cmd == "ler"){//_user _chat
            return (*r_chat.getValue(ui[2]))->getUnread(r_user.getValue(ui[1]));
        }
        else if(cmd == "notify"){//_user
            return r_user.getValue(ui[1])->notifications();
        }
        else if(cmd == "chats"){//_user
            return r_user.getValue(ui[1])->myChats();
        }
        else if(cmd == "allUsers"){
            string ret = "[ ";
            for(auto elem :  r_user.getValues()){
                ret.append(elem.id + " ");
            }
            return ret.append("]");
        }
        else if(cmd == "users"){//_chat
            return "" + (*r_chat.getValue(ui[1]))->getUsers();
        }
        else if(cmd == "showUsers"){
            string ret = "[ ";
            for(auto elem :  r_user.getValues()){
                ret.append(elem.id + " ");
            }
            return ret.append("]");
        }
        else
            return string("") + "comando invalido " + "[" + cmd + "]";
        return "done";
    }

    void commandLine(){
        string line;
        getline(cin, line);
        while(line != "fim"){
            try{
                cout << process(line, ' ') << endl;
                getline(cin, line);
            }
            catch(string &str){
                cout << str << endl;
                getline(cin, line);
            }
        }
        cout << "Encerrado" << endl;
    }
};

int main(){
    Whatsapp c;
    c.commandLine();

    return 0;
}
