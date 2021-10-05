#include "gps_functions.h"

/*************************/
/* Adds points to matrix */
/*************************/

char read_borders(char file_name[], char border_name[N_BORDERS][SIZE], float angular_coordinates[N_BORDERS][N_POINTS_BORDER][2], float final_coordinates[N_BORDERS][N_POINTS_BORDER][2])
{

    FILE *fp;
    int a, b;
    int convertions;
    int n_lines;
    char line[SIZE];
    float latitude, longitude;

    a = 0;

    fp = fopen(file_name, "r"); /* Opens the file */

    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return 0;
    }

    else
    {
        do
        {
            fgets(line, N_BORDERS, fp);

            if (line[0] == 'F')               /* Border name. */
                strcpy(border_name[a], line); /* Copies line content to vector. */

            else
            {
                convertions = sscanf(line, "%f %f", &latitude, &longitude);

                if (convertions == 2) /* Latitude and Longitude of the points. */
                {
                    for (b = 0; b < 4; b = b + 1)
                    {
                        sscanf(line, "%f %f", &angular_coordinates[a][b][0], &angular_coordinates[a][b][1]);
                        fgets(line, SIZE, fp);
                    }
                    a = a + 1;
                }
            }

            if (feof(fp)) /* End at EOF. */
                n_lines = -1;
        } while (n_lines != -1);
    }
    do
    {
        for (a = 0; a < 52; a++)
        {
            for (b = 0; b < 4; b++)
            {
                final_coordinates[a][b][0] = 6371 * tan(PI * (angular_coordinates[a][b][0] - IST_LATITUDE) / 180);
                final_coordinates[a][b][1] = 6371 * cos(PI * angular_coordinates[a][b][0] / 180) * tan(PI * (angular_coordinates[a][b][1] - IST_LONGITUDE) / 180);

                /* Assuming University of Lisbon - Instituto Superior Técnico as the center. */
            }

            if (feof(fp))
                n_lines = -2;
        }
    } while (n_lines != -2);

    fclose(fp);

    return 0;
}

/*************************************/
/* Draws map, zooms in ...           */
/*************************************/

