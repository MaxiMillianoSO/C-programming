#include <iostream>
#include <cmath>

using namespace std;

bool cp(int liczba) {
    if (liczba <= 1) return false;
    if (liczba <= 3) return true;

    if (liczba % 2 == 0 || liczba % 3 == 0) return false;

    for (int i = 5; i * i <= liczba; i += 6) {
        if (liczba % i == 0 || liczba % (i + 2) == 0)
            return false;
    }
    return true;
}

int znnp(int liczba) {
    int offset = 0;
    while (true) {
        if (cp(liczba - offset)) return liczba - offset;
        if (cp(liczba + offset)) return liczba + offset;
        offset++;
    }
}

int main() {
    int liczba;
    cout << "Podaj liczbe do sprawdzenia: ";
    cin >> liczba;
    if (cp(liczba)) {
        cout << "Podana liczba jest liczba pierwsza." << endl;
    } else {
        int najblizszaPierwsza = znnp(liczba);
        cout << "Podana liczba nie jest liczba pierwsza. Najblizsza jej liczba pierwsza to " << najblizszaPierwsza << "." << endl;
    }
    return 0;
}
