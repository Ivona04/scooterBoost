#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "functions.h"

#define MAX_MOTORA 10

static void prikaziIzbornik() {
    printf("Odaberite opciju:\n");
    printf("1. Dodaj motor\n");
    printf("2. Prikazi sve motore\n");
    printf("3. Azuriraj motor\n");
    printf("4. Obrisi motor\n");
    printf("5. Izlaz\n");
}

int main() {
    Motor* motori = (Motor*) malloc(MAX_MOTORA * sizeof(Motor));

    if (motori == NULL) {
        printf("Greska pri alociranju memorije.\n");
        exit(EXIT_FAILURE);
    }

    ucitajModifikacijeIzDatoteke("./../modifikacije.txt");

    int brojMotora = 0;
    int opcija, id;

    while (1) {
        prikaziIzbornik();
        printf("Unesite opciju: ");
        scanf("%d", &opcija);

        switch (opcija) {
            case 1:
                dodajMotor(motori, &brojMotora);
            break;
            case 2:
                prikaziMotore(motori, brojMotora);
            break;
            case 3:
                printf("\nUnesite ID motora za azuriranje: ");
            scanf("%d", &id);
            azurirajMotor(motori, brojMotora, id);
            break;
            case 4:
                printf("\nUnesite ID motora za brisanje: ");
            scanf("%d", &id);
            obrisiMotor(motori, &brojMotora, id);
            break;
            case 5:
                free(motori);
            motori = NULL;
            return 0;
            default:
                printf("Nepoznata opcija. Pokusajte ponovo.\n");
        }
    }

    return 0;
}