void draw_map(float final_coordinates[N_BORDERS][N_POINTS_BORDER][2], float ampliation, char border_name[N_BORDERS][SIZE], int argc, char *argv[], POINT *top, POINT *top2, LIST *x, char *resp, int center_x, int center_y, int time)
{

    char district[25];
    char color[25];
    char district1[25];
    char district2[25];
    int i, j, k, l, m;
    int d = g2_open_X11(500, 1000);
    int dev = g2_open_vd();
    int color_of_borders = 0;
    POINT *aux;
    LIST *auxe;

    /* Detect chosen district. */
    for (k = 0; k < argc; k++)
    {
        if (strcmp(argv[k], "-d") == 0)
        {
            strcpy(district, argv[k + 1]);
        }
    }

    /* Detect chosen zoom. */
    for (k = 0; k < argc; k++)
    {
        if (strcmp(argv[k], "-a") == 0)
        {
            ampliation = atof(argv[k + 1]);
        }
    }

    /* Detect chosen color. */
    for (k = 0; k < argc; k++)
    {
        if (strcmp(argv[k], "-c") == 0)
        {
            strcpy(color, argv[k + 1]);
            color_of_borders = border_color(color);
        }
    }

    g2_clear(d);
    g2_set_coordinate_system(d, center_x, center_y, ampliation, ampliation);
    g2_attach(d, dev);
    g2_set_auto_flush(d, 0);
    /*  g2_move_r (d, 41254265766, 46552457532); */
    g2_set_line_width(d, 3);
    g2_set_background(d, 1); /* Background color is black (DARK MODE). */
    g2_pen(d, 0);
    g2_set_font_size(d, 35);

    printf("\n\tO g2 esta agora aberto.\n");

    for (l = 0; l < 53; l++)
    {
        for (m = 0; m < 3; m++)
        {
            sscanf(border_name[l], "F (%[^)]) (%[^)])", district1, district2);

            if ((strcmp(district, district1) == 0) || ((strcmp(district, district2) == 0)))
                g2_pen(d, color_of_borders); /* If the district seeked in the border_name matrix is found, it's drawn with the desired color (supposedly) */

            else
                g2_pen(d, 0); /* Otherwise, the map is drawn normally, with the specified color (white). */

            g2_line(d, final_coordinates[l][m][1], final_coordinates[l][m][0], final_coordinates[l][m + 1][1], final_coordinates[l][m + 1][0]);
        }
        g2_flush(d);
    }

    if (argc == 1)
    {
        printf("\n How to highlight a district: \n\t./gps2013 -c vermelho -a 1.2 -d Viana_do_Castelo\n\tThe order of the commands is not important. The numbers represent the zoom value, colour and name of the district to highlight.");

        g2_pen(d, 0);

        for (i = 0; i < 53; i++)
        {
            for (j = 0; j < 3; j++)
            {
                g2_line(d, final_coordinates[i][j][1], final_coordinates[i][j][0], final_coordinates[i][j + 1][1], final_coordinates[i][j + 1][0]);
            }
        }
        g2_flush(d);
    }

    if (strcmp(resp, "Sim") == 0)
    { /* If yes, draw two routes, onde from destination to int, the other from int to the destination */
        aux = top;
        aux = aux->prox;

        while (aux != NULL)
        {
            aux->long1 = 6371 * cos(PI * aux->lat1 / 180) * tan(PI * (aux->long1 - IST_LONGITUDE) / 180);
            aux->lat1 = 6371 * tan(PI * (aux->lat1 - IST_LATITUDE) / 180);
            aux = (*aux).prox;
        }

        top = top->prox;
        while (top->prox != NULL)
        {
            g2_pen(d, 19);
            g2_line(d, top->long1, top->lat1, top->prox->long1, top->prox->lat1);
            g2_flush(d);
            top = (*top).prox;
        }

        aux = top2;
        aux = aux->prox;

        while (aux != NULL)
        {
            aux->long1 = 6371 * cos(PI * aux->lat1 / 180) * tan(PI * (aux->long1 - IST_LONGITUDE) / 180);
            aux->lat1 = 6371 * tan(PI * (aux->lat1 - IST_LATITUDE) / 180);
            aux = (*aux).prox;
        }

        top2 = top2->prox;
        while (top2->prox != NULL)
        {
            g2_pen(d, 19);
            g2_line(d, top2->long1, top2->lat1, top2->prox->long1, top2->prox->lat1);
            g2_flush(d);
            top2 = (*top2).prox;
        }
    }

    if (strcmp(resp, "Nao") == 0)
    {
        aux = top;
        aux = aux->prox;

        while (aux != NULL)
        {
            aux->long1 = 6371 * cos(PI * aux->lat1 / 180) * tan(PI * (aux->long1 - IST_LONGITUDE) / 180);
            aux->lat1 = 6371 * tan(PI * (aux->lat1 - IST_LATITUDE) / 180);
            aux = (*aux).prox;
        }

        top = top->prox; /* If not, draw a rout from the beggining to the end */
        while (top->prox != NULL)
        {
            g2_pen(d, 19);
            g2_line(d, top->long1, top->lat1, top->prox->long1, top->prox->lat1);
            g2_flush(d);
            top = (*top).prox;
        }
    }

    auxe = x->prox;
    g2_pen(d, 22);
    while (auxe->prox != NULL)
    {
        auxe->a.long1 = 6371 * cos(PI * auxe->a.lat1 / 180) * tan(PI * (auxe->a.long1 - IST_LONGITUDE) / 180);
        auxe->a.lat1 = 6371 * tan(PI * (auxe->a.lat1 - IST_LATITUDE) / 180);
        auxe->a.long2 = 6371 * cos(PI * auxe->a.lat2 / 180) * tan(PI * (auxe->a.long2 - IST_LONGITUDE) / 180);
        auxe->a.lat2 = 6371 * tan(PI * (auxe->a.lat2 - IST_LATITUDE) / 180);

        if ((*auxe).a.type1 == 'F')
        { /* Draws points in town coordinates, with different colors */
            g2_pen(d, 3);
            g2_filled_circle(d, auxe->a.long1, auxe->a.lat1, 1);
        }

        if ((*auxe).a.type2 == 'F')
        {
            g2_pen(d, 3);
            g2_filled_circle(d, auxe->a.long2, auxe->a.lat2, 1);
        }

        if ((*auxe).a.type1 == 'C')
        {
            g2_pen(d, 4);
            g2_filled_circle(d, auxe->a.long1, auxe->a.lat1, 2);
        }

        if ((*auxe).a.type2 == 'C')
        {
            g2_pen(d, 4);
            g2_filled_circle(d, auxe->a.long2, auxe->a.lat2, 2);
        }

        if ((*auxe).a.type1 == 'D')
        {
            g2_pen(d, 22);
            g2_filled_circle(d, auxe->a.long1, auxe->a.lat1, 4);
        }

        if ((*auxe).a.type2 == 'D')
        {
            g2_pen(d, 22);
            g2_filled_circle(d, auxe->a.long2, auxe->a.lat2, 4);
        }

        auxe = auxe->prox;
    }

    g2_pen(d, 0);
    g2_line(d, 6371 * cos(PI * 37.166737 / 180) * tan(PI * (-7.392426 - IST_LONGITUDE) / 180), 6371 * tan(PI * (37.166737 - IST_LATITUDE) / 180), 6371 * cos(PI * 37.522652 / 180) * tan(PI * (-7.510529 - IST_LONGITUDE) / 180), 6371 * tan(PI * (37.522652 - IST_LATITUDE) / 180));
    /* Desperade measure to correct a border which never showed up */

    g2_flush(d);
    sleep(time); /* Holds the window */
    g2_close(d);
}

