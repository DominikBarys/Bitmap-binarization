;---------------------------------------------------------------------
;
; BINARYZACJA MAPY BITOWEJ
; 11.12.2022 Dominik Baryœ
; Informatyka SSI semestr 5 rok akademicki 2022/2023
; v.1.0 - 11.12.2022
; 
; Implementacja algorytmu binaryzacji mapy bitowej.
; Poni¿szy fragment asemblera jest odpowiedzialny za obliczenie
; œredniej arytmetycznej ka¿dego z pikseli a nastêpnie podjêcie 
; decyzji czy dany piksel ustawiæ jako bia³y lub czarny na podstawie
; wprowadzonego przez u¿ytkownika progu binaryzacji.
; Algorytm  dzia³a wielow¹tkowo wiêc poni¿szy fragment
; kodu jest równie¿ odpowiedzialny za dopilnowanie aby ka¿dy z w¹tków
; pracowa³ na wyznaczonym dla niego obszarze pamiêci poprzez odpowiednie
; obliczenie adresu pocz¹tkowego tablicy oraz koñcowego a nastêpnie 
; sprawdzenie czy dany w¹tek wyszed³ poza swój zakres pamiêci
;
;---------------------------------------------------------------------

.code 

MyBinarization proc

	; Porzadek w rejestrach oraz operacje niezbêdne przed wejœciem w g³ówn¹ pêtle prodecury "mainloop"

	; Zmiana rejestru adresu tablicy
	mov RBX, RCX 

	; Przeniesienie progu binaryzacji do R15
	mov R15, RDX 

	; By wybraæ czy piksel ma byæ bia³y lub czarny nale¿y porównaæ œredni¹ arytmetyczn¹ 
	; jego wartoœci R G i B z progiem binaryzacji. Analogiczny efekt daje wymno¿enie progu
	; binaryzacji przez 3 i porównanie go z sum¹ wartoœci R G B piksela. 
	mov RAX, 3

	; Konwersja liczby 3 oraz progu binaryzacji w celu umieszczenia ich w XMM
	CVTSI2SD xmm0, RAX
	CVTSI2SD xmm1, R15

	;Wymno¿enie progu binaryzacji razy 3
	MULPD xmm0, xmm1

	; Konwersja obliczonej wartoœci na liczbê ca³kowit¹
	CVTSD2SI R15, xmm0

	; Wymnozenie start razy 3 pixele i razy 4 bajty na pixel, wiec lacznie razy 12
	; w celu wyznaczenia pocz¹tku tablicy dla danego w¹tku
	mov RAX, 12		
	mul R8			
	mov R8, RAX	
	
	; Zapamietanie starego adresu tablicy, sluzy on pozniej do porownania dokad ma dzialac watek
	mov R13, RCX	

	; Wyznaczenie startu adresu tablicy dla w¹tku
	add RBX, R8	

	; Wyznaczenie konca tablicy dla watku
	mov RAX, 12
	mul R9
	add R13, RAX

mainloop:

	; Konwersja wartoœci RGB piksela w celu umieszczenia informacji w rejestrach xmm
	; Tablica przechowuj¹ca dane o pikselach jest typu ca³kowitego wiêc po ka¿dym 
	; odczycie nale¿y przesun¹æ siê o 4 bajty dodaj¹c 4 do rejestru RBX
	CVTSI2SD xmm2, dword ptr [RBX]
	add RBX, 4
	CVTSI2SD xmm3, dword ptr [RBX]
	add RBX, 4
	CVTSI2SD xmm4, dword ptr [RBX]
	add RBX, 4

	;Zsumowanie wszystkich 3 wartoœci RGB do jednego rejestru xmm
	ADDSD xmm2, xmm3
	ADDSD xmm2, xmm4

	; Konwersja zsumowanych wartoœæi RGB do rejestru R10
	CVTSD2SI R10, xmm2

	; Je¿eli suma jest wiêksza lub równa progowi binaryzacji, to nale¿y piksel ustawiæ na 
	; kolor bia³y, w przeciwnym razie na czarny
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

	;Ustawianie piksela na kolor bia³y
	mov dword ptr [RBX], 0ffh
	add RBX, 4
	mov dword ptr [RBX], 0ffh
	add RBX, 4
	mov dword ptr [RBX], 0ffh
	add RBX, 4

ifend:

	; Je¿eli atualny adres tablicy RBX jest wiêkszy od adresu w R13 to oznacza to, ¿e
	; w¹tek zakoñczy³ pracê na swoim obszarze pamiêci i nale¿y powróciæ z procedury
	cmp R13, RBX 
	jge mainloop

	ret

MyBinarization endp

end