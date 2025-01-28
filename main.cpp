#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class ZlaNazwaZmiennej : public exception {
public:
    const char* what() const noexcept override {
        return "Niepoprawna nazwa zmiennej";
    }
};

class BrakWartosciZmiennej : public exception {
public:
    const char* what() const noexcept override {
        return "Brak wartosci dla zmiennej";
    }
};

class Wyrazenie {
private:
    int priorytet;
public:
    Wyrazenie(int p) : priorytet(p) {}
    Wyrazenie() = delete;
    int get_priorytet() const { return priorytet; }
    virtual ~Wyrazenie() = default;
    virtual string wylicz() = 0;
    virtual string wylicz(string[26]) = 0;
    virtual void wypisz() = 0;
    virtual Wyrazenie* kopia() const = 0;
};

class Stala : public Wyrazenie {
private:
    string tekst;
public:
    explicit Stala(const string& t) : Wyrazenie(3), tekst(t) {}
    Stala() = delete;
    ~Stala() override = default;
    string wylicz() override {
        return tekst;
    }
    string wylicz(string[26]) override {
        return tekst;
    }
    void wypisz() override {
        cout << "\"" << tekst << "\"";
    }
    Wyrazenie* kopia() const override {
        return new Stala(tekst);
    }
};

class Zmienna : public Wyrazenie {
private:
    char symbol;
public:
    explicit Zmienna(char s) : Wyrazenie(3), symbol(s) {
        if (s < 'a' || s > 'z') {
            throw ZlaNazwaZmiennej();
        }
    }
    Zmienna() = delete;
    ~Zmienna() override = default;
    string wylicz() override {
        return "";
    }
    string wylicz(string wartosci[26]) override {
        int idx = symbol - 'a';
        if (wartosci[idx] == "") {
            throw BrakWartosciZmiennej();
        }
        return wartosci[idx];
    }
    void wypisz() override {
        cout << symbol;
    }
    Wyrazenie* kopia() const override {
        return new Zmienna(symbol);
    }
};

class OperatorJednoargumentowy : public Wyrazenie {
protected:
    Wyrazenie* arg;
public:
    OperatorJednoargumentowy(Wyrazenie* a, int p=2)
        : Wyrazenie(p), arg(a->kopia()) {}
    OperatorJednoargumentowy() = delete;
    ~OperatorJednoargumentowy() override {
        delete arg;
    }
    virtual void wypisz_operator() = 0;
    void wypisz() override {
        wypisz_operator();
        if (arg->get_priorytet() < this->get_priorytet()) {
            cout << "(";
            arg->wypisz();
            cout << ")";
        } else {
            arg->wypisz();
        }
    }
};

class OperatorDwuargumentowy : public Wyrazenie {
protected:
    Wyrazenie* lhs;
    Wyrazenie* rhs;
public:
    OperatorDwuargumentowy(const Wyrazenie* l, const Wyrazenie* r, int p=1)
        : Wyrazenie(p), lhs(l->kopia()), rhs(r->kopia()) {}
    OperatorDwuargumentowy() = delete;
    ~OperatorDwuargumentowy() override {
        delete lhs;
        delete rhs;
    }
    virtual void wypisz_operator() = 0;
    void wypisz() override {
        if (lhs->get_priorytet() < this->get_priorytet()) {
            cout << "(";
            lhs->wypisz();
            cout << ")";
        } else {
            lhs->wypisz();
        }

        wypisz_operator();

        if (rhs->get_priorytet() < this->get_priorytet()) {
            cout << "(";
            rhs->wypisz();
            cout << ")";
        } else {
            rhs->wypisz();
        }
    }
};

class ZamienNaWielkie : public OperatorJednoargumentowy {
public:
    explicit ZamienNaWielkie(Wyrazenie* a) : OperatorJednoargumentowy(a) {}
    ZamienNaWielkie() = delete;
    ~ZamienNaWielkie() override = default;
    ZamienNaWielkie(const ZamienNaWielkie& z)
        : OperatorJednoargumentowy(z.arg->kopia()) {}
    string wylicz() override {
        string wynik = arg->wylicz();
        for (int i=0; i<(int)wynik.size(); i++) {
            if (wynik[i] >= 'a' && wynik[i] <= 'z') {
                wynik[i] = wynik[i] - 'a' + 'A';
            }
        }
        return wynik;
    }
    string wylicz(string wartosci[26]) override {
        string wynik = arg->wylicz(wartosci);
        for (int i=0; i<(int)wynik.size(); i++) {
            if (wynik[i] >= 'a' && wynik[i] <= 'z') {
                wynik[i] = wynik[i] - 'a' + 'A';
            }
        }
        return wynik;
    }
    void wypisz_operator() override {
        cout << "^";
    }
    Wyrazenie* kopia() const override {
        return new ZamienNaWielkie(*this);
    }
};

