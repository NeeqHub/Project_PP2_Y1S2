#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <string.h>

/** Maksymalna długość tytułu filmu. */
#define MAX_MOVIE_NAME_LENGHT 100
/** Maksymalna długość imienia i nazwiska reżysera filmu.. */
#define MAX_DIRECTOR_LENGHT 50
/** Maksymalna długość nazwy gatunku filmu. */
#define MAX_TYPE_LENGHT 25
/** Maksymalna długość imienia i nazwiska klienta. */
#define MAX_CLIENT_NAME_LENGHT 50

/** Struktura przechowująca dane o filmach. */
struct movie
{
    int id; /**< Pole id przechowujące identyfikator filmu, inkrementowane automatycznie z każdym dodanym filmem. */
    char movieName[MAX_MOVIE_NAME_LENGHT]; /**< Pole przechowujące nazwe filmu. */
    char director[MAX_DIRECTOR_LENGHT]; /**< Pole przechowujące reżysera filmu. */
    char type[MAX_TYPE_LENGHT]; /**< Pole przechowujące gatunek filmu. */
    int year; /**< Pole przechowujące rok produkcji filmu. */

    int amountOfCopies; /**< Pole przechowujące liczbę egzemplarzy danego filmu podaną przez użytkownika. */
    int amountOfRented; /**< Pole przechowujące liczbę wypożyczonych egzemplarzy danego filmu, deinkrementowane automatycznie z każdym wypożyczeniem. */

    struct movie *prev; /**< Wskaźnik na poprzedni element listy filmów. */
    struct movie *next; /**< Wskaźnik na następny element listy filmów. */

};

/** Zmienna globalna pomocnicza przechowująca wskaźnik na aktualny element listy filmów(ostatno element). */
struct movie *currentPointerMovie = NULL;

/** Zmienna globalna przechowująca liczbę filmów znajdujących się na liście. */
int amountOfDataMovie = 0;

/** Funkcja wczytuje dane o filmach z pliku movies.dat i zapisuje je do listy.
    \param mainPointerMovie Parametr wskaźnikowy na początek listy filmów.
*/
void loadDataMovie(struct movie **mainPointerMovie)
{
    FILE *file;

    if((file = fopen("movies.dat","rb")) == NULL)
    {
        fputs("Nie mozna odczytac z pliku movie.dat \n",stderr);
        return 0;
    }

    struct movie *tmp = NULL;

    while(fread(tmp=(struct movie *)malloc(sizeof(struct movie)),sizeof(struct movie),1,file) == 1)
    {
        if(amountOfDataMovie == 0)
        {
            (*mainPointerMovie) = tmp;
            (*mainPointerMovie)->prev = NULL;
            (*mainPointerMovie)->next = NULL;

            currentPointerMovie = (*mainPointerMovie);
        }
        else
        {
            struct movie *t = currentPointerMovie;

            currentPointerMovie->next = tmp;
            currentPointerMovie = tmp;
            currentPointerMovie->prev = t;
            currentPointerMovie->next = NULL;
        }

        amountOfDataMovie++;
    }

    fclose(file);
}

/** Funkcja zapisuje dane o filmach znajdujących się na liście do pliku movies.dat.
    \param mainPointerMovie Parametr wskaźnikowy typu (struct movie) na początek listy filmów.
*/
void saveDataMovie(struct movie *mainPointerMovie)
{
    FILE *file;

    if((file = fopen("movies.dat","wb")) == NULL)
    {
        fputs("Nie mozna otworzyc pliku i zapisac danych - movies.dat \n",stderr);
        return 0;
    }

    struct movie *tmp = mainPointerMovie;

    for(int i=0; i<amountOfDataMovie; i++)
    {
        fwrite(tmp,sizeof(struct movie),1,file);
        tmp = tmp->next;
    }

    fclose(file);
    printf("Zapisano. \n");
}

/** Funkcja tworzy liste filmów poprzez utworzenie pierwszego elementu, oraz wczytuje dane od użytkownika.
    \return Wskaźnik na nowo utworzoną strukturę z danymi.
*/
struct movie *createListMovie()
{
    struct movie *newNode = (struct movie *)malloc(sizeof(struct movie));

    if(newNode != NULL)
    {
        newNode->id = amountOfDataMovie;

        printf("Tytul: ");
        getchar();
        fgets(newNode->movieName,MAX_MOVIE_NAME_LENGHT,stdin);

        printf("Rok produkcji: ");
        scanf("%d",&newNode->year);

        printf("Rezyser: ");
        getchar();
        fgets(newNode->director,MAX_DIRECTOR_LENGHT,stdin);

        printf("Gatunek: ");
        scanf("%s",&newNode->type);

        printf("Liczba kopi: ");
        scanf("%d",&newNode->amountOfCopies);

        newNode->amountOfRented = 0;

        newNode->prev = newNode->next = NULL;
    }

    amountOfDataMovie++;

    return newNode;
};

/** Funkcja pobiera dane(tytuł,rok produkcji, reżyser, gatunek, liczba kopi) od użytkownika i zapisuje je w utworzonej dynamicznie strukturze.
    \return Wskaźnik na nowo utworzoną strukturę z danymi.
*/
struct movie *takeDataMovie()
{
        struct movie *newNode = (struct movie *)malloc(sizeof(struct movie));

        newNode->next = NULL;

        newNode->prev = currentPointerMovie;
        currentPointerMovie->next = newNode;

        newNode->id = amountOfDataMovie;

        printf("Tytul: ");
        getchar();
        fgets(newNode->movieName,MAX_MOVIE_NAME_LENGHT,stdin);

        printf("Rok produkcji: ");
        scanf("%d",&newNode->year);

        printf("Rezyser: ");
        getchar();
        fgets(newNode->director,MAX_DIRECTOR_LENGHT,stdin);

        printf("Gatunek: ");
        scanf("%s",&newNode->type);

        printf("Liczba kopi: ");
        scanf("%d",&newNode->amountOfCopies);

        newNode->amountOfRented = 0;

        amountOfDataMovie++;

        return newNode;
}

