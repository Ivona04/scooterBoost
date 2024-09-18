#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define MAX_MOTORA 10
#define MAX_MODIFIKACIJA 3
#define MAX_MODIFIKACIJA_TYP 21


#define PERCENTAGE(x) ((x) * 100)

static inline float izracunajPostotak(float osnovnaSnaga, float postotak) {
    return osnovnaSnaga * (postotak / 100);
}

typedef enum {
    USPJEH,
    NEUSPJEH,
    MOTOR_NIJE_PRONADEN
} Status;

typedef struct {
    char kategorija[50];
    char naziv[50];
    float postotakPovecanja;
} Modifikacija;

typedef struct {
    int motorId;
    float osnovnaSnaga;
    Modifikacija modifikacije[MAX_MODIFIKACIJA];
    int brojModifikacija;
} Motor;

extern void unosPodatakaOMotoru(Motor* motor, int id);
Status dodajMotor(Motor* motori, int* brojMotora);
void prikaziMotore(const Motor* motori, int brojMotora);
Status azurirajMotor(Motor* motori, int brojMotora, int id);
Status obrisiMotor(Motor* motori, int* brojMotora, int id);
float izracunajSnagu(const Motor motor);
void ucitajModifikacijeIzDatoteke(const char* imeDatoteke);
void prikaziKategorijeModifikacija();
void prikaziOpcijeModifikacija(const char* kategorija);
void spremiMotoreUDatoteku(const char* imeDatoteke, Motor* motori, int brojMotora);
int ucitajMotoreIzDatoteke(const char* imeDatoteke, Motor* motori, int* brojMotora);

// Prototipovi za sortiranje i pretraživanje
void sortirajMotore(Motor* motori, int brojMotora);
Motor* pronadiMotor(Motor* motori, int brojMotora, int id);

#endif // FUNCTIONS_H