class ZamienNaMale : public OperatorJednoargumentowy {
public:
    explicit ZamienNaMale(Wyrazenie* a) : OperatorJednoargumentowy(a) {}
    ZamienNaMale() = delete;
    ~ZamienNaMale() override = default;

    ZamienNaMale(const ZamienNaMale& z)
        : OperatorJednoargumentowy(z.arg->kopia()) {}
    string wylicz() override {
        string wynik = arg->wylicz();
        for (int i=0; i<(int)wynik.size(); i++) {
            if (wynik[i] >= 'A' && wynik[i] <= 'Z') {
                wynik[i] = wynik[i] - 'A' + 'a';
            }
        }
        return wynik;
    }
    string wylicz(string wartosci[26]) override {
        string wynik = arg->wylicz(wartosci);
        for (int i=0; i<(int)wynik.size(); i++) {
            if (wynik[i] >= 'A' && wynik[i] <= 'Z') {
                wynik[i] = wynik[i] - 'A' + 'a';
            }
        }
        return wynik;
    }
    void wypisz_operator() override {
        cout << "_";
    }
    Wyrazenie* kopia() const override {
        return new ZamienNaMale(*this);
    }
};

class Dlugosc : public OperatorJednoargumentowy {
public:
    explicit Dlugosc(Wyrazenie* a) : OperatorJednoargumentowy(a) {}
    Dlugosc() = delete;
    ~Dlugosc() override = default;

    Dlugosc(const Dlugosc& d)
        : OperatorJednoargumentowy(d.arg->kopia()) {}
    string wylicz() override {
        string wynik = arg->wylicz();
        return to_string(wynik.size());
    }
    string wylicz(string wartosci[26]) override {
        string wynik = arg->wylicz(wartosci);
        return to_string(wynik.size());
    }
    void wypisz_operator() override {
        cout << "#";
    }
    Wyrazenie* kopia() const override {
        return new Dlugosc(*this);
    }
};

class Odwrocenie : public OperatorJednoargumentowy {
public:
    explicit Odwrocenie(Wyrazenie* a) : OperatorJednoargumentowy(a) {}
    Odwrocenie() = delete;
    ~Odwrocenie() override = default;

    Odwrocenie(const Odwrocenie& o)
        : OperatorJednoargumentowy(o.arg->kopia()) {}
    string wylicz() override {
        string wynik = arg->wylicz();
        reverse(wynik.begin(), wynik.end());
        return wynik;
    }
    string wylicz(string wartosci[26]) override {
        string wynik = arg->wylicz(wartosci);
        reverse(wynik.begin(), wynik.end());
        return wynik;
    }
    void wypisz_operator() override {
        cout << "<";
    }
    Wyrazenie* kopia() const override {
        return new Odwrocenie(*this);
    }
};

class Konkatenacja : public OperatorDwuargumentowy {
public:
    Konkatenacja(const Wyrazenie* l, const Wyrazenie* r) : OperatorDwuargumentowy(l,r) {}
    Konkatenacja() = delete;
    ~Konkatenacja() override = default;

    Konkatenacja(const Konkatenacja& k)
        : OperatorDwuargumentowy(k.lhs->kopia(), k.rhs->kopia()) {}
    string wylicz() override {
        return lhs->wylicz() + rhs->wylicz();
    }
    string wylicz(string wartosci[26]) override {
        return lhs->wylicz(wartosci) + rhs->wylicz(wartosci);
    }
    void wypisz_operator() override {
        cout << " & ";
    }
    Wyrazenie* kopia() const override {
        return new Konkatenacja(*this);
    }
};

class Maskowanie : public OperatorDwuargumentowy {
public:
    Maskowanie(const Wyrazenie* l, const Wyrazenie* r) : OperatorDwuargumentowy(l,r) {}
    Maskowanie() = delete;
    ~Maskowanie() override = default;

    Maskowanie(const Maskowanie& m)
        : OperatorDwuargumentowy(m.lhs->kopia(), m.rhs->kopia()) {}
    string wylicz() override {
        string s1 = lhs->wylicz();
        string s2 = rhs->wylicz();
        string wynik;
        if (!s2.empty()) {
            for (int i=0; i<(int)s1.size(); i++) {
                if (s2[i % s2.size()] == '*') {
                    wynik += s1[i];
                }
            }
        }
        return wynik;
    }
    string wylicz(string wartosci[26]) override {
        string s1 = lhs->wylicz(wartosci);
        string s2 = rhs->wylicz(wartosci);
        string wynik;
        if (!s2.empty()) {
            for (int i=0; i<(int)s1.size(); i++) {
                if (s2[i % s2.size()] == '*') {
                    wynik += s1[i];
                }
            }
        }
        return wynik;
    }
    void wypisz_operator() override {
        cout << " * ";
    }
    Wyrazenie* kopia() const override {
        return new Maskowanie(*this);
    }
};

