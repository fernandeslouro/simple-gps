#include "funcoes.h"




/******************************************/
/* Passa e converte points para Matriz    */
/******************************************/

char read_borders(char file_name[], char border_name[N_BORDERS][SIZE], float angular_coordinates[N_BORDERS][N_POINTS_BORDER][2], float final_coordinates[N_BORDERS][N_POINTS_BORDER][2])
 {

  FILE *fp;
    int a, b;
    int convertions;
    int n_lines;
    char line[SIZE];
    float latitude, longitude;

	 a=0;

	 fp = fopen(file_name,"r");     /* Abre o ficheiro. */

     if (fp == NULL)
      {                                                 /* Mensagem de erro na abertura. */
	   printf ("Erro ao abrir o ficheiro.\n");
           return 0;
      }

     else
     {
	  do
       {
        fgets(line, N_BORDERS, fp);

          if(line[0]=='F')      		           /* Nome das fronteiras. */
            strcpy(border_name[a],line);       /* Copiar para o vector o conteúdo da linha. */

            else
             {
  	          convertions=sscanf(line, "%f %f", &latitude, &longitude);

	           if (convertions==2)          		           /* Latitude e Longitude dos points. */
                {
	             for(b=0;b<4;b=b+1)
	              {
	               sscanf(line, "%f %f", &angular_coordinates[a][b][0], &angular_coordinates[a][b][1]);
	               fgets(line, SIZE, fp);
	              }
	              a=a+1;
                }
             }

	       if (feof(fp))		   /* A funcao so termina com o fim do ficheiro. */
	       n_lines = -1;
        }
	  while (n_lines != -1);
     }
	do
     {
	  for(a=0;a<52;a++)
       {
	    for(b=0;b<4;b++)
         {
	       final_coordinates[a][b][0]=6371*tan(PI*(angular_coordinates[a][b][0]-IST_LATITUDE)/180);
	       final_coordinates[a][b][1]=6371*cos(PI*angular_coordinates[a][b][0]/180)*tan(PI*(angular_coordinates[a][b][1]-IST_LONGITUDE)/180);

	          /* Assumindo o grandioso Instituto Superior Tecnico como centro do nosso referencial. */
	     }

	    if(feof(fp))
	    n_lines=-2;
       }
      }
     while(n_lines!=-2);

  fclose(fp);

  return 0;
 }














/******************************************/
/* Desenha mapa, amplia, etc...           */
/******************************************/

