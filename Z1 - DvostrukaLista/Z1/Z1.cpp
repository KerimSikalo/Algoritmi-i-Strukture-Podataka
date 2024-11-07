#include <iostream>

using namespace std;

template <typename Tip> class Iterator;

// Apstraktna klasa //
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

template <typename Tip> class DvostrukaLista : public Lista<Tip> {

    struct Cvor {
        Tip a;
        Cvor* prije;
        Cvor* sljedeci;
    };

    Cvor* prvi;
    Cvor* posljednji;
    Cvor* tekuci;
    int brEl;

    friend class Iterator<Tip>;

    void DaLiJePrazan() const { if (brojElemenata() == 0) throw std::logic_error("Lista je prazna!"); }

public:

    DvostrukaLista<Tip>() : prvi(nullptr), posljednji(nullptr), tekuci(nullptr), brEl(0) {}


    DvostrukaLista(const DvostrukaLista& l) : prvi(nullptr), posljednji(nullptr), tekuci(nullptr), brEl(0)
    {
        Cvor* temp(l.prvi);
        if (prvi) pocetak();
        while (temp != nullptr)
        {
            dodajIza(temp->a);
            sljedeci();
            temp = temp->sljedeci;
            posljednji = tekuci;
        }
    }

    DvostrukaLista& operator=(const DvostrukaLista& l) 
    {
        if (this == &l) return *this;
        while (prvi != nullptr) 
        {
            tekuci = prvi;
            prvi = prvi->sljedeci;
            delete tekuci;
        }
        brEl = 0;
        if (l.brEl == 0) { prvi = nullptr; posljednji = nullptr; tekuci = nullptr; }
        else 
        {
            Cvor* temp(l.prvi);
            while (temp != nullptr) 
            {
                dodajIza(temp->a);
                sljedeci();
                temp = temp->sljedeci;
                posljednji = tekuci;
            }
        }
        return *this;
    }
 
    ~DvostrukaLista()
    {
        while (prvi != nullptr)
        {
            tekuci = prvi;
            prvi = prvi->sljedeci;
            delete tekuci;
        }
        posljednji = nullptr;
        tekuci = nullptr;
        brEl = 0;
    }

    int brojElemenata() const override { return brEl; }

    Tip& trenutni() const override { DaLiJePrazan(); return tekuci->a; }

    bool prethodni() override
    {
        DaLiJePrazan();
        if (tekuci == prvi) return false;
        tekuci = tekuci->prije;
        return true;
    }

    bool sljedeci() override
    {
        DaLiJePrazan();
        if (tekuci == posljednji) return false;
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
            Cvor* temp = prvi->sljedeci;
            delete prvi;
            prvi = temp;
            if (prvi) prvi->prije = nullptr;
            tekuci = prvi;
        }
        else if (tekuci == posljednji)
        {
            Cvor* temp = tekuci->prije;
            delete tekuci;
            posljednji = temp;
            if (posljednji) posljednji->sljedeci = nullptr;
            tekuci = prvi;
        }
        else
        {
            Cvor* temp = tekuci->prije;
            temp->sljedeci = tekuci->sljedeci;
            tekuci->sljedeci->prije = temp;
            delete tekuci;
            tekuci = temp->sljedeci;
        }
        brEl--;
    }

    void dodajIspred(const Tip& el) override
    {
        Cvor* novi = new Cvor{ el, nullptr, nullptr };
        if (prvi == nullptr)
        {
            prvi = posljednji = tekuci = novi;
        }
        else if (tekuci == prvi)
        {
            novi->sljedeci = prvi;
            prvi->prije = novi;
            prvi = novi;
        }
        else
        {
            novi->sljedeci = tekuci;
            novi->prije = tekuci->prije;
            tekuci->prije->sljedeci = novi;
            tekuci->prije = novi;
        }
        brEl++;
    }

    void dodajIza(const Tip& el) override
    {
        Cvor* novi = new Cvor{ el, nullptr, nullptr };
        if (prvi == nullptr)
        {
            prvi = posljednji = tekuci = novi;
        }
        else
        {
            novi->sljedeci = tekuci->sljedeci;
            tekuci->sljedeci = novi;
            novi->prije = tekuci;
            if (tekuci == posljednji) posljednji = novi;
        }
        brEl++;
    }

    Tip& operator[](int i) override
    {
        if (i < 0 || i >= brEl) throw std::range_error("Nema elemenata");
        Cvor* temp = prvi;
        for (int j = 0; j < i; j++) temp = temp->sljedeci;
        return temp->a;
    }

    const Tip& operator[](int i) const override
    {
        if (i < 0 || i >= brEl) throw std::range_error("Nema elemenata");
        Cvor* temp = prvi;
        for (int j = 0; j < i; j++) temp = temp->sljedeci;
        return temp->a;
    }
};

// Iterator //
template <typename Tip> class Iterator {

    const DvostrukaLista<Tip>* lista;
    typename DvostrukaLista<Tip>::Cvor* trenutnaLista;


public:

    Iterator(const DvostrukaLista<Tip>& n) : lista(&n), trenutnaLista(n.prvi) {}

    Tip& trenutni() const
    {
        if (lista != nullptr)
        {
            if (lista->brojElemenata() == 0) throw std::logic_error("Lista je prazna!");
            return trenutnaLista->a;
        }
        throw std::logic_error("Lista je prazna!");
    }

    bool prethodni()
    {
        if (lista != nullptr)
        {
            if (trenutnaLista == lista->prvi) return false;
            typename DvostrukaLista<Tip>::Cvor* temp = lista->prvi;
            while (temp->sljedeci != trenutnaLista) temp = temp->sljedeci;
            trenutnaLista = temp;
            return true;
        }
        return false;
    }

    bool sljedeci()
    {
        if (lista != nullptr)
        {
            if (trenutnaLista->sljedeci == nullptr) return false;
            trenutnaLista = trenutnaLista->sljedeci;
            return true;
        }
        return false;
    }

    void pocetak() { if (lista != nullptr) trenutnaLista = lista->prvi; }

    void kraj() { if (lista != nullptr) trenutnaLista = lista->posljednji; }

};



