#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <queue>
#include <algorithm>  
using namespace std;

ifstream fin("date.in.txt");
ofstream fout("date.out");

set <char> alfabet;  //alfabetabetul pe care il cream noi pe masura ce citim muchiile

class NFA
{
    int N, S;
    vector <map <char, set<int> > > adj;    //pe pozitia i cu l ajungem 
    vector <bool> f;
public:
    int getN() const { return N; }
    int getStareInitiala() const { return S; }
    vector <map <char, set<int> > > getTranz() const { return adj; }
    vector <bool> getF() const { return f; }
    friend istream& operator>>(istream&, NFA&);
    friend ostream& operator<<(ostream&, const NFA&);
};
istream& operator >>(istream& in, NFA& nfa)      //supraincarcare >> pt citire NFA
{
    int nrM, x, y, nrF;  char l;
    in >> nfa.N >> nrM;
    nfa.adj.resize(nfa.N + 1), nfa.f.resize(nfa.N + 1);  //indexare de la 1
    for (int i = 1; i <= nrM; ++i)
    {
        in >> x >> y >> l;
        nfa.adj[x][l].insert(y);
        alfabet.insert(l);
    }
    in >> nfa.S;
    in >> nrF;
    for (int i = 1, x; i <= nrF; ++i)  in >> x, nfa.f[x] = 1; 
    return in;
}
ostream& operator <<(ostream& out, const NFA& nfa)   //supraincarcare << pt afisare NFA
{
    out << nfa.N << " stari\n";
    out << "Starea initiala: " << nfa.S << '\n';
    out << "Stari finale: ";
    for (int i = 1; i <= nfa.N; ++i)
        if (nfa.f[i]) out << i << " ";
    out << '\n';
    for (int i = 1; i <= nfa.N; ++i)
        for (auto it : nfa.adj[i])
            for (auto j : it.second) out << i << " " << j << " " << it.first << '\n';

    return out;
}