/** Funkcja wyświetla wszystkie dane znajdujące się na liście filmów do chwili napotkania wartości NULL.
    \param front Parametr wskaźnikowy typu (struct movie) na początek listy filmów.
*/
void printList(struct movie *front)
{
    while(front != NULL)
    {
            printf(" ID: %d",front->id);
            printf(" Tytul: %s",front->movieName);
            printf(" Rok produkcji: %d",front->year);
            printf(" Rezyser: %s",front->director);
            printf(" Gatunek: %s",front->type);
            printf(" Dostepne: %d",front->amountOfCopies);
            printf(" Wypozyczone: %d \n",front->amountOfRented);
            printf("------------------------------------------------------------------------\n");
        front = front->next;
    }
}

/** Funkcja porównuje ze sobą dwa napisy.
    \param s1 Parametr jest tablicą typu char przechowującą napis do porównania.
    \param s2 Parametr jest tablicą typu char przechowującą napis do porównania.
    \return Jeśli pierwszy niepasujący znak napisu pierwszego znajduje się wcześniej w alfabecie niż drugi zwraca 1, w innym przypadku 0.
*/
int compareString(char *s1, char *s2)
{
    if((strcmp(s1,s2)) < 0)
        return 1;
    else
        return 0;
}

/** Funkcja sortuje filmy rosnąco lub malejąco po polach listy.
  * Dostępny jest wybór sortowania po id, nazwie filmu, reżyserze, gatunku i roku produkcji.
  \param head_ref Wskaźnik na początek listy filmów.
  \param newNode Wskaźnik na kolejny element listy.
  \param choice Parametr oznaczający wybór użytkownika w jaki sposób funkcja ma posortować dane.
*/
void sortedList(struct movie** head_ref, struct movie* newNode, int choice)
{
    struct movie* current;

    if ((*head_ref) == NULL)
        (*head_ref) = newNode;

        else if ((*head_ref)->id >= newNode->id && choice == 0) {
        newNode->next = (*head_ref);
        newNode->next->prev = newNode;
        (*head_ref) = newNode;
        }
        else if ((*head_ref)->id <= newNode->id && choice == 1) {
        newNode->next = (*head_ref);
        newNode->next->prev = newNode;
        (*head_ref) = newNode;
        }
        else if (compareString((*head_ref)->movieName,newNode->movieName) == 0 && choice == 2) {
        newNode->next = (*head_ref);
        newNode->next->prev = newNode;
        (*head_ref) = newNode;
        }
        else if (compareString((*head_ref)->movieName,newNode->movieName) == 1 && choice == 3) {
        newNode->next = (*head_ref);
        newNode->next->prev = newNode;
        (*head_ref) = newNode;
        }
        else if (compareString((*head_ref)->director,newNode->director) == 0 && choice == 4) {
        newNode->next = (*head_ref);
        newNode->next->prev = newNode;
        (*head_ref) = newNode;
        }
        else if (compareString((*head_ref)->director,newNode->director) == 1 && choice == 5) {
        newNode->next = (*head_ref);
        newNode->next->prev = newNode;
        (*head_ref) = newNode;
        }
        else if ((*head_ref)->year >= newNode->year && choice == 6) {
        newNode->next = (*head_ref);
        newNode->next->prev = newNode;
        (*head_ref) = newNode;
        }
        else if ((*head_ref)->year <= newNode->year && choice == 7) {
        newNode->next = (*head_ref);
        newNode->next->prev = newNode;
        (*head_ref) = newNode;
        }
        else if (compareString((*head_ref)->type,newNode->type) == 0 && choice == 8) {
        newNode->next = (*head_ref);
        newNode->next->prev = newNode;
        (*head_ref) = newNode;
        }
        else if (compareString((*head_ref)->type,newNode->type) == 1 && choice == 9) {
        newNode->next = (*head_ref);
        newNode->next->prev = newNode;
        (*head_ref) = newNode;
        }

        else {
            current = (*head_ref);

            if(choice == 0)
            while (current->next != NULL && current->next->id < newNode->id)
                current = current->next;
            if(choice == 1)
            while (current->next != NULL && current->next->id > newNode->id)
                current = current->next;
            if(choice == 2)
            while (current->next != NULL && (compareString(current->next->movieName,newNode->movieName)) == 1)
                current = current->next;
            if(choice == 3)
            while (current->next != NULL && (compareString(current->next->movieName,newNode->movieName)) == 0)
                current = current->next;
            if(choice == 4)
            while (current->next != NULL && (compareString(current->next->director,newNode->director)) == 1)
                current = current->next;
            if(choice == 5)
            while (current->next != NULL && (compareString(current->next->director,newNode->director)) == 0)
                current = current->next;
            if(choice == 6)
            while (current->next != NULL && current->next->year < newNode->year)
                current = current->next;
            if(choice == 7)
            while (current->next != NULL && current->next->year > newNode->year)
                current = current->next;
            if(choice == 8)
            while (current->next != NULL && (compareString(current->next->type,newNode->type)) == 1)
                current = current->next;
            if(choice == 9)
            while (current->next != NULL && (compareString(current->next->type,newNode->type)) == 0)
                current = current->next;

            newNode->next = current->next;

            if (current->next != NULL)
                newNode->next->prev = newNode;

            current->next = newNode;
            newNode->prev = current;
        }
}

/** Funkcja obsługująca sortowanie, wywołuje funkcje sortedList().
    \param front Wskaźnik na początek listy filmów.
    \param choice Parametr oznaczający wybór użytkownika w jaki sposób funkcja ma posortować dane.
    \return Wskaźnik na nowy początek listy(po posortowaniu).
*/
struct movie *sort(struct movie *front,int choice)
{
    struct movie* sorted = NULL;
    struct movie* current = front;
    while (current != NULL) {

        struct movie* next = current->next;
        current->prev = current->next = NULL;

        sortedList(&sorted, current,choice);
        current = next;
    }
    front = sorted;
    printList(front);
    return(front);
}

/** Funkcja wyświetlająca menu wyboru z możliwościami sortowania, wywołuje funkcje sort().
    \param front Wskaźnik na początek listy filmów.
    \return Wskaźnik na nowy początek listy(po posortowaniu).
*/
struct movie *showMovieOnList(struct movie *front)
{
    struct movie *result = NULL;

    int choice = 0;

    if(front == NULL)
    {
        printf("Brak filmow w bazie... \n");
        return 0;
    }

