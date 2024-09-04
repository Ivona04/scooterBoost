#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define MAX_MOTORA 10
#define MAX_MODIFIKACIJA 3
#define MAX_MODIFIKACIJA_TYP 21

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

void unosPodatakaOMotoru(Motor* motor, int id);
Status dodajMotor(Motor* motori, int* brojMotora);
void prikaziMotore(const Motor* motori, int brojMotora);
Status azurirajMotor(Motor* motori, int brojMotora, int id);
Status obrisiMotor(Motor* motori, int* brojMotora, int id);
float izracunajSnagu(const Motor motor);
void ucitajModifikacijeIzDatoteke(const char* imeDatoteke);
void prikaziModifikacije();
void ucitajModifikacijeIzDatoteke(const char* imeDatoteke);
void prikaziKategorijeModifikacija();
void prikaziOpcijeModifikacija(const char* kategorija);


#endif //FUNCTIONS_H