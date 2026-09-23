#include "client.h"
extern socket_t sock;
/* El adaptador oculta los detalles de red a client.c. Copy-restore. */
static int llamar(uint32_t operacion, Persona *persona) {
    uint32_t estado;
    Persona respuesta;
    if (enviar_u32(sock, operacion) || enviar_persona(sock, persona) ||
        recibir_u32(sock, &estado) || recibir_persona(sock, &respuesta)) return -1;
    if (estado != 0) return -1;
    *persona = respuesta;
    return 0;
}
int store(Persona *persona) { return llamar(1, persona); }
int retrieve(Persona *persona) { return llamar(2, persona); }
