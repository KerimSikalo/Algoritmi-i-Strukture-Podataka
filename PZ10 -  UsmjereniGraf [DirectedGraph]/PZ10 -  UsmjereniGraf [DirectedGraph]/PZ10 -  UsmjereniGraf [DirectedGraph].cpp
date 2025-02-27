#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;


template <typename TipOznake> class Grana;
template <typename TipOznake> class Cvor;
template <typename TipOznake> class GranaIterator;


template <typename TipOznake>
class UsmjereniGraf {

public:
    UsmjereniGraf(int p) {}
    virtual ~UsmjereniGraf() {}
    virtual void postaviBrojCvorova(int) = 0;
    virtual void postaviTezinuGrane(int, int, double) = 0;
    virtual void postaviOznakuCvora(int, TipOznake) = 0;
    virtual void postaviOznakuGrane(int, int, TipOznake) = 0;
    virtual void dodajGranu(int, int, double) = 0;
    virtual void obrisiGranu(int, int) = 0;
    virtual int dajBrojCvorova() const = 0;
    virtual double dajTezinuGrane(int, int) = 0;
    virtual TipOznake dajOznakuCvora(int) = 0;
    virtual TipOznake dajOznakuGrane(int, int) = 0;
    virtual Grana<TipOznake>& dajGranu(int, int) = 0;
    virtual Cvor<TipOznake>& dajCvor(int) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
    virtual bool postojiGrana(int, int) = 0;
};


template <typename TipOznake>
class Grana {
    int pocetak, kraj;
    double tezina;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake>* usmjereniGraf;

public:
    Grana(int p, int k, UsmjereniGraf<TipOznake>* g, double t) : pocetak(p), kraj(k), usmjereniGraf(g), tezina(t) {}

    TipOznake dajOznaku() const { return oznaka; }

    double dajTezinu() const { return tezina; }

    Cvor<TipOznake> dajPolazniCvor() { return usmjereniGraf->dajCvor(pocetak); }

    Cvor<TipOznake> dajDolazniCvor() { return usmjereniGraf->dajCvor(kraj); }

    void postaviOznaku(TipOznake o) 
    {
        oznaka = o;
        usmjereniGraf->dajGranu(pocetak, kraj).oznaka = o;
    }

    void postaviTezinu(double t) 
    {
        tezina = t;
        usmjereniGraf->dajGranu(pocetak, kraj).tezina = t;
    }
};


template <typename TipOznake>
class Cvor {
    int redniBroj;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake>* usmjereniGraf;

public:
    Cvor(UsmjereniGraf<TipOznake>* ug, int rb) : usmjereniGraf(ug), redniBroj(rb) {}

    int dajRedniBroj() const { return redniBroj; }

    TipOznake dajOznaku() const { return oznaka; }

    void postaviOznaku(TipOznake o) { oznaka = o; }
};


template <typename TipOznake>
class GranaIterator {
    int dolazniCvor, polazniCvor, pozicija;
    vector<Grana<TipOznake>> grane;
    vector<vector<bool>> vrijednosti;

    void trazi(int& brojac, int& red, int& kolona, bool uslov) 
    {
        for (int i = 0; i < vrijednosti.size(); i++) 
        {
            for (int j = 0; j < vrijednosti[i].size(); j++) 
            {
                if (brojac <= pozicija && vrijednosti[i][j]) 
                {
                    brojac++;
                    red = i;
                    kolona = j;
                }
                else if (brojac > pozicija) 
                {
                    uslov = true;
                    break;
                }
            }
            if (uslov) break;
        }
    }

public:
    GranaIterator(vector<vector<bool>> v, vector<Grana<TipOznake>> g, int p, int k, int poz) 
    {
        dolazniCvor = k;
        polazniCvor = p;
        pozicija = poz;
        grane = g;
        vrijednosti = v;
    }

    Grana<TipOznake> operator *() 
    {
        int brojac = 0, red = 0, kolona = 0;
        trazi(brojac, red, kolona, false);
        for (int i = 0; i < grane.size(); i++) 
            if (grane[i].dajPolazniCvor().dajRedniBroj() == red && grane[i].dajDolazniCvor().dajRedniBroj() == kolona)
                return grane[i];
        throw logic_error("Trazena grana ne postoji");
    }

    bool operator ==(const GranaIterator& it) const 
    {
        int brojac = 0, red = 0, kolona = 0;
        trazi(brojac, red, kolona, false);
        if (vrijednosti[red][kolona] == it.vrijednosti[red][kolona]) return true;
        return false;
    }

    bool operator !=(GranaIterator it) 
    {
        int brojac = 0, red = 0, kolona = 0;
        trazi(brojac, red, kolona, false);
        if (vrijednosti[red][kolona] != it.vrijednosti[red][kolona]) return false;
        if (pozicija == grane.size()) return false;
        return true;
    }

    bool operator !=(const GranaIterator& it) const 
    {
        int brojac = 0, red = 0, kolona = 0;
        trazi(brojac, red, kolona, false);
        if (vrijednosti[red][kolona] != it.vrijednosti[red][kolona]) return false;
        if (pozicija == grane.size()) return false;
        return true;
    }

    GranaIterator& operator ++() 
    {
        pozicija++;
        return *this;
    }