void draw_map(float final_coordinates[N_BORDERS][N_POINTS_BORDER][2], float ampliation, char border_name[N_BORDERS][SIZE], int argc, char * argv[], POINT*top, POINT*top2, LIST*x, char* resp, int center_x, int center_y, int time)
 {

   char district[25];
   char color[25];
   char district1[25];
   char district2[25];
   int i, j, k, l, m;
   int d=g2_open_X11(500, 1000);
   int dev=g2_open_vd();
   int color_of_borders = 0;
   POINT* aux;
   LIST* auxe;


/* Detectar o distrito escolhido. */
   for(k=0; k < argc; k++)
    {
	 if(strcmp(argv[k], "-d") == 0)
 	  {
       strcpy(district, argv[k+1]);
      }
    }

/* Detectar o factor de ampliação desejado. */
   for(k=0; k < argc; k++)
    {
	 if(strcmp(argv[k], "-a") == 0)
 	  {
       ampliation = atof(argv[k+1]);
      }
    }

/* Detectar a cor escolhida. */
   for(k=0; k < argc; k++)
    {
	 if(strcmp(argv[k], "-c") == 0)
 	  {
       strcpy(color, argv[k+1]);
       color_of_borders = border_color(color);
      }
    }

     g2_clear(d);
     g2_set_coordinate_system(d, center_x, center_y, ampliation, ampliation);
     g2_attach(d, dev);
     g2_set_auto_flush(d,0);
    /*  g2_move_r (d, 41254265766, 46552457532); */
     g2_set_line_width(d, 3);
     g2_set_background(d, 1);                      /* Definir a cor do fundo como preto (MODO NOCTURNO). */
     g2_pen(d, 0);
     g2_set_font_size(d, 35);


	printf("\n\tO g2 esta agora aberto.\n");

   for(l=0;l<53;l++)
    {
     for(m=0;m<3;m++)
      {
       sscanf(border_name[l], "F (%[^)]) (%[^)])", district1, district2);

	   if ((strcmp(district, district1) == 0 ) || ((strcmp(district, district2) == 0)))
		 g2_pen(d, color_of_borders);    /* Caso seja encontrado o distrito procurado na matriz border_name, este é desenhado com a cor desejada *supostamente* */

	   else
            g2_pen(d, 0);                     /* Caso contrário, o mapa é desenhado normalmente, com a cor especificada (branco). */

        g2_line(d, final_coordinates[l][m][1], final_coordinates[l][m][0], final_coordinates[l][m+1][1], final_coordinates[l][m+1][0]);
       }
        g2_flush(d);
     }

	if (argc == 1)
       {
		printf("\n Exemplo para destacar um distrito: \n\t./gps2013 -c vermelho -a 1.2 -d Viana_do_Castelo\n\tA ordem dos comandos nao importa. Os numeros representam o valor da ampliacao, a cor e o distrito a destacar pretendidos.");

             g2_pen(d, 0);

             for(i=0;i<53;i++)
              {
               for(j=0;j<3;j++)
                {
                 g2_line(d, final_coordinates[i][j][1], final_coordinates[i][j][0], final_coordinates[i][j+1][1], final_coordinates[i][j+1][0]);
                }
              }
        g2_flush(d);
       }


    if(strcmp(resp, "Sim")==0)
    {                                                           /*Se sim, desenha duas rotas, uma do destino até a int, e outra da int. ao destino*/
        aux=top;
        aux=aux->prox;

        while(aux!= NULL)
        {
            aux->long1= 6371*cos(PI*aux->lat1/180) * tan(PI*(aux->long1-IST_LONGITUDE)/180);
            aux->lat1 = 6371*tan(PI*(aux->lat1-IST_LATITUDE)/180);
            aux=(*aux).prox;
        }

        top=top->prox;
        while(top->prox!=NULL)
        {
            g2_pen(d, 19);
            g2_line(d, top->long1, top->lat1, top->prox->long1,top->prox->lat1 );
            g2_flush(d);
            top=(*top).prox;
        }


        aux=top2;
        aux=aux->prox;

        while(aux!= NULL)
        {
            aux->long1= 6371*cos(PI*aux->lat1/180) * tan(PI*(aux->long1-IST_LONGITUDE)/180);
            aux->lat1 = 6371*tan(PI*(aux->lat1-IST_LATITUDE)/180);
            aux=(*aux).prox;
        }

        top2=top2->prox;
        while(top2->prox!=NULL)
        {
            g2_pen(d, 19);
            g2_line(d, top2->long1, top2->lat1, top2->prox->long1,top2->prox->lat1 );
            g2_flush(d);
            top2=(*top2).prox;
        }
    }

    if(strcmp(resp, "Nao")==0)
    {
        aux=top;
        aux=aux->prox;

        while(aux!= NULL)
        {
            aux->long1= 6371*cos(PI*aux->lat1/180) * tan(PI*(aux->long1-IST_LONGITUDE)/180);
            aux->lat1 = 6371*tan(PI*(aux->lat1-IST_LATITUDE)/180);
            aux=(*aux).prox;
        }

        top=top->prox;                                                                          /*Se não, desenha uma rota do inico ate ao destino*/
        while(top->prox!=NULL)
        {
            g2_pen(d, 19);
            g2_line(d, top->long1, top->lat1, top->prox->long1,top->prox->lat1 );
            g2_flush(d);
            top=(*top).prox;
        }
    }





    auxe=x->prox;
    g2_pen(d, 22);
    while(auxe->prox!=NULL)
    {
        auxe->a.long1= 6371*cos(PI*auxe->a.lat1/180) * tan(PI*(auxe->a.long1-IST_LONGITUDE)/180);
        auxe->a.lat1 = 6371*tan(PI*(auxe->a.lat1-IST_LATITUDE)/180);
        auxe->a.long2= 6371*cos(PI*auxe->a.lat2/180) * tan(PI*(auxe->a.long2-IST_LONGITUDE)/180);
        auxe->a.lat2 = 6371*tan(PI*(auxe->a.lat2-IST_LATITUDE)/180);


        if((*auxe).a.type1=='F')
        {                                                                                    /*Desenha os pontos nas localidades, com cores diferentes*/
            g2_pen(d, 3);
            g2_filled_circle(d, auxe->a.long1,  auxe->a.lat1,  1);
        }

         if((*auxe).a.type2=='F')
         {
            g2_pen(d, 3);
            g2_filled_circle(d,  auxe->a.long2,  auxe->a.lat2, 1);
         }

        if((*auxe).a.type1=='C')
        {
            g2_pen(d, 4);
            g2_filled_circle(d, auxe->a.long1,  auxe->a.lat1,  2);
        }

        if((*auxe).a.type2=='C')
        {
            g2_pen(d, 4);
            g2_filled_circle(d,  auxe->a.long2,  auxe->a.lat2, 2);
        }

       if((*auxe).a.type1=='D')
       {
            g2_pen(d, 22);
            g2_filled_circle(d, auxe->a.long1,  auxe->a.lat1,  4);
       }

       if((*auxe).a.type2=='D')
       {
            g2_pen(d, 22);
            g2_filled_circle(d,  auxe->a.long2,  auxe->a.lat2, 4);
       }

        auxe=auxe->prox;
    }






    g2_pen(d, 0);
    g2_line(d,6371*cos(PI*37.166737/180)*tan(PI*(-7.392426-IST_LONGITUDE)/180), 6371*tan(PI*(37.166737-IST_LATITUDE)/180), 6371*cos(PI*37.522652/180)*tan(PI*(-7.510529-IST_LONGITUDE)/180), 6371*tan(PI*(37.522652-IST_LATITUDE)/180));
                                                                    /* Medida desesperada para corrigir uma fronteira que nunca aparecia */


    g2_flush(d);
    sleep(time);               /*aguenta a janela*/
    g2_close(d);
 }






