/*
Napraviti generičku klasu Stek sa sljedećim metodama:
 konstruktor koji kreira prazan stek;
 po potrebi destruktor, konstruktor kopije, operator dodjele;
 metodu brisi() koja briše sve elemente iz steka;
 metodu stavi(const Tip& el) koja stavlja jedan element na vrh steka;
 metodu skini() koja vraća element na vrhu steka i ujedno ga briše iz steka, odnosno baca
izuzetak ako je stek prazan;
 metodu vrh() koja vraća referencu na element na vrhu steka ali ga ne briše, odnosno baca
izuzetak ako je stek prazan;
 metodu brojElemenata() koja vraća broj elemenata koji se trenutno nalaze na steku.
Implementaciju ove klase možete uraditi na bilo koji način, ali imajte na umu da će se mjeriti i
performanse rješenja (vremenska i memorijska složenost). Veličina steka ne smije biti ograničena.
U pozadini klase stek je dozvoljeno koristiti predhodno implementiranu klasu Lista, nije dozvoljeno
korištenje bibliotečne klase vector.
Za svaku metodu klase je potrebno napisati funkciju koja će dokazati ispravnost te metode.
*/

#include <iostream>

using namespace std;

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


// Testovi za metode //

void testStavi() {
	Stek<int> stek;
	stek.stavi(5);
	if (stek.vrh() != 5) {
		std::cout << "testStavi pao!" << std::endl;
	}
	else {
		std::cout << "testStavi prosao!" << std::endl;
	}
}

void testSkini() {
	Stek<int> stek;
	stek.stavi(5);
	int value = stek.skini();
	if (value != 5 || stek.brojElemenata() != 0) {
		std::cout << "testSkini pao!" << std::endl;
	}
	else {
		std::cout << "testSkini prosao!" << std::endl;
	}
}

void testVrh() {
	Stek<int> stek;
	stek.stavi(5);
	if (stek.vrh() != 5) {
		std::cout << "testVrh pao!" << std::endl;
	}
	else {
		std::cout << "testVrh prosao!" << std::endl;
	}
}

void testBrojElemenata() {
	Stek<int> stek;
	stek.stavi(5);
	stek.stavi(10);
	if (stek.brojElemenata() != 2) {
		std::cout << "testBrojElemenata pao!" << std::endl;
	}
	else {
		std::cout << "testBrojElemenata prosao!" << std::endl;
	}
}

void testBrisi() {
	Stek<int> stek;
	stek.stavi(5);
	stek.brisi();
	if (stek.brojElemenata() != 0) {
		std::cout << "testBrisi pao!" << std::endl;
	}
	else {
		std::cout << "testBrisi prosao!" << std::endl;
	}
}

int main() {

	try
	{
		testStavi();
		testSkini();
		testVrh();
		testBrojElemenata();
		testBrisi();
	}
	catch (const std::exception&)
	{
		std::cout << "Greska!" << std::endl;
	}
	return 0;
}

