#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include <string.h>

#define MAX_KATEGORIJA 10

static Modifikacija predefiniraneModifikacije[MAX_MODIFIKACIJA_TYP];
static char kategorije[MAX_KATEGORIJA][50];
static int brojKategorija = 0;

void unosPodatakaOMotoru(Motor* motor, int id) {
    motor->motorId = id;
    printf("\nUnesite osnovnu snagu motora (u konjskim snagama): ");
    if (scanf("%f", &motor->osnovnaSnaga) != 1) {
        printf("Greska pri unosu osnovne snage.\n");
        return;
    }

    motor->brojModifikacija = 0;

    while (motor->brojModifikacija < MAX_MODIFIKACIJA) {
        int uneseniBroj = 0;
        prikaziKategorijeModifikacija();
        printf("\nUnesite kategoriju modifikacije (Za prekid unesite '0'): ");
        scanf("%d", &uneseniBroj);

        if (uneseniBroj == 0) break;

        if (uneseniBroj < 1 || uneseniBroj > brojKategorija) {
            printf("Nepoznata kategorija. Molimo unesite broj.\n");
            continue;
        }

        prikaziOpcijeModifikacija(kategorije[uneseniBroj - 1]);

        int opcija;
        printf("\nOdaberite opciju modifikacije: ");
        scanf("%d", &opcija);

        // Provjeri validnost opcije
        if (opcija < 1 || opcija > 3) {
            printf("Nepoznata opcija. Pokusajte ponovo.\n");
            continue;
        }

        // Pretraži i dodaj ili ažuriraj modifikaciju
        int modFound = 0;
        for (int i = 0; i < motor->brojModifikacija; i++) {
            printf("%s, %s", motor->modifikacije[i].kategorija, kategorije[uneseniBroj - 1]);
            if (strcmp(motor->modifikacije[i].kategorija, kategorije[uneseniBroj - 1]) == 0) {
                // Ažuriraj postojeću modifikaciju
                motor->modifikacije[i].postotakPovecanja = predefiniraneModifikacije[opcija - 1].postotakPovecanja;
                modFound = 1;
                break;
            }
        }
        if (!modFound && motor->brojModifikacija < MAX_MODIFIKACIJA) {
            // Dodaj novu modifikaciju
            strcpy(motor->modifikacije[motor->brojModifikacija].naziv, predefiniraneModifikacije[(uneseniBroj - 1) * 3 + opcija - 1].naziv);
            strcpy(motor->modifikacije[motor->brojModifikacija].kategorija, predefiniraneModifikacije[(uneseniBroj - 1) * 3 + opcija - 1].kategorija);
            motor->modifikacije[motor->brojModifikacija].postotakPovecanja = predefiniraneModifikacije[(uneseniBroj - 1) * 3 + opcija - 1].postotakPovecanja;

            motor->brojModifikacija++;
        }
    }
}

Status dodajMotor(Motor* motori, int* brojMotora) {
    if (*brojMotora < MAX_MOTORA) {
        unosPodatakaOMotoru(&motori[*brojMotora], *brojMotora + 1);
        (*brojMotora)++;
        spremiMotoreUDatoteku("motori.txt", motori, *brojMotora);
        return USPJEH;
    }
    else {
        printf("Dostignut je maksimalan broj motora.\n");
        return NEUSPJEH;
    }
}

void prikaziMotore(const Motor* motori, int brojMotora) {
    for (int i = 0; i < brojMotora; i++) {
        printf("Motor ID: %d\n", motori[i].motorId);
        printf("Osnovna snaga: %.2f konjskih snaga\n", motori[i].osnovnaSnaga);

        for (int j = 0; j < motori[i].brojModifikacija; j++) {
            printf("Modifikacija: %s, Povecanje: %.2f%%\n", motori[i].modifikacije[j].naziv, motori[i].modifikacije[j].postotakPovecanja);
        }

        printf("Ukupna snaga: %.2f konjskih snaga\n", izracunajSnagu(motori[i]));
        printf("-------------------------\n");
    }
}

Status azurirajMotor(Motor* motori, int brojMotora, int id) {
    for (int i = 0; i < brojMotora; i++) {
        if (motori[i].motorId == id) {
            printf("Azuriranje motora ID: %d\n", id);
            unosPodatakaOMotoru(&motori[i], id);
            spremiMotoreUDatoteku("motori.txt", motori, brojMotora);
            return USPJEH;
        }
    }
    printf("Motor s ID: %d nije prona?en.\n", id);
    return MOTOR_NIJE_PRONADEN;
}

Status obrisiMotor(Motor* motori, int* brojMotora, int id) {
    for (int i = 0; i < *brojMotora; i++) {
        if (motori[i].motorId == id) {
            for (int j = i; j < *brojMotora - 1; j++) {
                motori[j] = motori[j + 1];
            }
            (*brojMotora)--;
            spremiMotoreUDatoteku("motori.txt", motori, *brojMotora);
            printf("Motor s ID: %d je obrisan.\n", id);
            return USPJEH;
        }
    }
    printf("Motor s ID: %d nije pronadjen.\n", id);
    return MOTOR_NIJE_PRONADEN;
}

