# Codigo buscar Películas por distintos Criterios

<br>
<br>

Para ejecutar el codigo hay que usar estos comandos:
````console
gcc tdas/*.c tarea2.c -Wno-unused-result -o tarea2
````

Y luego ejecutar:
````console
./tarea2
````

<br>

<hr>

El codigo te abre un menu donde tu eliges que opcion te interesa.

````bash
  1) Cargar Películas
  2) Buscar por id
  3) Buscar por director
  4) Buscar por género
  5) Buscar por década
  6) Buscar por rango de calificaciones
  7) Buscar por década y género
  8) Salir
````

El funcionamiento es simple, carga las peliculas de un archivo externo csv y los almacena en un mapa hash clasificandolos en distintas keys en las posiciones de la csv correctas.

````c
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
````

<br>

### Prototipos de funciones principales


`void mostrarMenuPrincipal();`

`void cargar_peliculas(Map *pelis_byid);`

`void buscar_por_id(Map *pelis_byid);`

`void buscar_por_director(Map *pelis_direc);`

`void buscar_por_genero(Map *pelis_genero);`

`void buscar_por_decada(Map *pelis_decada);`

`void buscar_por_calificaciones (Map *pelis_calificaciones);`

`void buscar_por_decada_genero(Map *pelis_decada_genero);`

<br>

<hr>

### Observaciones
- Para poner decada se necesita poner el año como (1980, 1850, 2000) *multiples de 10*
- Cuando buscas un genero tiene que ser exactamente al del csv ya que no se implemento la diferencia de mayusculas y minusculas.
- el Director tambien tiene que estar escrito igual al csv con las mayusculas y minusculas correspondientes.
  
<br>

<br>

###### 💻 | Development By Sarwak y PatricioToledo
