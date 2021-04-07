///NFA
#include <bits/stdc++.h>
using namespace std;
ifstream fin("date.in");
ofstream fout("date.out");


vector<pair<int, char> > adj[10000];
int N, M;
map <int, int> mp;  ///mapa de tipul {eticheta nod: pozitie in lista de adiacenta}
map <int, int> mp2; ///mapa de tipul {pozitie in lista de adiacenta: eticheta nod}
int S;    ///stare initiala
vector <int> f;   ///stari finale
int nrF, NrCuv;
vector <pair<int,int>> dr;  ///(stare, lungime cuv)
int ulnod;

bool NFA(string cuv)
{
    int stc = mp[S];
    pair <int, int> x;
    queue <pair<int, int>> C;
    C.push(make_pair(stc, 0));
    dr.clear();

    while(C.size()>0)   ///BFS
    {
       x = C.front();   ///preluam primul elem din coada
       C.pop();
       dr.push_back(make_pair(x.first, x.second));
       if(x.second == cuv.size())
                   {
                    for(int i = 0; i < nrF; i++)     ///cautam stare finala
                           if(mp2[x.first] == f[i]) {ulnod = mp2[f[i]];return 1;}
                   }
       for( auto j = adj[x.first].begin(); j != adj[x.first].end(); j++)    ///cautam urmatorul nod de bagat in coada
            if(cuv[x.second] == j->second)    ///daca litera curenta e acceptata
                 C.push(make_pair(j->first, x.second+1));    ///o adaugam in coada
    }
    return 0;
}

void Lant(string cuv)
{
    int n = dr[dr.size()-1].first;
    int p = dr[dr.size()-1].second;
    vector <int> fi;   ///vector drum final
    fi.push_back(n);

    for(auto j = dr.end()-1; j != dr.begin(); j--)   ///parcurgem vectorul dr
        if(j->second == p-1 )     ///daca pozitiile sunt convenabile
             for(auto k = adj[j->first].begin(); k != adj[j->first].end(); k++)    ///cautam "tatal"
                if(k->first == n && cuv[p-1]==k->second)   ///avem muchie+tranzitie
                    {
                        n = j->first;      ///trecem la tatal nodului curent
                        p--;
                        fi.push_back(n);    ///il adaugam in vectorul final
                        break;
                    }
    for(int i = fi.size()-1; i >=0 ;i--) fout<<mp2[fi[i]]<<" ";    ///afisam de la cap la coada
}

void Citeste()
{
    int x, y; char l;
    string cuv;
    fin>>N;
    for(int i = 0; i < N; ++i)  {fin>>x; mp.insert({x,i});mp2.insert({i,x});}    ///citim nodurile si stabilim corespondentele cu numere consec
    fin>>M;
    for(int i = 0; i < M; ++i)
    {
        fin >> x >> y >> l;
        int u = mp[x], v = mp[y];
        adj[u].push_back(make_pair(v, l));    ///cream lista de adiac cu ponderi
    }
    fin>>S;
    fin>>nrF;
    for(int i = 0; i < nrF; ++i)  {fin>>x; f.push_back(x);}
    fin>>NrCuv;
    for(int i = 0; i < NrCuv; ++i)
    {
        fin>>cuv;
        if(NFA(cuv))
                {
                    fout<<"DA: "<<S<<" ";
                    Lant(cuv);
                    fout<<"\n";
                }
        else fout<<"NU\n";
    }
}

int main()
{
    Citeste();
    return 0;
}