class DFA
{
    int N, Sd;
    vector <map <char, int> > adj;
    vector <bool> f;
    void removeStates(vector <bool>& unreachable)
    {
        // starile in care nu putem ajunge
        int nod;
        queue <int> Q;
        vector <bool> viz(N + 1, 0);
        Q.push(Sd);  viz[Sd] = 1;
        while (!Q.empty())
        {
            nod = Q.front();
            Q.pop();
            for (auto it : adj[nod])
                if (!viz[it.second])
                {
                    viz[it.second] = 1;
                    Q.push(it.second);
                }
        }
        int out = 0;
        for (int i = 1; i <= N; ++i)
            if (!viz[i])
            {
                unreachable[i] = 1;
                out++;
            }
        N -= out;


        // stari din care nu ajungem intr-o stare finala
        viz.clear();
        viz.resize(N + 1, 0);
        vector <vector <int> > inv(N + 1);
        for (int i = 1; i <= N; ++i)
            for (auto it : adj[i]) inv[it.second].push_back(i);

        for (int i = 1; i <= N; ++i)
            if (f[i])
            {
                Q.push(i);
                viz[i] = 1;
            }
        while (!Q.empty())
        {
            nod = Q.front();
            Q.pop();
            for (auto it : inv[nod])
                if (!viz[it])
                {
                    viz[it] = 1;
                    Q.push(it);
                }
        }
        out = 0;
        for (int i = 1; i <= N; ++i)
            if (!viz[i])
            {
                unreachable[i] = 1;
                out++; 
            }
        N -= out;
    }
    int equiv(int x, vector <vector <int> > sets)
    {
        for (int i = 0; i < sets.size(); ++i)
            for (int j = 0; j < sets[i].size(); ++j)
                if (sets[i][j] == x) return i;
    }
public:
    friend ostream& operator <<(ostream&, const DFA&);    
    DFA(const NFA& nfa)     //constructor in care cream un DFA pe baza unui NFA => practic tranformam NFA in DFA
    {
        N = 0;
        map <set <int>, int> stari;     // map de forma {din ce e formata noua stare: nume stare}
        vector <map <char, set<int> > > nfaAdj = nfa.getTranz();
        vector <bool> nfaF = nfa.getF();  //stari finale in NFA
        f.push_back(0);                   // incepem de pe 1
        queue <set <int> > Q;
        set <int> s;

        Sd = nfa.getStareInitiala();    //starea initiala e aceeasi cu a NFA-ului
        s.insert(Sd);
        f.push_back(nfaF[Sd]);         //daca sd e stare finala 
        stari[s] = ++N;                 
        adj.push_back(map <char, int>()); adj.push_back(map <char, int>());   //initializam noua matrice de adiacenta sa avem destule pozitii unde putem adauga elemente
        Q.push(s);
        while (!Q.empty())
        {
            set <int> x = Q.front(); Q.pop();
            for (char c : alfabet)
            {
                s.clear();
                for (auto it : x)
                    if (nfaAdj[it].count(c))
                        for (auto urm : nfaAdj[it][c]) s.insert(urm);  //adaugam vecin
                if (s.size()!=0 && stari.count(s)==0)   //avem o stare inca nepusa in vectorul de stari
                {
                    stari[s] = ++N;
                    bool sf = 0;    
                    for (auto it : s)   //daca una dintre starile din componenta primei stari e finala in NFA=>noua stare e stare finala 
                        if (nfaF[it]) sf = 1;
                    f.push_back(sf);
                    adj.push_back(map <char, int>());
                    Q.push(s);   //adaugam in coada noua stare
                }
                adj[stari[x]][c] = stari[s];     //vecinul lui x (denumite stari[x]) in care se poate ajunge cu litera c este starea noua s denumite stari[s]
            }
        }
    }
    //documentatie algoritm de minimizare: https://www.geeksforgeeks.org/minimization-of-dfa/
    void minimisation()
    {
        vector <bool> unreachable(N + 1, 0);
        removeStates(unreachable);

        vector <int> aux1[2];
        for (int i = 1; i <= N; ++i)
            if (!unreachable[i]) aux1[f[i]].push_back(i);    //impartim starile in care putem ajungein stari initiale si finale
        vector <vector <int> > sets1, sets2;    
        sets1.push_back(aux1[0]); sets1.push_back(aux1[1]);    //impartim in cele doua multimi de stari

        sets2 = sets1;
        do
        {
            sets1 = sets2;

            for (int i = 0; i < sets2.size(); ++i)
                for (int j = 1; j < sets2[i].size(); ++j)
                {
                    bool ePusOK = 0;
                    for (int k = 0; k < j; ++k)
                    {
                        bool disting = 0;
                        for (char c : alfabet)
                            if (adj[sets2[i][j]].count(c) && adj[sets2[i][k]].count(c) && !unreachable[adj[sets2[i][j]][c]] && !unreachable[adj[sets2[i][k]][c]])
                                //doua stari sunt distinguishable in partitie daca pentru orice simbol c ajungem in seturi diferite din artitia anterioara
                                if (equiv(adj[sets2[i][j]][c], sets1) != equiv(adj[sets2[i][k]][c], sets1))
                                {
                                    disting = 1;
                                    break;
                                }
                        if (!disting)
                        {
                            ePusOK = 1;
                            break;
                        }
                    }
                    if (ePusOK) continue;

                    for (int I = sets1.size(); I < sets2.size() && !ePusOK; ++I)
                        for (int J = 0; J < sets2[I].size(); ++J)
                        {
                            bool disting = 0;
                            for (char c : alfabet)
                                if (adj[sets2[i][j]].count(c) && adj[sets2[I][J]].count(c) && !unreachable[adj[sets2[i][j]][c]] && !unreachable[adj[sets2[I][J]][c]])
                                    if (equiv(adj[sets2[i][j]][c], sets1) != equiv(adj[sets2[I][J]][c], sets1))
                                    {
                                        disting = 1;
                                        break;
                                    }
                            if (!disting)
                            {
                                sets2[I].push_back(sets2[i][j]);
                                sets2[i].erase(sets2[i].begin() + j);
                                ePusOK = 1;
                                break;
                            }
                        }
                    if (ePusOK) continue;
                    sets2.push_back(vector <int>({ sets2[i][j] }));
                    sets2[i].erase(sets2[i].begin() + j);
                }
        } while (sets1 != sets2);

        vector <int> newNodes(N + 1, 0);
        int n = 0;
        for (int i = 0; i < sets2.size(); ++i)      //cream noile noduri 
        {
            n++;
            for (int j = 0; j < sets2[i].size(); ++j) newNodes[sets2[i][j]] = n;
        }
        vector <map <char, int> > newTranz(n + 1, map <char, int>());
        for (int i = 0; i < sets2.size(); ++i)
        {
            int first = sets2[i][0];
            for (char c : alfabet)
                if (adj[first].count(c) && !unreachable[adj[first][c]]) newTranz[newNodes[first]][c] = newNodes[adj[first][c]];
        }
        vector <bool> newF(n + 1, 0);                  //noile stari finale
        for (int i = 0; i < sets2.size(); ++i)
            if (f[sets2[i][0]]) newF[newNodes[sets2[i][0]]] = 1;
        Sd = newNodes[equiv(Sd, sets2) + 1];    //noua stare initialakm
        N = n;
        f = newF;
        adj = newTranz;
    }
};
ostream& operator <<(ostream& out, const DFA& a)  //supraincarcare << pt afisare DFA
{
    out << a.N << " stari\n";
    out << "Starea initiala: " << a.Sd << '\n';
    out << "Stari finale: ";
    for (int i = 1; i <= a.N; ++i)
        if (a.f[i]) out << i << " ";
    out << '\n';
    for (int i = 1; i <= a.N; ++i)
        if (!a.adj[i].empty())
            for (auto it : a.adj[i]) 
                out << i << " " << it.second << " " << it.first << '\n';
    return out;
}


int main()
{
    NFA nfa;
    fin >> nfa;
    cout << "NFA initial:\n";  cout << nfa;
    DFA dfa(nfa);
    cout << "\nDFA obtinut din NFA:\n";  cout << dfa;
    vector <set <int>> P;
    dfa.minimisation();
    cout << "\nDFA minimizat:\n" << dfa;
    return 0;
}