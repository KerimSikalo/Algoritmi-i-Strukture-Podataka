/*Zadatak 1.
Napraviti klasu BinStabloMapa izvedenu iz klase Mapa koju ste definisali u prethodnim vježbama.
Klasa BinStabloMapa implementira mapu pomoću binarnog stabla. Svaki čvor stabla treba da
sadrži dva podatka (ključ i vrijednost), a stablo treba da bude organizirano po ključu radi brže
pretrage. Pored pokazivača na lijevo i desno podstablo, čvor treba imati i pokazivač na roditelja.

Zadatak 2.
U Zadatak 1 uključite i klasu NizMapa koju ste razvili na prošlom tutorijalu.
Napravite jedan testni glavni program koji upoređuje performanse NizMape i BinStabloMape.
Program treba u oba tipa mape “natrpati” veliku količinu slučajno generisanih podataka a zatim
izmjeriti prosječno vrijeme:
● dodavanja novog elementa u mapu,
● pristupa postojećeg elementa u mapi.
Izmjerene rezultate prokomentarišite koristeći komentare u kodu.*/


#include <iostream>
#include <list>
#include <utility>
#include <string>
#include <chrono>
#include <random>

using namespace std;

//APSTRAKTNA KLASA//
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

//NIZMAPA//
template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa < TipKljuca, TipVrijednosti> {
    int velicina, brEl;
    std::pair<TipKljuca, TipVrijednosti>** p;

public:
    NizMapa<TipKljuca, TipVrijednosti>()
        : p(new std::pair<TipKljuca, TipVrijednosti>* [100000]()), velicina(100000), brEl(0) {  }

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

//BINSTABLOMAPA//
template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca, TipVrijednosti> {

    struct Cvor 
    {
        TipKljuca kljuc;
        TipVrijednosti vrijednost;
        Cvor* lijevo, * desno, * roditelj;
        Cvor(TipKljuca i = TipKljuca(), TipVrijednosti u = TipVrijednosti())
            : kljuc(i), vrijednost(u), desno(nullptr), lijevo(nullptr), roditelj(nullptr) {}
    };

    Cvor* novi(TipKljuca i = TipKljuca(), TipVrijednosti u = TipVrijednosti()) 
    {
        Cvor* temp = new Cvor();
        temp->kljuc = i;
        temp->vrijednost = u;
        temp->desno = temp->lijevo = temp->roditelj = nullptr;
        return temp;
    }

    Cvor* pretraga(Cvor* p, TipKljuca i) const 
    {
        if (p == nullptr || p->kljuc == i) return p;
        if (p->kljuc < i) return pretraga(p->desno, i);
        else if (p->kljuc > i) return pretraga(p->lijevo, i);
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
        if (i > p->kljuc) p->desno = dodaj(p->desno, i, u, p);
        else p->lijevo = dodaj(p->lijevo, i, u, p);
        return p;
    }

    Cvor* nasljednik(Cvor* t) 
    {
        auto ovaj = t;
        while (ovaj && ovaj->lijevo != nullptr) ovaj = ovaj->lijevo;
        return ovaj;
    }

    void konstruktor(Cvor* a, Cvor* b) 
    {
        if (b == nullptr) return;
        konstruktor(a, b->lijevo);
        konstruktor(a, b->desno);
        dodaj(a, b->kljuc, b->vrijednost);
    }

    Cvor* obrisiC(Cvor* t, TipKljuca i) 
    {
        if (t == nullptr) return t;
        if (i < t->kljuc) t->lijevo = obrisiC(t->lijevo, i);
        else if (i > t->kljuc) t->desno = obrisiC(t->desno, i);
        else 
        {
            if (t->lijevo == nullptr) 
            {
                Cvor* temp = t->desno;
                delete t;
                return temp;
            }
            else if (t->desno == nullptr) 
            {
                Cvor* temp = t->lijevo;
                delete t;
                return temp;
            }
            auto q = nasljednik(t->desno);
            t->kljuc = q->kljuc;
            t->vrijednost = q->vrijednost;
            t->desno = obrisiC(t->desno, q->kljuc);
        }
        return t;
    }

    void obrisi_stablo(Cvor* t) 
    {
        if (t == nullptr) return;
        obrisi_stablo(t->lijevo);
        obrisi_stablo(t->desno);
        delete t;
    }

    int brEl;
    Cvor* p;

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

    const TipVrijednosti operator[](TipKljuca i) const {
        auto c = pretraga(p, i);
        if (c != nullptr)
            return c->vrijednost;
        return TipVrijednosti();
    }
};


/*U sljedećem kodu, dolazimo do zaključka da je binarno stablo približno 33 puta brže u odnosu na NizStablo
za ubačenih 20000 elemenata. Također, zaključujemo da ukoliko bi umetali elemente
tako da svaki naredni ključ bude veći od prethodnog, NizStablo se pokaže oko 9.5 puta brže pri umetanju i oko 6 puta brže pri pristupu nekom elementu, 
jer bi to ličilo na obični niz čvorova gdje bi se svaki element ubacivao kao desno dijete.*/

int main() 
{
    BinStabloMapa<int, double> t;
    NizMapa<int, double> m;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 20000; i++) m[std::rand()] = 1. / std::rand();
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "NizMapa ubacivanje: " << time.count() << "us " << std::endl;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 20000; i++) t[std::rand()] = 1. / std::rand();
    end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "BinStablo ubacivanje: " << time.count() << "us " << std::endl;
    start = std::chrono::high_resolution_clock::now();
    m[std::rand()] = std::rand();
    end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "NizMapa pristup: " << time.count() << "us " << std::endl;
    start = std::chrono::high_resolution_clock::now();
    t[std::rand()] = std::rand();
    end = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "BinStablo pristup: " << time.count() << "us " << std::endl;
    return 0;
}
