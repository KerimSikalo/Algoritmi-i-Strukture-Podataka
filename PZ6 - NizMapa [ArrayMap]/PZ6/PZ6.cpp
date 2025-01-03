/*Zadatak 1.
Napraviti apstraktnu generičku klasu Mapa.Mapa je vrlo često korišten apstraktni tip podataka u
programiranju.Mapa uvezuje podatak jednog proizvoljnog tipa(ključ) sa podatkom drugog
proizvoljnog i ne nužno istog tipa(vrijednost).Programer često doživljava mapu kao jednu posebnu
vrstu niza / vektora kod kojeg ključ ne mora biti cjelobrojnog tipa niti mora biti u nekom fiksnom
opsegu.Npr.mogli biste pisati :
NizMapa<string, string> glavni_gradovi;
glavni_gradovi[“Bosna i Hercegovina”] = “Sarajevo”;
glavni_gradovi[“Hrvatska”] = “Zagreb”;
glavni_gradovi[“Sjedinjene Američke Države”] = “Washington”;
Potrebno je napraviti apstraktnu klasu kako bismo mogli eksperimentisati sa raznim strategijama
implementacije Mape.Apstraktna klasa Mapa treba podržavati sljedeće metode :
● Konstruktor bez parametara koji kreira praznu mapu.Ovdje treba istaći da, pošto je Mapa
generička klasa sa dva tipa(TipKljuca i TipVrijednosti), a konstruktor nema parametara, prilikom
kreiranja objekta tipa Mapa morate navesti oba tipa na sljedeći način :
NizMapa<string, float> m;
● Preklopljeni operator uglaste zagrade[](konstantna i nekonstantna varijanta), kako bi se mapa
mogla koristiti u svim situacijama kao i niz(vidjeti primjer iznad).Default vrijednost člana mape
(ako ključ ne postoji u mapi) treba biti TipVrijednosti()
● Metodu int brojElemenata() koja vraća broj elemenata trenutno pohranjenih u mapi.
● Metodu void obrisi() koja briše sve elemente iz mape.Nakon poziva ove metode mapa mora i
dalje biti upotrebljiva(trebaju se moći dodavati novi elementi).
● Metodu void obrisi(const TipKljuca& kljuc) koja briše samo jedan element iz mape sa ključem
kljuc.Razlog za ovu metodu je što obrisati element nije isto što i pridružiti mu nulu ili prazan string,
jer se brisanjem oslobađa memorija.U slučaju da ključ ne postoji baciti izuzetak.
Da bi se ovakva Mapa ikako mogla implementirati, potrebno je da TipKljuca bude uporedivi tip
(mora podržavati operatore poređenja).Za TipVrijednosti se traži samo da posjeduje default
konstruktor, kao što je slučaj i prilikom kreiranja niza.Na primjer : za klasu std::vector je definisan
default konstruktor ali nije definisan operator poređenja.
Zadatak 2.
Napraviti klasu NizMapa izvedenu iz klase Mapa, kod koje je mapa implementirana sa dinamički
alociranim nizom koji koristi Pair(niz ključeva i niz vrijednosti).Voditi računa da veličina mape ne
bude ograničena ničim osim dostupnom memorijom na računaru, ali i da dodavanje novog elementa
u mapu ima optimalne performanse.Omogućiti da se NizMapa može slati funkcijama, vraćati iz
funkcija, da se nad konstantnom referencom na NizMapu mogu vršiti sve operacije koje ima smisla
vršiti, te da pri svemu tome ne dolazi do curenja memorije.Napravite i minimalno pet testnih
funkcija u kojim testirate osobine NizMape sa različitim tipovima ključa i vrijednosti.*/


#include <iostream>
#include <list>
#include <utility>
#include <string>

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
        :  p(new std::pair<TipKljuca, TipVrijednosti>* [100000]()), velicina(100000), brEl(0) {  }

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
            if (p[i] -> first == kljuc) return p[i] -> second;
        p[brEl] = new std::pair<TipKljuca, TipVrijednosti>(kljuc, TipVrijednosti()); brEl++;
        return p[brEl - 1] -> second;
    }

    const TipVrijednosti operator[](TipKljuca kljuc) const 
    {
        for (int i = 0; i < brEl; i++)
            if (p[i] -> first == kljuc) return p[i]->second;
        return TipVrijednosti();
    }
};