    printf("Sortuj: \n");
    printf("[0] Sortuj wedlug id rosnaco \n");
    printf("[1] Sortuj wedlug id malejaco \n");
    printf("[2] Sortuj wedlug nazwy rosnaco \n");
    printf("[3] Sortuj wedlug nazwy malejaco \n");
    printf("[4] Sortuj wedlug rezysera rosnaco \n");
    printf("[5] Sortuj wedlug rezysera malejaco \n");
    printf("[6] Sortuj wedlug roku rosnaco \n");
    printf("[7] Sortuj wedlug roku malejaco \n");
    printf("[8] Sortuj wedlug gatunku rosnaco \n");
    printf("[9] Sortuj wedlug gatunku malejaco \n");

    scanf("%d",&choice);

    result = sort(front,choice);

    return result;
}

/** Funkcja obsługuje możliwość edytowania danych wybranego filmu.
    \param mainPointerMovie Wskaźnik na początek listy filmów.
*/
int editMovie(struct movie *mainPointerMovie)
{
    int movieId = 0;

    if(mainPointerMovie == NULL)
        return 0;

    printf("Podaj id filmu do edycji: ");
    scanf("%d",&movieId);

    while(mainPointerMovie != NULL)
    {
        if(mainPointerMovie->id == movieId)
        {
            printf("Nowy tytul: ");
            getchar();
            fgets(mainPointerMovie->movieName,MAX_MOVIE_NAME_LENGHT,stdin);

            printf("Rok produkcji: ");
            scanf("%d",&mainPointerMovie->year);

            printf("Rezyser: ");
            getchar();
            fgets(mainPointerMovie->director,MAX_DIRECTOR_LENGHT,stdin);

            printf("Gatunek: ");
            scanf("%s",&mainPointerMovie->type);

            printf("Liczba kopi: ");
            scanf("%d",&mainPointerMovie->amountOfCopies);

            return 0;
        }
        else
        {
            mainPointerMovie = mainPointerMovie->next;
        }
    }

    printf("Nie znaleziono... \n");
}

/** Funkcja wyszukuje w liście wybrany film do usuniecia poprzez podany przez użytkownika identyfikator.
    \param mainPointerMovie Wskaźnik na początek listy filmów.
    \param movieId Identyfikator filmu podany przez użytkownika.
    \return Wskaźnik na wybrany film na liscie.
*/
struct movie *findToDelete(struct movie *mainPointerMovie,int movieId)
{
    while(mainPointerMovie != NULL && mainPointerMovie->id != movieId)
        mainPointerMovie = mainPointerMovie->next;

    return mainPointerMovie;
};

/** Funkcja usuwa wybrany element z listy znajdujący się wewnątrz.
    \param node Wskaźnik na element do usunięcia z listy.
*/
void delete_within (struct movie *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    free(node);
}

/** Funkcja usuwa wybrany element z listy znajdujący się na końcu listy.
    \param back Wskaźnik na element do usunięcia z listy.
*/
void delete_back(struct movie *back)
{
    back->prev->next = NULL;
    free(back);
}

/** Funkcja obsługująca usuwanie filmu.
    \param mainPointerMovie Wskaźnik na początek listy filmów.
    \return Wskąźnik na nowy początek listy jeśli został usunięty 1 element na liście.
*/
struct movie *deleteMovie(struct movie *mainPointerMovie)
{
    int movieId = 0;
    struct movie *tmp;

    if(mainPointerMovie == NULL)
        return 0;

    printf("Podaj id filmu do usuniecia: ");
    scanf("%d",&movieId);

    if(mainPointerMovie->id == movieId)
    {
        tmp = mainPointerMovie->next;

        if(tmp->next != NULL)
            tmp->prev = NULL;

        free(mainPointerMovie);

        amountOfDataMovie--;

        return tmp;
    }

    tmp = findToDelete(mainPointerMovie,movieId);

    if(tmp != NULL)
    {
        if(tmp->next != NULL)
        delete_within(tmp);
        else
        delete_back(tmp);

        amountOfDataMovie--;
    }

    return mainPointerMovie;
}

/** Funkcja sprawdzająca czy plik movies.dat istnieje.
    \return Zwraca wartość typu bool, false jeśli plik nie istnieje, true jeśli plik istnieje.
*/
bool isMoviesExist()
{
    FILE *file;
    file = fopen("movies.dat","rb");

    if(file == NULL)
    {
        fclose(file);
        return false;
    }
    else
    {
        fclose(file);
        return true;
    }
}

/** Funkcja wyszukująca i wyświetlająca wszystkie elementy na liście które posiadają pole reżysera podane przez użytkownika.
    \param mainPointerMovie Wskaźnik na początek listy filmów.
    \param director Tablica znaków zawierająca imie i nazwisko reżysera podane przez użytkownika.
*/
void findMovieByDirector(struct movie *mainPointerMovie,char *director)
{
    int amountOfMatching = 0;

    while(mainPointerMovie != NULL)
    {
        if((strcmp(mainPointerMovie->director,director)) == 0)
        {
            printf(" ID: %d",mainPointerMovie->id);
            printf(" Tytul: %s",mainPointerMovie->movieName);
            printf(" Rezyser: %s",mainPointerMovie->director);
            printf(" Gatunek: %s",mainPointerMovie->type);
            printf(" Rok produkcji: %d \n",mainPointerMovie->year);
            printf("------------------------------------------------------------------------\n");
            amountOfMatching++;
        }

        mainPointerMovie = mainPointerMovie->next;
    }

    if(amountOfMatching == 0)
        printf("Brak filmow spelniajacych kryteria... \n");

}

/** Funkcja wyszukująca i wyświetlająca wszystkie elementy na liście które posiadają pole gatunku podane przez użytkownika.
    \param mainPointerMovie Wskaźnik na początek listy filmów.
    \param type Tablica znaków zawierająca gatunek filmu podany przez użytkownika.
*/
void findMovieByType(struct movie *mainPointerMovie,char *type)
{
    int amountOfMatching = 0;

    while(mainPointerMovie != NULL)
    {
        if((strcmp(mainPointerMovie->type,type)) == 0)
        {
            printf(" ID: %d",mainPointerMovie->id);
            printf(" Tytul: %s",mainPointerMovie->movieName);
            printf(" Rezyser: %s",mainPointerMovie->director);
            printf(" Gatunek: %s",mainPointerMovie->type);
            printf(" Rok produkcji: %d \n",mainPointerMovie->year);
            printf("------------------------------------------------------------------------\n");
            amountOfMatching++;
        }

        mainPointerMovie = mainPointerMovie->next;
    }

    if(amountOfMatching == 0)
        printf("Brak filmow spelniajacych kryteria... \n");
}