class Przeplot : public OperatorDwuargumentowy {
public:
    Przeplot(const Wyrazenie* l, const Wyrazenie* r) : OperatorDwuargumentowy(l,r) {}
    Przeplot() = delete;
    ~Przeplot() override = default;

    Przeplot(const Przeplot& p)
        : OperatorDwuargumentowy(p.lhs->kopia(), p.rhs->kopia()) {}
    string wylicz() override {
        string s1 = lhs->wylicz();
        string s2 = rhs->wylicz();
        string wynik;
        int i=0, j=0;
        bool bierzZLewej = true;
        while (i<(int)s1.size() || j<(int)s2.size()) {
            if (bierzZLewej && i<(int)s1.size()) wynik += s1[i++];
            else if (!bierzZLewej && j<(int)s2.size()) wynik += s2[j++];
            bierzZLewej = !bierzZLewej;
        }
        return wynik;
    }
    string wylicz(string wartosci[26]) override {
        string s1 = lhs->wylicz(wartosci);
        string s2 = rhs->wylicz(wartosci);
        string wynik;
        int i=0, j=0;
        bool bierzZLewej = true;
        while (i<(int)s1.size() || j<(int)s2.size()) {
            if (bierzZLewej && i<(int)s1.size()) wynik += s1[i++];
            else if (!bierzZLewej && j<(int)s2.size()) wynik += s2[j++];
            bierzZLewej = !bierzZLewej;
        }
        return wynik;
    }
    void wypisz_operator() override {
        cout << " @ ";
    }
    Wyrazenie* kopia() const override {
        return new Przeplot(*this);
    }
};

class SklejOgonami : public OperatorDwuargumentowy {
public:
    SklejOgonami(const Wyrazenie* l, const Wyrazenie* r) : OperatorDwuargumentowy(l,r) {}
    SklejOgonami() = delete;
    ~SklejOgonami() override = default;

    SklejOgonami(const SklejOgonami& s)
        : OperatorDwuargumentowy(s.lhs->kopia(), s.rhs->kopia()) {}
    string wylicz() override {
        string s1 = lhs->wylicz();
        string s2 = rhs->wylicz();
        reverse(s2.begin(), s2.end());
        return s1 + s2;
    }
    string wylicz(string wartosci[26]) override {
        string s1 = lhs->wylicz(wartosci);
        string s2 = rhs->wylicz(wartosci);
        reverse(s2.begin(), s2.end());
        return s1 + s2;
    }
    void wypisz_operator() override {
        cout << " ~ ";
    }
    Wyrazenie* kopia() const override {
        return new SklejOgonami(*this);
    }
};

