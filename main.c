#include "funcoes.h"


int main(int argc, char * argv[])
{
    char border_name[N_BORDERS][SIZE];                           /*Lê até 100 caracteres por linha (para garantir que apanha toda a informacao*/
    float angular_coordinates[N_BORDERS][N_POINTS_BORDER][2];    /*  4 points de cada fronteira e as 2 coordenadas de cada point */
    float final_coordinates[N_BORDERS][N_POINTS_BORDER][2];      /*Coordenadas planares*/
    float ampliation=0.9;
    char nomed[MAXIMUM], nomei [MAXIMUM],nomeo[MAXIMUM], resp[20], resp2[20], resp3 [20], ch;
    POINT * point, *point2;
    TOWN*top;
    LIST * base;
    int time;


    /*print_list(base);*/

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\nGPS Turístico AeroLoc. Para uma viagem rápida e tranquila pelos recantos de Portugal...\n");

    while(1){
base = from_file_to_list();
    printf("\nInsira  o sítio onde se encontra:\n");
    scanf("%s",nomeo);
    strcpy(nomeo, put_parentesis(nomeo));
    no_more_segmentation_fault(base,nomeo);

    printf("\nInsira o sítio que deseja visitar:\n");
    scanf("%s",nomed);                                                                          /*Guarda os destinos.*/
    strcpy(nomed, put_parentesis(nomed));
    no_more_segmentation_fault(base,nomed);             /*Impede que haja segmentation fault quando se inserem destinos desconhecidos*/

    if(strcmp(nomeo, nomed)==0)
    {
        puts("Nao saiu do sítio...");
        return 0;
    }

    printf("\nDurante a sua viagem, há algum lugar em especial que gostaria de visitar? Sim ou Nao?\n");
    scanf("%s",resp);

    if(strcmp(resp, "Sim")==0)
    {
        printf("E que sítio seria esse?\n");
        scanf("%s",nomei);
        strcpy(nomei, put_parentesis(nomei));
        no_more_segmentation_fault(base,nomei);

        point=main_list_funcion(nomeo,nomei,base);
        point2=main_list_funcion(nomei,nomed,base);
    }

    if(strcmp(resp, "Nao")==0)
    {
        point=main_list_funcion(nomeo,nomed,base);
    }


    if(strcmp(resp,"Sim")!=0 && strcmp(resp,"Nao")!=0)
    {
        printf("Não me respondeu bem à pergunta. Sinto-me ofendido. Viage sozinho. :(\n");
        exit(0);
    }

    printf("\n\n\n\n\n");

    /*print_points(point);
    print_points(point2);*/


    read_borders("fronteiras_distritos.txt", border_name, angular_coordinates, final_coordinates);        /* Dados do ficheiro para matriz*/
 /* Irá ler os dados da matriz com as coordenadas e criar o mapa. */


     printf("\nQuantos segundos quer que o mapa fique no ecra até ser pedida nova rota?\n");
        scanf("%d", &time);

    if (time<=0 || time>1000)
        {
            printf("\nPara a próxima escolha um intervalo de tempo apropriado.\n");
            return 0;
        }





    printf("Deseja movimentar o mapa no referencial? Sim ou Nao?\n");
    scanf("%s", resp2);
        if(strcmp(resp2, "Nao")==0)
               draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 250, 500, time);
       /* Irá ler os dados da matriz com as coordenadas e criar o mapa. */

       if(strcmp(resp2, "Sim")==0)
       {
           printf("Em que direcção? C, B, D ou E?\n");
           scanf(" %c", &ch);

            if(ch=='D')
                draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 320, 500, time);

            if(ch=='E')
                draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 0, 500, time);

            if(ch=='C')
                draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 250, 800, time);

            if(ch=='B')
                draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 250, 200, time);

            if(ch !='B' && ch !='D' && ch !='E' && ch !='C')
               {
                printf("Direcção Inválida. Irei apresentar o mapa normalmente.\n");
                draw_map(final_coordinates, ampliation, border_name, argc, argv, point, point2, base, resp, 250, 500, time);
               }

        }

        if(strcmp(resp2, "Sim")!=0 && strcmp(resp2, "Nao")!=0)
        {
        printf("Não me respondeu bem à pergunta. Sinto-me ofendido. Viage sozinho. :(\n");
        exit(0);
        }


        printf("\nQuer viajar mais? Sim ou Nao?\n");
        scanf("%s", resp3);
            if(strcmp(resp3, "Nao")==0)
            {
                printf("\nFoi um prazer viajar consigo.\n");
                return 0;
            }

            if(strcmp(resp3, "Sim")!=0)
            {
                printf("\nIsso não é uma resposta válida.\n");
                return 0;
            }




    }




    return 0;
}