/** Funkcja wyszukująca i wyświetlająca wszystkie elementy na liście które posiadają pole roku produkcji podane przez użytkownika.
    \param mainPointerMovie Wskaźnik na początek listy filmów.
    \param year Rok produkcji filmu podany przez użytkownika.
*/
void findMovieByYear(struct movie *mainPointerMovie, int year)
{
    int amountOfMatching = 0;

    while(mainPointerMovie != NULL)
    {
        if(mainPointerMovie->year == year)
        {
            printf(" ID: %d",mainPointerMovie->id);
            printf(" Tytul: %s",mainPointerMovie->movieName);
            printf(" Rezyser: %s",mainPointerMovie->director);
            printf(" Gatunek: %s",mainPointerMovie->type);
            printf(" Rok produkcji: %d \n",mainPointerMovie->year);
            printf("------------------------------------------------------------------------\n");
            amountOfMatching++;
        }

        mainPointerMovie = mainPointerMovie->next;
    }
    if(amountOfMatching == 0)
        printf("Brak filmow spelniajacych kryteria... \n");
}

/** Funkcja obsługująca wyszukiwanie filmów po kryteriach(rok produkcji, reżyser, gatunek).
    \param mainPointerMovie Wskaźnik na początek listy filmów.
*/
int findMovie(struct movie *mainPointerMovie)
{
    int choice = 0;
    int year = 0;
    char director[MAX_DIRECTOR_LENGHT] = "";
    char type[MAX_TYPE_LENGHT] = "";

    if(mainPointerMovie == NULL)
    {
        printf("Brak filmow w bazie... \n");
        return 0;
    }

    printf("Znajdz filmy: \n");
    printf("[1] Rezyser \n");
    printf("[2] Gatunek \n");
    printf("[3] Rok produkcji \n");

    scanf("%d",&choice);

    switch(choice)
    {
        case 1:
                getchar();
                printf("Jaki rezyser: ");
                fgets(director,MAX_DIRECTOR_LENGHT,stdin);
                findMovieByDirector(mainPointerMovie,director);
            break;

        case 2:
                printf("Jaki gatunek: ");
                scanf("%s",&type);
                findMovieByType(mainPointerMovie,type);
            break;

        case 3:
                printf("Rok produkcji: ");
                scanf("%d",&year);
                findMovieByYear(mainPointerMovie,year);
            break;

        default:
                printf("Zly wybor... \n");
            return 0;
    }
}

/** Struktura przechowująca dane o klientach */
struct client
{
    int cardNumber; /**< Pole przechowujące numer karty klienta, inkrementowane automatycznie z każdym dodanym klientem. */
    char name[MAX_CLIENT_NAME_LENGHT]; /**< Pole przechowujące imie klienta. */
    char surname[MAX_CLIENT_NAME_LENGHT]; /**< Pole przechowujące nazwisko klienta. */

    int phoneNumber; /**< Pole przechowujące numer telefonu klienta. */
    char email[MAX_CLIENT_NAME_LENGHT]; /**< Pole przechowujące email klienta. */

    struct client *prev; /**< Wskaźnik na poprzedni element listy klientów. */
    struct client *next; /**< Wskaźnik na następny element listy klientów. */
};

/** Zmienna globalna pomocnicza przechowująca wskaźnik na aktualny element listy(ostatno element). */
struct client *currentPointerClient = NULL;

/** Zmienna globalna przechowująca liczbę klientów znajdujących się na liście. */
int amountOfDataClient = 0;

/** Funkcja wczytuje dane o klientach z pliku clients.dat i zapisuje je do listy.
    \param mainPointerClient Parametr wskaźnikowy na początek listy klientów.
*/
void loadDataClient(struct client **mainPointerClient)
{
    FILE *file;

    if((file = fopen("clients.dat","rb")) == NULL)
    {
        fputs("Nie mozna odczytac z pliku clients.dat \n",stderr);
        return 0;
    }

    struct client *tmp = NULL;

    while(fread(tmp=(struct client *)malloc(sizeof(struct client)),sizeof(struct client),1,file) == 1)
    {
        if(amountOfDataClient== 0)
        {
            (*mainPointerClient) = tmp;
            (*mainPointerClient)->prev = NULL;
            (*mainPointerClient)->next = NULL;

            currentPointerClient = (*mainPointerClient);
        }
        else
        {
            struct client *t = currentPointerClient;

            currentPointerClient->next = tmp;
            currentPointerClient = tmp;
            currentPointerClient->prev = t;
            currentPointerClient->next = NULL;
        }

        amountOfDataClient++;
    }

    fclose(file);
}

/** Funkcja zapisuje dane o klientach znajdujących się na liście do pliku clients.dat.
    \param mainPointerClient Parametr wskaźnikowy typu (struct client) na początek listy klientów.
*/
void saveDataClient(struct client *mainPointerClient)
{
    FILE *file;

    if((file = fopen("clients.dat","wb")) == NULL)
    {
        fputs("Nie mozna otworzyc pliku i zapisac danych - clients.dat \n",stderr);
        return 0;
    }

    struct client *tmp = mainPointerClient;

    for(int i=0; i<amountOfDataClient; i++)
    {
        fwrite(tmp,sizeof(struct client),1,file);
        tmp = tmp->next;
    }

    fclose(file);
    printf("Zapisano. \n");
}

/** Funkcja tworzy liste klientów poprzez utworzenie pierwszego elementu.
    \return Wskaźnik na nowo utworzoną strukturę z danymi.
*/
struct client *createListClient()
{
    struct client *newNode = (struct client *)malloc(sizeof(struct client));

    if(newNode != NULL)
    {
        newNode->cardNumber = amountOfDataClient;

        printf("Imie: ");
        getchar();
        fgets(newNode->name,MAX_CLIENT_NAME_LENGHT,stdin);

        printf("Nazwisko: ");
        scanf("%s",&newNode->surname);

        printf("Numer telefonu: ");
        scanf("%d",&newNode->phoneNumber);

        printf("Email: ");
        scanf("%s",&newNode->email);

        newNode->prev = newNode->next = NULL;
    }

    amountOfDataClient++;

    return newNode;
};

/** Funkcja pobiera dane(imie, nazwisko, numer telefonu, email) od użytkownika i zapisuje je w utworzonej dynamicznie strukturze.
    \return Wskaźnik na nowo utworzoną strukturę z danymi.
*/
struct client *takeDataClient()
{
        struct client *newNode = (struct client *)malloc(sizeof(struct client));