template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& l)
{
    if (l.brojElemenata() == 0) throw std::logic_error("Lista je prazna!");

    Iterator<Tip>& it = l;
    Tip temp = it.trenutni();
    it.pocetak();
    Tip max = it.trenutni();
    while (it.sljedeci()) if (it.trenutni() > max) max = it.trenutni();
    it.trenutni() = temp;
    return max;
}



// Testne funkcije //

void testDodajIspred()
{
    DvostrukaLista<int> lista;
    lista.dodajIspred(1);
    lista.dodajIspred(2);
    lista.dodajIspred(3);
    if (lista.brojElemenata() != 3) std::cout << "testDodajIspred pao: brojElemenata" << std::endl;
    if (lista.trenutni() != 1) std::cout << "testDodajIspred pao: trenutni" << std::endl;
    lista.pocetak();
    if (lista.trenutni() != 2) std::cout << "testDodajIspred pao: pocetak" << std::endl;
}

void testDodajIza()
{
    DvostrukaLista<int> lista;
    lista.dodajIza(1);
    lista.dodajIza(2);
    lista.dodajIza(3);
    if (lista.brojElemenata() != 3) std::cout << "testDodajIza pao: brojElemenata" << std::endl;
    if (lista.trenutni() != 1) std::cout << "testDodajIza pao: trenutni" << std::endl;
    lista.sljedeci();
    if (lista.trenutni() != 2) std::cout << "testDodajIza pao: sljedeci" << std::endl;
}

void testObrisi()
{
    DvostrukaLista<int> lista;
    lista.dodajIza(1);
    lista.dodajIza(2);
    lista.dodajIza(3);
    lista.obrisi();
    if (lista.brojElemenata() != 2) std::cout << "testObrisi pao: brojElemenata" << std::endl;
    if (lista.trenutni() != 1) std::cout << "testObrisi pao: trenutni" << std::endl;
    lista.sljedeci();
    lista.obrisi();
    if (lista.brojElemenata() != 1) std::cout << "testObrisi pao: brojElemenata 2" << std::endl;
    if (lista.trenutni() != 1) std::cout << "testObrisi pao: trenutni 2" << std::endl;
}

void testPrethodni()
{
    DvostrukaLista<int> lista;
    lista.dodajIza(1);
    lista.dodajIza(2);
    lista.dodajIza(3);
    lista.sljedeci();
    lista.sljedeci();
    if (lista.trenutni() != 3) std::cout << "testPrethodni pao: trenutni" << std::endl;
    lista.prethodni();
    if (lista.trenutni() != 2) std::cout << "testPrethodni pao: prethodni" << std::endl;
}

void testSljedeci()
{
    DvostrukaLista<int> lista;
    lista.dodajIza(1);
    lista.dodajIza(2);
    lista.dodajIza(3);
    if (lista.trenutni() != 1) std::cout << "testSljedeci pao: trenutni" << std::endl;
    lista.sljedeci();
    if (lista.trenutni() != 2) std::cout << "testSljedeci pao: sljedeci" << std::endl;
    lista.sljedeci();
    if (lista.trenutni() != 3) std::cout << "testSljedeci pao: sljedeci 2" << std::endl;
}

void testPocetak()
{
    DvostrukaLista<int> lista;
    lista.dodajIza(1);
    lista.dodajIza(2);
    lista.dodajIza(3);
    lista.sljedeci();
    lista.sljedeci();
    lista.pocetak();
    if (lista.trenutni() != 1) std::cout << "testPocetak pao: pocetak" << std::endl;
}

void testKraj()
{
    DvostrukaLista<int> lista;
    lista.dodajIza(1);
    lista.dodajIza(2);
    lista.dodajIza(3);
    lista.kraj();
    if (lista.trenutni() != 3) std::cout << "testKraj pao: kraj" << std::endl;
}

void testOperator()
{
    DvostrukaLista<int> lista;
    lista.dodajIza(1);
    lista.dodajIza(2);
    lista.dodajIza(3);
    if (lista[0] != 1) std::cout << "testOperator pao: operator[0]" << std::endl;
    if (lista[1] != 2) std::cout << "testOperator pao: operator[1]" << std::endl;
    if (lista[2] != 3) std::cout << "testOperator pao: operator[2]" << std::endl;
}

int main()
{
    DvostrukaLista<int> lista;
    for (int i(1); i <= 5; i++)
        lista.dodajIspred(i);
    {
        // Konstruktor kopije
        DvostrukaLista<int> lista2(lista);
        DvostrukaLista<int> lista3;
        // Operator dodjele
        (lista3 = lista).brojElemenata();
        // Destruktivna samododjela
        (lista = lista).brojElemenata();

        lista.obrisi();
        std::cout << lista2.brojElemenata() << " " << lista2[2];
        std::cout << " " << lista3.brojElemenata() << " " << lista3[2] << " ";
    }
    std::cout << lista.brojElemenata() << " " << lista[2];
    return 0;
}