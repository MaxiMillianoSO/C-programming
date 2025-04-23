#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
using namespace std;

vector<int> liczZnakiSlowa(const string &sciezka) {
    ifstream plik(sciezka);
    if (!plik.is_open()) return {0, 0, 0};
    int lzn = 0, lbzn = 0, lsl = 0;
    char znak;
    bool wsl = false;
    while (plik.get(znak)) {
        lzn++;
        if (isspace(znak)) {
            lbzn++;
            wsl = false;
        } else {
            if (!wsl) {
                lsl++;
                wsl = true;
            }
        }
    }
    plik.close();
    return {lzn, lbzn, lsl};
}
int main() {
    string sciezka;
    cout << "Podaj sciezke do pliku: ";
    cin >> sciezka;
    vector<int> wyniki = liczZnakiSlowa(sciezka);
    cout << "Liczba wszystkich znakow: " << wyniki[0] << endl;
    cout << "Liczba bialych znakow: " << wyniki[1] << endl;
    cout << "Liczba slow: " << wyniki[2] << endl;
    return 0;
}