int main() {
    cout << "Operatory jednoargumentowe:\n" << endl;
    cout << "^ - zamiana liter na wielkie" << endl;
    cout << "_ - zamiana liter na male" << endl;
    cout << "# - liczenie dlugosci napisu" << endl;
    cout << "< - odwrocenie napisu" << endl;
    cout << "\nOperatory dwuargumentowe:\n" << endl;
    cout << "& - sklejanie napisow" << endl;
    cout << "* - maskowanie" << endl;
    cout << "@ - przeplot" << endl;
    cout << "~ - sklejenie napisow koncami (pierwszy napis tak jak byl, drugi doklejony odwrocony)" << endl;
    
    // Testy

    Wyrazenie* s1 = new Stala("Hello");
    Wyrazenie* s2 = new Stala("World");
    Wyrazenie* s3 = new Stala("C++");
    Wyrazenie* s4 = new Stala("Test");
    Wyrazenie* s5 = new Stala("Case");
    Wyrazenie* s6 = new Stala("a*.*e");
    Wyrazenie* z1 = new Zmienna('a');
    Wyrazenie* z2 = new Zmienna('b');
    Wyrazenie* z3 = new Zmienna('c');
    Wyrazenie* z4 = new Zmienna('d');
    Wyrazenie* z5 = new Zmienna('e');

    string wartosci[26] = {""};
    wartosci['a' - 'a'] = "Dynamic";
    wartosci['b' - 'a'] = "Programming";
    wartosci['c' - 'a'] = "Language";
    wartosci['d' - 'a'] = "is";
    wartosci['e' - 'a'] = "fun";
    
    cout << "\n\n---TESTY: Stale i zmienne---\n" << endl;
    cout << "Nadawanie wartosciowan zmiennym: " << endl;
    cout << "'a' = " << wartosci['a' - 'a'] << endl;
    cout << "'b' = " << wartosci['b' - 'a'] << endl;
    cout << "c' = " << wartosci['c' - 'a'] << endl;
    cout << "'d' = " << wartosci['d' - 'a'] << endl;
    cout << "'e' = " << wartosci['e' - 'a'] << endl << endl;

    s1->wypisz(); cout << " = " << s1->wylicz() << endl;
    z1->wypisz(); cout << " = " << z1->wylicz(wartosci) << endl;
    z2->wypisz(); cout << " = " << z2->wylicz(wartosci) << endl;

    Wyrazenie* upper = new ZamienNaWielkie(s1);
    Wyrazenie* lower = new ZamienNaMale(s2);
    Wyrazenie* length = new Dlugosc(s3);
    Wyrazenie* reverse = new Odwrocenie(s4);

    cout << "\n\n---TESTY: Operacje jednoargumentowe---\n" << endl;
    upper->wypisz();   cout << " = " << upper->wylicz() << endl;
    lower->wypisz();   cout << " = " << lower->wylicz() << endl;
    length->wypisz();  cout << " = " << length->wylicz() << endl;
    reverse->wypisz(); cout << " = " << reverse->wylicz() << endl;

    Wyrazenie* concat = new Konkatenacja(s1, s2);
    Wyrazenie* mask = new Maskowanie(s1, s2);
    Wyrazenie* mask2 = new Maskowanie(s1, s6);
    Wyrazenie* interleave = new Przeplot(s1, s2);
    Wyrazenie* tails = new SklejOgonami(s1, s2);

    cout << "\n\n---TESTY: Operacje dwuargumentowe---\n" << endl;
    concat->wypisz();    cout << " = " << concat->wylicz() << endl;
    mask->wypisz();      cout << " = " << mask->wylicz() << endl;
    mask2->wypisz();     cout << " = " << mask2->wylicz(wartosci) << endl;
    interleave->wypisz();cout << " = " << interleave->wylicz() << endl;
    tails->wypisz();     cout << " = " << tails->wylicz() << endl;

    Wyrazenie* nestedConcat = new Konkatenacja(concat, upper);
    Wyrazenie* nestedInterleave = new Przeplot(nestedConcat, lower);
    Wyrazenie* nestedMask = new Maskowanie(nestedInterleave, reverse);

    cout << "\n\n---TESTY: Zagniezdzone operacje---\n" << endl;
    nestedConcat->wypisz();      cout << " = " << nestedConcat->wylicz(wartosci) << endl;
    nestedInterleave->wypisz();  cout << " = " << nestedInterleave->wylicz(wartosci) << endl;
    nestedMask->wypisz();        cout << " = " << nestedMask->wylicz(wartosci) << endl;

    Wyrazenie* upper2 = new ZamienNaWielkie(z3);
    Wyrazenie* lower2 = new ZamienNaMale(z4);
    Wyrazenie* length2 = new Dlugosc(s5);
    Wyrazenie* reverse2 = new Odwrocenie(s2);

    cout << "\n\n---TESTY: Operacje dwuargumentowe ze zmiennymi---\n" << endl;
    upper2->wypisz();   cout << " = " << upper2->wylicz(wartosci) << endl;
    lower2->wypisz();   cout << " = " << lower2->wylicz(wartosci) << endl;
    length2->wypisz();  cout << " = " << length2->wylicz() << endl;
    reverse2->wypisz(); cout << " = " << reverse2->wylicz() << endl;

    Wyrazenie* concat2 = new Konkatenacja(upper, lower);
    Wyrazenie* mask3 = new Maskowanie(reverse, length);
    Wyrazenie* interleave2 = new Przeplot(s3, s4);
    Wyrazenie* tails2 = new SklejOgonami(z1, z2);

    cout << "\n\n---TESTY: Zlozone operacje ze zmiennymi---\n" << endl;
    concat2->wypisz();     cout << " = " << concat2->wylicz(wartosci) << endl;
    mask3->wypisz();       cout << " = " << mask2->wylicz(wartosci) << endl;  // oryginalnie: mask2->wylicz(wartosci)
    interleave2->wypisz(); cout << " = " << interleave2->wylicz(wartosci) << endl;
    tails2->wypisz();      cout << " = " << tails2->wylicz(wartosci) << endl;

    Wyrazenie* complex1 = new Konkatenacja(concat2, interleave);
    Wyrazenie* complex2 = new Maskowanie(complex1, nestedMask);
    Wyrazenie* complex3 = new Przeplot(complex2, tails2);
    Wyrazenie* complex4 = new SklejOgonami(complex3, reverse2);

    cout << "\n\n---TESTY: Zlozone operacje z zagniezdzeniem---\n" << endl;
    complex1->wypisz(); cout << " = " << complex1->wylicz(wartosci) << endl;
    complex2->wypisz(); cout << " = " << complex2->wylicz(wartosci) << endl;
    complex3->wypisz(); cout << " = " << complex3->wylicz(wartosci) << endl;
    complex4->wypisz(); cout << " = " << complex4->wylicz(wartosci) << endl;

    Wyrazenie* zW = new ZamienNaWielkie(s3);
    Wyrazenie* odwr = new Odwrocenie(z5);
    Wyrazenie* konkat = new Konkatenacja(zW, odwr);
    konkat->wypisz(); cout << " = " << konkat->wylicz(wartosci) << endl;

    Wyrazenie* expr = new ZamienNaMale(
                           new Odwrocenie(
                               new Konkatenacja(
                                   new Stala("Deep"),
                                   new SklejOgonami(
                                       new ZamienNaWielkie(
                                           new Stala("Nesting")
                                       ),
                                       new Stala("Test")))));

    Wyrazenie* expr2 = new ZamienNaWielkie(
                           new Odwrocenie(
                               new Konkatenacja(
                                   new Stala("Kamil"),
                                   new Zmienna('d'))));

    Wyrazenie* expr3 = new Odwrocenie(
                           new Konkatenacja(
                               new Dlugosc(
                                   new Stala("Test")),
                               new Odwrocenie(
                                   new Przeplot(
                                       new Zmienna('a'),
                                       new Dlugosc(
                                           new Stala("Test"))))));

    cout << "\n\n---TESTY: Nawiasy---\n" << endl;
    expr->wypisz();  cout << " = " << expr->wylicz() << endl;
    delete expr;

    expr2->wypisz(); cout << " = " << expr2->wylicz(wartosci) << endl;
    delete expr2;

    expr3->wypisz(); cout << " = " << expr3->wylicz(wartosci) << endl;

    cout << "\n\n---TESTY: Wyjatki---\n" << endl;
    for (char ch : {'8', '#', '$'}) {
        try {
            Zmienna zInvalid(ch); // to jest lokalny obiekt do testu WYJĄTKU - OK
            cout << "BLAD !!! " << ch << endl;
        }
        catch (const exception& e) {
            if (string(e.what()) == "Niepoprawna nazwa zmiennej")
                cout << "Poprawnie wykryto blad dla zmiennej " << ch << endl;
            else
                cout << "nieok " << e.what() << endl;
        }
    }

    try {
        Zmienna zUnset('f'); // Brak wartosci dla zmiennej 'f'
        cout << "Zmienna f: " << zUnset.wylicz(wartosci) << endl;
    }
    catch (const exception& e) {
        if (string(e.what()) == "Brak wartosci dla zmiennej")
            cout << "Poprawnie wykryto brak wartosciowania dla zmiennej 'f'" << endl;
        else
            cout << "nieok " << e.what() << endl;
    }

    ofstream plik("wynik.txt");
    plik << "Przykladowe testy wyrazen (zapis do pliku):\n\n";

    plik << "[Wywolanie na s1 (Hello)]\n"
         << "s1->wylicz() = " << s1->wylicz() << "\n\n";

    plik << "[Wywolanie na expr3 (zlozone wyrazenie)]\n";
    plik << "expr3->wylicz(wartosci) = " << expr3->wylicz(wartosci) << "\n";
    Wyrazenie* expr3copy = expr3->kopia();

    plik << "expr3copy->wylicz(wartosci) = " << expr3copy->wylicz(wartosci) << "\n";
    delete expr3;
    delete expr3copy;

    plik << "\nKoniec testu do pliku.\n";

    delete upper;      delete lower;      delete length;      delete reverse;
    delete concat;     delete mask;       delete mask2;       delete interleave;
    delete tails;      delete nestedConcat;     delete nestedInterleave;  
    delete nestedMask; delete upper2;     delete lower2;      delete length2;
    delete reverse2;   delete concat2;    delete mask3;       delete interleave2;
    delete tails2;     delete complex1;   delete complex2;    delete complex3;
    delete complex4;   delete zW;         delete odwr;        delete konkat;

    delete s1; delete s2; delete s3; delete s4; delete s5; delete s6;
    delete z1; delete z2; delete z3; delete z4; delete z5;

    return 0;
}