//TESTNE FUNKCIJE//
void testIntString() 
{
    NizMapa<int, std::string> mapa;
    mapa[1] = "jedan";
    mapa[2] = "dva";
    mapa[3] = "tri";
    if (mapa.brojElemenata() != 3) std::cout << "Test failed: brojElemenata() != 3" << std::endl;
    if (mapa[1] != "jedan") std::cout << "Test failed: mapa[1] != 'jedan'" << std::endl;
    if (mapa[2] != "dva") std::cout << "Test failed: mapa[2] != 'dva'" << std::endl;
    if (mapa[3] != "tri") std::cout << "Test failed: mapa[3] != 'tri'" << std::endl;
    mapa.obrisi(2);
    if (mapa.brojElemenata() != 2) std::cout << "Test failed: brojElemenata() != 2" << std::endl;
    try 
    {
        mapa.obrisi(2);
    }
    catch (std::logic_error& e) 
    {
        std::cout << e.what() << std::endl;
    }
}

void testStringFloat() 
{
    NizMapa<std::string, float> mapa;
    mapa["pi"] = 3.14;
    mapa["e"] = 2.71;
    if (mapa.brojElemenata() != 2) std::cout << "Test failed: brojElemenata() != 2" << std::endl;
    if (mapa["pi"] != 3.14f) std::cout << "Test failed: mapa['pi'] != 3.14f" << std::endl;
    if (mapa["e"] != 2.71f) std::cout << "Test failed: mapa['e'] != 2.71f" << std::endl;
    mapa.obrisi("pi");
    if (mapa.brojElemenata() != 1) std::cout << "Test failed: brojElemenata() != 1" << std::endl;
    try 
    {
        mapa.obrisi("pi");
    }
    catch (std::logic_error& e) 
    {
        std::cout << e.what() << std::endl;
    }
}

void testCharDouble() 
{
    NizMapa<char, double> mapa;
    mapa['a'] = 1.1;
    mapa['b'] = 2.2;
    mapa['c'] = 3.3;
    if (mapa.brojElemenata() != 3) std::cout << "Test failed: brojElemenata() != 3" << std::endl;
    if (mapa['a'] != 1.1) std::cout << "Test failed: mapa['a'] != 1.1" << std::endl;
    if (mapa['b'] != 2.2) std::cout << "Test failed: mapa['b'] != 2.2" << std::endl;
    if (mapa['c'] != 3.3) std::cout << "Test failed: mapa['c'] != 3.3" << std::endl;
    mapa.obrisi('b');
    if (mapa.brojElemenata() != 2) std::cout << "Test failed: brojElemenata() != 2" << std::endl;
    try 
    {
        mapa.obrisi('b');
    }
    catch (std::logic_error& e) 
    {
        std::cout << e.what() << std::endl;
    }
}

void testStringString() 
{
    NizMapa<std::string, std::string> mapa;
    mapa["Bosna i Hercegovina"] = "Sarajevo";
    mapa["Hrvatska"] = "Zagreb";
    mapa["Sjedinjene Američke Države"] = "Washington";
    if (mapa.brojElemenata() != 3) std::cout << "Test failed: brojElemenata() != 3" << std::endl;
    if (mapa["Bosna i Hercegovina"] != "Sarajevo") std::cout << "Test failed: mapa['Bosna i Hercegovina'] != 'Sarajevo'" << std::endl;
    if (mapa["Hrvatska"] != "Zagreb") std::cout << "Test failed: mapa['Hrvatska'] != 'Zagreb'" << std::endl;
    if (mapa["Sjedinjene Američke Države"] != "Washington") std::cout << "Test failed: mapa['Sjedinjene Američke Države'] != 'Washington'" << std::endl;
    mapa.obrisi("Hrvatska");
    if (mapa.brojElemenata() != 2) std::cout << "Test failed: brojElemenata() != 2" << std::endl;
    try 
    {
        mapa.obrisi("Hrvatska");
    }
    catch (std::logic_error& e) 
    {
        std::cout << e.what() << std::endl;
    }
}

void testIntInt() {
    NizMapa<int, int> mapa;
    mapa[1] = 100;
    mapa[2] = 200;
    mapa[3] = 300;
    if (mapa.brojElemenata() != 3) std::cout << "Test failed: brojElemenata() != 3" << std::endl;
    if (mapa[1] != 100) std::cout << "Test failed: mapa[1] != 100" << std::endl;
    if (mapa[2] != 200) std::cout << "Test failed: mapa[2] != 200" << std::endl;
    if (mapa[3] != 300) std::cout << "Test failed: mapa[3] != 300" << std::endl;
    mapa.obrisi(2);
    if (mapa.brojElemenata() != 2) std::cout << "Test failed: brojElemenata() != 2" << std::endl;
    try {
        mapa.obrisi(2);
    }
    catch (std::logic_error& e) {
        std::cout << e.what() << std::endl;
    }
}

int main() 
{
    testIntString();
    testStringFloat();
    testCharDouble();
    testStringString();
    testIntInt();
    std::cout << "Svi prosli!" << std::endl;
    return 0;
}