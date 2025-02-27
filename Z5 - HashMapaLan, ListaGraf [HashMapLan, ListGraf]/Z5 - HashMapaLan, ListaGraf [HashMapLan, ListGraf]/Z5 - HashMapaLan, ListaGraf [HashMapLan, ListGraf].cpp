﻿#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <iomanip>
#include <vector>
using namespace std;


template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    Mapa<TipKljuca, TipVrijednosti>() {}
    virtual ~Mapa<TipKljuca, TipVrijednosti>() {}
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca& kljuc) = 0;
    virtual TipVrijednosti& operator[](TipKljuca i) = 0;
    virtual const TipVrijednosti operator[](TipKljuca i) const = 0;
};


template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa < TipKljuca, TipVrijednosti> {
    int velicina, brEl;
    pair<TipKljuca, TipVrijednosti>** p;

public:
    NizMapa<TipKljuca, TipVrijednosti>() : p(new pair<TipKljuca, TipVrijednosti>* [100000]()), velicina(100000), brEl(0) {}

    NizMapa<TipKljuca, TipVrijednosti>(const NizMapa<TipKljuca, TipVrijednosti>& a)
    {
        brEl = a.brEl;
        velicina = a.velicina;
        if (a.velicina == 0) p = nullptr;
        else
        {
            p = new pair<TipKljuca, TipVrijednosti>* [velicina]();
            for (int i = 0;i < a.brEl;i++) p[i] = new pair<TipKljuca, TipVrijednosti>(*a.p[i]);
        }
    }

    NizMapa<TipKljuca, TipVrijednosti>& operator=(const  NizMapa<TipKljuca, TipVrijednosti>& a)
    {
        if (this == &a) return *this;
        obrisi();
        brEl = a.brEl;
        if (a.Velicina() > Velicina())
        {
            delete[] p;  p = new std::pair<TipKljuca, TipVrijednosti>* [velicina + a.Velicina()]();
            velicina += a.velicina;
        }
        for (int i = 0; i < a.brEl; i++) p[i] = new pair<TipKljuca, TipVrijednosti>(*a.p[i]);
        return *this;
    }

    ~NizMapa<TipKljuca, TipVrijednosti>() { for (int i = 0;i < brEl;i++) delete p[i]; delete[] p; }

    int brojElemenata() const { return brEl; }

    int Velicina() const { return velicina; }

    void obrisi()
    {
        if (brEl == 0) throw range_error("Nema elemenata");
        for (int i = 0; i < brEl; i++) delete p[i];
        brEl = 0;
    }

    void obrisi(const TipKljuca& kljuc)
    {
        for (int i = 0; i < brEl; i++)
            if (p[i]->first == kljuc)
            {
                delete p[i];
                for (int j = i; j < brEl - 1; j++) p[j] = p[j + 1];
                p[brEl] = nullptr;
                brEl--;
                return;
            }
        throw logic_error("Nema kljuca");
    }

    TipVrijednosti& operator[](TipKljuca kljuc)
    {
        if (brEl == velicina)
        {
            pair<TipKljuca, TipVrijednosti>** temp = new pair<TipKljuca, TipVrijednosti>* [10000 + velicina]();
            for (int i = 0; i < brEl; i++)
            {
                temp[i] = new pair<TipKljuca, TipVrijednosti>(*p[i]);
                delete p[i];
            }
            delete[] p;
            velicina += 10000;
            p = temp;
            temp = nullptr;
        }
        for (int i = 0; i < brEl; i++)
            if (p[i]->first == kljuc) return p[i]->second;
        p[brEl] = new pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti()); brEl++;
        return p[brEl - 1]->second;
    }

    const TipVrijednosti operator[](TipKljuca kljuc) const
    {
        for (int i = 0; i < brEl; i++)
            if (p[i]->first == kljuc) return p[i]->second;
        return TipVrijednosti();
    }
};


