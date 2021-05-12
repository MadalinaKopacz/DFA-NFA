///NFA
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
#include <iterator>
using namespace std;
ifstream fin("date.in.txt");
ofstream fout("date.out");

vector<pair<int, char> > adj[10000];
int N, M;
map <int, int> mp;            ///mapa de tipul {eticheta nod: pozitie in lista de adiacenta}
map <int, int> mp2;           ///mapa de tipul {pozitie in lista de adiacenta: eticheta nod}
int S;                        ///stare initiala
vector <int> f;               ///stari finale
int nrF, NrCuv;
vector <pair<int, int>> dr;   ///(stare, lungime cuv)
vector <int> acc[10000];

//in cadrul acestei functii vom afla care sunt nodurile accesibile prin tranzitiile cu lamda
void elimLTranzitii(int nod)
{  
    int x;
    map <int, int> vizitat;
    for (int i = 0; i < N; ++i) vizitat.insert({ i,0 });
    queue <int> Q;
    Q.push(nod);
    vector <int> accesibil;
    while (!Q.empty())
    {
        x = Q.front();
        Q.pop();
        accesibil.push_back(x);
        vizitat[x] = 1;
        for (auto j = adj[x].begin(); j != adj[x].end(); j++)    ///cautam urmatorul nod de bagat in coada
            if (j->second == '$')   //daca e lamda tranzitie
                if (vizitat[j->first] == 0) Q.push(j->first);   //bagam nodul aferent in coada
            
    }
    for (int i = 0; i < accesibil.size(); i++)   //eliminam duplicatele
    {
        bool ok = 1;
        for (int j = 0; j < acc[nod].size(); j++)
            if (acc[nod][j] == accesibil[i])
            {
                ok = 0; break;
                
            }
        if (ok)  acc[nod].push_back(accesibil[i]);
    }
}

bool LNFA(string cuv)
{
    vector <int> stCurente; 
    stCurente = acc[mp[S]];
    for (int i = 0; i < cuv.size(); i++)    //verificam pentru fiecare litera a cuvantului
    { 
        vector <int> stUrmatoare;
        for (int j = 0; j < stCurente.size(); j++)
            for (auto k = adj[stCurente[j]].begin(); k != adj[stCurente[j]].end(); k++)
                if (cuv[i] == k->second)
                    stUrmatoare.push_back(k->first);
        if (!stUrmatoare.empty())   stCurente = stUrmatoare;    //trecem la noile stari curente
        else return 0;

    } 
    for (int i = 0; i < stCurente.size(); i++)    //verificam daca una dintre starile la care am ajuns e stare curenta
        for (int j = 0; j < f.size(); j++)
            if (mp2[stCurente[i]] == f[j]) return 1;
    return 0;
}

void Citeste()
{
    int x, y; char l;
    string cuv;
    fin >> N;
    for (int i = 0; i < N; ++i) { fin >> x; mp.insert({ x,i }); mp2.insert({ i,x }); }    ///citim nodurile si stabilim corespondentele cu numere consec
    fin >> M;
    for (int i = 0; i < M; ++i)
    {
        fin >> x >> y >> l;
        int u = mp[x], v = mp[y];
        adj[u].push_back(make_pair(v, l));    ///cream lista de adiac cu ponderi
    }
    fin >> S;
    fin >> nrF;
    for (int i = 0; i < nrF; ++i) { fin >> x; f.push_back(x); }

    for (int i = 0; i < N; ++i)  elimLTranzitii(i);

    fin >> NrCuv;
    for (int i = 0; i < NrCuv; ++i)
    {
        fin >> cuv;
        if (LNFA(cuv)==1) fout << "DA\n";
        else fout << "NU\n";
    }
}

int main()
{
    Citeste();
    return 0;
}