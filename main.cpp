#include <iostream>
#include <string>


using namespace std;

class ZlaNazwaZmiennej : public exception {
public:
    const char* what() const noexcept override { return "Niepoprawna nazwa zmiennej"; }
};

class BrakWartosciZmiennej : public exception {
public:
    const char* what() const noexcept override { return "Brak wartosci dla zmiennej"; }
};

class Wyrazenie {
    private:
        int priorytet;
    public:
        Wyrazenie(int p) : priorytet(p) {}
        int get_priorytet() {return priorytet;}
        virtual ~Wyrazenie() = default;
        virtual string wylicz() = 0;
        virtual string wylicz(string[26]) = 0;
        virtual void wypisz() = 0;
        virtual Wyrazenie* kopia() = 0;
};

class Stala : public Wyrazenie {
    private:
        string tekst;
    public:
        explicit Stala(string t) : Wyrazenie(3), tekst(t) {}
        string wylicz() override {return  tekst;}
        string wylicz(string[26]) override {return tekst;}
        void wypisz() override {cout << "\"" << tekst << "\"";}
        Wyrazenie* kopia() override {return new Stala(tekst);}
};

class Zmienna : public Wyrazenie {
    private:
        char symbol;
    public:
        explicit Zmienna(char s) : Wyrazenie(3), symbol(s) {if (s < 'a' || s > 'z') throw ZlaNazwaZmiennej();}
        string wylicz() override {return "";}
        string wylicz(string wartosci[26]) override {
            int idx = symbol - 'a';
            if (wartosci[idx] == "") throw BrakWartosciZmiennej();
            return wartosci[idx];
        }
        void wypisz() override {cout << symbol;}
        Wyrazenie* kopia() override {return new Zmienna(symbol);}
};

class OperatorJednoargumentowy : public Wyrazenie {
    protected:
        Wyrazenie* arg;
    public:
        OperatorJednoargumentowy(Wyrazenie* a, int p = 2) : Wyrazenie(p), arg(a) {}
        ~OperatorJednoargumentowy() override {};
        virtual void wypisz_operator() = 0;
        void wypisz() override {
            wypisz_operator();
            if (arg->get_priorytet() < this->get_priorytet()) {
                cout << "(";
                arg->wypisz();
                cout << ")";
            } else arg->wypisz();
        }
};

class OperatorDwuargumentowy : public Wyrazenie {
    protected:
        Wyrazenie* lhs;
        Wyrazenie* rhs;
    public:
        OperatorDwuargumentowy(Wyrazenie* l, Wyrazenie* r, int p = 1) : Wyrazenie(p), lhs(l), rhs(r) {}
        ~OperatorDwuargumentowy() override {};
        virtual void wypisz_operator() = 0;
        void wypisz() override {
            if (lhs->get_priorytet() < this->get_priorytet()) {
                cout << "(";
                lhs->wypisz();
                cout << ")";
            } else lhs->wypisz();

            wypisz_operator();

            if (rhs->get_priorytet() < this->get_priorytet()) {
                cout << "(";
                rhs->wypisz();
                cout << ")";
            } else rhs->wypisz();
        }    
};

class ZamienNaWielkie : public OperatorJednoargumentowy {
    public:
        explicit ZamienNaWielkie(Wyrazenie* a) : OperatorJednoargumentowy(a) {}
        ZamienNaWielkie(const ZamienNaWielkie& z) : OperatorJednoargumentowy(z.arg->kopia()) {}
        string wylicz() override {
            string wynik = arg->wylicz();
            for (int i = 0; i < (int)wynik.size(); i++) {
                if (wynik[i] >= 'a' && wynik[i] <= 'z') wynik[i] = wynik[i] - 'a' + 'A';
            }
            return wynik;
        }
        string wylicz(string wartosci[26]) override {
            string wynik = arg->wylicz(wartosci);
            for (int i = 0; i < (int)wynik.size(); i++) {
                if (wynik[i] >= 'a' && wynik[i] <= 'z') wynik[i] = wynik[i] - 'a' + 'A';
            }
            return wynik;
        }
        void wypisz_operator() override { cout << "^"; }

        Wyrazenie* kopia() override {return new ZamienNaWielkie(*this);}
};

