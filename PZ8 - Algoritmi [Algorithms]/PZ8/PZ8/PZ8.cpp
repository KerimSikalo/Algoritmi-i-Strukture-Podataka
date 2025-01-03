#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;


template <typename Tip>
void bubble_sort(Tip* niz, int vel)
{
    for (int i = 0; i < vel; i++)
    {
        for (int j = i; j < vel; j++)
        {
            if (niz[i] > niz[j])
            {
                auto temp = niz[i];
                niz[i] = niz[j];
                niz[j] = temp;
            }
        }
    }
}

template <typename Tip>
void selection_sort(Tip* niz, int vel) 
{
    if (niz == nullptr) return;
    for (int i = 0;i < vel;i++) 
    {
        Tip min = niz[i];
        int mini = i;
        for (int j = i + 1;j < vel;j++)
            if (niz[j] < min) { min = niz[j]; mini = j; }
        auto temp = niz[mini];
        niz[mini] = niz[i];
        niz[i] = temp;
    }
}


template <typename Tip>
int Particija(Tip* niz, const int prvi, const int zadnji) 
{
    Tip pivot = niz[prvi];
    int p = prvi + 1;
    while (p <= zadnji) 
    {
        if (niz[p] <= pivot) p++;
        else break;
    }
    for (int i = p + 1; i <= zadnji; i++) 
    {
        if (niz[i] < pivot) 
        {
            auto temp = niz[p];
            niz[p] = niz[i];
            niz[i] = temp;
            p++;
        }
    }
    auto temp = niz[p - 1];
    niz[p - 1] = niz[prvi];
    niz[prvi] = temp;
    return p - 1;
}


template <typename Tip>
void quick_sort(Tip* niz, int vel) 
{
    static int prvi = 0;
    static int zadnji = vel - 1;
    if (prvi < zadnji) 
    {
        int j = Particija(niz, prvi, zadnji);
        auto q = zadnji;
        zadnji = j - 1;
        quick_sort(niz, vel);
        prvi = j + 1; zadnji = q;
        quick_sort(niz, vel);
    }
}


template <typename Tip>
void Merge(Tip* niz, const int l, const int p, const int q, const int u) 
{
    int i(0), j(q - l), k(l);
    Tip* B = new Tip[u - l + 1];
    for (int m = 0; m <= u - l; m++) B[m] = niz[l + m];
    while (i <= p - l && j <= u - l) 
    {
        if (B[i] < B[j]) niz[k] = B[i++];
        else niz[k] = B[j++];
        k++;
    }
    while (i <= p - l) niz[k++] = B[i++];
    while (j <= u - l) niz[k++] = B[j++];
    delete[] B;
}

template <typename Tip>
void merge_sort(Tip* niz, int vel) 
{
    static int l = 0;
    static int u = vel - 1;
    if (u > l) 
    {
        int p = (l + u - 1) / 2;
        int q = p + 1;
        auto z = u;
        auto y = l;
        u = p;
        merge_sort(niz, vel);
        u = z; l = q;
        merge_sort(niz, vel);
        l = y;
        Merge(niz, l, p, q, u);
    }
}


void ucitaj(std::string filename, int*& niz, int& vel) 
{
    int c;
    ifstream ulaz(filename);
    std::vector<int> v;
    while (ulaz >> c) v.push_back(c);
    vel = v.size();
    niz = new int[vel];
    for (int i = 0;i < vel;i++)niz[i] = v[i];
    ulaz.close();
}


void generisi(std::string filename, int vel) 
{
    ofstream ulaz(filename);
    for (int i = 0; i < vel; i++) ulaz.put(char(rand()));
    ulaz.close();
}


void funkcija(std::string filename, int*& niz, int& vel, std::string izlaz)
{
    ucitaj(filename, niz, vel);
    std::cout << "Niz glasi: ";
    for (int i = 0; i < vel; i++) 
    {
        std::cout << niz[i]; 
        if (i != vel - 1) std::cout << " ";
    }
    std::cout << std::endl;

start:
    std::cout << "Za: merge sort - 1 , quick sort - 2 , bubble sort - 3 , selection sort - 4 : ";
    int n;
    std::cin >> n;
    auto start = std::chrono::high_resolution_clock::now();
    switch (n) 
    {
        case 1: merge_sort(niz, vel);break;
        case 2:quick_sort(niz, vel);break;
        case 3:bubble_sort(niz, vel);break;
        case 4:selection_sort(niz, vel);break;
        default: { std::cout << "Unesite ispravnu opciju!" << std::endl; goto start; }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    for (int i = 0; i < vel - 1; i++) if (niz[i] > niz[i + 1]) std::cout << "GRESKA!";
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "Niz jeste sortiran\nTrajanje sortiranja: " << duration.count() << " ms\nNiz sortiran: ";
    for (int i = 0; i < vel; i++) 
    {
        std::cout << niz[i]; 
        if (i != vel - 1) std::cout << " ";
    }
    std::ostringstream os;
    std::ofstream izlazz(izlaz);
    for (int i = 0; i < vel; i++) 
    { 
        os << niz[i]; 
        if (i != vel - 1) os << ", "; 
    }
    std::string s(os.str());
    izlazz << s;
    izlazz.close();
}


int main()
{
    int* niz; 
    int vel;
    funkcija("ulaz.txt", niz, vel, "primjer.txt");
}