/*************************************/
/* Gere as cores                          */
/************************************/

int border_color(char cor[25])
 {
  if(strcmp(cor, "preto") == 0)
   {
	printf("\n\nA cor escolhida nao e valida. Escolha entre verde, amarelo, vermelho, azul, cor-de-laranja e cor-de-rosa\n\n");
   }

  if(strcmp(cor, "vermelho") == 0)
   {
	return 19;
   }

  if(strcmp(cor, "azul") == 0)
   {
	return 3;
   }

   if(strcmp(cor, "verde") == 0)
    {
	 return 4;
    }

   if(strcmp(cor, "amarelo") == 0)                             /* gera as cores*/
    {
	 return 25;
    }

   if(strcmp(cor, "cor-de-laranja") == 0)
    {
	 return 22;
    }

    if(strcmp(cor, "cor-de-rosa") == 0)
     {
	  return 23;
     }

  return 0;
 }






LIST * create_base()
{
    LIST * top;
    top=(LIST*)malloc(sizeof(LIST));
    strcpy((*top).a.place_name1,"\0");
    strcpy((*top).a.place_name2, "\0");
    strcpy((*top).a.road,"\0");
    (*top).a.lat1 = 0;                                     /*cria uma baze vazia do tipo LIST*/
    (*top).a.lat2 = 0;
    (*top).a.long1 = 0;
    (*top).a.long2 = 0;
    (*top).a.type1=' ';
    (*top).a.type2=' ';
    (*top).prox=NULL;

return top;
}


void add_road(LIST * base, char * place_name1, char * place_name2, char * road, float lat1, float long1, float lat2, float long2,char type1,char type2)
{
    LIST * aux, *ptr;
    aux=base;
    if(aux==NULL)
    printf("erro na base da list");
    while((*aux).prox!=NULL)
    {
        aux=(*aux).prox;
    }

    ptr=(LIST *)malloc(sizeof(LIST));
    strcpy((*ptr).a.place_name1,place_name1);                      /*adiciona uma nova estrada a uma lista inicial de estradas*/
    strcpy((*ptr).a.place_name2,place_name2);
    strcpy((*ptr).a.road,road);
    (*ptr).a.lat1=lat1;
    (*ptr).a.lat2=lat2;
    (*ptr).a.long2=long2;
    (*ptr).a.long1=long1;
    (*ptr).a.type1=type1;
    (*ptr).a.type2=type2;
    (*ptr).prox=NULL;
    (*aux).prox=ptr;
}

LIST * from_file_to_list()
{
    char linha[100];
    char nl1[MAXIMUM];
    char nl2[MAXIMUM];
    char etd[MAXIMUM];
    float lt1, lg1, lt2, lg2;                                                          /*vai buscar dados ao ficheiro e adiciona à lista de*/
    char type1,type2;                                                                  /*estradas usando a funcao anterior*/
    LIST * base;
    FILE *fp;
    char*p;
    if((fp=fopen("vias_localidades.txt","r"))==NULL)
        printf("Erro na leitura do ficheiro\n");

    base=create_base();
    do
    {
        p=fgets(linha,100,fp);
        sscanf(linha,"%c %s %f %f %s %c %s %f %f",&type1,nl1,&lt1,&lg1,etd,&type2,nl2,&lt2,&lg2);
        /*if (sscanf(linha,"%c %s %f %f %s %c %s %f %f",&type1,nl1,&lt1,&lg1,etd,&type2,nl2,&lt2,&lg2)!=9)
            {
                printf("\nFicheiro corrompido.\n");
                exit(0);
            }*/

        add_road(base,nl1,nl2,etd,lt1,lg1,lt2,lg2,type1,type2);
    }
    while(p!=NULL);

    return base;
}