class ZamienNaMale : public OperatorJednoargumentowy {
    public:
        explicit ZamienNaMale(Wyrazenie* a) : OperatorJednoargumentowy(a) {}
        ZamienNaMale(const ZamienNaMale& z) : OperatorJednoargumentowy(z.arg->kopia()) {}
        string wylicz() override {
            string wynik = arg->wylicz();
            for (int i = 0; i < (int)wynik.size(); i++) {
                if (wynik[i] >= 'A' && wynik[i] <= 'Z') wynik[i] = wynik[i] - 'A' + 'a';
            }
            return wynik;
        }
        string wylicz(string wartosci[26]) override {
            string wynik = arg->wylicz(wartosci);
            for (int i = 0; i < (int)wynik.size(); i++) {
                if (wynik[i] >= 'A' && wynik[i] <= 'Z') wynik[i] = wynik[i] - 'A' + 'a';
            }
            return wynik;
        }
        void wypisz_operator() override { cout << "_"; }
        Wyrazenie* kopia() override {return new ZamienNaMale(*this);}
};
class Dlugosc : public OperatorJednoargumentowy {
    public:
        explicit Dlugosc(Wyrazenie* a) : OperatorJednoargumentowy(a) {}
        Dlugosc(const Dlugosc& d) : OperatorJednoargumentowy(d.arg->kopia()) {}
        string wylicz() override {
            string wynik = arg->wylicz();
            return to_string(wynik.size());
        }
        string wylicz(string wartosci[26]) override {
            string wynik = arg->wylicz(wartosci);
            return to_string(wynik.size());
        }
        void wypisz_operator() override {cout << "#";}  
        Wyrazenie* kopia() override {return new Dlugosc(*this);}
};

class Odwrocenie : public OperatorJednoargumentowy {
    /*
    Odwraca caly string
    */
    public:
        explicit Odwrocenie(Wyrazenie* a) : OperatorJednoargumentowy(a) {}
        Odwrocenie(const Odwrocenie& o) : OperatorJednoargumentowy(o.arg->kopia()) {}
        string wylicz() override {
            string wynik = arg -> wylicz();
            reverse(wynik.begin(), wynik.end());
            return wynik;
        }
        string wylicz(string wartosci[26]) override {
            string wynik = arg->wylicz(wartosci);
            reverse(wynik.begin(), wynik.end());
            return wynik;
        }
        void wypisz_operator() override {cout << "<";}
        Wyrazenie* kopia() override {return new Odwrocenie(*this);}
};

class Konkatenacja : public OperatorDwuargumentowy {
    public:
        Konkatenacja(Wyrazenie* l, Wyrazenie* r) : OperatorDwuargumentowy(l, r) {}
        Konkatenacja(const Konkatenacja& k) : OperatorDwuargumentowy(k.lhs->kopia(), k.rhs->kopia()) {}
        string wylicz() override {
            string wynik = lhs->wylicz() + rhs->wylicz();
            return wynik;
        }
        string wylicz(string wartosci[26]) override {
            string wynik = lhs->wylicz(wartosci) + rhs->wylicz(wartosci);
            return wynik;
        }
        void wypisz_operator() override {cout << " & ";}
        Wyrazenie* kopia() override {return new Konkatenacja(*this);}
};

class Maskowanie : public OperatorDwuargumentowy {
    public:
        Maskowanie(Wyrazenie* l, Wyrazenie* r) : OperatorDwuargumentowy(l, r) {}
        Maskowanie(const Maskowanie& m) : OperatorDwuargumentowy(m.lhs->kopia(), m.rhs->kopia()) {}
        string wylicz() override {
            string s1 = lhs->wylicz();
            string s2 = rhs->wylicz();
            string wynik = "";
            if (!s2.empty()) {
                for (int i = 0; i < (int)s1.size(); i++) {
                    if (s2[i % s2.size()] == '*') wynik += s1[i];
                }
            }
            return wynik;
        }
        string wylicz(string wartosci[26]) override {
            string s1 = lhs->wylicz(wartosci);
            string s2 = rhs->wylicz(wartosci);
            string wynik = "";
            if (!s2.empty()) {
                for (int i = 0; i < (int)s1.size(); i++) {
                    if (s2[i % s2.size()] == '*') wynik += s1[i];
                }
            }
            return wynik;
        }
        void wypisz_operator() override {cout << " * ";}
        Wyrazenie* kopia() override {return new Maskowanie(*this);}
};