template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {

    struct Cvor
    {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor* l_strana, * d_strana, * roditelj;
        Cvor(TipKljuca i = TipKljuca(), TipVrijednosti u = TipVrijednosti()) : kljuc(i), vrijednost(u), d_strana(nullptr), l_strana(nullptr), roditelj(nullptr) {}
    };

    int brEl;
    Cvor* p;

    Cvor* novi(TipKljuca i = TipKljuca(), TipVrijednosti u = TipVrijednosti())
    {
        Cvor* temp = new Cvor();
        temp->kljuc = i;
        temp->vrijednost = u;
        temp->d_strana = temp->l_strana = temp->roditelj = nullptr;
        return temp;
    }

    Cvor* pretraga(Cvor* p, TipKljuca i) const
    {
        if (p == nullptr || p->kljuc == i) return p;
        if (p->kljuc < i) return pretraga(p->d_strana, i);
        else if (p->kljuc > i) return pretraga(p->l_strana, i);
        return p;
    }

    Cvor* dodaj(Cvor* p, TipKljuca i, TipVrijednosti u = TipVrijednosti(), Cvor* rod = nullptr)
    {
        if (p == nullptr)
        {
            p = novi(i, u);
            p->roditelj = rod;
            return p;
        }
        if (i > p->kljuc) p->d_strana = dodaj(p->d_strana, i, u, p);
        else p->l_strana = dodaj(p->l_strana, i, u, p);
        return p;
    }

    Cvor* nasljednik(Cvor* t)
    {
        auto ov = t;
        while (ov && ov->l_strana != nullptr) ov = ov->l_strana;
        return ov;
    }

    void konstruktor(Cvor* a, Cvor* b)
    {
        if (b == nullptr) return;
        konstruktor(a, b->l_strana);
        konstruktor(a, b->d_strana);
        dodaj(a, b->kljuc, b->vrijednost);
    }

    Cvor* obrisiC(Cvor* t, TipKljuca i)
    {
        if (t == nullptr) return t;
        if (i < t->kljuc) t->l_strana = obrisiC(t->l_strana, i);
        else if (i > t->kljuc) t->d_strana = obrisiC(t->d_strana, i);
        else
        {
            if (t->l_strana == nullptr)
            {
                Cvor* temp = t->d_strana;
                delete t;
                return temp;
            }
            else if (t->d_strana == nullptr)
            {
                Cvor* temp = t->l_strana;
                delete t;
                return temp;
            }
            auto q = nasljednik(t->d_strana);
            t->kljuc = q->kljuc;
            t->vrijednost = q->vrijednost;
            t->d_strana = obrisiC(t->d_strana, q->kljuc);
        }
        return t;
    }

    void obrisi_stablo(Cvor* t)
    {
        if (t == nullptr) return;
        obrisi_stablo(t->l_strana);
        obrisi_stablo(t->d_strana);
        delete t;
    }

public:
    BinStabloMapa<TipKljuca, TipVrijednosti>() : brEl(0), p(nullptr) {}

    ~BinStabloMapa<TipKljuca, TipVrijednosti>() { obrisi_stablo(p); }

    BinStabloMapa<TipKljuca, TipVrijednosti>(const BinStabloMapa<TipKljuca, TipVrijednosti>& a)
    {
        brEl = a.brEl;
        if (a.brEl == 0) p = nullptr;
        else
        {
            p = novi(a.p->kljuc, a.p->vrijednost);
            konstruktor(p, a.p);
        }
    }

    BinStabloMapa<TipKljuca, TipVrijednosti>& operator=(const BinStabloMapa<TipKljuca, TipVrijednosti>& a)
    {

        if (this == &a) return *this;
        obrisi();
        brEl = a.brEl;
        if (a.brEl == 0) p = nullptr;
        else
        {
            p->kljuc = a.p->kljuc; p->vrijednost = a.p->vrijednost;
            konstruktor(p, a.p);
        }
        return *this;
    }

    int brojElemenata() const { return brEl; }

    void obrisi()
    {
        obrisi_stablo(p);
        p = novi();
        brEl = 0;
    }

    void obrisi(const TipKljuca& kljuc)
    {
        if (p->kljuc == kljuc) p = obrisiC(p, kljuc);
        else obrisiC(p, kljuc);
        brEl--;
    }

    TipVrijednosti& operator[](TipKljuca i)
    {
        if (p == nullptr)
        {
            p = novi(i);
            brEl++;
            return p->vrijednost;
        }
        auto c = pretraga(p, i);
        if (c == nullptr)
        {
            dodaj(p, i);
            brEl++;
            c = pretraga(p, i);
        }
        return c->vrijednost;
    }

    const TipVrijednosti operator[](TipKljuca i) const
    {
        auto c = pretraga(p, i);
        if (c != nullptr) return c->vrijednost;
        return TipVrijednosti();
    }
};


template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
protected:
    int brEl;
    vector<list<pair<TipKljuca, TipVrijednosti>*>> p;
    unsigned int (*funkcija)(TipKljuca, unsigned int);

