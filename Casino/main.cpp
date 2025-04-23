#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// Klasa bazowa abstrakcyjna dla wszystkich stanowisk
class Gra {
public:
    virtual void play() = 0;
    virtual ~Gra() = default; // wirt destruktor
};

class Gracz {
private:
    string imie;
    int saldo;
    int poczSaldo;
    vector<string> karty;
    int pnkt;

public:
    Gracz(string imiegr) {
        imie = imiegr;
        saldo = 75 + (rand() % 1926);
        poczSaldo = saldo;
        pnkt = 0;
    }

    string dodimie() const {
        return imie;
    }

    int dodsald() const {
        return saldo;
    }

    int doposaldo() const {
        return poczSaldo;
    }

    void ussaldo(int nsaldo) {
        saldo = nsaldo;
    }

    void dodkarte(string karta, bool peirwlos = false) {
        karty.push_back(karta);
        if (karta == "K" || karta == "Q" || karta == "J") {
            pnkt += 10;
        } else if (karta == "A") {
            if (peirwlos) {
                pnkt += 11;
            } else {
                int wartosc;
                while (true) {
                    cout << "Masz asa! Wprowadz 1 lub 11: ";
                    cin >> wartosc;
                    if (cin.fail() || (wartosc != 1 && wartosc != 11)) {
                        cout << "Niewlasciwa wartosc. Sprobuj ponownie!" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    } else {
                        pnkt += wartosc;
                        break;
                    }
                }
            }
        } else {
            pnkt += stoi(karta);
        }
    }

    void dostPierwAsa() {
        for (auto &karta : karty) {
            if (karta == "A") {
                int wartosc;
                while (true) {
                    cout << "Masz asa! Wprowadz 1 lub 11: ";
                    cin >> wartosc;
                    if (cin.fail() || (wartosc != 1 && wartosc != 11)) {
                        cout << "Niewlasciwa wartosc. Sprobuj ponownie!" << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    } else {
                        pnkt += wartosc - 11;
                        break;
                    }
                }
                break;
            }
        }
    }

    vector<string> dodkarty() const {
        return karty;
    }

    int dodpnkt() const {
        return pnkt;
    }

    // resetuje stan kart i punktów gracza na początku każdej rundy
    void reset() {
        karty.clear();
        pnkt = 0;
    }
};

// blackjack po klasie Game
class Blackjack : public Gra {
private:
    Gracz& gracz;
    void pnktkoncowe(int pnktgracz, int pnktkrup) {
        cout << "Punkty gracza: " << pnktgracz << endl;
        cout << "Punkty krupiera: " << pnktkrup << endl;
        cout << "========================" << endl;
    }

    string dodloskarte() {
        int karta = rand() % 13 + 1;
        if (karta == 1) return "A";
        else if (karta == 11) return "J";
        else if (karta == 12) return "Q";
        else if (karta == 13) return "K";
        else return to_string(karta);
    }

public:
    Blackjack(Gracz& p) : gracz(p) {} // implementuje metode 'play'