class Przeplot : public OperatorDwuargumentowy {
    public:
        Przeplot(Wyrazenie* l, Wyrazenie* r) : OperatorDwuargumentowy(l, r) {}
        Przeplot(const Przeplot& p) : OperatorDwuargumentowy(p.lhs->kopia(), p.rhs->kopia()) {}
        string wylicz() override {
            string s1 = lhs->wylicz();
            string s2 = rhs->wylicz();
            string wynik = "";
            int i = 0, j = 0;
            bool bierzZLewej = true;
            while (i < (int)s1.size() || j < (int)s2.size()) {
                if (bierzZLewej && i < (int)s1.size()) wynik += s1[i++];
                else if (!bierzZLewej && j < (int)s2.size()) wynik += s2[j++];
                bierzZLewej = !bierzZLewej;
            }
            return wynik;
        }
        string wylicz(string wartosci[26]) override {
            string s1 = lhs->wylicz(wartosci);
            string s2 = rhs->wylicz(wartosci);
            string wynik = "";
            int i = 0, j = 0;
            bool bierzZLewej = true;
            while (i < (int)s1.size() || j < (int)s2.size()) {
                if (bierzZLewej && i < (int)s1.size()) wynik += s1[i++];
                else if (!bierzZLewej && j < (int)s2.size()) wynik += s2[j++];
                bierzZLewej = !bierzZLewej;
            }
            return wynik;
        }
        void wypisz_operator() override {cout << " @ ";}
        Wyrazenie* kopia() override {return new Przeplot(*this);}
};

class SklejOgonami : public OperatorDwuargumentowy {
    /*
    Skleja dwa stringi - pierwszy normalny, drugi odwrocony
    */
    public:
        SklejOgonami(Wyrazenie* l, Wyrazenie* r) : OperatorDwuargumentowy(l, r) {}
        SklejOgonami(const SklejOgonami& s) : OperatorDwuargumentowy(s.lhs->kopia(), s.rhs->kopia()) {}
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
        void wypisz_operator() override {cout << " ~ ";}
        Wyrazenie* kopia() override {return new SklejOgonami(*this);}
};

