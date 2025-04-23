#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int N;
    cout << "Podaj liczbe N: ";
    cin >> N;
    if (N <= 0) return 1;
    int* liczby = new int[N];
    for (int i = 0; i < N; i++) {
        cout << "Podaj " << i + 1 << ". liczbe: ";
        cin >> liczby[i];
        if (liczby[i] < 0 || liczby[i] >= 100) {
            cout << "Liczba musi byc z zakresu [0, 100]." << endl;
            delete[] liczby;
            return 1;
        }
    }
    sort(liczby, liczby + N);
    int polowa = N / 2;
    for (int i = 0; i < polowa; i++) {
        int suma = liczby[i] + liczby[N - 1 - i];
        cout << liczby[i] << " + " << liczby[N - 1 - i] << " = " << suma << endl;
    }
    delete[] liczby;
    return 0;
}