        newNode->next = NULL;

        newNode->prev = currentPointerClient;
        currentPointerClient->next = newNode;

        newNode->cardNumber = amountOfDataClient;

        printf("Imie: ");
        getchar();
        fgets(newNode->name,MAX_CLIENT_NAME_LENGHT,stdin);

        printf("Nazwisko: ");
        scanf("%s",&newNode->surname);

        printf("Numer telefonu: ");
        scanf("%d",&newNode->phoneNumber);

        printf("Email: ");
        scanf("%s",&newNode->email);


        amountOfDataClient++;

        return newNode;
}

/** Funkcja obsługuje możliwość edytowania danych wybranego klienta.
    \param mainPointerClient Wskaźnik na początek listy klientów.
*/
int editClient(struct client *mainPointerClient)
{
    int cardN = 0;

    if(mainPointerClient == NULL)
        return 0;

    printf("Podaj numer karty klienta do edycji: ");
    scanf("%d",&cardN);

    while(mainPointerClient != NULL)
    {
        if(mainPointerClient->cardNumber == cardN)
        {
            printf("Imie: ");
            getchar();
            fgets(mainPointerClient->name,MAX_CLIENT_NAME_LENGHT,stdin);

            printf("Nazwisko: ");
            scanf("%s",&mainPointerClient->surname);

            printf("Numer telefonu: ");
            scanf("%d",&mainPointerClient->phoneNumber);

            printf("Email: ");
            scanf("%s",&mainPointerClient->email);

            return 0;
        }
        else
        {
            mainPointerClient = mainPointerClient->next;
        }
    }

    printf("Nie znaleziono... \n");
}

/** Funkcja wyszukuje w liście wybranego klienta do usuniecia poprzez podany przez użytkownika numer karty.
    \param mainPointerClient Wskaźnik na początek listy klientów.
    \param cardNumber Identyfikator klienta podany przez użytkownika.
    \return Wskaźnik na wybranego klienta na liscie.
*/
struct client *findToDeleteClient(struct client *mainPointerClient,int cardNumber)
{
    while(mainPointerClient != NULL && mainPointerClient->cardNumber != cardNumber)
        mainPointerClient= mainPointerClient->next;

    return mainPointerClient;
};

/** Funkcja usuwa wybrany element z listy znajdujący się wewnątrz.
    \param node Wskaźnik na element do usunięcia z listy.
*/
void delete_withinClient(struct client *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    free(node);
}

/** Funkcja usuwa wybrany element z listy znajdujący się na końcu listy.
    \param back Wskaźnik na element do usunięcia z listy.
*/
void delete_backClient(struct client *back)
{
    back->prev->next = NULL;
    free(back);
}

/** Funkcja obsługująca usuwanie klientów.
    \param mainPointerClient Wskaźnik na początek listy klientów.
    \return Wskąźnik na nowy początek listy jeśli został usunięty 1 element na liście.
*/
struct client *deleteClient(struct client *mainPointerClient)
{
    int cardNumber = 0;
    struct client *tmp;

    if(mainPointerClient == NULL)
        return 0;

    printf("Podaj numer karty klienta do usuniecia: ");
    scanf("%d",&cardNumber);

    if(mainPointerClient->cardNumber == cardNumber)
    {
        tmp = mainPointerClient->next;

        if(tmp->next != NULL)
            tmp->prev = NULL;

        free(mainPointerClient);

        amountOfDataClient--;

        return tmp;
    }

    tmp = findToDeleteClient(mainPointerClient,cardNumber);

    if(tmp != NULL)
    {
        if(tmp->next != NULL)
        delete_withinClient(tmp);
        else
        delete_backClient(tmp);

        amountOfDataClient--;
    }

    return mainPointerClient;
}

/** Funkcja wyświetla wszystkie dane znajdujące się na liście do chwili napotkania wartości NULL.
    \param front Parametr wskaźnikowy typu (struct client) na początek listy klientów.
*/
void showClientOnList(struct client *front)
{
    if(front == NULL)
    {
        printf("Brak klientow w bazie... \n");
        return 0;
    }

    for (; front != NULL; front = front->next)
    {
        printf(" nrKarty: %d",front->cardNumber);
        printf(" Imie: %s",front->name);
        printf(" Nazwisko: %s",front->surname);
        printf(" Numer: %d",front->phoneNumber);
        printf(" Email: %s \n",front->email);
        printf("------------------------------------------------------------------------\n");
    }
}

/** Funkcja sprawdzająca czy plik clients.dat istnieje.
    \return Zwraca wartość typu bool, false jeśli plik nie istnieje, true jeśli plik istnieje.
*/
bool isClientsExist()
{
    FILE *file;
    file = fopen("clients.dat","rb");

    if(file == NULL)
    {
        fclose(file);
        return false;
    }
    else
    {
        fclose(file);
        return true;
    }
}

/** Struktura przechowująca dane o wypożyczeniach. */
struct rent
{
    int movieID; /**< Pole przechowujące identyfikator filmu. */
    int clientID; /**< Pole przechowujące numer karty klienta. */
    int dd_start; /**< Pole przechowujące dzień wypożyczenia. */
    int mm_start; /**< Pole przechowujące miesiąc wypożyczenia. */
    int yy_start; /**< Pole przechowujące rok wypożyczenia. */
    int dd; /**< Pole przechowujące dzień oddania. */
    int mm; /**< Pole przechowujące miesiąc oddania. */
    int yy; /**< Pole przechowujące rok oddania. */

    struct rent *next; /**< Wskaźnik na następny element listy wypożyczeń. */
    struct rent *prev; /**< Wskaźnik na poprzedni element listy wypożyczeń. */
};

/** Zmienna globalna przechowująca liczbę wypożyczonych filmów znajdujących się na liście. */
int amountOfDataRent = 0;

/** Zmienna globalna pomocnicza przechowująca wskaźnik na aktualny element listy(ostatno element). */
struct rent *currentPointerRent = NULL;