/******************/
/* Manages colors */
/******************/

int border_color(char cor[25])
{
    if (strcmp(cor, "black") == 0)
    {
        printf("\n\nThe chosen color is not valid. Choose between green, yellow, red, blue, orange or pink\n\n");
    }

    if (strcmp(cor, "red") == 0)
    {
        return 19;
    }

    if (strcmp(cor, "blue") == 0)
    {
        return 3;
    }

    if (strcmp(cor, "green") == 0)
    {
        return 4;
    }

    if (strcmp(cor, "yellow") == 0) 
    {
        return 25;
    }

    if (strcmp(cor, "orange") == 0)
    {
        return 22;
    }

    if (strcmp(cor, "pink") == 0)
    {
        return 23;
    }

    return 0;
}

LIST *create_base()
{
    LIST *top;
    top = (LIST *)malloc(sizeof(LIST));
    strcpy((*top).a.place_name1, "\0");
    strcpy((*top).a.place_name2, "\0");
    strcpy((*top).a.road, "\0");
    (*top).a.lat1 = 0; /* Creates an empty base of type LIST */
    (*top).a.lat2 = 0;
    (*top).a.long1 = 0;
    (*top).a.long2 = 0;
    (*top).a.type1 = ' ';
    (*top).a.type2 = ' ';
    (*top).prox = NULL;

    return top;
}

void add_road(LIST *base, char *place_name1, char *place_name2, char *road, float lat1, float long1, float lat2, float long2, char type1, char type2)
{
    LIST *aux, *ptr;
    aux = base;
    if (aux == NULL)
        printf("Error on base of list");
    while ((*aux).prox != NULL)
    {
        aux = (*aux).prox;
    }

    ptr = (LIST *)malloc(sizeof(LIST));
    strcpy((*ptr).a.place_name1, place_name1); /* Adds a new road and an initial list of roads */
    strcpy((*ptr).a.place_name2, place_name2);
    strcpy((*ptr).a.road, road);
    (*ptr).a.lat1 = lat1;
    (*ptr).a.lat2 = lat2;
    (*ptr).a.long2 = long2;
    (*ptr).a.long1 = long1;
    (*ptr).a.type1 = type1;
    (*ptr).a.type2 = type2;
    (*ptr).prox = NULL;
    (*aux).prox = ptr;
}

LIST *from_file_to_list()
{
    char linha[100];
    char nl1[MAXIMUM];
    char nl2[MAXIMUM];
    char etd[MAXIMUM];
    float lt1, lg1, lt2, lg2; /* Fetches data from file and adds to list of roads using the previous function */
    char type1, type2;        
    LIST *base;
    FILE *fp;
    char *p;
    if ((fp = fopen("roads_towns.txt", "r")) == NULL)
        printf("Error reading file\n");

    base = create_base();
    do
    {
        p = fgets(linha, 100, fp);
        sscanf(linha, "%c %s %f %f %s %c %s %f %f", &type1, nl1, &lt1, &lg1, etd, &type2, nl2, &lt2, &lg2);
        /*if (sscanf(linha,"%c %s %f %f %s %c %s %f %f",&type1,nl1,&lt1,&lg1,etd,&type2,nl2,&lt2,&lg2)!=9)
            {
                printf("\nCorrupted file.\n");
                exit(0);
            }*/

        add_road(base, nl1, nl2, etd, lt1, lg1, lt2, lg2, type1, type2);
    } while (p != NULL);

    return base;
}