float izracunajSnagu(const Motor motor) {
    float ukupnaSnaga = motor.osnovnaSnaga;
    for (int i = 0; i < motor.brojModifikacija; i++) {
        ukupnaSnaga += izracunajPostotak(ukupnaSnaga, motor.modifikacije[i].postotakPovecanja);
    }
    return ukupnaSnaga;
}

void ucitajModifikacijeIzDatoteke(const char* imeDatoteke) {
    FILE* datoteka = fopen(imeDatoteke, "r");
    if (datoteka == NULL) {
        printf("Ne mogu otvoriti datoteku s modifikacijama.\n");
        exit(EXIT_FAILURE);
    }

    int brojModifikacija = 0;
    char kategorija[50], naziv[50];
    float postotakPovecanja;

    while (fscanf(datoteka, "%49[^,],%49[^,],%f\n", kategorija, naziv, &postotakPovecanja) == 3) {
        // Dodaj novu kategoriju ako ne postoji
        int kategorijaPostoji = 0;
        for (int i = 0; i < brojKategorija; i++) {
            if (strcmp(kategorije[i], kategorija) == 0) {
                kategorijaPostoji = 1;
                break;
            }
        }
        if (!kategorijaPostoji && brojKategorija < MAX_KATEGORIJA) {
            strcpy(kategorije[brojKategorija++], kategorija);
        }

        if (brojModifikacija < MAX_MODIFIKACIJA_TYP) {
            strcpy(predefiniraneModifikacije[brojModifikacija].naziv, naziv);
            strcpy(predefiniraneModifikacije[brojModifikacija].kategorija, kategorija);
            predefiniraneModifikacije[brojModifikacija].postotakPovecanja = postotakPovecanja;
            brojModifikacija++;
        }
    }

    fclose(datoteka);
}

void prikaziKategorijeModifikacija() {
    printf("\nDostupne kategorije modifikacija:\n");
    for (int i = 0; i < brojKategorija; i++) {
        printf("%d. %s\n", i + 1, kategorije[i]);
    }
}

void prikaziOpcijeModifikacija(const char* kategorija) {
    printf("\nDostupne opcije za kategoriju '%s':\n", kategorija);
    int i = 1;
    for (int j = 0; j < MAX_MODIFIKACIJA_TYP; j++) {
        if (strcmp(predefiniraneModifikacije[j].kategorija, kategorija) == 0) {
            printf("%d. %s (Povecanje: %.2f%%)\n", i++, predefiniraneModifikacije[j].naziv, predefiniraneModifikacije[j].postotakPovecanja);
        }
    }
}

void spremiMotoreUDatoteku(const char* imeDatoteke, Motor* motori, int brojMotora) {
    FILE* datoteka = fopen(imeDatoteke, "w");
    if (datoteka == NULL) {
        printf("Ne mogu otvoriti datoteku za spremanje.\n");
        return;
    }

    for (int i = 0; i < brojMotora; i++) {
        fprintf(datoteka, "%d,%.2f\n", motori[i].motorId, motori[i].osnovnaSnaga);
        for (int j = 0; j < motori[i].brojModifikacija; j++) {
            fprintf(datoteka, "%s,%s,%.2f\n", motori[i].modifikacije[j].kategorija, motori[i].modifikacije[j].naziv, motori[i].modifikacije[j].postotakPovecanja);
        }
    }

    fclose(datoteka);
}

int ucitajMotoreIzDatoteke(const char* imeDatoteke, Motor* motori, int* brojMotora) {
    FILE* datoteka = fopen(imeDatoteke, "r");
    if (datoteka == NULL) {
        printf("Ne mogu otvoriti datoteku za ucitavanje.\n");
        return 0;
    }

    int id;
    float snaga;
    while (fscanf(datoteka, "%d,%f\n", &id, &snaga) == 2) {
        motori[*brojMotora].motorId = id;
        motori[*brojMotora].osnovnaSnaga = snaga;
        motori[*brojMotora].brojModifikacija = 0;
        (*brojMotora)++;
    }

    fclose(datoteka);
    return *brojMotora;
}

// Funkcija za usporedbu motora (za qsort)
int usporedbaMotora(const void* a, const void* b) {
    Motor* motorA = (Motor*)a;
    Motor* motorB = (Motor*)b;
    return (motorA->osnovnaSnaga > motorB->osnovnaSnaga) - (motorA->osnovnaSnaga < motorB->osnovnaSnaga);
}

// Funkcija za pretraživanje motora po ID-u (za bsearch)
int usporedbaId(const void* a, const void* b) {
    int idA = *(int*)a;
    Motor* motorB = (Motor*)b;
    return (idA - motorB->motorId);
}

// Funkcija za sortiranje motora
void sortirajMotore(Motor* motori, int brojMotora) {
    qsort(motori, brojMotora, sizeof(Motor), usporedbaMotora);
}

// Funkcija za pretraživanje motora po ID-u
Motor* pronadiMotor(Motor* motori, int brojMotora, int id) {
    return (Motor*)bsearch(&id, motori, brojMotora, sizeof(Motor), usporedbaId);
}
