#include <iostream>
#include <string>
#include <vector>
using namespace std;
bool czyPalindrom(const string &wyraz) {
    int dlugosc = wyraz.length();
    for (int i = 0; i < dlugosc / 2; i++) {
        if (wyraz[i] != wyraz[dlugosc - i - 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    string wyraz;
    cout << "Podaj wyraz do sprawdzenia: ";
    cin >> wyraz;

    if (czyPalindrom(wyraz)) {
        cout << "Wyraz '" << wyraz << "' jest palindromem." << endl;
    } else {
        cout << "Wyraz '" << wyraz << "' nie jest palindromem." << endl;
    }

    return 0;
}
