/*Potrebno je implementirati klasu DvostraniRed koja ima osobine bibliotečne klase deque. Klasa
DvostraniRed, pored konstruktora, destruktora, konstruktora kopije, operatora dodjele i metoda
brisi() i brojElemenata() treba da ima i sljedeće metode:
● staviNaVrh()
● skiniSaVrha()
● staviNaCelo()
● skiniSaCela()
● vrh()
● celo()
Ove metode trebaju da budu analogne metodama klasa Stek i Red po pitanju broja i tipa
parametara. Sve metode trebaju da imaju složenost O(1) osim metode brisi(). Pri implementaciji
klase DvostraniRed nije dozvoljeno korištenje bibliotečnih klasa stack, queue, deque, vector.
Za svaku metodu napisati funkciju koja dokazuje ispravnost tih metoda.*/

#include <iostream>
#include <vector>

using namespace std;

template<typename Tip> class DvostraniRed {
	struct Cvor {
		Tip a;
		Cvor* sljedeci;
		Cvor* prethodni;
	};
	Cvor* pocetak;
	Cvor* kraj;
	int brEl;

	void Test() { if (brEl == 0) throw "Red je prazan!"; }


public:

	DvostraniRed() : brEl(0), pocetak(nullptr), kraj(nullptr) {}

	~DvostraniRed()
	{
		kraj = nullptr;
		while (pocetak != nullptr)
		{
			Cvor* temp = pocetak;
			pocetak = pocetak->sljedeci;
			delete temp;
		}
		brEl = 0;
	}

	DvostraniRed(const DvostraniRed& r)
	{
		brEl = 0;
		pocetak = nullptr;
		if (r.brEl == 0) { pocetak = nullptr; kraj = nullptr; }
		else
		{
			Cvor* temp(r.pocetak);
			while (temp != nullptr)
			{
				staviNaVrh(temp->a);
				temp = temp->sljedeci;
			}
		}
	}

	DvostraniRed& operator=(const DvostraniRed& r) 
	{
		if (this == &r) return *this;
		while (pocetak != kraj)
		{
			Cvor* tekuci = pocetak;
			pocetak = pocetak->sljedeci;
			delete tekuci;
		}
		brEl = 0;
		if (r.brEl == 0) pocetak = nullptr;
		else
		{
			Cvor* temp(r.pocetak);
			while (temp != nullptr)
			{
				staviNaVrh(temp->a);
				temp = temp->sljedeci;
			}
			return *this;
		}
	}

	void brisi()
	{
		kraj = nullptr;
		while (pocetak != nullptr)
		{
			Cvor* temp = pocetak;
			pocetak = pocetak->sljedeci;
			delete temp;
		}
		brEl = 0;
	}

	void staviNaCelo(const Tip& el)
	{
		Cvor* novi = new Cvor{ el, nullptr };
		if (pocetak == nullptr)
		{
			kraj = novi;
			pocetak = novi;
			pocetak->sljedeci = pocetak->prethodni = nullptr;
			kraj->sljedeci = kraj->prethodni = nullptr;
		}
		else
		{
			novi->sljedeci = pocetak;
			prvi->prethodni = novi;
			prvi = novi;
			prvi->sljedeci = nullptr;
		}
		brEl++;
	}

	Tip skiniSaCela()
	{
		Test();
		Tip a = pocetak->a;
		Cvor* temp = pocetak;
		temp = temp->sljedeci;
		delete pocetak;
		pocetak = temp;
		brEl--;
		return a;
	}

	void staviNaVrh(const Tip& el)
	{
		Cvor* novi = new Cvor{ el, nullptr };
		if (pocetak == nullptr)
		{
			kraj = novi;
			pocetak = novi;
			pocetak->sljedeci = pocetak->prethodni = nullptr;
			kraj->sljedeci = kraj->prethodni = nullptr;
		}
		else
		{
			kraj->sljedeci = novi;
			kraj->prethodni = novi;
			kraj = novi;
			kraj->sljedeci = nullptr;
		}
		brEl++;
	}

	Tip skiniSaVrha()
	{
		Test();
		Tip a = kraj->a;
		Cvor* temp = kraj;
		temp = temp->prethodni;
		delete kraj;
		kraj = temp;
		brEl--;
		return a;
	}

	Tip& vrh() { return kraj->a; }

	Tip& celo() { return pocetak->a; }

	int brojElemenata() { return brEl; }
};


