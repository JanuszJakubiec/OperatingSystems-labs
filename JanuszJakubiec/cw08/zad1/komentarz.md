Jak widac w zalaczonym pliku Times.txt, czasy przetwarzania po wybraniu opcji "block" byly duzo krotsze niz po wybraniu opcji "numbers".

Po wybraniu opcji numbers, watki przechodza po calej tablicy co zajmuje im duzo czasu. Jest to praca bezsensowna, gdyz zamieniaja tylko niewielka
 czesc z elementow po ktorych przechodza. Taki podzial pracy moglby miec sens, przy skomplikowanych czasowo operacjach, w porownaniu z ktorymi
 czas przejscia po tablicy jest niewielki. Do funkcji zamieniajacej piksele po wybraniu opcji "numbers" przekazywany jest zbior wartosci, w postaci tablicy, przez co
 aby przeprowadzic operacje zamiany piksela, watek musi najpierw przejsc po zbiorze i sprawdzic czy wartosc znajduje sie w przedziale, co dodatkowo zabiera czas.

 Testy programu przy uzyciu funkcji block daly znakomite rezultaty. Najlepszy wynik zostal osiagniety przy 5-6 watkach. Przy wartosciach wiekszych niz 10, znaczacy
 zaczyna byc czas potrzebny do utworzenia watkow, przez co staje sie wiekszy niz czas potrzebny do zamiany pikseli
