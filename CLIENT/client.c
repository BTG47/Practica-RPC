#include "client.h"

socket_t sock;
char *host;

static void mostrar(const Persona *p) {
    printf("ID: %d\nNombre: %s\nDireccion: %s %d, %s\n",
           p->id, p->nombre, p->direccion.calle, p->direccion.numero, p->direccion.ciudad);
}

static int numero_positivo(const char *texto, int32_t *resultado) {
    // Transforma una cadena a un número. Verifica que contega los valores númericos y rechaza el cero
    // negativos y números demasiado grandes
    char *fin;
    long n = strtol(texto, &fin, 10);
    if (!*texto || *fin || n < 1 || n > 2147483647L) return -1;
    *resultado = (int32_t)n;
    return 0;
}

int main(int argc, char **argv) {
    Persona p = {0};
    int resultado;
    if (argc < 4) {
        fprintf(stderr, "Uso: %s <IP> guardar <nombre> <calle> <numero> <ciudad>\n"
                        "     %s <IP> recuperar <id>\n", argv[0], argv[0]);
        return 1;
    }
    host = argv[1];
    // Comprobar resultados anomalos
    if (!strcmp(argv[2], "guardar") && argc == 7) {
        if (strlen(argv[3]) >= NOMBRE_MAX || strlen(argv[4]) >= CALLE_MAX ||
            strlen(argv[6]) >= CIUDAD_MAX || numero_positivo(argv[5], &p.direccion.numero)) {
            fprintf(stderr, "Campos invalidos o demasiado largos\n"); return 1;
        }
        strcpy(p.nombre, argv[3]);
        strcpy(p.direccion.calle, argv[4]);
        strcpy(p.direccion.ciudad, argv[6]);
    } else if (!strcmp(argv[2], "recuperar") && argc == 4) {
        if (numero_positivo(argv[3], &p.id)) return fprintf(stderr, "ID invalido\n"), 1;
    } else {
        fprintf(stderr, "Operacion o numero de argumentos incorrecto\n"); return 1;
    }
    sock = connection();
    if (sock == INVALID_SOCKET) return 1;
    if (!strcmp(argv[2], "guardar")) {
        puts("Antes de la llamada remota:"); mostrar(&p);
        resultado = store(&p);
    } else resultado = retrieve(&p);
    close_socket(sock);
    if (resultado) return fprintf(stderr, "Error remoto o registro inexistente\n"), 1;
    puts("Despues de la llamada remota:"); mostrar(&p);
    return 0;
}
