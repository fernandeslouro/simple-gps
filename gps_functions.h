
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <g2.h>
#include <g2_X11.h>


#define PI 3.14159265359
#define SIZE 100
#define N_BORDERS 53
#define N_POINTS_BORDER 4
#define IST_LATITUDE 38.736663
#define IST_LONGITUDE -9.139316
#define MAXIMUM 100




typedef struct road
{
    char place_name1[MAXIMUM];
    char place_name2[MAXIMUM];
    char road[MAXIMUM];
    float lat1, long1, lat2, long2;
    char type1,type2;
}ROAD;

typedef struct list
{
    ROAD a;
    struct list *prox;
}LIST;

typedef struct town
{
    char nome[MAXIMUM];
    float lat1, long1, distance;
    char road[MAXIMUM];
    struct town * prox;
}TOWN;

typedef struct point
{
    float lat1,long1;
    struct point*prox;
} POINT;


LIST * create_base();
void add_road(LIST * base, char * place_name1, char * place_name2, char * road, float lat1, float long1, float lat2, float long2,char type1,char type2);
LIST * from_file_to_list();
void print_list(LIST*base);
float calculate_distance(float lat1,float long1,float lat2,float long2);
TOWN * create_top();
void add_town(TOWN*top,char * nome1,char*road,float lat1,float long1,float latd,float longd);
TOWN * search_for_roads(char*nomeo, LIST* base, float latd,float longd);
POINT * points_destination_or_origins (LIST* base,char*nomed);
void print_town(TOWN*base);
TOWN * order_town(TOWN * top);
POINT * create_inicio();
void print_points(POINT*base);
POINT * main_list_funcion(char * nomeo,char * nomed,LIST * base);
char * put_parentesis (char* town);
int no_more_segmentation_fault(LIST*base,char*nome);
char read_borders(char file_name[], char border_name[N_BORDERS][SIZE], float angular_coordinates[N_BORDERS][N_POINTS_BORDER][2], float final_coordinates[N_BORDERS][N_POINTS_BORDER][2]);
void draw_map(float final_coordinates[N_BORDERS][N_POINTS_BORDER][2], float ampliation, char border_name[N_BORDERS][SIZE], int argc, char * argv[], POINT*top, POINT*top2, LIST*x, char*resp, int center_x, int center_y, int time);
int border_color(char cor[25]);




