#ifndef CLIENT_H
#define CLIENT_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif
typedef SOCKET socket_t;
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
typedef int socket_t;
#define INVALID_SOCKET (-1)
#endif
#define PORT_NUM 8888
#define NOMBRE_MAX 64
#define CALLE_MAX 64
#define CIUDAD_MAX 64

// Definición de las principales estructuras para el programa
typedef struct { char calle[CALLE_MAX]; int32_t numero; char ciudad[CIUDAD_MAX]; } Direccion;
typedef struct { int32_t id; char nombre[NOMBRE_MAX]; Direccion direccion; } Persona;

socket_t connection(void);
int close_socket(socket_t sock);
int enviar_todo(socket_t sock, const void *datos, unsigned int cantidad);
int recibir_todo(socket_t sock, void *datos, unsigned int cantidad);
int enviar_u32(socket_t sock, uint32_t numero);
int recibir_u32(socket_t sock, uint32_t *numero);
int enviar_persona(socket_t sock, const Persona *persona);
int recibir_persona(socket_t sock, Persona *persona);
int store(Persona *persona);
int retrieve(Persona *persona);
#endif
