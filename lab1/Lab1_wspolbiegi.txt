ZARZ¥DZANIE PROCESAMI I KOMUNIKACJA PRZEZ WSPÓ£DZIELONE PLIKI W PROGRAMACH W BASH'U

1. Utworzyæ kilka (2 - 3) egzemplarzy skryptów w bash'u:

   while true
   do
      echo ....(jakiœ napis)....
      sleep ....(liczba sekund)....
   done
   exit 0

   Uruchomiæ je wszystkie w tle poleceniem o postaci

      skrypt1 & skrypt2 & skrypt3 &

   nastêpnie po pewnym czasie po kolei je zatrzymaæ: a) przy u¿yciu poleceñ
   wydawanych w innym oknie tekstowym; b) przy u¿yciu poleceñ wydawanych
   w tym samym oknie tekstowym.


2. W zespo³ach 2 - 3-osobowych wypróbowaæ mo¿liwoœæ komunikacji przez wspó³dzielony
   plik: na koncie jednego z cz³onków zespo³u utworzyæ pusty plik, przydzieliæ
   odpowiednie prawa dostêpu (do pliku i do katalogu domowego), nastêpnie zapisywaæ
   i odczytywaæ komunikaty przy u¿yciu poleceñ o postaci:

   echo ....(jakiœ napis).... > plik      # zapisanie komunikatu

   cat plik                               # odczytanie komunikatu

   Sprawdziæ, ¿e je¿eli operator > zast¹pimy operatorem >> , uzyskamy dopisywanie
   komunikatów zamiast nadpisywania.

   Wzbogaciæ rozwi¹zanie powy¿szego zadania przez utworzenie skryptu prostego
   komunikatora tekstowego, który w pêtli nieskoñczonej bêdzie oferowa³
   u¿ytkownikowi menu:

                          0 - odczyt komunikatu
                          1 - zapis komunikatu
                          2 - koniec pracy

   Skrypt powinien byæ wywo³ywany z argumentem równym pe³nej nazwie œcie¿kowej
   pliku komunikacyjnego, np.:

      rozmowa /home/studinf/....

3. Napisaæ skrypt, który wyœwietla PID swojego procesu, czeka kilka sekund,
   nastêpnie rekurencyjnie tworzy dwa nowe procesy wspó³bie¿ne (w tle) i koñczy
   dzia³anie.

   UWAGA: ograniczyæ g³êbokoœæ wywo³añ rekurencyjnych do 5.

4. (PODLEGA ODBIOROWI - CZAS 2 ZAJÊCIA)

   Utworzyæ prost¹ parê programów klient - serwer iteracyjny, komunikuj¹c¹ siê
   przez dwa pliki (dla danych i wyników), dzia³aj¹c¹ na zasadzie ci¹g³ego
   odpytywania plików (w pêtli aktywnego czekania).
                                ----------
    wczytanie                   |  dane  |
    -------->|--------------|-->------------>|--------------|
             |proces klienta|                |proces serwera|
    <--------|--------------|<------------<--|--------------|
    wyœwietlenie                | wyniki |
                                ----------

    Klient pobiera z klawiatury i zapisuje do pliku dane pojedyncz¹ liczbê
    ca³kowit¹. Serwer pobiera dan¹ z pliku, oblicza jak¹œ prost¹ funkcjê
    arytmetyczn¹ (np. niedu¿y wielomian) i wynik zapisuje do pliku wyniki.
    Klient odbiera odpowiedŸ z pliku, wyœwietla i koñczy dzia³anie. Serwer
    dzia³a nadal w pêtli oczekuj¹c na kolejne zg³oszenia.

    UWAGI:

    1) Nie wprowadzaæ ¿adnych "sztucznych opóŸnieñ" (sleep, wyœwietlanie
       tekstów przez serwer itp.).

    2) Pamiêtaæ o natychmiastowym (zaraz po odczytaniu) opró¿nianiu buforów
       po stronie serwera i po stronie klienta.

    3) Uruchamiaæ najpierw serwer, a potem dopiero klienta.
