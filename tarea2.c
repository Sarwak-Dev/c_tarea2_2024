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
    if (campos[8] != NULL)
      peli->rating = atof(campos[8]);
    // peli->genres = list_create(); // No se está usando, asegúrate de liberar
    // memoria
    peli->year = (campos[10] != NULL)
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
    printf("\n\nTítulo: %s, Año: %d, Calificacion: %.1f\n\n", peli->title, peli->year, peli->rating);
  } 
  else {
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
  printf("\n");

  while (pair != NULL) {

    Film *peli = pair->value;
    char direc_copy[400];
    strcpy(direc_copy, peli->director);

    char *encontrar_direc = strstr(direc_copy, director);
    if (encontrar_direc != NULL) {
      printf("%d) Título: %s || %s\n", count, peli->title, peli->director);
      size++;
      count++;
    }

    pair = map_next(pelis_direc);
  }

  printf("\n");

  if (size == 0)
    printf("No se encontraron películas para el director %s\n", director);
}


void buscar_por_genero(Map *pelis_genero) {
  char genero[400];
  int size = 0;
  int count = 1;

  printf("Ingrese el género de la película: ");
  scanf(" %99[^\n]", genero);

  MapPair *pair = map_first(pelis_genero);
  printf("\n");
  while (pair != NULL) {
    Film *peli = pair->value;
    char genres_copy[400];
    strcpy(genres_copy, peli->genres);

    char *encontrar_genre = strstr(genres_copy, genero);

    if (encontrar_genre != NULL) {
      printf("%d) Título: %s || %s\n", count, peli->title, peli->genres);
      size++;
      count++;
    }

    pair = map_next(pelis_genero);
  }

  if (size == 0)
    printf("No se encontraron películas del género %s\n", genero);
}

void buscar_por_decada(Map *pelis_decada) {
  int decada;
  int size = 0;
  int count = 1;
  printf("Ingrese la década de la película: ");
  scanf("%d", &decada);
  MapPair *pair = map_first(pelis_decada);
  limpiarPantalla();
  while (pair != NULL)
    {
      Film *peli = pair->value;
      if (peli->year >= decada && peli->year < decada + 10)
      {
        printf("%d) Título: %s || Año: %d\n", count, peli->title, peli->year);
        size++;
        count++;
      }
      pair = map_next(pelis_decada);
    }

  if (size == 0)
    printf("No se encontraron películas de la década %d\n", decada);

}

void buscar_por_calificaciones (Map *pelis_calificaciones) {
  float calificacion1;
  float calificacion2;
  char opcion;
  int size = 0;
  int count = 1;

  printf("Ingrese el rango de calificacion que quieras: \n\n");
  puts("1) 4.1 - 5.0");
  puts("2) 5.1 - 6.0");
  puts("3) 6.1 - 7.0");
  puts("4) 7.1 - 8.0");
  puts("5) 8.1 - 9.9\n");

  printf("Ingrese su opción: ");
  scanf(" %c", &opcion);

  switch (opcion) {
    case '1':
      calificacion1 = 4.1;
      calificacion2 = 5.0;
      break;
    case '2':
      calificacion1 = 5.1;
      calificacion2 = 6.0;
      break;
    case '3':
      calificacion1 = 6.1;
      calificacion2 = 7.0;
      break;
    case '4':  
      calificacion1 = 7.1;
      calificacion2 = 8.0;
      break;
    case '5':  
      calificacion1 = 8.1;
      calificacion2 = 9.9;
      break;
    default:
      printf("Opción inválida. Por favor, seleccione una opción válida.\n");
  }

  printf("Tu rango de calificacion elegida es de: %.1f - %.1f\n", calificacion1, calificacion2);

  MapPair *pair = map_first(pelis_calificaciones);
  printf("\n");

  while (pair != NULL) {
    Film *peli = pair->value;

    if (peli->rating >= calificacion1 && peli->rating <= calificacion2) {
      printf("%d) Título: %s || Calificacion: %.1f\n", count, peli->title, peli->rating);
      size++;
      count++;
    }
    pair = map_next(pelis_calificaciones);
  }
  printf("\n");

}

void buscar_por_decada_genero(Map *pelis_decada_genero) {
  char genero[400];
  int decada;
  char opcion;

  int size = 0;
  int count = 1;

  printf("Ingrese el género de la película: ");
  scanf(" %99[^\n]", genero);
  limpiarPantalla();
  mostrarMenuPrincipal();

  printf("Ingrese la década de la película: ");
  scanf("%d", &decada);
  limpiarPantalla();
  mostrarMenuPrincipal();

  MapPair *pair = map_first(pelis_decada_genero);
  printf("\n");

  while (pair != NULL) {
    Film *peli = pair->value;
    char genres_copy[400];
    strcpy(genres_copy, peli->genres);

    char *encontrar_genre = strstr(genres_copy, genero);

    if (encontrar_genre != NULL && peli->year >= decada && peli->year < decada + 10) {
      printf("%d) Título: %s || Año: %d || Géneros: %s\n", count, peli->title, peli->year, peli->genres);
      size++;
      count++;
    }
    pair = map_next(pelis_decada_genero);
  }

  if (size == 0){
    printf("No se encontraron películas con el/los géneros entre %s y en la década %d.\n", genero, decada);
  }
  printf("\n");
}

int main() {
  char opcion;
  // Crea un mapa para almacenar películas, utilizando una función de
  // comparación que trabaja con claves de tipo string.
  Map *pelis_id = map_create(is_equal_str);  

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
      buscar_por_decada(pelis_id);
      break;
    case '6':
      buscar_por_calificaciones(pelis_id);
      break;
    case '7':
      buscar_por_decada_genero(pelis_id);
      break;
    default:
      printf("Opción inválida. Por favor, seleccione una opción válida.\n");
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}