// Stek //
template <typename Tip> class Stek {
	struct Cvor {
		Tip a;
		Cvor* veza;
	};
	Cvor* vrhh;
	int brEl;

	void Test() { if (!vrhh) throw std::logic_error("Prazan stek!"); }

	Cvor* Kopiraj(const Stek& s)
	{
		Cvor* vrhh = nullptr;
		Cvor* prethodni = nullptr;
		try
		{
			for (Cvor* tekuci = s.vrhh; tekuci != nullptr; tekuci = tekuci->veza)
			{
				Cvor* novi = new Cvor{ tekuci->a, nullptr };
				if (!vrhh) vrhh = novi;
				else prethodni->veza = novi;
				prethodni = novi;
			}
		}
		catch (...) {
			while (vrhh != nullptr)
			{
				Cvor* temp = vrhh;
				vrhh = vrhh->veza;
				delete temp;
			}
			throw;
		}
		return vrhh;
	}

public:

	Stek() : vrhh(nullptr), brEl(0) {}

	Stek(const Stek& s) : vrhh(Kopiraj(s)), brEl(s.brEl) {}

	~Stek()
	{
		while (vrhh != nullptr)
		{
			Cvor* temp = vrhh;
			vrhh = vrhh->veza;
			delete temp;
		}

	}

	Stek& operator=(const Stek& s)
	{
		if (&s != this) {
			Cvor* novi_vrh = Kopiraj(s);
			while (vrhh != nullptr)
			{
				Cvor* temp = vrhh;
				vrhh = vrhh->veza;
				delete temp;
			}
			vrhh = novi_vrh;
		}
		brEl = s.brEl;
		return *this;
	}


	void brisi()
	{
		while (vrhh != nullptr)
		{
			Cvor* temp = vrhh;
			vrhh = vrhh->veza;
			delete temp;
			brEl--;
		}
	}

	void stavi(const Tip& el)
	{
		if (brEl == 0) vrhh = new Cvor{ el, nullptr };
		else
		{
			Cvor* temp = vrhh;
			vrhh = new Cvor{ el, temp };
		}
		brEl++;
	}

	Tip skini()
	{
		Test();
		Cvor* novi = vrhh;
		Tip a = novi->a;
		vrhh = vrhh->veza;
		delete novi;
		brEl--;
		return a;
	}

	Tip& vrh()
	{
		Test();
		return vrhh->a;
	}

	int brojElemenata() const { return brEl; }

};


/*Implementirati funkciju koja prima stek s i parametar trazeni element. Funkcija treba da pronađe element u jednom od
vektora koji se nalaze na steku. Kada se pronađe odgovarajući vektor, dalju pretragu je potrebno
obaviti koristeći binarno pretraživanje vektora. Stek treba da ostane nepromijenjen nakon poziva
funkcije. U slučaju da se element ne nalazi ni u jednom od vektora, potrebno je ispisati „Nema
elementa“, a ako je element pronađen potrebno je ispisati index elementa u pronađenom vektoru
i koliko se vektora nalazi na steku koji su ispod pronađenog vektora. Predpostaviti da su elementi
u svakom vektoru sortirani rastuće i da su svi elementi svakog vektora manji od svih elemenata
vektora koji je na steku iznad njega.

Primjer: Za ulazne parametre (prvi element je na dnu steka) stek
s = {{1,2,3,4,5},{6,8},{9,10,130},{157,226,3424}} i trazeni = 10 program treba da ispise „1 2“.
Nije dozvoljeno koristenje dodatnih funkcija niti kolekcija osim date kolekcije Stek primljene
kao parametar, jednog vektora samo za primanje vektora koji se nalaze na steku. Napisati tri
funkcije koje dokazuju ispravan rad funkcije.*/


void pretraga(Stek<vector<int> >& s, int trazeni)
{
	
	
	
	
	std::cout << "Nema elementa" << std::endl;
}


// Testne funkcije //



int main() 
{ 
	return 0;
}