/*
Zadatak 1.
Implementirati apstraktnu generičku klasu Lista koja omogućuje pohranjivanje proizvoljnog broja
elemenata proizvoljnog (ali istog) tipa. Veličina liste nije ograničena! Metode koje treba ponuditi
su:
 konstruktor bez parametara, koji kreira praznu listu;
 metodu brojElemenata() koja vraća broj elemenata stavljenih u listu (na početku nula);
 metodu trenutni() koja vraća trenutni element u listi; svaka klasa izvedena iz Liste imaće
neki atribut (ili više atributa) pomoću kojih pamti koji je element trenutni, ali korisnik taj
atribut ne vidi direktno (treba biti privatan); metoda trenutni() treba se moći koristiti i za
čitanje i za izmjenu trenutnog elementa (npr. l.trenutni()=13);
 metode prethodni() i sljedeci() koje pomjeraju trenutni element odnosno mijenjaju
vrijednost koju će vratiti trenutni(); ako je trenutni element prvi (posljednji), metoda
prethodni (sljedeci) ne treba uraditi ništa, ali treba vratiti logičku vrijednost false, u
suprotnom ako je pomjeranje trenutnog bilo uspješno treba vratiti true;
 metode pocetak() i kraj() koje postavljaju trenutni element na početak odnosno kraj liste;
 metoda obrisi() koja briše iz liste trenutni element; nakon brisanja sljedeći element u listi
postaje trenutni; ako je obrisan posljednji element u listi, element prije njega treba postati
trenutni;
 ako je lista prazna, metode trenutni, prethodni, sljedeci, pocetak, kraj i obrisi trebaju baciti
izuzetak;
 metode dodajIspred(const Tip& el) i dodajIza(const Tip& el) koje dodaju element ispred
odnosno iza trenutnog elementa; ako je lista prazna, obje metode dodaju element na prvo
mjesto u listi i taj element postaje trenutni element; u suprotnom, nakon dodavanja elementa
ne treba se promijeniti trenutni element, odnosno trenutni element ostaje isti kao i ranije;
Primjer:
 5 3 2 7 (trenutni je 2)
 dodajIspred(6)
 5 3 6 2 7 (trenutni je 2)
● preklopljeni operator [] koji omogućuje direktan pristup i-tom članu niza, uključujući
njegovo čitanje i izmjenu; u slučaju da je i negativan broj ili da je veći ili jednak broju
elemenata niza, operator treba baciti izuzetak.
Prilikom implementacije obratite pažnju na sljedeće stvari:
 klasa Lista je apstraktna klasa, nijednu metodu ne treba implementirati niti treba imati
atribute;
 u definicijama metoda namjerno nije definisan tip podataka koje metode vraćaju - odredite
ga sami; na pitanja o tome nećete dobiti odgovor jer imate dovoljno podataka u tekstu;
 možete u definiciju klase dodati sve ono što je potrebno da pri radu sa izvedenim klasama
nikada ne dolazi do curenja memorije (npr. prilikom dodjele, slanja i vraćanja iz funkcija
itd.);
 omogućite da se izvedene klase mogu slati u funkciju preko konstantne reference (npr. float
dajProsjek(const Lista<float>& l) ) i da se nad tom referencom mogu pozivati sve metode
koje ima smisla pozivati.

Zadatak 2.
Implementirati klasu NizLista izvedenu iz klase Lista. U ovoj klasi sve nabrojane metode trebaju
biti implementirane pomoću običnog C++ niza. Nije dozvoljeno korištenje bibliotečnih klasa
(vector, deque, list…).
 Za svaku metodu klase je potrebno napisati funkciju koja će dokazati ispravnost te metode.
 Sve funkcije sa dokazom ispravnosti se pozivaju u main metodi. Funkcijama za dokaz
ispravnosti se prosljeđuju proizvoljne vrijednosti tj. vrijednosti se ne unose preko standarnog
ulaza/izaza.
 Napomena: Funkcije sa dokazom ispravnosti ne smiju biti copy/paste testova sa c9 sa
izmjenjenim vrijednostima.

Zadatak 3.
Implementirati klasu JednostrukaLista izvedenu iz klase Lista. Ovdje sve metode trebaju biti
implementirane pomoću jednostruko povezane dinamički alocirane liste. Koristiti dinamičku
alokaciju strukture Cvor. Također nije dozvoljeno korištenje bibliotečnih klasa.
 Za svaku metodu klase je potrebno napisati funkciju koja će dokazati ispravnost te metode.
 Sve funkcije sa dokazom ispravnosti se pozivaju u main metodi. Funkcijama za dokaz
ispravnosti se prosljeđuju proizvoljne vrijednosti tj. vrijednosti se ne unose preko standarnog
ulaza/izaza.
 Napomena: Funkcije sa dokazom ispravnosti ne smiju biti copy/paste testova sa c9 sa
izmjenjenim vrijednostima.
*/


