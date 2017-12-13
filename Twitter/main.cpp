#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <sstream>

using namespace std;


string vetToStr(vector<string> vet);

class Tweet{
public:
    int idTw;
    string username;
    string msg;
    vector<string> likes;

    Tweet(int idTw = 0, string username = "", string msg = "")
    {
        this->idTw = idTw;
        this->username = username;
        this->msg = msg;
    }
    void darLike(string username){
        for(auto like: likes)
            if(like == username)
                return;
        likes.push_back(username);
    }

    string toString(){
        string s = idTw+" "+ username+ ": " + msg +"{" + vetToStr(likes) + "}";
        return s;
    }
};


class User{
public:
    string username;
    vector<User*> seguidores;
    vector<User*> seguidos;
    list<Tweet*> myTweets;
    list<Tweet*> timeLine;
    int naoLidas;

    User(string username = ""):
        naoLidas(0)
    {
        this->username = username;
    }

    void seguir(User * other){
        seguidos.push_back(other);
        other->seguidores.push_back(this);
    }

    void twittar(Tweet * tweet){
        myTweets.push_front(tweet);
        for(User * seguidor: seguidores){
            seguidor->naoLidas++;
            seguidor->timeLine.push_front(tweet);
        }
    }
    void darLike(int idTw){
        for(Tweet * tweet : timeLine){
            if((tweet->idTw == idTw)){
                tweet->darLike(this->username);
                return;
            }
        }
        throw "tweet "+ to_string(idTw) +" não existe";
    }

    string getUnread(){
        vector<Tweet*> vet;
        string s = "";
        for(auto msg : timeLine){
            if(this->naoLidas == 0)
                break;
            vet.push_back(msg);
            this->naoLidas--;
        }
        if(vet.size() == 0)
            return "Não há novos tweets\n";
        for(auto tw : vet)
            s.append(to_string(tw->idTw) + " " + tw->username + ": "+tw->msg +"{ "+vetToStr(tw->likes)+"}\n");
        return s;
    }

    string getTimeline(){
        string s = "";
        for(Tweet * tw: timeLine)
            s.append(to_string(tw->idTw) + " " + tw->username + ": "+tw->msg +"{ "+vetToStr(tw->likes)+"}\n");
        this->naoLidas = 0;
        return s;
    }
    string getMyTwitts(){
        string s = "";
        for(Tweet * tw: myTweets)
            s.append(to_string(tw->idTw) + " " + tw->username + ": "+tw->msg +"\n");
        return s;
    }

    string toString(){
        return username;
    }
};
string vetToStr(vector<string> vet){
    string str = "";
    for(auto elem: vet){
        str.append(elem + " ");
    }
    return str;
}


template <class T>
class Repository{
    map<string, T> mapa;
    string tipoRepository;

public:
    Repository(string tipoRepository = ""){
        this->tipoRepository = tipoRepository;
    }

    void add(string chave, T t){
        if(!mapa.insert(make_pair(chave,t)).second)
            throw tipoRepository + " " + chave + " já existe";
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

class TweetGenerator{
public:
    Repository<Tweet> * r_tw;
    int nextId = 0;

    TweetGenerator(Repository<Tweet> * r_tw){
        this->r_tw = r_tw;
    }
    Tweet * create(string username, string msg){
        Tweet tw(nextId, username, msg);
        r_tw->add(to_string(nextId), tw);
        nextId++;
        return r_tw->getValue(to_string(nextId-1));
    }
};
vector<string> split(string str, char delim);

class Controller{
public:
    Repository<User> r_user = Repository<User>("User");
    Repository<Tweet> r_tw = Repository<Tweet>("Tweet");
    TweetGenerator twG = TweetGenerator((&r_tw));

    void comandLine(){
        string cmd;
        getline(cin, cmd);
        while(cmd != "fim"){
            try{
                vector<string> linha = split(cmd, ' ');
                if(linha[0] == "addUser"){
                    r_user.add(linha[1], User(linha[1]));
                    cout << "done" << endl;
                }
                else if(linha[0] == "users"){
                    string s = "[ ";
                    for(auto elem: r_user.getValues()){
                        s.append(elem.toString() + " ");
                    }
                    cout << s.append("]\n");
                }
                else if(linha[0] == "seguir"){
                    r_user.getValue(linha[1])->seguir(r_user.getValue(linha[2]));
                    cout << "done" << endl;
                }
                else if(linha[0] == "seguidos"){
                    string s = "[ ";
                    auto user = r_user.getValue(linha[1]);
                    for(auto elem: user->seguidos){
                        s.append(elem->toString() + " ");
                    }
                    cout << s.append("]\n");
                }
                else if(linha[0] == "seguidores"){
                    string s = "[ ";
                    auto user = r_user.getValue(linha[1]);
                    for(auto elem: user->seguidores){
                        s.append(elem->toString() + " ");
                    }
                    cout << s.append("]\n");
                }
                else if(linha[0] == "twittar"){
                    string s = "";
                    for(int i = 2; i < (int) linha.size(); i++)
                        s.append(linha[i] +" ");
                    User * user =  r_user.getValue(linha[1]);
                    Tweet * tw = twG.create(linha[1], s);
                    user->twittar(tw);
                    cout << "done" << endl;
                }
                else if(linha[0] == "timeline"){
                    cout << r_user.getValue(linha[1])->getTimeline();
                }
                else if(linha[0] == "myTweets"){
                    cout << r_user.getValue(linha[1])->getMyTwitts();
                }
                else if(linha[0] == "unread"){
                    cout << r_user.getValue(linha[1])->getUnread();
                }
                else if(linha[0] == "like"){
                    r_user.getValue(linha[1])->darLike(std::stoi(linha[2]));
                    cout << "done" << endl;
                }else{
                    cout << "comando invalido [" << linha[0] << "]" << endl;
                }
                getline(cin, cmd);
            }
            catch(string &str){
                cout << str << endl;
                getline(cin, cmd);
            }
        }
    }

    vector<string> split(string str, char delim = ' ') {
        stringstream ss(str);
        string tok;
        vector<string> vec;
        while (getline(ss, tok, delim)) {
            if (!tok.empty())
                vec.push_back(tok);
        }
        return vec;
    }
};


int main()
{
    Controller c;
    c.comandLine();

}
