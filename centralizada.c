#include "SERVER/server.h"
/* Incluye el servicio para conservar la compilacion del archivo independiente. */
#include "SERVER/service.c"

int main(int argc, char **argv) {
    Persona p = {0}, recuperada;
    if (argc != 2 || strcmp(argv[1], "demo"))
        return fprintf(stderr, "Uso: %s demo\n", argv[0]), 1;
    strcpy(p.nombre, "Persona local");
    strcpy(p.direccion.calle, "Reforma");
    p.direccion.numero = 10;
    strcpy(p.direccion.ciudad, "Puebla");
    if (store(&p)) return fprintf(stderr, "Error al guardar\n"), 1;
    printf("ID local asignado: %d\n", p.id);
    memset(&recuperada, 0, sizeof recuperada);
    recuperada.id = p.id;
    if (retrieve(&recuperada)) return fprintf(stderr, "Error al recuperar\n"), 1;
    printf("Recuperado: %s, %s %d, %s\n", recuperada.nombre,
           recuperada.direccion.calle, recuperada.direccion.numero, recuperada.direccion.ciudad);
    return 0;
}