    GranaIterator operator ++(int) 
    {
        GranaIterator temp = *this;
        ++*this;
        return temp;
    }
};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake> {
    vector<Grana<TipOznake>> grane;
    vector<Cvor<TipOznake>> cvorovi;
    vector<vector<bool>> vrijednosti;

public:
    MatricaGraf(int bc) : UsmjereniGraf<TipOznake>(bc) 
    {
        if (bc < 0) throw logic_error("Broj cvorova ne smije biti negativan!");
        vrijednosti.resize(bc);
        for (int i = 0; i < bc; i++) 
        {
            vrijednosti[i].resize(bc);
            cvorovi.push_back(Cvor<TipOznake>(this, i));
            for (int j = 0; j < bc; j++) vrijednosti[i][j] = false;
        }
    }

    ~MatricaGraf() {}

    MatricaGraf(const MatricaGraf<TipOznake>& mg) 
    {
        grane = mg.grane;
        cvorovi = mg.cvorovi;
        vrijednosti = mg.vrijednosti;
    }

    MatricaGraf<TipOznake>& operator =(const MatricaGraf<TipOznake>& mg) 
    {
        if (&mg != this) 
        {
            grane = mg.grane;
            cvorovi = mg.cvorovi;
            vrijednosti = mg.vrijednosti;
        }
        return *this;
    }

    void postaviBrojCvorova(int bc) override 
    {
        if (bc < cvorovi.size()) throw logic_error("Ne smije broj cvorova biti manji od prethodnog");
        if (vrijednosti.size() == bc) return;
        for (int i = vrijednosti.size(); i < bc; i++) cvorovi.push_back(Cvor<TipOznake>(this, i));
        int staraDimenzijaMatrice = vrijednosti.size();
        vrijednosti.resize(bc);
        int ubaci = 0;
        for (int i = 0; i < bc; i++) 
        {
            vrijednosti[i].resize(bc);
            if (i >= staraDimenzijaMatrice) ubaci = 0;
            else ubaci = staraDimenzijaMatrice;
            for (int j = ubaci; j < bc; j++) vrijednosti[i][j] = false;
        }
    }

    void postaviTezinuGrane(int p, int k, double t) override 
    {
        for (int i = 0; i < grane.size(); i++) 
            if (p == grane[i].dajPolazniCvor().dajRedniBroj() && k == grane[i].dajDolazniCvor().dajRedniBroj()) 
            { grane[i].postaviTezinu(t); return; }
        throw logic_error("Ne mozete postaviti tezinu jer data grana ne postoji!");
    }

    void postaviOznakuCvora(int rb, TipOznake o) override { cvorovi[rb].postaviOznaku(o); }

    void postaviOznakuGrane(int p, int k, TipOznake o) override 
    {
        for (int i = 0; i < grane.size(); i++) 
            if (p == grane[i].dajPolazniCvor().dajRedniBroj() && k == grane[i].dajDolazniCvor().dajRedniBroj()) 
            { grane[i].postaviOznaku(o); return; }
        throw domain_error("Ne mozete postaviti oznaku jer data grana ne postoji!");
    }

    void dodajGranu(int p, int k, double t = 0) override 
    {
        if (!postojiGrana(p, k)) 
        {
            vrijednosti[p][k] = true;
            grane.push_back(Grana<TipOznake>(p, k, this, t));
        }
        else throw logic_error("Grana vec postoji!");
    }

    void obrisiGranu(int p, int k) override 
    {
        for (int i = 0; i < grane.size(); i++) 
            if (grane[i].dajPolazniCvor().dajRedniBroj() == p && grane[i].dajDolazniCvor().dajRedniBroj() == k) 
            {
                vrijednosti[p][k] = false;
                grane.erase(grane.begin() + i);
                return;
            }
        throw logic_error("Granu koju zelite obrisati ne postoji!");
    }

    int dajBrojCvorova() const override { return cvorovi.size(); }

    double dajTezinuGrane(int p, int k)  override 
    {
        for (int i = 0; i < grane.size(); i++) 
            if (p == grane[i].dajPolazniCvor().dajRedniBroj() && k == grane[i].dajDolazniCvor().dajRedniBroj())
            { return grane[i].dajTezinu(); }
        throw logic_error("Trazena grana ne postoji!");
    }

    TipOznake dajOznakuCvora(int rb)  override 
    {
        return cvorovi[rb].dajOznaku();
        throw logic_error("Trazeni cvor ne postoji!");
    }

    TipOznake dajOznakuGrane(int p, int k)  override 
    {
        for (int i = 0; i < grane.size(); i++) 
            if (p == grane[i].dajPolazniCvor().dajRedniBroj() && k == grane[i].dajDolazniCvor().dajRedniBroj()) 
            { return grane[i].dajOznaku(); }
        throw logic_error("Trazena grana ne postoji!");
    }

    Grana<TipOznake>& dajGranu(int p, int k) override 
    {
        for (int i = 0; i < grane.size(); i++) 
            if (grane[i].dajPolazniCvor().dajRedniBroj() == p && grane[i].dajDolazniCvor().dajRedniBroj() == k)
                return grane[i];
        throw logic_error("Trazena grana ne postoji!");
    }

    Cvor<TipOznake>& dajCvor(int i) override { return cvorovi[i]; }

    GranaIterator<TipOznake> dajGranePocetak() override 
    {
        return GranaIterator<TipOznake>(vrijednosti, grane, grane[0].dajPolazniCvor().dajRedniBroj(), grane[0].dajDolazniCvor().dajRedniBroj(), 0);
    }

    GranaIterator<TipOznake> dajGraneKraj() override 
    {
        return GranaIterator<TipOznake>(vrijednosti, grane, grane[grane.size() - 1].dajPolazniCvor().dajRedniBroj(), grane[grane.size() - 1].dajDolazniCvor().dajRedniBroj(), 0);
    }

    bool postojiGrana(int p, int k) override { return vrijednosti[p][k]; }
};


int main() 
{
    return 0;
}