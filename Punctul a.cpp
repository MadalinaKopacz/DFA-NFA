///DFA

#include <bits/stdc++.h>

using namespace std;
ifstream fin("date.in");
ofstream fout("date.out");

vector<pair<int, char> > adj[10000];
int N;
int M;
map <int, int> mp; ///mapa de tipul {eticheta nod: pozitie in lista de adiacenta}
map <int, int> mp2;///mapa de tipul {pozitie in lista de adiacenta: eticheta nod}
int S;   ///stare initiala
vector <int> f;  ///stari finale
int nrF, NrCuv;

void DFA(string cuv)
{
    bool ok = 0;
    int stc = mp[S], stu;     ///stare curenta, stare urmatoare
    for(int i = 0; i< cuv.size(); i++)
        {ok = 0;
         for( auto j = adj[stc].begin(); j != adj[stc].end(); j++)    ///parcurgem vecinii fiecarui nod
            if(j->second == cuv[i])    ///daca avem vecin care accepta litera curenta
                {ok = 1; stu = j -> first; break;}     ///memoram nodul urmator
        if(ok)   stc = stu;     ///daca am gasit updatam starea
        else {fout<<"NU\n"; return;}  ///daca nu gasim, iesim din program => cuv neacceptat

        }
    for(int i = 0; i < nrF; i++)
        if(mp2[stc] == f[i]) {fout<<"DA\n"; return;}     ///daca e stare finala
    fout<<"NU\n";  ///daca nu
}

void Citeste()
{
    int x, y; char l;
    string cuv;
    fin>>N;
    for(int i = 0; i < N; ++i)  {fin>>x; mp.insert({x,i});mp2.insert({i,x});}   ///citim nodurile si stabilim corespondentele cu numere consec
    fin>>M;
    for(int i = 0; i < M; ++i)
    {
        fin >> x >> y >> l;
        int u = mp[x], v = mp[y];
        adj[u].push_back(make_pair(v, l));   ///cream lista de adiac cu ponderi
    }
    fin>>S;
    fin>>nrF;
    for(int i = 0; i < nrF; ++i)  {fin>>x; f.push_back(x);}
    fin>>NrCuv;
    for(int i = 0; i < NrCuv; ++i)
    {
        fin>>cuv;
        DFA(cuv);
    }
}

int main()
{
    Citeste();
    return 0;
}