void print_list(LIST *base)
{

    LIST *aux; /* Prints list (was used for tests fusing the development) */
    aux = base;

    aux = (*aux).prox;
    while (aux != NULL)
    {
        printf("%s %f %f\n%s\n%s %f %f\n\n\n", (*aux).a.place_name1, (*aux).a.lat1, (*aux).a.long1, (*aux).a.road, (*aux).a.place_name2, (*aux).a.lat2, (*aux).a.long2);
        aux = (*aux).prox;
    }
}

float calculate_distance(float lat1, float long1, float lat2, float long2)
{
    float distance;
    distance = sqrt((lat1 - lat2) * (lat1 - lat2) + (long1 - long2) * (long1 - long2));

    return distance;
}

TOWN *create_top()
{
    TOWN *top;
    top = (TOWN *)malloc(sizeof(TOWN)); /* Creates an empty base of type TOWN (this list will be used to, e.g. */
    strcpy((*top).road, "\0");          /* choosing LISBOA as the orgin, store all towns connected to LISBOA */
    strcpy((*top).nome, "\0");          /* these locations will the be ordered based on their distance to the destination */
    (*top).lat1 = 0;                    /* and the one with the smallest distance will be turned into a POINT */
    (*top).distance = 0;
    (*top).long1 = 0;
    (*top).prox = NULL;
    return top;
}

void add_town(TOWN *top, char *nome1, char *road, float lat1, float long1, float latd, float longd)
{

    TOWN *aux, *ptr;
    float distance;
    aux = top;
    if (aux == NULL)
        printf("Something is not right on the base of the list.\n"); /* Adds towns that, in this case, connect to LISBOA */

    while ((*aux).prox != NULL)
    {
        aux = (*aux).prox;
    }

    ptr = (TOWN *)malloc(sizeof(TOWN));
    strcpy((*ptr).nome, nome1);
    strcpy((*ptr).road, road);
    (*ptr).lat1 = lat1;
    (*ptr).long1 = long1;
    (*ptr).distance = calculate_distance(lat1, long1, latd, longd);
    (*ptr).prox = NULL;

    (*aux).prox = ptr;
}

TOWN *search_for_roads(char *nomeo, LIST *base, float latd, float longd)
{
    LIST *aux;
    TOWN *top;
    aux = base;
    top = create_top();
    /* With the previous example, looks for all towns that connect to LISBOA */
    while ((*aux).prox != NULL) /* And uses the previous function to add to this intermediate list of towns */
    {

        aux = (*aux).prox;

        if (strcmp(nomeo, (*aux).a.place_name1) == 0)
        {
            add_town(top, (*aux).a.place_name2, (*aux).a.road, (*aux).a.lat2, (*aux).a.long2, latd, longd);
        }

        if (strcmp(nomeo, (*aux).a.place_name2) == 0)
        {
            add_town(top, (*aux).a.place_name1, (*aux).a.road, (*aux).a.lat1, (*aux).a.long1, latd, longd);
        }
    }
    return top;
}

POINT *points_destination_or_origins(LIST *base, char *nomed)
{
    LIST *aux;
    POINT *destino;
    aux = base;
    destino = (POINT *)malloc(sizeof(POINT));

    while ((*aux).prox != NULL)
    {

        aux = (*aux).prox;

        if (strcmp(nomed, (*aux).a.place_name1) == 0)
        {
            (*destino).lat1 = (*aux).a.lat1;
            (*destino).long1 = (*aux).a.long1;
            (*destino).prox = NULL;
            return destino;
            break;
        }
        if (strcmp(nomed, (*aux).a.place_name2) == 0)
        {
            (*destino).lat1 = (*aux).a.lat2;
            (*destino).long1 = (*aux).a.long2;
            (*destino).prox = NULL;
            return destino;
            break;
        }
    }
}

