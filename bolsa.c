#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include "bolsa.h"
#include <string.h>
#include <stdio.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t comp = PTHREAD_COND_INITIALIZER;
int nv=0; 
char nom_ven[10];
char nom_comp[10];
int nc=0; 
int p_actual = INT_MAX;

int vendo(int precio, char *vendedor, char *comprador){
	pthread_mutex_lock(&m);

	while(nc){
		pthread_cond_wait(&comp, &m);
	}

	if(nv==0 || p_actual>precio){
		nv=1;
		p_actual = precio;
		strcpy(nom_ven, vendedor);
		pthread_cond_broadcast(&comp);

		while((nc==0) && strcmp(vendedor, nom_ven)==0){
			pthread_cond_wait(&comp, &m);
		}

		if(nc==1 && (strcmp(nom_ven, vendedor)==0)){
			strcpy(comprador, nom_comp);
			nc=0;
			p_actual = INT_MAX;
			nv=0;
			pthread_mutex_unlock(&m);
			return 1;
		}

		else{
			pthread_mutex_unlock(&m);
			return 0;
		}
		
		
	}

	else{
		pthread_mutex_unlock(&m);
		return 0;
	}
	

}



int compro(char *comprador, char *vendedor){
	pthread_mutex_lock(&m);

	while(nc){
		pthread_cond_wait(&comp, &m);
	}

	if(nv==0){
		pthread_mutex_unlock(&m);
		return 0;
	}

	else{
		nc=1;
		strcpy(nom_comp, comprador);
		strcpy(vendedor, nom_ven);
		int p = p_actual;
		p_actual = INT_MAX;
		nv=0;
		
		pthread_cond_broadcast(&comp);
		pthread_mutex_unlock(&m);
		return p;

	}
	
}