public:
    HashMapaLan<TipKljuca, TipVrijednosti>() : Mapa<TipKljuca, TipVrijednosti>(), brEl(0), funkcija(nullptr) { p.resize(10000); }

    ~HashMapaLan<TipKljuca, TipVrijednosti>() { obrisi(); }

    HashMapaLan<TipKljuca, TipVrijednosti>(const HashMapaLan<TipKljuca, TipVrijednosti>& a) 
    {
        brEl = a.brEl;
        funkcija = a.funkcija;
        for (int i = 0; i < a.p.size(); i++) 
        {
            p.push_back(list<pair<TipKljuca, TipVrijednosti> *>());
            auto it = a.p[i].begin();
            while (it != a.p[i].end()) 
            {
                p[i].push_back(new pair<TipKljuca, TipVrijednosti>((*it)->first, (*it)->second));
                it++;
            }
        }
    }

    HashMapaLan<TipKljuca, TipVrijednosti>& operator=(const HashMapaLan<TipKljuca, TipVrijednosti>& a) 
    {
        if (this == &a) return *this;
        obrisi();
        while (p.size() < a.p.size()) p.push_back(std::list<std::pair<TipKljuca, TipVrijednosti> *>());
        brEl = a.brEl;
        funkcija = a.funkcija;
        for (int i = 0; i < a.p.size(); i++) 
        {
            auto it = a.p[i].begin();
            while (it != a.p[i].end()) 
            {
                p[i].push_back(new std::pair<TipKljuca, TipVrijednosti>((*it)->first, (*it)->second));
                it++;
            }
        }
        return *this;
    }

    void obrisi() 
    {
        if (!funkcija) throw domain_error("Hash funkcija?");
        if (brEl == 0) throw range_error("Nema elemenata");
        for (int i = 0; i < p.size(); i++) 
        {
            auto it = p[i].begin();
            while (it != p[i].end()) 
            {
                delete* it;
                it++;
            }
            p[i].resize(0);
        }
        brEl = 0;
    }

    void obrisi(const TipKljuca& kljuc) 
    {
        if (!funkcija) throw domain_error("Hash funkcija?");
        int in = funkcija(kljuc, p.size());
        typename list<pair<TipKljuca, TipVrijednosti>*>::iterator it = p[in].begin();
        while (it != p[in].end()) 
        {
            if ((*it)->first == kljuc) 
            {
                delete* it;
                p[in].erase(it);
                brEl--;
                return;
            }
            it++;
        }
        throw logic_error("Nema kljuca");
    }

    int brojElemenata() const { return brEl; }

    int TraziHash(const TipKljuca& k) const 
    {
        int in = funkcija(k, p.size());
        typename std::list<std::pair<TipKljuca, TipVrijednosti>*> l = p[in];
        typename std::list<std::pair<TipKljuca, TipVrijednosti>*>::iterator it = l.begin();
        int i = 0;
        while (it != l.end()) 
        {
            if ((*it)->first == k) return i;
            i++;
            it++;
        }
        return -1;
    }

    TipVrijednosti& operator[](TipKljuca kljuc) 
    {
        if (!funkcija) throw domain_error("Hash funkcija?");
        int in = funkcija(kljuc, p.size());
        typename std::list<std::pair<TipKljuca, TipVrijednosti>*>::iterator it = p[in].begin();
        if (p[in].empty()) 
        {
            p[in].push_back(new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti()));
            brEl++;
            it = p[in].begin();
            return (*it)->second;
        }
        if ((*it)->first == kljuc) return (*it)->second;
        else 
        {
            int q = TraziHash(kljuc);
            if (q == -1) 
            {
                brEl++;
                p[in].push_back(new pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti()));
                p[in].sort([](const pair<TipKljuca, TipVrijednosti>* a,
                    const pair<TipKljuca, TipVrijednosti>* b) -> bool { return a->first < b->first; });
                q = TraziHash(kljuc);
                it = p[in].begin();
            }
            while (q > 0) 
            {
                it++;
                q--;
            }
        }
        return (*it)->second;
    }

    const TipVrijednosti operator[](TipKljuca kljuc) const 
    {

        if (!funkcija) throw domain_error("Hash funkcija?");
        int in = funkcija(kljuc, p.size());
        typename std::list<std::pair<TipKljuca, TipVrijednosti>*> l = p[in];
        typename std::list<std::pair<TipKljuca, TipVrijednosti>*>::iterator it = l.begin();
        if (!p[in].empty() && (*it)->first == kljuc) return (*it)->second;
        else if (p[in].empty()) return TipVrijednosti();
        else 
        {
            in = TraziHash(kljuc);
            if (in == -1) return TipVrijednosti();
            else
                while (in > 0) 
                {
                    it++;
                    in--;
                }
        }
        return (*it)->second;
    }

    void definisiHashFunkciju(unsigned int (*func)(TipKljuca, unsigned int)) 
    {
        funkcija = func;
        return;
    }
};