#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>

template <typename Tip> class Lista {
public:
    virtual ~Lista() {};
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip& operator[](int i) = 0;
    virtual const Tip& operator[](int i) const = 0;
};


// NizLista //
template <typename Tip>
class NizLista : public Lista<Tip> {
    int broj_elemenata, kapacitet, index;
    Tip** elementi;

    void DaLiJePrazan() const { if (brojElemenata() == 0) throw std::logic_error("Lista je prazna!"); }

    void Realociraj()
    {
        kapacitet *= 2;
        Tip** noviEl = new Tip * [kapacitet];
        std::copy(elementi, elementi + broj_elemenata, noviEl);
        delete[] elementi;
        elementi = noviEl;
    }

    void Dealociraj()
    {
        for (int i = 0; i < broj_elemenata; i++) delete elementi[i];
        delete[] elementi;
    }

public:
    NizLista() : broj_elemenata(0), kapacitet(10), index(0), elementi(new Tip* [10]) {}

    ~NizLista() { Dealociraj(); }

    NizLista(const NizLista& l) : broj_elemenata(l.broj_elemenata), kapacitet(l.kapacitet), index(l.index)
    {
        elementi = new Tip * [kapacitet];
        for (int i = 0; i < broj_elemenata; i++) elementi[i] = new Tip(*l.elementi[i]);
    }

    NizLista& operator=(const NizLista& l)
    {
        if (this == &l) return *this;
        Dealociraj();
        broj_elemenata = l.broj_elemenata;
        kapacitet = l.kapacitet;
        index = l.index;
        elementi = new Tip * [kapacitet];
        for (int i = 0; i < broj_elemenata; i++) elementi[i] = new Tip(*l.elementi[i]);
        return *this;
    }

    int brojElemenata() const override { return broj_elemenata; };

    Tip& trenutni() const override { DaLiJePrazan(); return *elementi[index]; };

    bool prethodni() override
    {
        if (index == 0) return false;
        index--;
        return true;
    }


    bool sljedeci() override
    {
        DaLiJePrazan();
        if (index == brojElemenata() - 1) return false;
        index++;
        return true;
    }

    void pocetak() override { DaLiJePrazan(); index = 0; };

    void kraj() override { DaLiJePrazan(); index = brojElemenata() - 1; };

    void obrisi() override
    {
        DaLiJePrazan();
        delete elementi[index];
        for (int i = index; i < brojElemenata() - 1; i++) elementi[i] = elementi[i + 1];
        broj_elemenata--;
        if (index == brojElemenata() && brojElemenata() > 0) index--;
    }

    void dodajIspred(const Tip& el) override {
        if (brojElemenata() == kapacitet) Realociraj();
        if (broj_elemenata == 0) elementi[index] = new Tip(el);
        else
        {
            for (int i = broj_elemenata; i > index; i--) elementi[i] = elementi[i - 1];
            elementi[index++] = new Tip(el);
        }
        broj_elemenata++;
    }

    void dodajIza(const Tip& el) override
    {
        if (brojElemenata() == kapacitet) Realociraj();
        if (broj_elemenata == 0) elementi[index] = new Tip(el);
        else
        {
            for (int i = broj_elemenata; i > index + 1; i--) elementi[i] = elementi[i - 1];
            elementi[index + 1] = new Tip(el);
        }
        broj_elemenata++;
    }



    Tip& operator[](int i) override
    {
        if (i < 0 || i >= brojElemenata()) throw std::range_error("Indeks izvan opsega");
        return *elementi[i];
    }

    const Tip& operator[](int i) const override
    {
        if (i < 0 || i >= brojElemenata()) throw std::range_error("Indeks izvan opsega");
        return *elementi[i];
    }

};


