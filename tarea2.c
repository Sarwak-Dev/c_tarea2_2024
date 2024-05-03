#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char title[100];
  char genres[400];
  char director[300];
  float rating;
  int year;
} Film;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Películas");
  puts("========================================");

  puts("1) Cargar Películas");
  puts("2) Buscar por id");
  puts("3) Buscar por director");
  puts("4) Buscar por género");
  puts("5) Buscar por década");
  puts("6) Buscar por rango de calificaciones");
  puts("7) Buscar por década y género");
  puts("8) Salir");
}

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}

void cargar_peliculas(Map *pelis_byid) {
  // Intenta abrir el archivo CSV que contiene datos de películas
  FILE *archivo = fopen("data/Top1500.csv", "r");
  if (archivo == NULL) {
    perror(
        "Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }

  char **campos;
  campos = leer_linea_csv(archivo, ',');

  if (campos == NULL) {
    fclose(archivo);
    return;
  }

  // Lee cada línea del archivo CSV hasta el final
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    // Crea una nueva estructura Film y almacena los datos de cada película
    Film *peli = (Film *)malloc(sizeof(Film));
    if (peli == NULL) {
      perror("Error al asignar memoria para la película");
      fclose(archivo); // Cerrar el archivo antes de salir
      return;
    }

    // Copia los datos solo si los campos no son NULL
    if (campos[1] != NULL)
      strcpy(peli->id, campos[1]);
    if (campos[5] != NULL)
      strcpy(peli->title, campos[5]);
    if (campos[14] != NULL)
      strcpy(peli->director, campos[14]);
    if (campos[11] != NULL)
      strcpy(peli->genres, campos[11]);
    // peli->genres = list_create(); // No se está usando, asegúrate de liberar
    // memoria
    peli->year =
        (campos[10] != NULL)
            ? atoi(campos[10])
            : 0; // Asegúrate de manejar correctamente la conversión a entero

    // Inserta la película en el mapa usando el ID como clave
    map_insert(pelis_byid, peli->id, peli);
  }
  fclose(archivo);

  // Itera sobre el mapa para mostrar las películas cargadas
  int i = 0;
  MapPair *pair = map_first(pelis_byid);
  while (pair != NULL) {
    i++;
    printf("%d: ,", i);
    Film *peli = pair->value;
    printf("ID: %s, Título: %s, Director: %s, Año: %d\n", peli->id, peli->title,
           peli->director, peli->year);
    pair = map_next(pelis_byid);
  }
}

/*
 * Busca y muestra la información de una película por su ID en un mapa.
 */

void buscar_por_id(Map *pelis_byid) {
  char id[10];

  printf("Ingrese el id de la película: ");
  scanf("%s", id);

  MapPair *pair = map_search(pelis_byid, id);

  if (pair != NULL) {
    Film *peli =
        pair->value; // Obtiene el puntero a la estructura de la película
    printf("\n\nTítulo: %s, Año: %d\n\n", peli->title, peli->year);
  } else {
    printf("La película con id %s no existe\n", id);
  }
}

void buscar_por_director(Map *pelis_direc) {
  char director[100];
  int size = 0;
  int count = 1;

  printf("Ingrese el director de la película: ");
  scanf(" %99[^\n]", director);

  MapPair *pair = map_first(pelis_direc);

  while (pair != NULL) {
    Film *peli = pair->value;
    char direc_copy[400];
    strcpy(direc_copy, peli->director);

    char *encontrar_direc = strstr(direc_copy, director);

    if (encontrar_direc != NULL) {
      printf("%d Título: %s || %s\n\n", count, peli->title, peli->director);
      size++;
      count++;
    }

    pair = map_next(pelis_direc);
  }

  if (size == 0)
    printf("No se encontraron películas para el director %s\n", director);
}

void buscar_por_genero(Map *pelis_genero) {
  char genero[400];
  int size = 0;
  int count = 1;

  printf("Ingrese el género de la película: ");
  scanf("%s", genero);

  MapPair *pair = map_first(pelis_genero);

  while (pair != NULL) {
    Film *peli = pair->value;
    char genres_copy[400];
    strcpy(genres_copy, peli->genres);

    char *encontrar_genre = strstr(genres_copy, genero);

    if (encontrar_genre != NULL) {
      printf("%d Título: %s || %s\n\n", count, peli->title, peli->genres);
      size++;
      count++;
    }

    pair = map_next(pelis_genero);
  }

  if (size == 0)
    printf("No se encontraron películas del género %s\n", genero);
}

int main() {
  char opcion;
  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_id = map_create(is_equal_str);
  Map *pelis_direc = map_create(is_equal_str);

  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_peliculas(pelis_id);
      break;
    case '2':
      buscar_por_id(pelis_id);
      break;
    case '3':
      buscar_por_director(pelis_id);
      break;
    case '4':
      buscar_por_genero(pelis_id);
      break;
    case '5':
      break;
    case '6':
      break;
    case '7':
      break;
    default:
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}