template <typename TipOznake> class GranaIterator;
template <typename TipOznake> class Grana;
template <typename TipOznake> class Cvor;
template <typename TipOznake> class ListaGraf;


template <typename TipOznake> 
class UsmjereniGraf {
public:
    UsmjereniGraf() {}
    UsmjereniGraf(int a) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int a) = 0;
    virtual void dodajGranu(int p, int d, float t) = 0;
    virtual void obrisiGranu(int p, int d) = 0;
    virtual bool  postojiGrana(int p, int d) = 0;
    virtual void postaviTezinuGrane(int p, int d, float t) = 0;
    virtual TipOznake dajOznakuGrane(int p, int d) = 0;
    virtual TipOznake dajOznakuCvora(int b) = 0;
    virtual float dajTezinuGrane(int p, int d) = 0;
    virtual void postaviOznakuCvora(int b, TipOznake a) = 0;
    virtual void postaviOznakuGrane(int p, int d, TipOznake a) = 0;
    Grana<TipOznake> dajGranu(int p, int d) { return Grana<TipOznake>(this, p, d); }
    Cvor<TipOznake> dajCvor(int b) { return Cvor<TipOznake>(this, b); }
    GranaIterator<TipOznake> dajGranePocetak() 
    {
        GranaIterator<TipOznake> it(this, 0, -1);
        return ++it;
    }
    GranaIterator<TipOznake> dajGraneKraj() 
    {
        GranaIterator<TipOznake> it(this, -1, -1);
        return it;
    }
    virtual GranaIterator<TipOznake> dajSljedecuGranu(int p, int d) = 0;
};


template <typename TipOznake> 
class Cvor {
    UsmjereniGraf<TipOznake>* u;
    TipOznake oznaka;
    int broj;
    vector<Grana<TipOznake>> grane;
    vector<Cvor<TipOznake>> susjedi;

public:
    Cvor(UsmjereniGraf<TipOznake>* a, int b) : u(a), broj(b) {}

    int dajRedniBroj() const { return broj; }

    void postaviOznaku(TipOznake a) { u->postaviOznakuCvora(broj, a); }

    TipOznake dajOznaku() const { return u->dajOznakuCvora(broj); }

    void dodajVezu(Grana<TipOznake> g, Cvor<TipOznake> s) 
    {
        grane.push_back(g);
        susjedi.push_back(s);
    }

    void obrisiVezu(Grana<TipOznake> g, Cvor<TipOznake> s) 
    {
        for (int i = 0; i < susjedi.size(); i++)
            if (s == susjedi[i]) 
            {
                for (int j = i; j < susjedi.size() - 1; j++) 
                {
                    susjedi[j] = susjedi[j + 1];
                    grane[j] = grane[j + 1];
                }
                break;
            }
        susjedi.pop_back();
        grane.pop_back();
    }

    vector<Grana<TipOznake>> dajGrane() const { return grane; }

    vector<Cvor<TipOznake>> dajSusjede() const { return susjedi; }
};


template <typename TipOznake> 
class Grana {
    UsmjereniGraf<TipOznake>* u;
    TipOznake oznaka;
    float tezina;
    int p, d;

public:
    Grana(UsmjereniGraf<TipOznake>* ug, int polazni, int dolazni) : u(ug), p(polazni), d(dolazni) {}

    float dajTezinu() const { return u->dajTezinuGrane(p, d); }

    void postaviTezinu(float a) { u->postaviTezinuGrane(p, d, a); }

    void postaviOznaku(TipOznake a) { u->postaviOznakuGrane(p, d, a); }

    TipOznake dajOznaku() const { return u->dajOznakuGrane(p, d); }

    Cvor<TipOznake> dajPolazniCvor() const { return u->dajCvor(p); };