    void play() override {
        gracz.reset();
        int zakl;

        cout << "Twoje saldo: $" << gracz.dodsald() << endl;
        cout << "Podaj stawke: ";
        while (true) {
            cin >> zakl;
            if (cin.fail() || zakl < 0 || zakl > gracz.dodsald()) {
                cout << "Niewlasciwa wartosc stawki. Sprobuj ponownie!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Podaj stawke: ";
            } else {
                break;
            }
        }

        gracz.ussaldo(gracz.dodsald() - zakl);

        gracz.dodkarte(dodloskarte(), true);  // pierwsza karta
        gracz.dodkarte(dodloskarte(), true);  // druga karta

        cout << "Twoje karty:" << endl;
        for (const auto &karta : gracz.dodkarty()) {
            cout << "Karta: " << karta << endl;
        }
        gracz.dostPierwAsa();  // dostosuj pierwszy As, jeśli jest obecny
        cout << "Suma punktow: " << gracz.dodpnkt() << endl;

        int pnktkrup = 4 + (rand() % 18); // punkty krupiera od 4 do 21

        // gracz
        while (gracz.dodpnkt() < 21) {
            int wybor;
            while (true) {
                cout << "Co dalej?\n1. Wez karte\n2. Nie odbierac karty\n";
                cin >> wybor;
                if (cin.fail() || (wybor != 1 && wybor != 2)) {
                    cout << "Niewlasciwa opcja. Sprobuj ponownie!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    break;
                }
            }

            if (wybor == 1) {
                gracz.dodkarte(dodloskarte());
                cout << "Twoje karty:" << endl;
                for (const auto &karta : gracz.dodkarty()) {
                    cout << "Karta: " << karta << endl;
                }
                cout << "Suma punktow: " << gracz.dodpnkt() << endl;

                if (gracz.dodpnkt() == 21) {
                    cout << "Wygrales! Masz dokladnie 21 punkt." << endl;
                    gracz.ussaldo(gracz.dodsald() + zakl * 2);
                    pnktkoncowe(gracz.dodpnkt(), pnktkrup);
                    return;
                }
            } else {
                break;
            }
        }

        // if dla wina krupiera jeśli gracz ma wiecej niż 21 punkt
        if (gracz.dodpnkt() > 21) {
            cout << "Przegrales. Masz wiecej niz 21 punkt." << endl;
            pnktkoncowe(gracz.dodpnkt(), pnktkrup);
            return;
        }

        // krupier
        while (pnktkrup < 17) {
            int dodatkowepnkt = 2 + (rand() % 10);
            pnktkrup += dodatkowepnkt;
            cout << "Krupier wzial karte. Jego punkty wzrosly." << endl;
        }

        cout << "Suma punktow krupiera: " << pnktkrup << endl;

        // winer
        if (pnktkrup > 21) {
            cout << "Wygrales! Krupier ma wiecej niz 21 punkt." << endl;
            gracz.ussaldo(gracz.dodsald() + zakl * 2);
        } else if (gracz.dodpnkt() > pnktkrup) {
            cout << "Wygrales! Masz wiecej punktow niz krupier." << endl;
            gracz.ussaldo(gracz.dodsald() + zakl * 2);
        } else if (gracz.dodpnkt() == pnktkrup) {
            cout << "Remis! Masz tyle samo punktow co krupier." << endl;
            gracz.ussaldo(gracz.dodsald() + zakl);
        } else {
            cout << "Krupier wygral. Ma wiecej punktow niz ty." << endl;
        }
        pnktkoncowe(gracz.dodpnkt(), pnktkrup);
    }
};

// jednoręki bandyta
class bandyt : public Gra {
private:
    Gracz& gracz;

public:
    bandyt(Gracz& p) : gracz(p) {}

    void play() override {
        int zakl;

        cout << "Twoje saldo: $" << gracz.dodsald() << endl;
        cout << "Podaj stawke: ";
        while (true) {
            cin >> zakl;
            if (cin.fail() || zakl < 0 || zakl > gracz.dodsald()) {
                cout << "Niewlasciwa wartosc stawki. Sprobuj ponownie!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Podaj stawke: ";
            } else {
                break;
            }
        }

        gracz.ussaldo(gracz.dodsald() - zakl);

        // generowanie trzech liczb w przedziale od 0 do 7
        int wynik[3];
        for (int i = 0; i < 3; ++i) {
            wynik[i] = rand() % 8;
        }

        cout << "Wynik: [" << wynik[0] << "] [" << wynik[1] << "] [" << wynik[2] << "]" << endl;

        // sprawdzanie wyników
        int wyplata = 0;
        if (wynik[0] == wynik[1] && wynik[1] == wynik[2]) {
            if (wynik[0] == 7) {
                wyplata = zakl * 20; // trzy siódemki
            } else {
                wyplata = zakl * 15; // trzy identyczne liczby 0-6
            }
        } else if (wynik[0] == wynik[1] || wynik[1] == wynik[2] || wynik[0] == wynik[2]) {
            if (wynik[0] == 7 || wynik[1] == 7 || wynik[2] == 7) {
                wyplata = zakl * 10; // dwie siódemki
            } else {
                wyplata = zakl * 2; // dwie identyczne liczby 0-6
            }
        }

        if (wyplata > 0) {
            cout << "Wygrales! Zarobiles $" << wyplata << endl;
            gracz.ussaldo(gracz.dodsald() + wyplata);
        } else {
            cout << "Przegrales." << endl;
        }

        cout << "Twoje saldo: $" << gracz.dodsald() << endl;
    }
};