void print_list(LIST*base)
{

    LIST*aux;                                  /*imprime a lista (foi usado para testes durante o desenvolvimento do programa*/
    aux=base;

    aux=(*aux).prox;
    while(aux!=NULL)
    {
        printf("%s %f %f\n%s\n%s %f %f\n\n\n",(*aux).a.place_name1,(*aux).a.lat1,(*aux).a.long1,(*aux).a.road,(*aux).a.place_name2,(*aux).a.lat2,(*aux).a.long2);
        aux=(*aux).prox;

    }
}

float calculate_distance(float lat1,float long1,float lat2,float long2)
{
    float distance;
                                                                                                   /*é auto-explicatória*/
    distance=sqrt((lat1-lat2)*(lat1-lat2)+(long1-long2)*(long1-long2));

    return distance;

}

TOWN * create_top()
{
    TOWN * top;
    top=(TOWN*)malloc(sizeof(TOWN));                           /*cria uma base vazia do tipo TOWN (estalista servirá para, por exemplo*/
    strcpy((*top).road,"\0");                               /*se escolhermos como ponto de partida 'LISBOA', armazenar todas as localiades ligadas a lisboa*/
    strcpy((*top).nome,"\0");                                    /*estas localidades serão depois ordenadas com base na sua distancia ao destino*/
    (*top).lat1 = 0;                                              /*e será depois transformada a que tiver menor distancia num POINT*/
    (*top).distance = 0;
    (*top).long1 = 0;
    (*top).prox=NULL;
    return top;
}

void add_town(TOWN*top,char * nome1,char * road,float lat1,float long1,float latd,float longd)
{

    TOWN * aux, *ptr;
    float distance;
    aux=top;
    if(aux==NULL)
    printf("Há um mau funcionamento na base da lista.\n");                                     /*adiciona localidades, neste caso, que ligam com LISBOA*/


    while((*aux).prox!=NULL)
        {
        aux=(*aux).prox;

        }


        ptr=(TOWN *)malloc(sizeof(TOWN));
        strcpy((*ptr).nome,nome1);
        strcpy((*ptr).road,road);
        (*ptr).lat1=lat1;
        (*ptr).long1=long1;
        (*ptr).distance=calculate_distance(lat1,long1,latd,longd);
        (*ptr).prox=NULL;

        (*aux).prox=ptr;

}



TOWN * search_for_roads(char*nomeo, LIST* base, float latd,float longd)
{
    LIST* aux;
    TOWN* top;
    aux=base;
    top=create_top();
                                                                   /*Com o exemplo anterior, procura todas as localidades que liga a Lisboa*/
    while((*aux).prox!=NULL)                                     /*e usa a funcao anterior para adicionar a essa lista intermedi de localidades*/
    {

       aux=(*aux).prox;

        if(strcmp(nomeo, (*aux).a.place_name1)==0)
        {
            add_town(top,(*aux).a.place_name2,(*aux).a.road,(*aux).a.lat2,(*aux).a.long2,latd,longd);
        }

        if(strcmp(nomeo, (*aux).a.place_name2)==0)
        {
            add_town(top,(*aux).a.place_name1,(*aux).a.road,(*aux).a.lat1,(*aux).a.long1,latd,longd);
        }
    }
    return top;
}



POINT * points_destination_or_origins (LIST* base,char*nomed)
 {
    LIST*aux;
    POINT*destino;
    aux=base;
    destino=(POINT*)malloc(sizeof(POINT));


    while((*aux).prox!=NULL)
     {

        aux=(*aux).prox;

        if(strcmp(nomed, (*aux).a.place_name1)==0)
            {
                (*destino).lat1=(*aux).a.lat1;
                (*destino).long1=(*aux).a.long1;
                (*destino).prox=NULL;
                return destino;
                break;
            }
        if(strcmp(nomed, (*aux).a.place_name2)==0)
            {
                (*destino).lat1=(*aux).a.lat2;
                (*destino).long1=(*aux).a.long2;
                (*destino).prox=NULL;
                return destino;
                break;
            }
    }

}




