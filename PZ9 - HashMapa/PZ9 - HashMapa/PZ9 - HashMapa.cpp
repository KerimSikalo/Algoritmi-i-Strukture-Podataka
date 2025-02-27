#include <iomanip>
#include <iostream>
#include <list>
#include <string>
#include <utility>
#include <vector>
#include <chrono>
#include <functional>
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
    std::pair<TipKljuca, TipVrijednosti>** p;

public:
    NizMapa<TipKljuca, TipVrijednosti>()
        : p(new std::pair<TipKljuca, TipVrijednosti>* [100000]()), velicina(100000), brEl(0) {
    }

    NizMapa<TipKljuca, TipVrijednosti>(const NizMapa<TipKljuca, TipVrijednosti>& a)
    {
        brEl = a.brEl;
        velicina = a.velicina;
        if (a.velicina == 0) p = nullptr;
        else
        {
            p = new std::pair<TipKljuca, TipVrijednosti>* [velicina]();
            for (int i = 0;i < a.brEl;i++)
                p[i] = new std::pair<TipKljuca, TipVrijednosti>(*a.p[i]);
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
        for (int i = 0; i < a.brEl; i++)
            p[i] = new std::pair<TipKljuca, TipVrijednosti>(*a.p[i]);
        return *this;
    }

    ~NizMapa<TipKljuca, TipVrijednosti>() { for (int i = 0;i < brEl;i++) delete p[i]; delete[] p; }

    int brojElemenata() const { return brEl; }

    int Velicina() const { return velicina; }

    void obrisi()
    {
        if (brEl == 0) throw std::range_error("Nema elemenata");
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
        throw std::logic_error("Nema kljuca");
    }

    TipVrijednosti& operator[](TipKljuca kljuc)
    {
        if (brEl == velicina)
        {
            std::pair<TipKljuca, TipVrijednosti>** temp = new std::pair<TipKljuca, TipVrijednosti>* [10000 + velicina]();
            for (int i = 0; i < brEl; i++)
            {
                temp[i] = new std::pair<TipKljuca, TipVrijednosti>(*p[i]);
                delete p[i];
            }
            delete[] p;
            velicina += 10000;
            p = temp;
            temp = nullptr;
        }
        for (int i = 0; i < brEl; i++)
            if (p[i]->first == kljuc) return p[i]->second;
        p[brEl] = new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti()); brEl++;
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
        while (p != nullptr && p->kljuc != i) {
            if (p->kljuc < i) p = p->d_strana;
            else p = p->l_strana;
        }
        return p;
    }

    Cvor* dodaj(Cvor* p, TipKljuca i, TipVrijednosti u = TipVrijednosti(), Cvor* rod = nullptr)
    {
        Cvor* noviCvor = novi(i, u);
        noviCvor->roditelj = rod;
        if (p == nullptr) {
            return noviCvor;
        }
        Cvor* trenutni = p;
        Cvor* roditelj = nullptr;
        while (trenutni != nullptr) {
            roditelj = trenutni;
            if (i < trenutni->kljuc) trenutni = trenutni->l_strana;
            else trenutni = trenutni->d_strana;
        }
        noviCvor->roditelj = roditelj;
        if (i < roditelj->kljuc) roditelj->l_strana = noviCvor;
        else roditelj->d_strana = noviCvor;
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
            p = dodaj(p, i);
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
class HashMapa : public Mapa<TipKljuca, TipVrijednosti> {
protected:
    int velicina;
    int brEl;
    pair<TipKljuca, TipVrijednosti>** p;
    bool* prazno;
    unsigned int (*funkcija)(TipKljuca, unsigned int);

public:
    HashMapa<TipKljuca, TipVrijednosti>()
        : Mapa<TipKljuca, TipVrijednosti>(), p(new std::pair<TipKljuca, TipVrijednosti>* [100000]()), velicina(100000), brEl(0), prazno(new bool[100000]), funkcija(nullptr) 
        { for (int i = 0; i < 100000; i++) prazno[i] = false; }

    ~HashMapa<TipKljuca, TipVrijednosti>() 
    {
        if (p) 
        {
            for (int i = 0; i < velicina; i++) delete p[i];
            delete[] p;
        }
        if (prazno) delete[] prazno;
    }

    HashMapa<TipKljuca, TipVrijednosti>(const HashMapa<TipKljuca, TipVrijednosti>& a) 
    {
        brEl = a.brEl;
        velicina = a.velicina;
        funkcija = a.funkcija;
        prazno = new bool[velicina];
        if (a.velicina == 0) 
        {
            p = nullptr;
            for (int i = 0; i < velicina; i++) prazno[i] = false;
        }
        else 
        {
            p = new pair<TipKljuca, TipVrijednosti>* [velicina]();
            for (int i = 0; i < a.velicina; i++) 
            {
                if (a.p[i]) p[i] = new std::pair<TipKljuca, TipVrijednosti>(*a.p[i]);
                prazno[i] = a.prazno[i];
            }
        }
    }

    HashMapa<TipKljuca, TipVrijednosti>& operator=(const HashMapa<TipKljuca, TipVrijednosti>& a) 
    {
        if (this == &a) return *this;
        if (a.Velicina() > Velicina()) prosiri(a.velicina);
        for (int i = 0; i < Velicina(); i++) prazno[i] = false;
        brEl = a.brEl;
        funkcija = a.funkcija;
        for (int i = 0; i < a.velicina; i++) 
        {
            if (a.p[i]) 
            {
                delete p[i];
                p[i] = new std::pair<TipKljuca, TipVrijednosti>(*a.p[i]);
            }
            prazno[i] = a.prazno[i];
        }
        return *this;
    }

    void prosiri(int a = 10000) 
    {
        pair<TipKljuca, TipVrijednosti>** temp = new std::pair<TipKljuca, TipVrijednosti>* [a + velicina]();
        bool* q = new bool[a + velicina]();
        for (int i = 0; i < velicina; i++) 
        {
            if (p[i]) temp[i] = new std::pair<TipKljuca, TipVrijednosti>(*p[i]);
            delete p[i];
            q[i] = prazno[i];
        }
        for (int i = 0; i < a; i++) q[velicina + i] = false;
        delete[] p;
        delete[] prazno;
        velicina += a;
        p = temp;
        prazno = q;
    }

    void obrisi() 
    {
        if (brEl == 0) throw range_error("Nema elemenata");
        for (int i = 0; i < velicina; i++) prazno[i] = false;
        brEl = 0;
    }

    void obrisi(const TipKljuca& kljuc) 
    {

        for (int i = 0; i < velicina; i++)
            if (p[i] && p[i]->first == kljuc) 
            {
                prazno[i] = false;
                brEl--;
                return;
            }
        throw logic_error("Nema kljuca");
    }

	int brojElemenata() const { return brEl; }

    int Velicina() const { return velicina; }

    bool umetniHash(const TipKljuca& k) 
    {
        int j, i = 1;
        do 
        {
            do 
            {
                j = funkcija(k, i);
                if (prazno[j] == false) 
                {
                    delete p[j];
                    p[j] = new pair<TipKljuca, TipVrijednosti>(k, TipVrijednosti());
                    prazno[j] = true;
                    return true;
                }
                else i = i + 1;
            } while (i < velicina);
            prosiri();
        } while (velicina < 100000000);
        return false;
    }

    int TraziHash(const TipKljuca& k) const 
    {
        int indeks, i(1);
        do 
        {
            indeks = funkcija(k, i);
            if (prazno[indeks] && p[indeks]->first == k) return indeks;
            else i = i + 1;
        } while ((prazno[indeks] == true && i != velicina) || prazno[funkcija(k, i)] == true);
        return -1;
    }

    TipVrijednosti& operator[](TipKljuca kljuc) 
    {
        if (!funkcija) throw bad_exception();
        if (brEl == velicina) prosiri();
        int in = funkcija(kljuc, velicina);
        if (!prazno[in]) 
        {
            delete p[in];
            p[in] = new pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti());
            brEl++;
            prazno[in] = true;
            return p[in]->second;
        }
        if (prazno[in] && p[in]->first == kljuc) return p[in]->second;
        else 
        {
            in = TraziHash(kljuc);
            if (in == -1) 
            {
                umetniHash(kljuc);
                brEl++;
                in = TraziHash(kljuc);
                prazno[in] = true;
            }
        }
        return p[in]->second;
    }

    const TipVrijednosti operator[](TipKljuca kljuc) const 
    {
        if (!funkcija) throw bad_exception();
        int in = funkcija(kljuc, velicina);
        if (prazno[in] && p[in]->first == kljuc) return p[in]->second;
        else 
        {
            in = TraziHash(kljuc);
            if (in == -1) return TipVrijednosti();
        }
        return p[in]->second;
    }

    void definisiHashFunkciju(unsigned int (*func)(TipKljuca, unsigned int)) 
    {
        funkcija = func;
        return;
    }
};