// ruletka
class ruletka : public Gra {
private:
    Gracz& gracz;
    vector<pair<int, int>> mzakl;
    vector<int> jlzakl;
    const vector<int> czarnlb = {2, 4, 6, 8, 10, 11, 13, 15, 17, 20, 22, 24, 26, 28, 29, 31, 33, 35};

    bool czarne(int liczba) {
        return find(czarnlb.begin(), czarnlb.end(), liczba) != czarnlb.end();
    }

    void pwzakl() {
        int typzakl;
        int ilzakl;

        cout << "Twoje saldo: $" << gracz.dodsald() << endl;
        cout << "Podaj stawke: ";
        while (true) {
            cin >> ilzakl;
            if (cin.fail() || ilzakl < 0 || ilzakl > gracz.dodsald()) {
                cout << "Niewlasciwa wartosc stawki. Sprobuj ponownie!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Podaj stawke: ";
            } else {
                break;
            }
        }

        while (true) {
            cout << "Jak chcesz grac?\n";
            cout << "1. Postawic na pojedynczy numer\n";
            cout << "2. Postawic na wszystkie liczby parzyste\n";
            cout << "3. Postawic na wszystkie nieparzyste liczby\n";
            cout << "4. Postawic na wszystkie czarne\n";
            cout << "5. Postawic na wszystkie czerwone\n";
            cout << "6. Zaakceptuj zaklad\n";
            cin >> typzakl;
            if (cin.fail() || typzakl < 1 || typzakl > 6) {
                cout << "Niewlasciwa opcja. Sprobuj ponownie!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                break;
            }
        }

        if (typzakl == 1) {
            int liczba;
            while (true) {
                cout << "Na ktory numer chcesz postawic (0-36): ";
                cin >> liczba;
                if (cin.fail() || liczba < 0 || liczba > 36) {
                    cout << "Niewlasciwy numer. Sprobuj ponownie!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    jlzakl.push_back(liczba);
                    break;
                }
            }
        }

        if (typzakl >= 1 && typzakl <= 5) {
            mzakl.push_back(make_pair(typzakl, ilzakl));
            gracz.ussaldo(gracz.dodsald() - ilzakl);
        }
    }

public:
    ruletka(Gracz& p) : gracz(p) {}

    void play() override {
        while (true) {
            pwzakl();
            int wybor;
            while (true) {
                cout << "Czy chcesz postawic kolejny zaklad?\n1. Tak\n2. Nie\n";
                cin >> wybor;
                if (cin.fail() || (wybor != 1 && wybor != 2)) {
                    cout << "Niewlasciwa opcja. Sprobuj ponownie!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    break;
                }
            }
            if (wybor == 2) break;
        }

        // wynik ruletki
        int wynik = rand() % 37;
        string kolor;
        if (wynik == 0) {
            kolor = "zielone";
        } else {
            kolor = czarne(wynik) ? "czarne" : "czerwone";
        }
        string parzyste = (wynik == 0) ? "" : (wynik % 2 == 0) ? "parzyste" : "nieparzyste";

        cout << "Wynik: " << wynik << ", " << parzyste << ", " << kolor << endl;

        int calwynik = 0;

        // sprawdzanie zakładów
        for (const auto& bet : mzakl) {
            int typzakl = bet.first;
            int ilzakl = bet.second;
            int wyplata = 0;

            if (typzakl == 1) {
                for (int number : jlzakl) {
                    if (number == wynik) {
                        wyplata += (number == 0) ? ilzakl * 50 : ilzakl * 35;
                    }
                }
            } else if (typzakl == 2 && wynik != 0 && wynik % 2 == 0) {
                wyplata = ilzakl * 2; // na  parzystych
            } else if (typzakl == 3 && wynik != 0 && wynik % 2 != 0) {
                wyplata = ilzakl * 2; // na nieparzystych
            } else if (typzakl == 4 && czarne(wynik)) {
                wyplata = ilzakl * 2; // na czarnych
            } else if (typzakl == 5 && !czarne(wynik) && wynik != 0) {
                wyplata = ilzakl * 2; // na czerwonych
            }

            calwynik += wyplata;
        }

        if (calwynik > 0) {
            cout << "Twoja wygrana: $" << calwynik << endl;
            gracz.ussaldo(gracz.dodsald() + calwynik);
        } else {
            cout << "Twoja wygrana: $0" << endl;
        }

        cout << "Twoje saldo: $" << gracz.dodsald() << endl;
    }
};