void print_town(TOWN *base)
{

    TOWN *aux; /* Prints the list of points that will then be united. Not used */
    aux = base;

    while (aux != NULL)
    {
        printf("%s %f %f\n\n", (*aux).nome, (*aux).lat1, (*aux).long1);
        aux = (*aux).prox;
    }
}

TOWN *order_town(TOWN *top)
{
    TOWN *aux1, *aux2, *aux3;
    aux1 = top;
    aux2 = top;
    aux3 = top; /* Orders the intermediate list of towns */
    aux2 = (*aux1).prox;

    if ((*aux1).prox == NULL)
        printf("Town with no roads");

    if ((*aux2).prox == NULL)
        printf("Town with a single road");

    else
    {
        aux3 = (*aux2).prox;

        while (aux3 != NULL)
        {

            if ((*aux2).distance > (*aux3).distance)
            {
                (*aux1).prox = aux3;
                (*aux2).prox = (*aux3).prox;
                (*aux3).prox = aux2;
                aux1 = top;
                aux2 = (*aux1).prox;
                aux3 = (*aux2).prox;
            }

            else
            {
                aux1 = (*aux1).prox;
                aux2 = (*aux2).prox;
                aux3 = (*aux3).prox;
            }
        }
    }
    return top;
}

POINT *create_inicio()
{
    POINT *top;
    top = (POINT *)malloc(sizeof(POINT));
    (*top).lat1 = 0; /* Creates an empty base for the list of points to unite */
    (*top).long1 = 0;
    (*top).prox = NULL;
    return top;
}

char *adiciona_point(POINT *top_points, TOWN *top_town)
{
    POINT *aux, *ptr;
    TOWN *p;
    p = top_town;
    aux = top_points;

    while ((*aux).prox != NULL)
        aux = (*aux).prox; /* Adds a point to the list of points to unite */

    p = (*p).prox;
    ptr = (POINT *)malloc(sizeof(POINT));
    (*ptr).lat1 = (*p).lat1;
    (*ptr).long1 = (*p).long1;
    (*ptr).prox = NULL;
    (*aux).prox = ptr;
    return (*p).nome;
}

POINT *main_list_funcion(char *nomeo, char *nomed, LIST *base)
{

    POINT *point_destino, *point_origem, *top_list_points; /* Main function. Repeats the process explained */
    char novaorigem[MAXIMUM];                              /* previously with the LISBOA example for the */
    TOWN *top_temporary;                                   /* following points, until the destination is reached */


    top_temporary = create_top();
    point_destino = points_destination_or_origins(base, nomed);
    point_origem = points_destination_or_origins(base, nomeo);
    top_list_points = create_inicio();
    (*top_list_points).prox = point_origem;

    while (1)
    {
        TOWN *top_temporary, *aux;
        int a = 0;

        top_temporary = search_for_roads(nomeo, base, (*point_destino).lat1, (*point_destino).long1);
        top_temporary = order_town(top_temporary);
        aux = top_temporary;
        aux = (*aux).prox;

        strcpy(nomeo, adiciona_point(top_list_points, top_temporary));

        printf("Siga pela %s até ", (*aux).road);
        printf("%s.\n", (*aux).nome);

        if ((*aux).distance == 0)
        {
            break;
        }

        (*top_temporary).prox = NULL;

        /*  a++;
        if (a>100)
            {
                printf("\nRoute error.\n");
                return (0);
            }*/
    }
    return top_list_points;
}

/*
void print_points(POINT*base)
{

    POINT*aux;
    aux=base;


    while(aux!=NULL)
    {
        printf("%f %f\n\n",(*aux).lat1,(*aux).long1);
        aux=(*aux).prox;
    }
}

*/

char *put_parentesis(char *town)
{
    char string[100], copy[100];
    strcpy(copy, town); /* Adds parentheses. This way, if we ask for LISBOA as destination, the program will seek (LISBOA) in the file */
    strcpy(town, "(");
    strcat(town, copy);
    strcat(town, ")");
    return town;
}

int no_more_segmentation_fault(LIST *base, char *nome)
{
    LIST *aux;
    aux = base;

    while (aux != NULL)
    {
        if (strcmp((*aux).a.place_name1, nome) == 0 || strcmp((*aux).a.place_name2, nome) == 0)
        {
            return 0;
        }

        aux = (*aux).prox;
    }

    printf("Invalid location. Exiting. \n");
    exit(0);
}