    Cvor<TipOznake> dajDolazniCvor() const { return u->dajCvor(d); };
};


template <typename TipOznake> 
class GranaIterator {
    int p, d;
    UsmjereniGraf<TipOznake>* u;

public:
    GranaIterator(UsmjereniGraf<TipOznake>* us, int a, int b) :p(a), d(b), u(us) {};

    Grana<TipOznake> operator*() { return u->dajGranu(p, d); }

    bool operator==(const GranaIterator& it)const { return it.u == u && it.p == p && it.d == d; }

    bool operator!=(const GranaIterator& it)const { return it.u != u || it.p != p || it.d != d; }

    GranaIterator& operator++() 
    {
        GranaIterator<TipOznake> sljedeci = u->dajSljedecuGranu(p, d);
        p = sljedeci.p;
        d = sljedeci.d;
        return *this;
    }

    GranaIterator operator++(int) 
    {
        GranaIterator<TipOznake> ovaj = *this;
        ++(*this);
        return ovaj;
    }
};


template <typename TipOznake> 
class ListaGraf : public UsmjereniGraf<TipOznake> {
    struct grana {
        TipOznake oznaka;
        float tezina;
        bool postoji;
    };
    vector<vector<grana>> lista;
    vector<TipOznake> oznake;

public:
    ListaGraf(int b) : UsmjereniGraf<TipOznake>(b) { postaviBrojCvorova(b); }

    int dajBrojCvorova() const override { return lista.size(); }

    void postaviBrojCvorova(int a) override 
    {
        grana g;
        g.postoji = false;
        for (int i = 0; i < lista.size(); i++) lista[i].resize(a, g);
        std::vector<grana> ubaci(a, g);
        lista.resize(a, ubaci);
        oznake.resize(a);
    }

    bool postojiGrana(int p, int d) override { return lista[p][d].postoji; }

    void dodajGranu(int p, int d, float t) override { grana g; g.tezina = t; g.postoji = true; lista[p][d] = g; }

    void obrisiGranu(int p, int d) override { lista[p][d].postoji = false; }

    void postaviTezinuGrane(int p, int d, float t) override { lista[p][d].tezina = t; }

    TipOznake dajOznakuCvora(int b) override { return oznake[b]; }

    TipOznake dajOznakuGrane(int p, int d) override { return lista[p][d].oznaka; }

    float dajTezinuGrane(int p, int d) override { return lista[p][d].tezina; }

    void postaviOznakuCvora(int b, TipOznake a) override { oznake[b] = a; }

    void postaviOznakuGrane(int p, int d, TipOznake a)override { lista[p][d].oznaka = a; }

    GranaIterator<TipOznake> dajGranePocetak() { GranaIterator<TipOznake> it(this, 0, -1); ++it; return it; }

    GranaIterator<TipOznake> dajGraneKraj() { GranaIterator<TipOznake> it(this, -1, -1); return it; }

    GranaIterator<TipOznake> dajSljedecuGranu(int p, int d)override 
    {
        for (int i = p; i < lista.size(); i++) 
            for (int j = 0; j < lista.size(); j++) 
            {
                if (i == p && j <= d) continue;
                if (lista[i][j].postoji) return GranaIterator<TipOznake>(this, i, j);
            }
        return GranaIterator<TipOznake>(this, -1, -1);
    }
};


template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake>* g, vector<Cvor<TipOznake>>& b, Cvor<TipOznake> c) 
{
    bool dodati = false;
    for (int i = 0; i < b.size(); i++) 
        if (b[i].dajRedniBroj() == c.dajRedniBroj()) 
        {
            dodati = true;
            break;
        }
    if (!dodati) b.push_back(c);
    int br = g->dajBrojCvorova();
    for (int i = 0; i < br; i++) if (!dodati) bfs(g, b, g->dajCvor(i));
}


template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake>* g, std::vector<Cvor<TipOznake>>& b, Cvor<TipOznake> c)
{
    bool dodati = false;
    for (int i = 0; i < b.size(); i++) 
        if (c.dajRedniBroj() == b[i].dajRedniBroj()) 
        {
            dodati = true;
            break;
        }
    if (!dodati) b.push_back(c);
    int brojCvorova = g->dajBrojCvorova();
    for (int i = 0; i < brojCvorova; i++) if (g->postojiGrana(c.dajRedniBroj(), i) && !dodati) dfs(g, b, g->dajCvor(i));
}


int main() 
{
    return 0;
}