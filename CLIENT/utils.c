#include "client.h"
extern char *host;

socket_t connection(void) {
    socket_t sock;
    struct sockaddr_in server;
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa)) return INVALID_SOCKET;
#endif
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) return INVALID_SOCKET;
    memset(&server, 0, sizeof server);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT_NUM);
    server.sin_addr.s_addr = inet_addr(host);
    if (server.sin_addr.s_addr == INADDR_NONE ||
        connect(sock, (struct sockaddr *)&server, sizeof server) < 0) {
        close_socket(sock);
        return INVALID_SOCKET;
    }
    return sock;
}
int close_socket(socket_t sock) {
#ifdef _WIN32
    int resultado = closesocket(sock);
    WSACleanup();
    return resultado;
#else
    return close(sock);
#endif
}

/* Formato de red comun: campos fijos y enteros en network byte order. */
int enviar_todo(socket_t sock, const void *datos, unsigned int cantidad) {
    const char *p = (const char *)datos;
    while (cantidad) {
        int n = send(sock, p, (int)cantidad, 0);
        if (n <= 0) return -1;
        p += n; cantidad -= (unsigned int)n;
    }
    return 0;
}
int recibir_todo(socket_t sock, void *datos, unsigned int cantidad) {
    char *p = (char *)datos;
    while (cantidad) {
        int n = recv(sock, p, (int)cantidad, 0);
        if (n <= 0) return -1;
        p += n; cantidad -= (unsigned int)n;
    }
    return 0;
}
int enviar_u32(socket_t sock, uint32_t numero) {
    uint32_t red = htonl(numero);
    return enviar_todo(sock, &red, sizeof red);
}
int recibir_u32(socket_t sock, uint32_t *numero) {
    uint32_t red;
    if (recibir_todo(sock, &red, sizeof red)) return -1;
    *numero = ntohl(red);
    return 0;
}
int enviar_persona(socket_t sock, const Persona *p) {
    if (enviar_u32(sock, (uint32_t)p->id) ||
        enviar_todo(sock, p->nombre, NOMBRE_MAX) ||
        enviar_todo(sock, p->direccion.calle, CALLE_MAX) ||
        enviar_u32(sock, (uint32_t)p->direccion.numero) ||
        enviar_todo(sock, p->direccion.ciudad, CIUDAD_MAX)) return -1;
    return 0;
}
int recibir_persona(socket_t sock, Persona *p) {
    uint32_t id, numero;
    memset(p, 0, sizeof *p);
    if (recibir_u32(sock, &id) ||
        recibir_todo(sock, p->nombre, NOMBRE_MAX) ||
        recibir_todo(sock, p->direccion.calle, CALLE_MAX) ||
        recibir_u32(sock, &numero) ||
        recibir_todo(sock, p->direccion.ciudad, CIUDAD_MAX)) return -1;
    if (!memchr(p->nombre, 0, NOMBRE_MAX) ||
        !memchr(p->direccion.calle, 0, CALLE_MAX) ||
        !memchr(p->direccion.ciudad, 0, CIUDAD_MAX)) return -1;
    p->id = (int32_t)id;
    p->direccion.numero = (int32_t)numero;
    return 0;
}