// JednostrukaLista //
template <typename Tip> class JednostrukaLista : public Lista<Tip> {

    struct Cvor {
        Tip a;
        Cvor* sljedeci;
    };

    Cvor* prvi;
    Cvor* posljednji;
    Cvor* tekuci;
    int broj_elemenata;

    void DaLiJePrazan() const { if (brojElemenata() == 0) throw std::logic_error("Lista je prazna!"); }

public:
    JednostrukaLista<Tip>() : prvi(nullptr), posljednji(nullptr), tekuci(nullptr), broj_elemenata(0) {}

    JednostrukaLista(const JednostrukaLista& l) : prvi(nullptr), posljednji(nullptr), tekuci(nullptr), broj_elemenata(0)
    {
        Cvor* temp = l.prvi;
        while (temp != nullptr)
        {
            dodajIza(temp->a);
            if (broj_elemenata == 1) tekuci = prvi;
            temp = temp->sljedeci;
        }
        tekuci = prvi;
    }

    JednostrukaLista& operator=(const JednostrukaLista& l)
    {
        if (this == &l) return *this;
        while (prvi != nullptr)
        {
            Cvor* temp = prvi;
            prvi = prvi->sljedeci;
            delete temp;
        }
        prvi = posljednji = tekuci = nullptr;
        broj_elemenata = 0;
        Cvor* temp = l.prvi;
        while (temp != nullptr)
        {
            dodajIza(temp->a);
            if (broj_elemenata == 1) tekuci = prvi;
            temp = temp->sljedeci;
        }
        tekuci = prvi;
        return *this;
    }

    ~JednostrukaLista()
    {
        while (prvi != nullptr)
        {
            tekuci = prvi;
            prvi = prvi->sljedeci;
            delete tekuci;
        }
        broj_elemenata = 0;
    }

    int brojElemenata() const override { return broj_elemenata; }

    Tip& trenutni() const override { DaLiJePrazan(); return tekuci->a; }

    bool prethodni() override
    {
        DaLiJePrazan();
        if (tekuci == prvi)  return false;
        Cvor* temp(prvi);
        while (temp->sljedeci != tekuci) temp = temp->sljedeci;
        tekuci = temp;
        return true;
    }

    bool sljedeci() override
    {
        DaLiJePrazan();
        if (tekuci->sljedeci == nullptr) return false;
        tekuci = tekuci->sljedeci;
        return true;
    }

    void pocetak() override { DaLiJePrazan(); tekuci = prvi; }

    void kraj() override { DaLiJePrazan(); tekuci = posljednji; }

    void obrisi() override
    {
        DaLiJePrazan();
        if (tekuci == prvi)
        {
            prvi = prvi->sljedeci;
            delete tekuci;
            tekuci = prvi; broj_elemenata--;
            if (broj_elemenata == 0) posljednji = nullptr;
        }
        else
        {
            Cvor* temp(prvi);
            while (temp->sljedeci != tekuci) temp = temp->sljedeci;
            temp->sljedeci = tekuci->sljedeci;
            delete tekuci;
            tekuci = temp->sljedeci;
            if (tekuci == nullptr) posljednji = tekuci = temp;
            broj_elemenata--;
        }
    }

    void dodajIspred(const Tip& a) override
    {
        Cvor* novi = new Cvor;
        novi->a = a;
        if (prvi == nullptr)
        {
            prvi = posljednji = tekuci = novi;
            prvi->sljedeci = nullptr;
            tekuci->sljedeci = nullptr;
        }
        else if (prvi == tekuci)
        {
            novi->sljedeci = prvi;
            prvi = novi;
        }
        else
        {
            Cvor* temp = prvi;
            while (temp->sljedeci != tekuci) temp = temp->sljedeci;
            temp->sljedeci = novi;
            novi->sljedeci = tekuci;
        }
        broj_elemenata++;
    }

    void dodajIza(const Tip& a) override
    {
        Cvor* novi = new Cvor;
        novi->a = a;
        if (prvi == nullptr)
        {
            prvi = posljednji = tekuci = novi;
            prvi->sljedeci = nullptr;
            novi->sljedeci = nullptr;
            tekuci->sljedeci = nullptr;
        }
        else
        {
            novi->sljedeci = tekuci->sljedeci;
            tekuci->sljedeci = novi;
            if (tekuci == posljednji) posljednji = novi;
        }
        broj_elemenata++;
    }


    Tip& operator[](int i) override
    {
        if (i < 0 || i >= broj_elemenata) throw std::range_error("Nema elemenata");
        Cvor* temp(prvi);
        for (int j = 0; j < i; j++) temp = temp->sljedeci;
        return temp->a;
    }

    const Tip& operator[](int i) const override
    {
        if (i < 0 || i >= broj_elemenata) throw  std::range_error("Nema elemenata");
        Cvor* temp(prvi);
        for (int j = 0; j < i; j++) temp = temp->sljedeci;
        return temp->a;
    }

};


// TESTIRANJE //

void testDodajIspred()
{
    NizLista<int> lista;
    lista.dodajIspred(5);
    lista.dodajIspred(3);
    lista.dodajIspred(1);
    lista.pocetak();
    if (lista.trenutni() == 3 && lista[1] == 1 && lista[2] == 5)
        std::cout << "Test PROŠAO\n";
    else std::cout << "Test PAO\n";
}

