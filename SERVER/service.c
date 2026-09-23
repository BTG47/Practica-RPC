#include "server.h"
#define ARCHIVO "personas.dat"
/* La estructura se guarda en el archivo local del servidor. */
int store(Persona *persona) {
    FILE *fp;
    Persona actual;
    int32_t siguiente = 1;
    fp = fopen(ARCHIVO, "rb");
    if (fp) {
        while (fread(&actual, sizeof actual, 1, fp) == 1) {
            if (actual.id >= siguiente) {
                if (actual.id == INT32_MAX) { fclose(fp); return -1; }
                siguiente = actual.id + 1;
            }
        }
        if (ferror(fp)) { fclose(fp); return -1; }
        fclose(fp);
    }
    fp = fopen(ARCHIVO, "ab");
    if (!fp) return -1;
    persona->id = siguiente; /* Modificacion visible para el cliente. */
    int escrito = fwrite(persona, sizeof *persona, 1, fp) == 1;
    int cerrado = fclose(fp) == 0;
    return escrito && cerrado ? 0 : -1;
}
int retrieve(Persona *persona) {
    FILE *fp = fopen(ARCHIVO, "rb");
    Persona actual;
    int32_t id = persona->id;
    if (!fp) return -1;
    while (fread(&actual, sizeof actual, 1, fp) == 1) {
        if (actual.id == id) { *persona = actual; fclose(fp); return 0; }
    }
    fclose(fp);
    return -1;
}