unsigned int mojHash(int ulaz, unsigned int max) { return ulaz % max; }

bool umetniHash(std::vector<int>& a, const int& k) 
{
    int i = 1;
    int j = mojHash(k, a.size());
    if (a[j] == 0 || a[j] == -1) 
    {
        a[j] = k;
        int l = 0;
        for (int i = 0; i < a.size();i++) if (a[i] != 0 && a[i] != -1)l++;
        if (l == a.size()) a.resize(a.size() + 3, 0);
        return true;
    }
    j = mojHash(k, i);
    do 
    {
        if (a[j] == 0 || a[j] == -1) 
        {
            a[j] = k;
            int l = 0;
            for (int i = 0; i < a.size();i++) if (a[i] != 0 && a[i] != -1) l++;
            if (l == a.size()) a.resize(a.size() + 3, 0);
            return true;
        }
        else j = j + 1;
        if (j == a.size()) j = 0;
    } while (j < a.size());
    return false;
}


int main() 
{
    try 
    {
        HashMapa<int, std::string> hashMapa;
        hashMapa.definisiHashFunkciju(mojHash);
        hashMapa[1] = "Jedan";
        hashMapa[2] = "Dva";
        hashMapa[3] = "Tri";
        std::cout << hashMapa[1] << std::endl;
        std::cout << hashMapa[2] << std::endl;
        std::cout << hashMapa[3] << std::endl;
        for (int i = 4; i <= 100; i++) hashMapa[i] = to_string(i);
        std::cout << "Broj elemenata: " << hashMapa.brojElemenata() << std::endl;
        std::cout << "Vrijednost za kljuc 50: " << hashMapa[50] << std::endl;
        hashMapa.obrisi(50);
        std::cout << "Kljuc 50 obrisan" << std::endl;
        try { std::cout << "Vrijednost za kljuc 50 nakon brisanja: " << hashMapa[50] << std::endl; }
        catch (const std::logic_error& e) { std::cout << e.what() << std::endl; }
    }
    catch (const std::exception& e) { std::cout << e.what() << std::endl; }
    return 0;
}