void testDodajIza()
{
    NizLista<int> lista;
    lista.dodajIspred(1);
    lista.dodajIza(3);
    lista.dodajIza(5);
    lista.pocetak();
    if (lista[0] == 1 && lista[1] == 5 && lista[2] == 3)
        std::cout << "Test PROŠAO\n";
    else std::cout << "Test PAO\n";
}

void testObrisi()
{
    NizLista<int> lista;
    lista.dodajIspred(1);
    lista.dodajIza(3);
    lista.dodajIza(5);
    lista.pocetak();
    lista.obrisi();
    if (lista.trenutni() == 5 && lista.brojElemenata() == 2)
        std::cout << "Test PROŠAO\n";
    else std::cout << "Test PAO\n";
}

void testPrethodniSljedeci()
{
    NizLista<int> lista;
    lista.dodajIspred(1);
    lista.dodajIza(3);
    lista.dodajIza(5);
    lista.pocetak();
    lista.sljedeci();
    if (lista.trenutni() == 5)
    {
        lista.prethodni();
        if (lista.trenutni() == 1) std::cout << "Test PROŠAO\n";
        else std::cout << "Test PAO\n";
    }
    else std::cout << "Test PAO\n";
}

void testOperatorIndeksiranja()
{
    NizLista<int> lista;
    lista.dodajIspred(1);
    lista.dodajIza(3);
    lista.dodajIza(5);
    if (lista[0] == 1 && lista[1] == 5 && lista[2] == 3)
        std::cout << "Test operator[]: PROŠAO\n";
    else std::cout << "Test operator[]: PAO\n";
}

// TESTOVI ZA DRUGU KLASU //

void testDodajIspredV2()
{
    JednostrukaLista<int> lista;
    lista.dodajIspred(5);
    lista.dodajIspred(3);
    lista.dodajIspred(1);
    lista.pocetak();
    if (lista.trenutni() == 3 && lista[1] == 1 && lista[2] == 5)
        std::cout << "Test dodajIspred: PROŠAO\n";
    else std::cout << "Test dodajIspred: PAO\n";
}

void testDodajIzaV2()
{
    JednostrukaLista<int> lista;
    lista.dodajIspred(1);
    lista.dodajIza(3);
    lista.dodajIza(5);
    lista.pocetak();
    if (lista[0] == 1 && lista[1] == 5 && lista[2] == 3)
        std::cout << "Test dodajIza: PROŠAO\n";
    else std::cout << "Test dodajIza: PAO\n";
}

void testObrisiV2()
{
    JednostrukaLista<int> lista;
    lista.dodajIspred(1);
    lista.dodajIza(3);
    lista.dodajIza(5);
    lista.pocetak();
    lista.obrisi();
    if (lista.trenutni() == 5 && lista.brojElemenata() == 2)
        std::cout << "Test obrisi: PROŠAO\n";
    else std::cout << "Test obrisi: PAO\n";
}

void testPrethodniSljedeciV2()
{
    JednostrukaLista<int> lista;
    lista.dodajIspred(1);
    lista.dodajIza(3);
    lista.dodajIza(5);
    lista.pocetak();
    lista.sljedeci();
    if (lista.trenutni() == 5)
    {
        lista.prethodni();
        if (lista.trenutni() == 1)
            std::cout << "Test prethodni/sljedeci: PROŠAO\n";
        else std::cout << "Test prethodni/sljedeci: PAO\n";
    }
    else std::cout << "Test prethodni/sljedeci: PAO\n";
}
void testOperatorIndeksiranjaV2()
{
    JednostrukaLista<int> lista;
    lista.dodajIspred(1);
    lista.dodajIza(3);
    lista.dodajIza(5);
    if (lista[0] == 1 && lista[1] == 5 && lista[2] == 3)
        std::cout << "Test operator[]: PROŠAO\n";
    else std::cout << "Test operator[]: PAO\n";
}


int main()
{
    try
    {
        testDodajIspred();
        testDodajIza();
        testObrisi();
        testPrethodniSljedeci();
        testOperatorIndeksiranja();
        std::cout << std::endl;
        testDodajIspredV2();
        testDodajIzaV2();
        testObrisiV2();
        testPrethodniSljedeciV2();
        testOperatorIndeksiranjaV2();
    }
    catch (const std::logic_error& e)
    {
        std::cout << "Logic error: " << e.what();
    }
    catch (const std::range_error& e)
    {
        std::cout << "Range error: " << e.what();
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: " << e.what();
    }
    catch (...)
    {
        std::cout << "ERROR";
    }
    return 0;
}
