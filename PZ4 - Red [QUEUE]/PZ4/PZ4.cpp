/*
Napraviti generičku klasu Red sa sljedećim metodama:
● konstruktor koji kreira prazan red;
● po potrebi destruktor, konstruktor kopije, operator dodjele;
● metodu brisi() koja briše sve elemente iz reda;
● metodu stavi(const Tip& el) koja stavlja jedan element na kraj reda;
● metodu skini() koja vraća element na čelu (početku) reda i ujedno ga briše iz reda, odnosno
baca izuzetak ako je red prazan;
● metodu celo() koja vraća referencu na element na čelu (početku) reda ali ga ne briše iz reda,
odnosno baca izuzetak ako je red prazan;
● metodu brojElemenata() koja vraća broj elemenata koji se trenutno nalaze u redu.
Implementaciju ove klase možete uraditi na bilo koji način, ali imajte na umu da će se mjeriti i
performanse rješenja (vremenska i memorijska složenost). U pozadini klase Red je dozvoljeno
 koristiti predhodno implementiranu klasu Lista, nije dozvoljeno korištenje bibliotečne klase vector..
Veličina reda ne smije biti ograničena.
*/

#include <iostream>
using namespace std;

template<typename Tip> class Red {
	struct Cvor {
		Tip a;
		Cvor* sljedeci;
	};
	Cvor* pocetak;
	Cvor* kraj;
	int brEl;

	void Test() { if (brEl == 0) throw "Red je prazan!"; }


public:

	Red() : brEl(0), pocetak(nullptr), kraj(nullptr) {}

	~Red()
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

	Red(const Red& r)
	{
		brEl = 0;
		pocetak = nullptr;
		if (r.brEl == 0) { pocetak = nullptr; kraj = nullptr; }
		else
		{
			Cvor* temp(r.pocetak);
			while (temp != nullptr)
			{
				stavi(temp->a);
				temp = temp->sljedeci;
			}
		}
	}

	Red& operator=(const Red& r) {

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
				stavi(temp->a);
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

	void stavi(const Tip& el)
	{
		Cvor* novi = new Cvor{ el, nullptr };
		if (pocetak == nullptr)
		{
			kraj = novi;
			pocetak = novi;
			pocetak->sljedeci = nullptr;
			kraj->sljedeci = nullptr;
		}
		else
		{
			kraj->sljedeci = novi;
			kraj = novi;
			kraj->sljedeci = nullptr;
		}
		brEl++;
	}

	Tip skini()
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

	Tip& celo() { return pocetak->a; }

	int brojElemenata() { return brEl; }
};



// STEK //
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


// FUNKCIJE SA LABA //

/*Napisati funkciju koja prima red i modifikuje ga tako da budu izbačene sve vrijednosti veće od
srednje vrijednosti njegovih elemenata.*/

template <typename Tip>
Red<Tip> modifikuj(Red<Tip>& r)
{
	int suma = 0;
	for (int i = 0; i < r.brojElemenata(); i++) 
	{
		suma += r.celo();
		r.stavi(r.celo());
		r.skini();
	}
	int srednjaVr = suma / r.brojElemenata();
	for (int i = 0; i <= r.brojElemenata(); i++)
	{
		if (r.celo() > srednjaVr) 
		{
			r.skini();
		} 
		else
		{
			r.stavi(r.celo());
			r.skini();
		}
	}
	return r;
}



/*Napisati funkciju koja prima stek, red i varijablu pocetnaDuzinaReda tipa int koja će
elemente iz reda ubaciti na dno steka u obrnutom redoslijedu.Primjer: Neka je dat red sa
elementima{ 1, 2, 3, 4 } gdje je element 1 na čelu reda i stek sa elementima{ 5, 6, 7, 8 } gdje je
element 8 na vrhu steka.Rezultantni stek se sastoji od elemenata{ 1, 2, 3, 4, 5, 6, 7, 8 } gdje je
element 8 na vrhu steka*/

template <typename Tip>
void ubaciNaDno(Stek<Tip>& s, Red<Tip>& r, int pocetnaDuzinaReda)
{
	if (pocetnaDuzinaReda == 0) return;

	Tip element = r.skini();
	ubaciNaDno(s, r, pocetnaDuzinaReda - 1);
	s.stavi(element);
}


// TESTOVI METODA I FUNKCIJA// 

void testStavi()
{
	Red<int> red;
	red.stavi(5);
	std::cout << "Test stavi: " << (red.brojElemenata() == 1 && red.celo() == 5 ? "Prosao" : "Pao") << std::endl;
}

void testSkini()
{
	Red<int> red;
	red.stavi(5);
	int vrijednost = red.skini();
	std::cout << "Test skini: " << (vrijednost == 5 && red.brojElemenata() == 0 ? "Prosao" : "Pao") << std::endl;
}

void testCelo()
{
	Red<int> red;
	red.stavi(5);
	std::cout << "Test celo: " << (red.celo() == 5 ? "Prosao" : "Pao") << std::endl;
}

void testBrojElemenata()
{
	Red<int> red;
	red.stavi(5);
	red.stavi(10);
	std::cout << "Test brojElemenata: " << (red.brojElemenata() == 2 ? "Prosao" : "Pao") << std::endl;
}

void testBrisi()
{
	Red<int> red;
	red.stavi(5);
	red.brisi();
	std::cout << "Test brisi: " << (red.brojElemenata() == 0 ? "Prosao" : "Pao") << std::endl;
}


void testModifikuj()
{
	Red<int> red;
	red.stavi(1);
	red.stavi(2);
	red.stavi(3);
	red.stavi(4);
	red.stavi(5);

	Red<int> modifikovaniRed = modifikuj(red);

	std::cout << "Test modifikuj: ";
	bool passed = true;
	while (modifikovaniRed.brojElemenata() > 0)
	{
		int element = modifikovaniRed.skini();
		if (element > 3) 
		{
			passed = false;
			break;
		}
	}
	std::cout << (passed ? "Prosao" : "Pao") << std::endl;
}

int main()
{
	testStavi();
	testSkini();
	testCelo();
	testBrojElemenata();
	testBrisi();
	testModifikuj();
	return 0;
}