int main() {
    // Przedstawienie operatorow 

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

    Stala s1("Hello"), s2("World"), s3("C++"), s4("Test"), s5("Case"), s6("a*.*e");
    Zmienna z1('a'), z2('b'), z3('c'), z4('d'), z5('e');

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
    s1.wypisz(); cout << " = " << s1.wylicz() << endl;
    z1.wypisz(); cout << " = " << z1.wylicz(wartosci) << endl;
    z2.wypisz(); cout << " = " << z2.wylicz(wartosci) << endl;

    ZamienNaWielkie upper(&s1);
    ZamienNaMale lower(&s2);
    Dlugosc length(&s3);
    Odwrocenie reverse(&s4);

    cout << "\n\n---TESTY: Operacje jednoargumentowe---\n" << endl;
    upper.wypisz(); cout << " = " << upper.wylicz() << endl;
    lower.wypisz(); cout << " = " << lower.wylicz() << endl;
    length.wypisz(); cout << " = " << length.wylicz() << endl;
    reverse.wypisz(); cout << " = " << reverse.wylicz() << endl;

    Konkatenacja concat(&s1, &s2);
    Maskowanie mask(&s1, &s2);
    Maskowanie mask2(&s1, &s6);
    Przeplot interleave(&s1, &s2);
    SklejOgonami tails(&s1, &s2);

    cout << "\n\n---TESTY: Operacje dwuargumentowe---\n" << endl;
    concat.wypisz(); cout << " = " << concat.wylicz() << endl;
    mask.wypisz(); cout << " = " << mask.wylicz() << endl;
    mask2.wypisz(); cout << " = " << mask2.wylicz(wartosci) << endl;
    interleave.wypisz(); cout << " = " << interleave.wylicz() << endl;
    tails.wypisz(); cout << " = " << tails.wylicz() << endl;

    Konkatenacja nestedConcat(&concat, &upper);
    Przeplot nestedInterleave(&nestedConcat, &lower);
    Maskowanie nestedMask(&nestedInterleave, &reverse);

    cout << "\n\n---TESTY: Zagniezdzone operacje---\n" << endl;
    nestedConcat.wypisz(); cout << " = " << nestedConcat.wylicz(wartosci) << endl;
    nestedInterleave.wypisz(); cout << " = " << nestedInterleave.wylicz(wartosci) << endl;
    nestedMask.wypisz(); cout << " = " << nestedMask.wylicz(wartosci) << endl;

    ZamienNaWielkie upper2(&z3);
    ZamienNaMale lower2(&z4);
    Dlugosc length2(&s5);
    Odwrocenie reverse2(&s2);

    cout << "\n\n---TESTY: Operacje dwuargumentowe ze zmiennymi---\n" << endl;
    upper2.wypisz(); cout << " = " << upper2.wylicz(wartosci) << endl;
    lower2.wypisz(); cout << " = " << lower2.wylicz(wartosci) << endl;
    length2.wypisz(); cout << " = " << length2.wylicz() << endl;
    reverse2.wypisz(); cout << " = " << reverse2.wylicz() << endl;

    Konkatenacja concat2(&upper, &lower);
    Maskowanie mask3(&reverse, &length);
    Przeplot interleave2(&s3, &s4);
    SklejOgonami tails2(&z1, &z2);

    cout << "\n\n---TESTY: Zlozone operacje ze zmiennymi---\n" << endl;
    concat2.wypisz(); cout << " = " << concat2.wylicz(wartosci) << endl;
    mask3.wypisz(); cout << " = " << mask2.wylicz(wartosci) << endl;
    interleave2.wypisz(); cout << " = " << interleave2.wylicz(wartosci) << endl;
    tails2.wypisz(); cout << " = " << tails2.wylicz(wartosci) << endl;

    Konkatenacja complex1(&concat2, &interleave);
    Maskowanie complex2(&complex1, &nestedMask);
    Przeplot complex3(&complex2, &tails2);
    SklejOgonami complex4(&complex3, &reverse2);

    cout << "\n\n---TESTY: Zlozone operacje z zagniezdzeniem---\n" << endl;
    complex1.wypisz(); cout << " = " << complex1.wylicz(wartosci) << endl;
    complex2.wypisz(); cout << " = " << complex2.wylicz(wartosci) << endl;
    complex3.wypisz(); cout << " = " << complex3.wylicz(wartosci) << endl;
    complex4.wypisz(); cout << " = " << complex4.wylicz(wartosci) << endl;

    ZamienNaWielkie zW(&s3);
    Odwrocenie odwr(&z5);
    Konkatenacja konkat(&zW, &odwr);
    konkat.wypisz(); cout << " = " << konkat.wylicz(wartosci) << endl;

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
    expr->wypisz(); cout << " = " << expr->wylicz() << endl;
    delete expr;

    expr2->wypisz(); cout << " = " << expr2->wylicz(wartosci) << endl;
    delete expr2;

    expr3->wypisz(); cout << " = " << expr3->wylicz(wartosci) << endl;
    delete expr3;

    cout << "\n\n---TESTY: Wyjatki---\n" << endl;

    for (char ch : {'8', '#', '$'}) {
        try {
            Zmienna zInvalid(ch);
            cout << "BLAD !!! " << ch << endl;
        } catch (const exception& e) {
            if (string(e.what()) == "Niepoprawna nazwa zmiennej") cout << "Poprawnie wykryto blad dla zmiennej " << ch << endl;
            else cout << "nieok " << e.what() << endl;
        }
    }

    try {
        Zmienna zUnset('f'); // Brak wartosci dla zmiennej 'f'
        cout << "Zmienna f: " << zUnset.wylicz(wartosci) << endl;
    } catch (const exception& e) {
        if (string(e.what()) == "Brak wartosci dla zmiennej") cout << "Poprawnie wykryto brak wartosciowania dla zmiennej 'f'" << endl;
        else cout << "nieok " << e.what() << endl;
    }

    return 0;
}