/** Funkcja wczytuje dane o wypożyczeniach z pliku rents.dat i zapisuje je do listy.
    \param mainPointerRent Parametr wskaźnikowy na początek listy wypożyczeń.
*/
void loadDataRent(struct rent **mainPointerRent)
{
    FILE *file;

    if((file = fopen("rents.dat","rb")) == NULL)
    {
        fputs("Nie mozna odczytac z pliku rents.dat \n",stderr);
        return 0;
    }

    struct rent *tmp = NULL;

    while(fread(tmp=(struct rent *)malloc(sizeof(struct rent)),sizeof(struct rent),1,file) == 1)
    {
        if(amountOfDataRent== 0)
        {
            (*mainPointerRent) = tmp;
            (*mainPointerRent)->prev = NULL;
            (*mainPointerRent)->next = NULL;

            currentPointerRent = (*mainPointerRent);
        }
        else
        {
            struct rent *t = currentPointerRent;

            currentPointerRent->next = tmp;
            currentPointerRent = tmp;
            currentPointerRent->prev = t;
            currentPointerRent->next = NULL;
        }

        amountOfDataRent++;
    }

    fclose(file);
}

/** Funkcja zapisuje dane o wypożyczeniach znajdujących się na liście do pliku rents.dat.
    \param mainPointerRent Parametr wskaźnikowy typu (struct rent) na początek listy wypożyczeń.
*/
void saveDataRent(struct rent *mainPointerRent)
{
    FILE *file;

    if((file = fopen("rents.dat","wb")) == NULL)
    {
        fputs("Nie mozna otworzyc pliku i zapisac danych - rents.dat \n",stderr);
        return 0;
    }

    struct rent *tmp = mainPointerRent;

    for(int i=0; i<amountOfDataRent; i++)
    {
        fwrite(tmp,sizeof(struct rent),1,file);
        tmp = tmp->next;
    }

    fclose(file);
    printf("Zapisano. \n");
}

/** Funkcja wyszukuje film o podanym przez użytkownika id, jeśli istnieje.
    \param id Parametr przechowujący identyfikator filmu podany przez użytkownika.
    \param mainPointerMovie Parametr wskaźnikowy typu (struct movie) na początek listy filmów.
    \return Wskaźnik na znaleziony film.
*/
struct movie *FindAMovieById(int id,struct movie *mainPointerMovie)
{
    if(mainPointerMovie == NULL)
        return 0;

    do
    {
        if(mainPointerMovie->id == id)
            return mainPointerMovie;

    mainPointerMovie = mainPointerMovie->next;

    }while(mainPointerMovie != NULL);

    return NULL;
}

/** Funkcja wyszukuje klienta o podanym przez użytkownika numerze karty, jeśli istnieje.
    \param id Parametr przechowujący identyfikator klienta podany przez użytkownika.
    \param mainPointerClient Parametr wskaźnikowy typu (struct client) na początek listy klientów.
    \return Wskaźnik na znalezionego klienta.
*/
struct client* FindAClientById(int id,struct client *mainPointerClient)
{
    if(mainPointerClient == NULL)
        return 0;

    do
    {
        if(mainPointerClient->cardNumber == id)
            return mainPointerClient;

        mainPointerClient = mainPointerClient->next;

    }while(mainPointerClient != NULL);

    return NULL;
}

/** Funkcja sprawdza czy dostępne są jeszcze kopie danego filmu.
    \param pointer Wskaźnik na liste wypoźyczeń.
*/
int CheckMovieAvailability(struct movie *pointer)
{
    if(pointer->amountOfCopies == 0)
    {
        return 0;
    }
    else{
        printf("Liczba kopi: %d\n",pointer->amountOfCopies);
        return 1;
    }
}

/** Funkcja oblicza date oddania filmu na podstawie dnia obecnego i ilości dni wypożyczenia.
    \param newRent Wskaźnik na liste wypoźyczeń.
    \param dni Ilość dni, na które klient wypożycza film.
*/
void CorrectingDate(struct rent *newRent,int dni)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int daysOfMonth = 0;
    int month = tm.tm_mon;

    if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
        daysOfMonth = 31;
    else if(month == 4 || month == 6 || month == 9 || month == 11)
        daysOfMonth = 30;
    else
        daysOfMonth = 28;

    if(tm.tm_mday + dni > daysOfMonth)
    {
        newRent->dd = abs(daysOfMonth-(tm.tm_mday + dni));
        newRent->mm = tm.tm_mon + 1 + 1;
        newRent->yy = tm.tm_year + 1900;
    }
    else
    {
        newRent->dd = tm.tm_mday + dni;
        newRent->mm = tm.tm_mon + 1;
        newRent->yy = tm.tm_year + 1900;
    }
}

/** Funkcja sprawdza czy podany film jest już wypożyczony przez podanego klienta.
    \param idMovie Identyfikator filmu.
    \param idClient Identyfikator klienta.
    \param mainPointerRent Wskaźnik na początek listy wypożyczeń.
*/
int CheckRent(int idMovie, int idClient, struct rent *mainPointerRent)
{
    while(mainPointerRent != NULL)
    {
        if(mainPointerRent->clientID == idClient && mainPointerRent->movieID == idMovie)
            return 1;

        mainPointerRent = mainPointerRent->next;
    }

    return 0;
}

/** Funkcja wyszukuje filmy wypożyczone przez klienta, którego podał użytkownik.
    \param mainPointerMovie Wskaźnik na początek listy filmów.
    \param mainPointerClient Wskaźnik na początek listy klientów.
    \param mainPointerRent Wskaźnik na początek listy wypożyczeń.
    \return Wskaźnik na wypożyczony element na liście.
*/
struct rent *rentMovie(struct movie *mainPointerMovie, struct client *mainPointerClient,struct rent *mainPointerRent)
{
    int idMovie;
    int idClient;
    int dni = 100;
    struct movie* tmp;


    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf("Jaki film?[id filmu]: ");
    scanf("%d",&idMovie);

    tmp = FindAMovieById(idMovie,mainPointerMovie);

    if(tmp == NULL || CheckMovieAvailability(tmp) == 0)
    {
        printf("Nie znaleziono filmu o podanym id lub brak kopi. \n");
        return NULL;
    }

    printf("Kto wyporzycza?[Numer karty klienta]: ");
    scanf("%d",&idClient);

    if((FindAClientById(idClient,mainPointerClient))==NULL)
    {
        printf("Nie znaleziono klienta o podanym numerze karty.\n");
        return NULL;
    }

    if(mainPointerRent!=NULL)
        if((CheckRent(idMovie,idClient,mainPointerRent)) == 1)
        {
            printf("Film jest juz wypozyczony przez wybranego klienta. \n");
                return NULL;
        }

