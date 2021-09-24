#include "gps_functions.h"

int main(int argc, char *argv[])
{
    char border_name[N_BORDERS][SIZE];                        /* Reads up to 100 chars per line (to make sure all information is captured */
    float angular_coordinates[N_BORDERS][N_POINTS_BORDER][2]; /* 4 points from each border and 2 coordinates from each point */
    float final_coordinates[N_BORDERS][N_POINTS_BORDER][2];   /* Planar coordinates */
    float ampliation = 0.9;
    char nomed[MAXIMUM], nomei[MAXIMUM], nomeo[MAXIMUM], resp[20], resp2[20], resp3[20], ch;
    POINT *point, *point2;
    TOWN *top;
    LIST *base;
    int time;

    /*print_list(base);*/

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\nAeroLoc Touristic GPS - For a quick and tranquil journey through beautiful Portugal...\n");
    while (1)
    {

        base = from_file_to_list();
        printf("\nSpecify your current location:\n");
        scanf("%s", nomeo);
        strcpy(nomeo, put_parentesis(nomeo));
        no_more_segmentation_fault(base, nomeo);

        printf("\nSpecify your travel destination:\n");
        scanf("%s", nomed); /*Saves destinations*/
        strcpy(nomed, put_parentesis(nomed));
        no_more_segmentation_fault(base, nomed); /*Prevents segmentations fault when unknown destinations are provided*/

        if (strcmp(nomeo, nomed) == 0)
        {
            puts("Current location is the same as the destination.\n");
            return 0;
        }

        printf("\nDurante a sua viagem, há algum lugar em especial que gostaria de visitar? Sim ou Nao?\n");
        scanf("%s", resp);

        if (strcmp(resp, "Yes") == 0)
        {
            printf("Which town would you like to visit your way to ...?\n");
            scanf("%s", nomei);
            strcpy(nomei, put_parentesis(nomei));
            no_more_segmentation_fault(base, nomei);

            point = main_list_funcion(nomeo, nomei, base);
            point2 = main_list_funcion(nomei, nomed, base);
        }

        if (strcmp(resp, "No") == 0)
        {
            point = main_list_funcion(nomeo, nomed, base);
        }

        if (strcmp(resp, "Yes") != 0 && strcmp(resp, "No") != 0)
        {
            printf("Please answer 'Yes' or 'No'\n");
            exit(0);
        }

        printf("\n\n\n\n\n");

        /*print_points(point);
        print_points(point2);*/

        /* File data to matrix */
        /* Reads matrix with coordinate data and creates the map */
        read_borders("fronteiras_distritos.txt", border_name, angular_coordinates, final_coordinates); 

        printf("\nHow many seconds would you like to look at the map before inputting a new route?\n");
        scanf("%d", &time);

        if (time <= 0 || time > 1000)
        {
            printf("\nChoose a time between 1 and 1000 seconds.\n");
            return 0;
        }

        printf("Would you like the map to be moved around on the screen? Yes/No\n");
        scanf("%s", resp2);
        if (strcmp(resp2, "No") == 0)
            draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 250, 500, time);

        if (strcmp(resp2, "Yes") == 0)
        {
            printf("In which directon? (U)p, (D)own, (R)ight ou (L)eft?\n");
            scanf(" %c", &ch);

            if (ch == 'R')
                draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 320, 500, time);

            if (ch == 'L')
                draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 0, 500, time);

            if (ch == 'U')
                draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 250, 800, time);

            if (ch == 'D')
                draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 250, 200, time);

            if (ch != 'R' && ch != 'L' && ch != 'U' && ch != 'D')
            {
                printf("Invalid direction. Presenting map normally.\n");
                draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 250, 500, time);
            }
        }

        if (strcmp(resp2, "Yes") != 0 && strcmp(resp2, "No") != 0)
        {
            printf("Please answer 'Yes' or 'No'\n");
            exit(0);
        }

        printf("\nWould you like to travel again? Yes/No?\n");
        scanf("%s", resp3);
        if (strcmp(resp3, "No") == 0)
        {
            printf("\nPleasure travelling with you.\n");
            return 0;
        }

        if (strcmp(resp3, "Yes") != 0)
        {
            printf("Please answer 'Yes' or 'No'\n");
            return 0;
        }
    }

    return 0;
}