class kasyno {
private:
    const string filename = "top100.txt";

    struct PlayerRecord {
        string imie;
        int poczSaldo;
        int aktsaldo;

        bool operator<(const PlayerRecord& other) const {
            return aktsaldo > other.aktsaldo; // sortowanie
        }
    };

    void zapgracz(const Gracz& gracz) {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << gracz.dodimie() << "," << gracz.doposaldo() << "," << gracz.dodsald() << endl;
            file.close();
        }
    }

    void poklist() {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            vector<PlayerRecord> records;

            while (getline(file, line)) {
                stringstream ss(line);
                string imie;
                int poczSaldo, aktsaldo;
                char comma;

                getline(ss, imie, ',');
                ss >> poczSaldo >> comma >> aktsaldo;

                records.push_back({imie, poczSaldo, aktsaldo});
            }
            file.close();

            // sortowanie według aktualnego salda
            sort(records.begin(), records.end());

            int rank = 1;
            for (const auto& record : records) {
                if (rank > 100) break;
                cout << rank << ". " << record.imie << ", saldo poczatkowe: " << record.poczSaldo
                     << ", aktualne saldo: " << record.aktsaldo << endl;
                rank++;
            }
        } else {
            cout << "Nie mozna otworzyc pliku z lista graczy." << endl;
        }
    }

public:
    void start() {   // tworzy obiekty klas pochodntch (bl, rul, band) i wywoluje na nich metode 'play'
        cout << "Symulator kasyna. Prosze wpisac imie gracza: ";
        string imiegr;
        getline(cin, imiegr);

        Gracz gracz(imiegr);

        cout << "Prosze bardzo " << gracz.dodimie() << "! Twoje poczatkowe saldo: $" << gracz.dodsald() << endl;
        int wybor;
        while (true) {
            cout << "\nWybierz stanowisko:\n";
            cout << "1. Blackjack z komputerem\n";
            cout << "2. Jednoreki bandyta\n";
            cout << "3. Ruletka\n";
            cout << "4. Lista TOP-100 najlepszych graczy\n";
            cout << "5. Wyjscie\n";
            while (true) {
                cin >> wybor;
                if (cin.fail() || wybor < 1 || wybor > 5) {
                    cout << "Wybrano niewlasciwa opcje, sprobuj ponownie!" << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Wybierz stanowisko:\n";
                    cout << "1. Blackjack z komputerem\n";
                    cout << "2. Jednoreki bandyta\n";
                    cout << "3. Ruletka\n";
                    cout << "4. Lista TOP-100 najlepszych graczy\n";
                    cout << "5. Wyjscie\n";
                } else {
                    break;
                }
            }

            if (wybor == 5) {
                zapgracz(gracz);
                break;
            } else if (wybor == 4) {
                poklist();
            } else if (wybor == 1) {
                Blackjack blackjack(gracz);
                blackjack.play();
            } else if (wybor == 2) {
                bandyt bandyt(gracz);
                bandyt.play();
            } else if (wybor == 3) {
                ruletka ruletka(gracz);
                ruletka.play();
            }
        }
    }
};

int main() {
    srand(time(0));

    kasyno kasyno;
    kasyno.start();

    return 0;
}




