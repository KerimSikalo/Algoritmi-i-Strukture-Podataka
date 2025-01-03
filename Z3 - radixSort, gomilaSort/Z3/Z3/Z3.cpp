#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <cmath>
using  namespace std;


int vectorMax(vector<int> v)
{
    int vel = v.size();
    int p = v[0];
    for (int i = 1; i < vel; i++) if (v[i] > p) p = v[i];
    return p;
}


void countSort(vector<int>& v, int exp)
{
    vector<int> iz = v;
    int n = v.size();
    int i = 0;
    int co[10] = { 0 };
    for (i = 0; i < n; i++) co[(v[i] / exp) % 10]++;
    for (i = 1; i < 10; i++) co[i] += co[i - 1];
    for (i = n - 1; i >= 0; i--) 
    {
        iz[co[(v[i] / exp) % 10] - 1] = v[i];
        co[(v[i] / exp) % 10]--;
    }
    for (i = 0; i < n; i++) v[i] = iz[i];
}


void radixSort(vector<int>& a)
{
    int m = vectorMax(a);
    for (int exp = 1; m / exp > 0; exp *= 10) countSort(a, exp);
}


void swapp(vector<int>& a, int i, int j) 
{
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}


int l_dijete(int i) { return 2 * i + 1; }
int d_dijete(int i) { return 2 * i + 2; }
int roditelj(int i) { return (i - 1) / 2; }


void popravi_dolje(vector <int>& v, int i, int vel) 
{
    while (!(i >= vel / 2 && i < vel)) 
    {
        int veci = l_dijete(i);
        int des = d_dijete(i);
        if ((des < vel) && (v[des] > v[veci])) veci = des;
        if (v[i] > v[veci]) return;
        swapp(v, i, veci);
        i = veci;
    }
}


void popravi_gore(vector <int>& v, int i) 
{
    while ((i != 0) && (v[i] > v[roditelj(i)])) 
    {
        swapp(v, i, roditelj(i));
        i = roditelj(i);
    }
}


int izbaciPrvi(vector<int>& a, int& velicina) 
{
    if (a.size() == 0) throw domain_error("nema");
    --velicina;
    swapp(a, 0, velicina);
    if (velicina != 0)popravi_dolje(a, 0, velicina);
    return a[velicina];
}


void stvoriGomilu(vector<int>& a) 
{
    int n = a.size();
    for (int i = floor((n / 2.));i >= 0;i--) popravi_dolje(a, i, n);
}


void umetniUGomilu(vector<int>& a, int umetnuti, int& velicina) 
{
    a.push_back(umetnuti);
    ++velicina;
    popravi_gore(a, velicina - 1);
}


void gomilaSort(vector<int>& a) 
{
    stvoriGomilu(a);
    int n = a.size();
    for (int i = n - 1;i > 0;i--) 
    {
        swapp(a, 0, i);
        popravi_dolje(a, 0, i);
    }
}


int main()
{
    std::cout << "Hello World!\n";
	return 0;
}






