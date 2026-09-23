#include "server.h"
/* El adaptador recibe la estructura y despacha la funcion local. */
int main(void) {
    socket_t servidor = initialization();
    if (servidor == INVALID_SOCKET) return 1;
    for (;;) {
        socket_t cliente = connection(servidor);
        if (cliente == INVALID_SOCKET) continue;
        for (;;) { /* Mantiene la capacidad de varias llamadas por conexion. */
            uint32_t operacion;
            Persona p;
            int resultado;
            if (recibir_u32(cliente, &operacion)) break;
            if (recibir_persona(cliente, &p)) break;
            if (operacion == 1) resultado = store(&p);
            else if (operacion == 2) resultado = retrieve(&p);
            else resultado = -1;
            if (enviar_u32(cliente, resultado ? 1u : 0u) || enviar_persona(cliente, &p)) break;
        }
        close_socket(cliente);
    }
    close_socket(servidor);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}
