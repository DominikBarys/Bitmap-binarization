;---------------------------------------------------------------------
;
; BINARYZACJA MAPY BITOWEJ
; 11.12.2022 Dominik Bary�
; Informatyka SSI semestr 5 rok akademicki 2022/2023
; v.1.0 - 11.12.2022
; 
; Implementacja algorytmu binaryzacji mapy bitowej.
; Poni�szy fragment asemblera jest odpowiedzialny za obliczenie
; �redniej arytmetycznej ka�dego z pikseli a nast�pnie podj�cie 
; decyzji czy dany piksel ustawi� jako bia�y lub czarny na podstawie
; wprowadzonego przez u�ytkownika progu binaryzacji.
; Algorytm  dzia�a wielow�tkowo wi�c poni�szy fragment
; kodu jest r�wnie� odpowiedzialny za dopilnowanie aby ka�dy z w�tk�w
; pracowa� na wyznaczonym dla niego obszarze pami�ci poprzez odpowiednie
; obliczenie adresu pocz�tkowego tablicy oraz ko�cowego a nast�pnie 
; sprawdzenie czy dany w�tek wyszed� poza sw�j zakres pami�ci
;
;---------------------------------------------------------------------

.code 

MyBinarization proc

	; Porzadek w rejestrach oraz operacje niezb�dne przed wej�ciem w g��wn� p�tle prodecury "mainloop"

	; Zmiana rejestru adresu tablicy
	mov RBX, RCX 

	; Przeniesienie progu binaryzacji do R15
	mov R15, RDX 

	; By wybra� czy piksel ma by� bia�y lub czarny nale�y por�wna� �redni� arytmetyczn� 
	; jego warto�ci R G i B z progiem binaryzacji. Analogiczny efekt daje wymno�enie progu
	; binaryzacji przez 3 i por�wnanie go z sum� warto�ci R G B piksela. 
	mov RAX, 3

	; Konwersja liczby 3 oraz progu binaryzacji w celu umieszczenia ich w XMM
	CVTSI2SD xmm0, RAX
	CVTSI2SD xmm1, R15

	;Wymno�enie progu binaryzacji razy 3
	MULPD xmm0, xmm1

	; Konwersja obliczonej warto�ci na liczb� ca�kowit�
	CVTSD2SI R15, xmm0

	; Wymnozenie start razy 3 pixele i razy 4 bajty na pixel, wiec lacznie razy 12
	; w celu wyznaczenia pocz�tku tablicy dla danego w�tku
	mov RAX, 12		
	mul R8			
	mov R8, RAX	
	
	; Zapamietanie starego adresu tablicy, sluzy on pozniej do porownania dokad ma dzialac watek
	mov R13, RCX	

	; Wyznaczenie startu adresu tablicy dla w�tku
	add RBX, R8	

	; Wyznaczenie konca tablicy dla watku
	mov RAX, 12
	mul R9
	add R13, RAX

mainloop:

	; Konwersja warto�ci RGB piksela w celu umieszczenia informacji w rejestrach xmm
	; Tablica przechowuj�ca dane o pikselach jest typu ca�kowitego wi�c po ka�dym 
	; odczycie nale�y przesun�� si� o 4 bajty dodaj�c 4 do rejestru RBX
	CVTSI2SD xmm2, dword ptr [RBX]
	add RBX, 4
	CVTSI2SD xmm3, dword ptr [RBX]
	add RBX, 4
	CVTSI2SD xmm4, dword ptr [RBX]
	add RBX, 4

	;Zsumowanie wszystkich 3 warto�ci RGB do jednego rejestru xmm
	ADDSD xmm2, xmm3
	ADDSD xmm2, xmm4

	; Konwersja zsumowanych warto��i RGB do rejestru R10
	CVTSD2SI R10, xmm2

	; Je�eli suma jest wi�ksza lub r�wna progowi binaryzacji, to nale�y piksel ustawi� na 
	; kolor bia�y, w przeciwnym razie na czarny
	cmp R10, R15
	jge white

	
black:

	;Ustawianie piksela na kolor czarny
	mov dword ptr [RBX], 00h
	add RBX, 4
	mov dword ptr [RBX], 00h
	add RBX, 4
	mov dword ptr [RBX], 00h
	add RBX, 4
	jmp ifend

white:

	;Ustawianie piksela na kolor bia�y
	mov dword ptr [RBX], 0ffh
	add RBX, 4
	mov dword ptr [RBX], 0ffh
	add RBX, 4
	mov dword ptr [RBX], 0ffh
	add RBX, 4

ifend:

	; Je�eli atualny adres tablicy RBX jest wi�kszy od adresu w R13 to oznacza to, �e
	; w�tek zako�czy� prac� na swoim obszarze pami�ci i nale�y powr�ci� z procedury
	cmp R13, RBX 
	jge mainloop

	ret

MyBinarization endp

end