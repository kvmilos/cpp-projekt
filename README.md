# Projekt zaliczeniowy z przedmiotu Programowanie Obiektowe i C++
 
### Polecenie:
<p align="center">
Matematyka.POiC++.24/25,<br>
Zadanie zaliczeniowe – wyrażenia tekstowe<br>
Wer. 1.01,
</p>

 W tym zadaniu naszym celem jest stworzenie klas, które pozwolą działać na wyrażeniach
 tekstowych, czyli wyrażeniach, których wartościami są napisy (teksty). Chcemy:
 - Tworzyć te wyrażenia (konstruktorami w kodzie programu, nie musimy ich wczytywać z
 wejścia),
 - Wyliczać ich wartość: 
    - bez wartościowania zmiennych (dając napis jako wynik lub wartość null, jeśli bez
 wartościowania nie da się policzyć wartości wyrażenia),
    - przy zadanym wartościowaniu (dając napis jako wynik lub sygnalizując błąd
 wyjątkiem).
 - Wypisywać te wyrażenia na strumień.

 Rozważane przez nas w tym zadaniu wyrażenia tekstowe mogą zawierać:
 - Stałe tekstowe (ujęte w cudzysłów),
 - Zmienne (oznaczone pojedynczymi małymi literami alfabetu łacińskiego),
 - Operatory jednoargumentowe:
    - Zamiany na wielkie litery (^), znaki różne od liter pozostają bez zmiany, ma działać dla 
liter alfabetu angielskiego, dla innych może, ale nie musi.
 Np. ^"c i c++" daje wynik "C I C++".
    - Zamiany na małe litery (_), znaki różne od liter pozostają bez zmiany, ma działać dla 
liter alfabetu angielskiego, dla innych może, ale nie musi.
 Np. _"PanDas" daje wynik "pandas".
    - Liczenia długości napisu (#), wynikiem ma być napis będący tą długością.
 Np. #"C++" daje wynik "3".
    - ... .
 - Operatory dwuargumentowe:
    - Sklejania napisów (&). W wyniku najpierw jest pierwszy, potem drugi argument.
 Np. "C" & "++" daje wynik "C++".
    - Maskowania (\*). Wynikiem jest napis zawierający te znaki z pierwszego argumentu, 
którym odpowiada znak '\*' w drugim argumencie. Jeśli drugi argument jest krótszy niż 
pierwszy, to drugi argument jest powtarzany odpowiednio wiele razy. Jeśli drugi 
argument jest pusty, to wynikiem jest pusty napis.
 Np. "Basic" \* ".\*" daje wynik "ai".
    - Przeplotu (@): ustawia na przemian znaki z pierwszego i drugiego napisu, na końcu 
dokleja ogon dłuższego napisu. Pierwszy znak pochodzi z pierwszego napisu (o ile nie 
jest on pusty).
 Np. "C++" @ "Python" daje wynik "CP+y+thon".
    - … .

 Żeby policzyć wartość wyrażenia zawierającego zmienne, musimy znać wartości tych zmiennych.
 Takie przyporządkowanie zmiennym ich wartości nazywamy wartościowaniem. W tym programie
 chcemy móc zadawać wartości zmiennych (p. przykład) oraz chcemy wykrywać (sygnalizować
 zgłaszanymi wyjątkami) niepoprawne odwołania do zmiennych. Niepoprawne odwołanie do
 zmiennej może wynikać z tego, że podano niedozwoloną nazwę (np. '8', '\*' lub 'A'), albo z tego, że
 próbujemy odczytać wartość zmiennej, której wartości nie określono (czyli nie zakładamy, że
 wszystkie zmienne są początkowo zainicjowane). Zadbaj w swoim programie, by prawidłowo
 rozpoznawać i zgłaszać te sytuacje za pomocą wyjątków.

 Uwagi:
 - Przy wypisywaniu wyrażeń na strumień należy zadbać o zachowanie znaczenia wyrażeń,
 wstawiając nawiasy. Przyjmujemy, że operatory jednoargumentowe mają wszystkie taki sam
 priorytet, 
który jest wyższy od (jednego wspólnego) priorytetu operatorów
 dwuargumentowych. Operatory o takim samym priorytecie liczymy od lewej do prawej.
 Można wstawiać nawiasy zawsze dookoła argumentów, nawet jeśli z priorytetów
 operatorów wynikałoby, że takie nawiasy są zbędne, ale będzie za to odjęty jeden punkt.
 - Ponieważ liczba małych liter alfabetu łacińskiego nie jest wielka, to do pamiętania wartości
 zmiennych wystarczy użyć tablic o indeksach od 0 do liczby liter alfabetu łacińskiego - 1
 (jakim wyrażeniem w C++ można policzyć tę liczbę?) i o wartościach będących napisami.
 Jakim wyrażeniem w C++ przejść od nazwy zmiennej (np. 'a') do odpowiadającego jej
 indeksu w tablicy (np. 0)? Wskazówka: typ char jest w C++ typem liczbowym, więc
 pozwala na wykonywanie operacji takich jak +, -, \*, /, itp., może któraś z nich się tu przyda?
 - Można zdefiniować dodatkowe klasy lub metody.
 - Można korzystać z biblioteki standardowej (ale żadne klasy spoza omawianych na
 wykładzie nie są tu konieczne).
 - Należy zadbać o obiektowość rozwiązania (dziedziczenie zdecydowanie będzie dobrym
 pomysłem!). Warto zacząć rozwiązywanie od stworzenia projektu.
 - Należy zadbać o poprawne kopiowanie wyrażeń i zwalnianie pamięci (ogólnie o cztery
 kłopotliwe operacje wymieniane na wykładzie, prosimy o implementowanie lub jawne
 zaznaczanie, gdy nie są potrzebne).

 Życzymy powodzenia!
