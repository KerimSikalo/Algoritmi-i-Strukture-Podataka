#include <iostream>
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

	DvostraniRed(const Red& r)
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

	DvostraniRed& operator=(const Red& r) {

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

	void staviNaCelo(const Tip& el)
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



int main() 
{
	return 0;
}