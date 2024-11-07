#include <iostream>

/*
Zadatak 1.
Koristeći funkciju fib_petlja, napraviti rekurzivnu verziju fib2_0(int n) koja ima O(n) vrijeme
izvršenja i koristi repnu rekurziju. Dozvoljeno je dodati nove parametre u prototip pod uslovom da
imaju default vrijednost.
*/

/*
int fib_petlja(int n) {
    if (n <= 1) return n; // 0 i 1
    int pretprosli(0), prosli(1), rezultat;
    for (int i(2); i <= n; i++) {
        rezultat = pretprosli + prosli;
        pretprosli = prosli;
        prosli = rezultat;
    }
    return rezultat;
}*/

int fib2_0(int n,int a = 0, int b=1) 
{
	if (n == 0) return a;
	if (n == 1) return b;
	int rez = a + b;
	a = b;
	b = rez;
    return fib2_0(n - 1, a, b);
}

int fib_petlja(int n) {
    if (n <= 1) return n; // 0 i 1
    return fib2_0(n-1);
}


/*
Zadatak 2.
Napisati funkciju int nzd(int x, int y) koja računa najveći zajednički djelilac (NZD) dva broja
koristeći Euklidov algoritam.
*/

int nzd(int x, int y)
{
   if(y==0) return x;
   return nzd(y, x % y);
}



// MAIN //

int main()
{
    std::cout << nzd(4840, 1188) << std::endl; //44 TACNO
    std::cout << fib_petlja(18); //1597 TACNO
    return 0;
}

/*Zaključak: rekurziju treba koristiti tamo gdje ona doprinosi boljoj čitljivosti koda, ali vodeći računa
o performansama rekurzije i mogućem eksponencijalnom rastu kompleksnosti kada funkcija više
puta poziva samu sebe. Pokušati koristiti repnu-rekurziju kad god je to moguće.*/