        while(dni > 28)
        {
            printf("Na ile dni wypozyczany[Max 4 tygodnie]: ");
            scanf("%d",&dni);
        }


        struct rent *newRent = (struct rent *)malloc(sizeof(struct rent));

        newRent->clientID = idClient;
        newRent->movieID = tmp->id;

        newRent->dd_start = tm.tm_mday;
        newRent->mm_start = tm.tm_mon + 1;
        newRent->yy_start = tm.tm_year + 1900;

        CorrectingDate(newRent,dni);

        amountOfDataRent++;

        tmp->amountOfCopies--;
        tmp->amountOfRented++;

        newRent->prev = currentPointerRent;

        if(amountOfDataRent>1)
        currentPointerRent->next = newRent;

        newRent->next = NULL;

        return newRent;
}

/** Funkcja wyszukuje filmy wypożyczone przez klienta, którego podał użytkownik.
    \param nrKarty Identyfikator klienta podany przez użytkownika.
    \param mPointerRent Wskaźnik na początek listy wypożyczeń.
    \return 1 jeśli znaleziono filmy wypożyczone przez klienta, 0 jeśli nie znaleziono filmów wypożyczonych przez podanego klienta.
*/
int ClientMovies(int nrKarty, struct rent *mPointerRent)
{
    int finded = 0;
    struct rent *tmp = mPointerRent;

    while(tmp!=NULL)
    {
            if(nrKarty == tmp->clientID)
            {
                printf("Wypozyczone: \n");
                printf("nr filmu: %d \n",tmp->movieID);
                printf("od:%d-%d-%d \n",tmp->dd_start,tmp->mm_start,tmp->yy_start);
                printf("do:%d-%d-%d \n",tmp->dd,tmp->mm,tmp->yy);
                finded++;
            }
        tmp = tmp->next;
    }
    if(finded>0)
        return 1;
    else
        return 0;
}

/** Funkcja obsługująca usuwanie listy wypożyczeń.
    \param mainPointerRent Wskaźnik na początek listy wypożyczeń.
*/
void removeRentList(struct rent **mainPointerRent)
{
    while(NULL != *mainPointerRent)
    {
        struct rent *next = (*mainPointerRent)->next;
        free(mainPointerRent);
        *mainPointerRent = next;
    }
}

/** Funkcja wyszukująca wypożyczenia o podanym id klienta i id filmu.
    \param mainPointerRent Wskaźnik na początek listy wypożyczeń.
    \param idClient Idetyfikator klienta podany przez użytkownika.
    \param idMovie Idetyfikator filmu podany przez użytkownika.
*/
struct rent *findToDeleteRent(struct rent *mainPointerRent, int idClient, int idMovie)
{
    while(mainPointerRent != NULL && mainPointerRent->clientID != idClient && mainPointerRent->movieID != idMovie)
        mainPointerRent= mainPointerRent->next;

    return mainPointerRent;
};

/** Funkcja usuwa wybrany element z listy znajdujący się wewnątrz.
    \param node Wskaźnik na element do usunięcia z listy.
*/
void delete_withinRent(struct rent*node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    free(node);
}

/** Funkcja usuwa wybrany element z listy znajdujący się na końcu listy.
    \param back Wskaźnik na element do usunięcia z listy.
*/
void delete_backRent(struct rent *back)
{
    back->prev->next = NULL;
    free(back);
}

/** Funkcja obsługująca oddawanie filmu przez klienta. Usuwa elementy z listy wypożyczeń.
    \param mainPointerRent Wskaźnik na początek listy wypożyczeń.
    \param mainPointerMovie Wskaźnik na początek listy filmów.
    \return Wskąźnik na nowy początek listy jeśli został usunięty 1 element na liście.
*/
struct rent *giveBackMovie(struct rent *mainPointerRent, struct movie *mainPointerMovie)
{
    int idClient;
    int idMovie;

    if(mainPointerRent == NULL)
    {
        printf("Baza wypozyczen jest pusta... \n");
            return mainPointerRent;
    }

    printf("Kto oddaje?: ");
    scanf("%d",&idClient);

    if((ClientMovies(idClient,mainPointerRent)) == 0)
    {
        printf("Brak wypozyczonych filmow... \n");
        return mainPointerRent;
    }

    printf("Jaki film?: ");
    scanf("%d",&idMovie);

    struct movie *tmpMovie;
    tmpMovie = FindAMovieById(idMovie,mainPointerMovie);

    if(mainPointerRent->clientID == idClient && mainPointerRent->movieID == idMovie)
    {
        struct rent *next = mainPointerRent->next;

        if(next != NULL)
        next->prev = NULL;

        free(mainPointerRent);

        amountOfDataRent--;

        tmpMovie->amountOfCopies++;
        tmpMovie->amountOfRented--;
        printf("Oddano... \n");

        return next;
    }
    else
    {
        printf("Nie znaleziono... \n");
        return mainPointerRent;
    }

    struct rent *tmp = findToDeleteRent(mainPointerRent,idClient,idMovie);

    if(tmp != NULL)
    {
        if(tmp->next != NULL)
        delete_withinRent(tmp);
        else
        delete_backRent(tmp);

        amountOfDataRent--;
    }
    else
    {
        printf("Nie znaleziono... \n");
        return mainPointerRent;
    }

    tmpMovie->amountOfCopies++;
    tmpMovie->amountOfRented--;
    printf("Oddano... \n");

    return mainPointerRent;
}

/** Funkcja wyświetla wszystkie dane znajdujące się na liście wypożyczeń do chwili napotkania wartości NULL.
    \param node Parametr wskaźnikowy typu (struct rent) na początek listy wypożyczeń.
*/
void showRentOnList(struct rent *node)
{
    if(node == NULL)
    {
        printf("Baza wypozyczen jest pusta... \n");
            return 0;
    }

    for (; node != NULL; node = node->next)
    {
        printf("Nr karty klienta: %d \n",node->clientID);
        printf("ID filmu: %d \n",node->movieID);
        printf("od:%d-%d-%d \n",node->dd_start,node->mm_start,node->yy_start);
        printf("do:%d-%d-%d \n",node->dd,node->mm,node->yy);
    }
}

/** Funkcja sprawdzająca czy plik rents.dat istnieje.
    \return Zwraca wartość typu bool, false jeśli plik nie istnieje, true jeśli plik istnieje.
*/
bool isRentsExist()
{
    FILE *file;
    file = fopen("rents.dat","rb");

    if(file == NULL)
    {
        fclose(file);
        return false;
    }
    else
    {
        fclose(file);
        return true;
    }
}