void print_town(TOWN*base)
{

    TOWN*aux;                                          /*imprime a lista de pontos que será depois unida. Nao é usada.*/
    aux=base;


    while(aux!=NULL)
    {
        printf("%s %f %f\n\n",(*aux).nome,(*aux).lat1,(*aux).long1);
        aux=(*aux).prox;
    }
}


TOWN * order_town(TOWN * top)
{
    TOWN*aux1,*aux2,*aux3;
    aux1=top;
    aux2=top;
    aux3=top;                                                      /*ordena a lista intermédia de localidades.*/
    aux2=(*aux1).prox;


    /**testa se não tem 1*/
    if((*aux1).prox==NULL)
        printf("localidade sem roads");

    if((*aux2).prox==NULL)
        printf("tem apenas uma road ");

    else
    {
        aux3=(*aux2).prox;

        while(aux3!=NULL)
       {

            if((*aux2).distance>(*aux3).distance)
            {
                (*aux1).prox=aux3;
                (*aux2).prox=(*aux3).prox;
                (*aux3).prox=aux2;
                aux1=top;
                aux2=(*aux1).prox;
                aux3=(*aux2).prox;
            }

            else
            {
                aux1=(*aux1).prox;
                aux2=(*aux2).prox;
                aux3=(*aux3).prox;
            }
       }
    }
    return top;
}


POINT * create_inicio()
{
    POINT * top;
    top=(POINT*)malloc(sizeof(POINT));
    (*top).lat1 = 0;                                           /*cria uma base vazia para a lista de pontos a unir.*/
    (*top).long1 = 0;
    (*top).prox=NULL;
    return top;
}


char * adiciona_point (POINT* top_points, TOWN* top_town)
{
    POINT* aux,*ptr;
    TOWN* p;
    p=top_town;
    aux=top_points;

    while((*aux).prox!=NULL)
        aux=(*aux).prox;                                                   /*adiciona um ponto à lista de pontos a unir.*/

    p=(*p).prox;
    ptr=(POINT*)malloc(sizeof(POINT));
    (*ptr).lat1=(*p).lat1;
    (*ptr).long1=(*p).long1;
    (*ptr).prox=NULL;
    (*aux).prox=ptr;
    return (*p).nome;
}

POINT * main_list_funcion(char * nomeo,char * nomed,LIST * base)
{
    POINT * point_destino,*point_origem,*top_list_points;                                          /*Funçao principal.Repete o processo explicado*/
    char novaorigem[MAXIMUM];                                                      /*anteriormente com o exemplo Lisboa.*/
    TOWN * top_temporary;                                                                      /*para os pontos seguintes, até chegar ao destino.*/

    top_temporary=create_top();
    point_destino=points_destination_or_origins(base,nomed);
    point_origem=points_destination_or_origins(base,nomeo);
    top_list_points=create_inicio();
    (*top_list_points).prox=point_origem;

    while(1)
    {
        TOWN * top_temporary,*aux;
        int a=0;

        top_temporary=search_for_roads(nomeo,base, (*point_destino).lat1,(*point_destino).long1);
        top_temporary=order_town(top_temporary);
        /*puts("aqui");*/
        aux=top_temporary;
        aux=(*aux).prox;

        strcpy(nomeo,adiciona_point (top_list_points, top_temporary));

        printf("Siga pela %s até ",(*aux).road);
        printf("%s.\n",(*aux).nome);

        if((*aux).distance==0)
        {
            break;
        }

        (*top_temporary).prox=NULL;

      /*  a++;
        if (a>100)
            {
                printf("\nErro nas rotas.\n");
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


char * put_parentesis (char* town)
{
    char string [100], copy[100];
    strcpy (copy, town);                        /*Poe os parentesis. Assim, ao pedirmos como destino "Lisboa" o programa procurará "(Lisboa) no ficheiro.*/
    strcpy(town, "(");
    strcat(town, copy);
    strcat(town, ")");
    return town;
}



int no_more_segmentation_fault(LIST*base,char*nome)
{
    LIST*aux;
    aux=base;

    while(aux!=NULL)                                           /*explicada na main*/
    {
        if(strcmp((*aux).a.place_name1,nome)==0||strcmp((*aux).a.place_name2,nome)==0)
        {
            return 0;
        }

    aux=(*aux).prox;
    }

    printf("Esse sítio não existe... Vejo que o estereotipo de turista que não conhece o seu país lhe assenta bem... Oriente-se e volte mais tarde. \n");
    exit(0);

}