/** Funkcja obsługująca wyświetlanie przypomnień o zaległych zwrotach. Oblicza różnice między dniem dzisiejszym, a zapisaną datą zwrotu.
    \param mainPointerRent Wskaźnik na początek listy wypożyczeń.
*/
void computeTimeDifference(struct rent *mainPointerRent)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    int day2 = tm.tm_mday;
    int mon2 = tm.tm_mon + 1;
    int year2 = tm.tm_year + 1900;

    int sum2 = day2 * 1 + mon2 * 30 + year2 * 365;

    while(mainPointerRent != NULL)
    {
        int day1 = mainPointerRent->dd;
        int mon1 = mainPointerRent->mm;
        int year1 = mainPointerRent->yy;

    int sum1 = day1 * 1 + mon1 * 30 + year1 * 365;

    if(sum1<sum2)
        if(day2 < day1)
        {
            if(mon2 == 2)
                day2 += 28;
            else if (mon2 == 4 || mon2 == 6 || mon2 == 9 || mon2 == 11)
                day2 += 30;
            else
                day2 += 31;

            mon2 = mon2 - 1;
        }

        if (mon2 < mon1)
        {
            mon2 += 12;
            year2 -= 1;
        }

        int day_diff = day2 - day1;
        int mon_diff = mon2 - mon1;
        int year_diff = year2 - year1;

        if(day_diff > 0 || mon_diff > 0 || year_diff > 0)
        {
        printf("Termin oddania przekroczony--------------------!!!\n");
        printf("Klient id: %d",mainPointerRent->clientID);
        printf(" Film id: %d \n",mainPointerRent->movieID);
        if(day_diff>0)
        printf("o %d day/days",day_diff);
        if(mon_diff>0)
        printf("    %d month/months",mon_diff);
        if(year_diff>0)
        printf("    %d year/years",year_diff);

        printf("\nKara 2zl za kazdy dzien zwloki: %dzl(razem) \n",2*(day_diff * 1 + mon_diff * 30 + year_diff * 365));

        printf("\n");
        }

    mainPointerRent = mainPointerRent->next;

    }

    return 0;
}

/** Funkcja obsługująca wyświetlanie przypomnień o zaległych zwrotach filmów.
    \param mainPointerRent Wskaźnik na początek listy wypożyczeń.
*/
int notifications(struct rent *mainPointerRent)
{
    if(mainPointerRent==NULL)
        return 0;

   computeTimeDifference(mainPointerRent);
}

int main()
{
    int menu = -1;

    struct movie *mainPointerMovie = NULL;
    struct client *mainPointerClient = NULL;
    struct rent *mainPointerRent = NULL;

    if(isMoviesExist() == true)
    loadDataMovie(&mainPointerMovie);

    if(isClientsExist() == true)
    loadDataClient(&mainPointerClient);

    if(isRentsExist() == true)
    loadDataRent(&mainPointerRent);

    notifications(mainPointerRent);

    while(menu != 0)
    {
        printf("[1] Filmy \n");
        printf("[2] Klienci \n");
        printf("[3] Wypozyczenia \n");
        printf("[0] Zamknij program i zapisz \n");

        scanf("%d",&menu);

        if(menu == 0)
        {
            saveDataMovie(mainPointerMovie);
            saveDataClient(mainPointerClient);
            saveDataRent(mainPointerRent);

            printf("Zamykanie... \n");
            return 0;
        }
        if(menu == 1)
        {
            while(menu != 6)
            {
            printf("######################################################################## \n");
            printf("[1] Dodaj film \n");
            printf("[2] Edytuj film \n");
            printf("[3] Usun film \n");
            printf("[4] Wyswietl filmy \n");
            printf("[5] Wyszukaj film \n");
            printf("[6] Powrot... \n");
            printf("######################################################################## \n");

            scanf("%d",&menu);

            switch(menu)
            {
                case 1:
                        if(amountOfDataMovie == 0)
                            currentPointerMovie = mainPointerMovie = createListMovie();
                        else
                            currentPointerMovie = takeDataMovie();

                        break;

                case 2:
                        editMovie(mainPointerMovie); break;

                case 3:
                        mainPointerMovie = deleteMovie(mainPointerMovie); break;

                case 4:
                        mainPointerMovie = showMovieOnList(mainPointerMovie); break;

                case 5: findMovie(mainPointerMovie); break;

                case 6: break;

                default: printf("Niepoprawny wybor... \n");

            }
        }
    }

        if(menu == 2)
        {
            while(menu != 5)
            {
            printf("######################################################################## \n");
            printf("[1] Dodaj klienta \n");
            printf("[2] Edytuj klienta \n");
            printf("[3] Usun klienta \n");
            printf("[4] Wyswietl klienta \n");
            printf("[5] Powrot... \n");
            printf("######################################################################## \n");

            scanf("%d",&menu);

            switch(menu)
            {
            case 1:
                    if(amountOfDataClient == 0)
                        currentPointerClient = mainPointerClient = createListClient();
                    else
                        currentPointerClient = takeDataClient();

                    break;

            case 2:
                    editClient(mainPointerClient);
                    break;

            case 3:
                    mainPointerClient = deleteClient(mainPointerClient);
                    break;

            case 4: showClientOnList(mainPointerClient); break;

            case 5: break;

            default: printf("Niepoprawny wybor... \n");

            }
        }
    }

        if(menu == 3)
        {
            while(menu != 4)
            {
            printf("######################################################################## \n");
            printf("[1] Wypozycz \n");
            printf("[2] Oddaj film \n");
            printf("[3] Sprawdz wypozyczone \n");
            printf("[4] Powrot \n");
            printf("######################################################################## \n");

            scanf("%d",&menu);

            switch(menu)
            {
            case 1:
                    if(amountOfDataRent == 0)
                        currentPointerRent = mainPointerRent = rentMovie(mainPointerMovie,mainPointerClient,mainPointerRent);
                    else
                        currentPointerRent = rentMovie(mainPointerMovie,mainPointerClient,mainPointerRent);

                    break;

            case 2: mainPointerRent =giveBackMovie(mainPointerRent,mainPointerMovie); break;

            case 3: showRentOnList(mainPointerRent); break;

            case 4: break;

            default: printf("Niepoprawny wybor... \n");

            }
            }
        }
    }
}
