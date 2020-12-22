/*
 *
 *  Progetto di algoritmi e strutture dati - A.A. 2019
 *  Sistema di monitoraggio di relazioni tra elementi
 *
 *  Autore: Luca Minotti
 *  Matricola: 886716
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LUNG_MAX_ID 32
#define LUNG_MAX_REL 21

//albero entità
struct ent
{
	char id_ent[LUNG_MAX_ID];
	struct ent *p;
	struct ent *right;
	struct ent *left;
	char col;
};

//albero tipi relazioni
struct tipi
{
	char id_rel[LUNG_MAX_REL];
	
	//albero destinazioni
	struct dest
	{
		char id_dest[LUNG_MAX_ID];
		
		//albero origenti
		struct orig
		{
			char id_orig[LUNG_MAX_ID];
			struct orig *p;
			struct orig *right;
			struct orig *left;
			//char col;
		} *orig;
		
		int numorig;
		struct dest *p;
		struct dest *right;
		struct dest *left;
		char col;
	} *dest;
	
	char valid;
	int max;
	int nummax;
	
	struct tipi *p;
	struct tipi *right;
	struct tipi *left;
	char col;
};


struct ent *testa_ent;
struct ent *nil_ent;

struct tipi *testa_tipi;
struct tipi *nil_tipi;

struct orig *nil_orig;
struct dest *nil_dest;

/***************  GESTIONE INTERFACCIAMENTO ***************/
void addent(char *id_ent);
void delent(char *id_ent);
void addrel(char *id_orig,char *id_dest,char *id_rel);
void delrel(char *id_orig,char *id_dest,char *id_rel);
void report(void);


/***************  GESTIONE ALBERI ***************/
void left_rotate_entity(struct ent *x);
void right_rotate_entity(struct ent *x);
struct ent *entity_successor(struct ent *x);
struct ent *entity_search(struct ent *x,char *id_ent);

void left_rotate_tipi(struct tipi *x);
void right_rotate_tipi(struct tipi *x);
struct tipi *tipi_search(struct tipi *x,char *id_rel);

void left_rotate_dest(struct tipi *tipo,struct dest *x);
void right_rotate_dest(struct tipi *tipo,struct dest *x);
struct dest *dest_search(struct dest *x,char *id_rel);

void left_rotate_orig(struct dest *dest,struct orig *x);
void right_rotate_orig(struct dest *dest,struct orig *x);
struct orig *orig_search(struct orig *x,char *id_rel);



int main(int argc, char const *argv[])
{
	nil_ent=malloc(sizeof(struct ent));
	nil_ent->col='n';
	nil_ent->p=nil_ent;
	nil_ent->left=nil_ent;
	nil_ent->right=nil_ent;
	testa_ent=nil_ent;
	
	nil_tipi=malloc(sizeof(struct tipi));
	nil_tipi->col='n';
	nil_tipi->p=nil_tipi;
	nil_tipi->left=nil_tipi;
	nil_tipi->right=nil_tipi;
	testa_tipi=nil_tipi;
	
	nil_dest=malloc(sizeof(struct dest));
	nil_dest->col='n';
	nil_dest->p=nil_dest;
	nil_dest->right=nil_dest;
	nil_dest->left=nil_dest;
	
	nil_orig=malloc(sizeof(struct orig));
	//nil_orig->col='n';
	nil_orig->p=nil_orig;
	nil_orig->left=nil_orig;
	nil_orig->right=nil_orig;
	
	/* DEBUG ONLY */
	//freopen("/Users/luca/OneDrive - Politecnico di Milano/Università/Progetto Algoritmi e Strutture Dati/input6.2.txt","r",stdin);
	
	char *buffer;
	size_t dim=0;
	char *token1,*token2,*token3;
	
	do
	{
		getline(&buffer, &dim, stdin);
		switch (buffer[0])
		{
			case 'a':
				if(buffer[3]=='e')
				{
					//sscanf("addent \"%s\"\n", id_orig);
					strtok(buffer, "\"");
					token1=strtok(NULL, "\"");
					//printf("ADDENT -%s-\n",token1);
					addent(token1);
				}
				else
				{
					//sscanf("addrel \"%s\" \"%s\" \"%s\"\n", id_orig,id_dest,id_rel);
					strtok(buffer, "\"");
					token1=strtok(NULL, "\"");
					strtok(NULL, "\"");
					token2=strtok(NULL, "\"");
					strtok(NULL, "\"");
					token3=strtok(NULL, "\"");
					//printf("ADDREL -%s-%s-%s-\n",token1,token2,token3);
					addrel(token1,token2,token3);
				}
				break;
			case 'd':
				if(buffer[3]=='e')
				{
					//sscanf("delent \"%s\"\n", id_orig);
					strtok(buffer, "\"");
					token1=strtok(NULL, "\"");
					//printf("DELENT -%s-\n",token1);
					delent(token1);
				}
				else
				{
					//sscanf("delrel \"%s\" \"%s\" \"%s\"\n", id_orig,id_dest,id_rel);
					strtok(buffer, "\"");
					token1=strtok(NULL, "\"");
					strtok(NULL, "\"");
					token2=strtok(NULL, "\"");
					strtok(NULL, "\"");
					token3=strtok(NULL, "\"");
					//printf("DELREL -%s-%s-%s-\n",token1,token2,token3);
					delrel(token1,token2,token3);
				}
				break;
			case 'r':
				//printf("REPORT: -%s-\n",buffer);
				report();
				break;
			default:
				return 0;
		}
	}while(1);
}


/**************************************
 ***************ALBERI*****************
 ***************ALBERI*****************
 ***************ALBERI*****************
 ***************ALBERI*****************
 **************************************/
void left_rotate_entity(struct ent *x)
{
	struct ent *y;
	y=x->right;
	x->right=y->left;
	if(y->left!=nil_ent)
	{
		y->left->p=x;
	}
	y->p=x->p;
	if(x->p==nil_ent)
	{
		testa_ent=y;
	}
	else
	{
		if(x==x->p->left)
		{
			x->p->left=y;
		}
		else
		{
			x->p->right=y;
		}
	}
	y->left=x;
	x->p=y;
}
void right_rotate_entity(struct ent *x)
{
	struct ent *y;
	y=x->left;
	x->left=y->right;
	if(y->right!=nil_ent)
	{
		y->right->p=x;
	}
	y->p=x->p;
	if(x->p==nil_ent)
	{
		testa_ent=y;
	}
	else
	{
		if(x==x->p->right)
		{
			x->p->right=y;
		}
		else
		{
			x->p->left=y;
		}
	}
	y->right=x;
	x->p=y;
}
struct ent *entity_successor(struct ent *x)
{
	struct ent *y;
	if(x->right!=nil_ent)
	{
		y=x->right;
		while(y->left!=nil_ent)
		{
			y=y->left;
		}
		return y;
	}
	y=x->p;
	while(y!=nil_ent && x==y->right)
	{
		x=y;
		y=y->p;
	}
	return y;
}
struct ent *entity_search(struct ent *x,char *id_ent)
{
	int ris=0;
	if(x!=nil_ent)
		ris=strcmp(id_ent,x->id_ent);
	else
		return nil_ent;
	while(ris!=0)
	{
		if(ris<0)
			x=x->left;
		else
			x=x->right;
		if(x!=nil_ent)
			ris=strcmp(id_ent,x->id_ent);
		else
			return nil_ent;
	}
	return x;
}
int single_entity_search(struct ent *x,char *id_ent)
{
	int ris=0;
	if(x!=nil_ent)
		ris=strcmp(id_ent,x->id_ent);
	else
		return 0;
	while(ris!=0)
	{
		if(ris<0)
			x=x->left;
		else
			x=x->right;
		if(x!=nil_ent)
			ris=strcmp(id_ent,x->id_ent);
		else
			return 0;
	}
	return 1;
}
int double_entity_search(struct ent *x,char *id_orig,char *id_dest)
{
	int ris_orig,ris_dest;
	if(x==nil_ent)
		return 0;
	ris_orig=strcmp(id_orig,x->id_ent);
	ris_dest=strcmp(id_dest,x->id_ent);
	if(ris_orig==0)
	{
		if(ris_dest==0)
			return 1;
		else
		{
			if(ris_dest<0)
				return single_entity_search(x->left,id_dest);
			else
				return single_entity_search(x->right,id_dest);
		}
	}
	if(ris_dest==0)
	{
		if(ris_orig==0)
			return 1;
		else
		{
			if(ris_orig<0)
				return single_entity_search(x->left,id_orig);
			else
				return single_entity_search(x->right,id_orig);
		}
	}
	
	if(ris_orig<0)
	{
		if(ris_dest<0)
			return double_entity_search(x->left,id_orig,id_dest);
		else
			return single_entity_search(x->left,id_orig) && single_entity_search(x->right,id_dest);
	}
	else
	{
		if(ris_dest<0)
			return single_entity_search(x->right,id_orig) && single_entity_search(x->left,id_dest);
		else
			return double_entity_search(x->right,id_orig,id_dest);
	}
}


void left_rotate_tipi(struct tipi *x)
{
	struct tipi *y;
	y=x->right;
	x->right=y->left;
	if(y->left!=nil_tipi)
	{
		(y->left)->p=x;
	}
	y->p=x->p;
	if(x->p==nil_tipi)
	{
		testa_tipi=y;
	}
	else
	{
		if(x==(x->p)->left)
		{
			(x->p)->left=y;
		}
		else
		{
			(x->p)->right=y;
		}
	}
	y->left=x;
	x->p=y;
}
void right_rotate_tipi(struct tipi *x)
{
	struct tipi *y;
	y=x->left;
	x->left=y->right;
	if(y->right!=nil_tipi)
	{
		(y->right)->p=x;
	}
	y->p=x->p;
	if(x->p==nil_tipi)
	{
		testa_tipi=y;
	}
	else
	{
		if(x==(x->p)->right)
		{
			(x->p)->right=y;
		}
		else
		{
			(x->p)->left=y;
		}
	}
	y->right=x;
	x->p=y;
}
struct tipi *tipi_search(struct tipi *x,char *id_rel)
{
	int ris=0;
	if(x!=nil_tipi)
		ris=strcmp(id_rel,x->id_rel);
	else
		return nil_tipi;
	while(ris!=0)
	{
		if(ris<0)
			x=x->left;
		else
			x=x->right;
		if(x!=nil_tipi)
			ris=strcmp(id_rel,x->id_rel);
		else
			return nil_tipi;
	}
	return x;
}


void left_rotate_dest(struct tipi *tipo,struct dest *x)
{
	struct dest *y;
	y=x->right;
	x->right=y->left;
	if(y->left!=nil_dest)
	{
		y->left->p=x;
	}
	y->p=x->p;
	if(x->p==nil_dest)
	{
		tipo->dest=y;
	}
	else
	{
		if(x==x->p->left)
		{
			x->p->left=y;
		}
		else
		{
			x->p->right=y;
		}
	}
	y->left=x;
	x->p=y;
}
void right_rotate_dest(struct tipi *tipo,struct dest *x)
{
	struct dest *y;
	y=x->left;
	x->left=y->right;
	if(y->right!=nil_dest)
	{
		y->right->p=x;
	}
	y->p=x->p;
	if(x->p==nil_dest)
	{
		tipo->dest=y;
	}
	else
	{
		if(x==x->p->right)
		{
			x->p->right=y;
		}
		else
		{
			x->p->left=y;
		}
	}
	y->right=x;
	x->p=y;
}
struct dest *dest_successor(struct dest *x)
{
	struct dest *y;
	if(x->right!=nil_dest)
	{
		y=x->right;
		while(y->left!=nil_dest)
		{
			y=y->left;
		}
		return y;
	}
	y=x->p;
	while(y!=nil_dest && x==y->right)
	{
		x=y;
		y=y->p;
	}
	return y;
}
struct dest *dest_search(struct dest *x,char *id_dest)
{
	int ris=0;
	if(x!=nil_dest)
		ris=strcmp(id_dest,x->id_dest);
	else
		return nil_dest;
	while(ris!=0)
	{
		if(ris<0)
			x=x->left;
		else
			x=x->right;
		if(x!=nil_dest)
			ris=strcmp(id_dest,x->id_dest);
		else
			return nil_dest;
	}
	return x;
}


void left_rotate_orig(struct dest *dest,struct orig *x)
{
	struct orig *y;
	y=x->right;
	x->right=y->left;
	if(y->left!=nil_orig)
	{
		y->left->p=x;
	}
	y->p=x->p;
	if(x->p==nil_orig)
	{
		dest->orig=y;
	}
	else
	{
		if(x==x->p->left)
		{
			x->p->left=y;
		}
		else
		{
			x->p->right=y;
		}
	}
	y->left=x;
	x->p=y;
}
void right_rotate_orig(struct dest *dest,struct orig *x)
{
	struct orig *y;
	y=x->left;
	x->left=y->right;
	if(y->right!=nil_orig)
	{
		y->right->p=x;
	}
	y->p=x->p;
	if(x->p==nil_orig)
	{
		dest->orig=y;
	}
	else
	{
		if(x==x->p->right)
		{
			x->p->right=y;
		}
		else
		{
			x->p->left=y;
		}
	}
	y->right=x;
	x->p=y;
}
struct orig *orig_successor(struct orig *x)
{
	struct orig *y;
	if(x->right!=nil_orig)
	{
		y=x->right;
		while(y->left!=nil_orig)
		{
			y=y->left;
		}
		return y;
	}
	y=x->p;
	while(y!=nil_orig && x==y->right)
	{
		x=y;
		y=y->p;
	}
	return y;
}
struct orig *orig_search(struct orig *x,char *id_orig)
{
	int ris=0;
	if(x!=nil_orig)
		ris=strcmp(id_orig,x->id_orig);
	else
		return nil_orig;
	while(ris!=0)
	{
		if(ris<0)
			x=x->left;
		else
			x=x->right;
		if(x!=nil_orig)
			ris=strcmp(id_orig,x->id_orig);
		else
			return nil_orig;
	}
	return x;
}

struct dest *dest_del;
void orig_free(struct orig *x)
{
	if(x!=nil_orig)
	{
		orig_free(x->left);
		orig_free(x->right);
		free(x);
	}
}
void orig_delete(struct dest *dest,struct orig *z)
{
	struct orig *y,*x;
	
	if(z->left==nil_orig || z->right==nil_orig)
	{
		y=z;
	}
	else
	{
		y=orig_successor(z);
	}
	if(y->left!=nil_orig)
	{
		x=y->left;
	}
	else
	{
		x=y->right;
	}
	x->p=y->p;
	if(y->p==nil_orig)
	{
		dest->orig=x;
	}
	else
	{
		if(y==y->p->left)
		{
			y->p->left=x;
		}
		else
		{
			y->p->right=x;
		}
	}
	if(y!=z)
	{
		strcpy(z->id_orig,y->id_orig);
	}
	/*if(y->col=='n')
	{
		//entity_delete_fixup(x);
		while(x!=dest->orig && x->col=='n')
		{
			if(x==x->p->left)
			{
				z=x->p->right;
				if(z->col=='r')
				{
					z->col='n';
					x->p->col='r';
					left_rotate_orig(dest,x->p);
					z=x->p->right;
				}
				if(z->left->col=='n' && z->right->col=='n')
				{
					z->col='r';
					x=x->p;
				}
				else
				{
					if(z->right->col=='n')
					{
						z->left->col='n';
						z->col='r';
						right_rotate_orig(dest,z);
						z=x->p->right;
					}
					z->col=x->p->col;
					x->p->col='n';
					z->right->col='n';
					left_rotate_orig(dest,x->p);
					x=dest->orig;
				}
			}
			else
			{
				z=x->p->left;
				if(z->col=='r')
				{
					z->col='n';
					x->p->col='r';
					right_rotate_orig(dest,x->p);
					z=x->p->left;
				}
				if(z->right->col=='n' && z->left->col=='n')
				{
					z->col='r';
					x=x->p;
				}
				else
				{
					if(z->left->col=='n')
					{
						z->right->col='n';
						z->col='r';
						left_rotate_orig(dest,z);
						z=x->p->left;
					}
					z->col=x->p->col;
					x->p->col='n';
					z->left->col='n';
					right_rotate_orig(dest,x->p);
					x=dest->orig;
				}
			}
		}
		x->col='n';
		//end_of_entity_delete_fixup(x);
	}*/
	
	free(y); //elimino l'entità definitivamente
}
void dest_delete(struct tipi *tipo,struct dest *z)
{
	struct dest *y,*x;
	
	//la destinazione è da eliminare
	orig_free(z->orig); //libero tutte le origini collegate
	//libero la destinazione
	if(z->left==nil_dest || z->right==nil_dest)
	{
		y=z;
	}
	else
	{
		y=dest_successor(z);
	}
	if(y->left!=nil_dest)
	{
		x=y->left;
	}
	else
	{
		x=y->right;
	}
	x->p=y->p;
	if(y->p==nil_dest)
	{
		tipo->dest=x;
	}
	else
	{
		if(y==y->p->left)
		{
			y->p->left=x;
		}
		else
		{
			y->p->right=x;
		}
	}
	if(y!=z)
	{
		strcpy(z->id_dest,y->id_dest);
		z->orig=y->orig;
		z->numorig=y->numorig;
	}
	if(y->col=='n')
	{
		//entity_delete_fixup(x);
		while(x!=tipo->dest && x->col=='n')
		{
			if(x==x->p->left)
			{
				z=x->p->right;
				if(z->col=='r')
				{
					z->col='n';
					x->p->col='r';
					left_rotate_dest(tipo,x->p);
					z=x->p->right;
				}
				if(z->left->col=='n' && z->right->col=='n')
				{
					z->col='r';
					x=x->p;
				}
				else
				{
					if(z->right->col=='n')
					{
						z->left->col='n';
						z->col='r';
						right_rotate_dest(tipo,z);
						z=x->p->right;
					}
					z->col=x->p->col;
					x->p->col='n';
					z->right->col='n';
					left_rotate_dest(tipo,x->p);
					x=tipo->dest;
				}
			}
			else
			{
				z=x->p->left;
				if(z->col=='r')
				{
					z->col='n';
					x->p->col='r';
					right_rotate_dest(tipo,x->p);
					z=x->p->left;
				}
				if(z->right->col=='n' && z->left->col=='n')
				{
					z->col='r';
					x=x->p;
				}
				else
				{
					if(z->left->col=='n')
					{
						z->right->col='n';
						z->col='r';
						left_rotate_dest(tipo,z);
						z=x->p->left;
					}
					z->col=x->p->col;
					x->p->col='n';
					z->left->col='n';
					right_rotate_dest(tipo,x->p);
					x=tipo->dest;
				}
			}
		}
		x->col='n';
		//end_of_entity_delete_fixup(x);
	}
	
	free(y); //elimino l'entità definitivamente
}
void dest_fixup(struct tipi *tipo,struct dest *z,char *id_ent)
{
	struct orig *orig_del;
	if(z!=nil_dest)
	{
		dest_fixup(tipo,z->left, id_ent);
		if(strcmp(z->id_dest,id_ent)==0)
		{
			dest_del=z;
		}
		else
		{
			//scorro le sorgenti
			orig_del=orig_search(z->orig,id_ent);
			if(orig_del!=nil_orig)
			{
				//elimino la sorgente
				orig_delete(z,orig_del);
				
				if(z->numorig==tipo->max)
				{
					if(tipo->nummax<=1)
						tipo->valid=0;
					else
						tipo->nummax--;
				}
				
				z->numorig--;
			}
		}
		dest_fixup(tipo,z->right, id_ent);
	}
}
void tipi_fixup(struct tipi *x,char *id_ent)
{
	if(x!=nil_tipi)
	{
		tipi_fixup(x->left, id_ent);
		dest_del=NULL;
		dest_fixup(x,x->dest,id_ent);
		if(dest_del!=NULL)
		{
			if(dest_del->numorig==x->max)
			{
				if(x->nummax<=1)
					x->valid=0;
				else
					x->nummax--;
			}
			dest_delete(x,dest_del);
			
		}
		tipi_fixup(x->right, id_ent);
	}
}

int max,nummax;
char flag_dest,flag_tipi;
void inorder_dest_walk(struct dest *x,char *id_rel)
{
	if(x!=nil_dest)
	{
		inorder_dest_walk(x->left,id_rel);
		if(x->numorig==max)
		{
			if(flag_tipi!=0)
			{
				fputc(' ',stdout);
			}
			flag_tipi=1;
			
			if(flag_dest==0)
			{
				fputc('\"',stdout);
				fputs(id_rel,stdout);
				fputc('\"',stdout);
				fputc(' ',stdout);
				//printf("\"%s\" ",id_rel);
				
			}
			flag_dest=1;
			fputc('\"',stdout);
			fputs(x->id_dest,stdout);
			fputc('\"',stdout);
			//printf("\"%s\"",x->id_dest);
		}
		inorder_dest_walk(x->right,id_rel);
	}
}
void find_max(struct dest *x)
{
	if(x!=nil_dest)
	{
		find_max(x->left);
		if(x->numorig>max)
		{
			max=x->numorig;
			nummax=1;
		}
		else
		{
			if(x->numorig==max)
				nummax++;
		}
		find_max(x->right);
	}
}
void inorder_tipi_walk(struct tipi *x)
{
	if(x!=nil_tipi)
	{
		inorder_tipi_walk(x->left);
		if(x->valid==1)
		{
			if(x->max!=0)
			{
				//scorro tutte le destinazioni presenti e stampo ogni volta che trovo un max
				max=x->max;
				flag_dest=0;
				inorder_dest_walk(x->dest,x->id_rel);
				if(flag_dest==1)
					printf(" %d;",max);
			}
		}
		else
		{
			//cerco il max
			max=0;
			nummax=0;
			find_max(x->dest);
			//scorro tutte le destinazioni presenti e stampo ogni volta che trovo un max
			x->max=max;
			x->nummax=nummax;
			x->valid=1;
			if(x->max!=0)
			{
				flag_dest=0;
				inorder_dest_walk(x->dest,x->id_rel);
				if(flag_dest==1)
					printf(" %d;",max);
			}
		}
		
		inorder_tipi_walk(x->right);
	}
}

/**************************************
 **********INTERFACCIAMENTO************
 **********INTERFACCIAMENTO************
 **********INTERFACCIAMENTO************
 **********INTERFACCIAMENTO************
 **************************************/
void addent(char *id_ent)
{
	//if(FLAG_DEBUG==1)printf("**** ADDENT %s \n", id_ent);
	struct ent *x, *y, *z;
	int ris;
	y=nil_ent;
	x=testa_ent;
	while(x!=nil_ent)
	{
		y=x;
		ris=strcmp(id_ent, y->id_ent);
		if(ris==0) //entità già presente
			return;
		else
		{
			if(ris<0)
				x=x->left;
			else
				x=x->right;
		}
	}
	z=malloc(sizeof(struct ent));
	strcpy(z->id_ent, id_ent);
	z->p=y;
	if(y==nil_ent)
		testa_ent=z;
	else
	{
		if(strcmp(z->id_ent, y->id_ent) < 0)
			y->left=z;
		else
			y->right=z;
	}
	z->right=nil_ent;
	z->left=nil_ent;
	z->col='r';
	//entity_insert_fixup(z);
	while(z->p->col=='r')
	{
		if(z->p==z->p->p->left)
		{
			y=z->p->p->right;
			if(y->col=='r')
			{
				z->p->col='n';
				y->col='n';
				z->p->p->col='r';
				z=z->p->p;
			}
			else
			{
				if(z==z->p->right)
				{
					z=z->p;
					left_rotate_entity(z);
				}
				z->p->col='n';
				z->p->p->col='r';
				right_rotate_entity(z->p->p);
			}
		}
		else
		{
			y=z->p->p->left;
			if(y->col=='r')
			{
				z->p->col='n';
				y->col='n';
				z->p->p->col='r';
				z=z->p->p;
			}
			else
			{
				if(z==z->p->left)
				{
					z=z->p;
					right_rotate_entity(z);
				}
				z->p->col='n';
				z->p->p->col='r';
				left_rotate_entity(z->p->p);
			}
		}
	}
	testa_ent->col='n';
	//end_of_entity_insert_fixup(z);
}
void delent(char *id_ent)
{
	//if(FLAG_DEBUG==1)printf("**** DELENT %s\n",id_ent );
	struct ent *z,*x,*y;
	
	z=entity_search(testa_ent,id_ent);
	if(z==nil_ent)
		return;
	
	//l'elemento esiste ed è z: lo elimino
	if(z->left==nil_ent || z->right==nil_ent)
	{
		y=z;
	}
	else
	{
		y=entity_successor(z);
	}
	if(y->left!=nil_ent)
	{
		x=y->left;
	}
	else
	{
		x=y->right;
	}
	x->p=y->p;
	if(y->p==nil_ent)
	{
		testa_ent=x;
	}
	else
	{
		if(y==y->p->left)
		{
			y->p->left=x;
		}
		else
		{
			y->p->right=x;
		}
	}
	if(y!=z)
	{
		strcpy(z->id_ent,y->id_ent);
	}
	if(y->col=='n')
	{
		//entity_delete_fixup(x);
		while(x!=testa_ent && x->col=='n')
		{
			if(x==x->p->left)
			{
				z=x->p->right;
				if(z->col=='r')
				{
					z->col='n';
					x->p->col='r';
					left_rotate_entity(x->p);
					z=x->p->right;
				}
				if(z->left->col=='n' && z->right->col=='n')
				{
					z->col='r';
					x=x->p;
				}
				else
				{
					if(z->right->col=='n')
					{
						z->left->col='n';
						z->col='r';
						right_rotate_entity(z);
						z=x->p->right;
					}
					z->col=x->p->col;
					x->p->col='n';
					z->right->col='n';
					left_rotate_entity(x->p);
					x=testa_ent;
				}
			}
			else
			{
				z=x->p->left;
				if(z->col=='r')
				{
					z->col='n';
					x->p->col='r';
					right_rotate_entity(x->p);
					z=x->p->left;
				}
				if(z->right->col=='n' && z->left->col=='n')
				{
					z->col='r';
					x=x->p;
				}
				else
				{
					if(z->left->col=='n')
					{
						z->right->col='n';
						z->col='r';
						left_rotate_entity(z);
						z=x->p->left;
					}
					z->col=x->p->col;
					x->p->col='n';
					z->left->col='n';
					right_rotate_entity(x->p);
					x=testa_ent;
				}
			}
		}
		x->col='n';
		//end_of_entity_delete_fixup(x);
	}
	
	free(y); //elimino l'entità definitivamente
	
	tipi_fixup(testa_tipi,id_ent);
}
void addrel(char *id_orig,char *id_dest,char *id_rel)
{
	//if(FLAG_DEBUG==1)printf("**** ADDREL %s to %s ,%s\n",id_orig,id_dest,id_rel );
	struct tipi *x_tipi,*y_tipi,*z_tipi,*w_tipi;
	struct dest *x_dest,*y_dest,*z_dest,*w_dest;
	struct orig *x_orig,*y_orig,*z_orig;
	int ris;
	char ctr=0;
	
	
	//aggiunta o ricerca del tipo
	y_tipi=nil_tipi;
	x_tipi=testa_tipi;
	w_tipi=nil_tipi;
	while(x_tipi!=nil_tipi)
	{
		y_tipi=x_tipi;
		ris=strcmp(id_rel, y_tipi->id_rel);
		if(ris==0)
		{
			//tipo presente
			w_tipi=y_tipi;
			break;
		}
		else
		{
			if(ris<0)
				x_tipi=x_tipi->left;
			else
				x_tipi=x_tipi->right;
		}
	}
	if(w_tipi==nil_tipi)
	{
		z_tipi=malloc(sizeof(struct tipi));
		w_tipi=z_tipi;
		strcpy(z_tipi->id_rel,id_rel);
		z_tipi->dest=nil_dest;
		z_tipi->valid=1;
		z_tipi->max=0;
		z_tipi->nummax=0;
		z_tipi->p=y_tipi;
		if(y_tipi==nil_tipi)
			testa_tipi=z_tipi; //albero entità vuoto
		else
		{
			if(strcmp(z_tipi->id_rel, y_tipi->id_rel) < 0)
				y_tipi->left=z_tipi;
			else
				y_tipi->right=z_tipi;
		}
		z_tipi->right=nil_tipi;
		z_tipi->left=nil_tipi;
		z_tipi->col='r';
		//tipi_insert_fixup(z_tipi);
		while(z_tipi->p->col=='r')
		{
			if(z_tipi->p==z_tipi->p->p->left)
			{
				y_tipi=z_tipi->p->p->right;
				if(y_tipi->col=='r')
				{
					z_tipi->p->col='n';
					y_tipi->col='n';
					z_tipi->p->p->col='r';
					z_tipi=z_tipi->p->p;
				}
				else
				{
					if(z_tipi==z_tipi->p->right)
					{
						z_tipi=z_tipi->p;
						left_rotate_tipi(z_tipi);
					}
					z_tipi->p->col='n';
					z_tipi->p->p->col='r';
					right_rotate_tipi(z_tipi->p->p);
				}
			}
			else
			{
				y_tipi=z_tipi->p->p->left;
				if(y_tipi->col=='r')
				{
					z_tipi->p->col='n';
					y_tipi->col='n';
					z_tipi->p->p->col='r';
					z_tipi=z_tipi->p->p;
				}
				else
				{
					if(z_tipi==z_tipi->p->left)
					{
						z_tipi=z_tipi->p;
						right_rotate_tipi(z_tipi);
					}
					z_tipi->p->col='n';
					z_tipi->p->p->col='r';
					left_rotate_tipi(z_tipi->p->p);
				}
			}
		}
		testa_tipi->col='n';
		//end_of_tipi_insert_fixup(z_tipi);
	}
	
	//w_tipi è il tipo richiesto (creato o trovato)
	//aggiunta o ricerca della destinazione
	y_dest=nil_dest;
	x_dest=w_tipi->dest;
	w_dest=nil_dest;
	while(x_dest!=nil_dest)
	{
		y_dest=x_dest;
		ris=strcmp(id_dest, y_dest->id_dest);
		if(ris==0)
		{
			//destinazione presente
			w_dest=y_dest;
			break;
		}
		else
		{
			if(ris<0)
				x_dest=x_dest->left;
			else
				x_dest=x_dest->right;
		}
	}
	if(w_dest==nil_dest)
	{
		//controllo se esistono entrambe le entità
		if(double_entity_search(testa_ent,id_orig,id_dest)==0)
			return;
		ctr=1;
		z_dest=malloc(sizeof(struct dest));
		w_dest=z_dest;
		strcpy(z_dest->id_dest,id_dest);
		z_dest->orig=nil_orig;
		z_dest->numorig=0;
		z_dest->p=y_dest;
		if(y_dest==nil_dest)
			w_tipi->dest=z_dest; //albero entità vuoto
		else
		{
			if(strcmp(z_dest->id_dest, y_dest->id_dest) < 0)
				y_dest->left=z_dest;
			else
				y_dest->right=z_dest;
		}
		z_dest->right=nil_dest;
		z_dest->left=nil_dest;
		z_dest->col='r';
		//dest_insert_fixup(z_dest);
		while(z_dest->p->col=='r')
		{
			if(z_dest->p==z_dest->p->p->left)
			{
				y_dest=z_dest->p->p->right;
				if(y_dest->col=='r')
				{
					z_dest->p->col='n';
					y_dest->col='n';
					z_dest->p->p->col='r';
					z_dest=z_dest->p->p;
				}
				else
				{
					if(z_dest==z_dest->p->right)
					{
						z_dest=z_dest->p;
						left_rotate_dest(w_tipi,z_dest);
					}
					z_dest->p->col='n';
					z_dest->p->p->col='r';
					right_rotate_dest(w_tipi,z_dest->p->p);
				}
			}
			else
			{
				y_dest=z_dest->p->p->left;
				if(y_dest->col=='r')
				{
					z_dest->p->col='n';
					y_dest->col='n';
					z_dest->p->p->col='r';
					z_dest=z_dest->p->p;
				}
				else
				{
					if(z_dest==z_dest->p->left)
					{
						z_dest=z_dest->p;
						right_rotate_dest(w_tipi,z_dest);
					}
					z_dest->p->col='n';
					z_dest->p->p->col='r';
					left_rotate_dest(w_tipi,z_dest->p->p);
				}
			}
		}
		w_tipi->dest->col='n';
		//end_of_dest_insert_fixup(z_dest);
	}
	
	//w_dest è la destinazione richiesta (creata o trovata)
	//aggiunta o ricerca di origine
	y_orig=nil_orig;
	x_orig=w_dest->orig;
	while(x_orig!=nil_orig)
	{
		y_orig=x_orig;
		ris=strcmp(id_orig, y_orig->id_orig);
		if(ris==0)
		{
			//origine presente: la relazione è già presente
			return;
		}
		else
		{
			if(ris<0)
				x_orig=x_orig->left;
			else
				x_orig=x_orig->right;
		}
	}
	if(ctr==0)
	{
		if(single_entity_search(testa_ent,id_orig)==0)
			return;
	}
	z_orig=malloc(sizeof(struct orig));
	strcpy(z_orig->id_orig,id_orig);
	z_orig->p=y_orig;
	if(y_orig==nil_orig)
		w_dest->orig=z_orig; //albero entità vuoto
	else
	{
		if(strcmp(z_orig->id_orig, y_orig->id_orig) < 0)
			y_orig->left=z_orig;
		else
			y_orig->right=z_orig;
	}
	z_orig->right=nil_orig;
	z_orig->left=nil_orig;
	/*z_orig->col='r';
	//orig_insert_fixup(z_orig);
	while(z_orig->p->col=='r')
	{
		if(z_orig->p==z_orig->p->p->left)
		{
			y_orig=z_orig->p->p->right;
			if(y_orig->col=='r')
			{
				z_orig->p->col='n';
				y_orig->col='n';
				z_orig->p->p->col='r';
				z_orig=z_orig->p->p;
			}
			else
			{
				if(z_orig==z_orig->p->right)
				{
					z_orig=z_orig->p;
					left_rotate_orig(w_dest,z_orig);
				}
				z_orig->p->col='n';
				z_orig->p->p->col='r';
				right_rotate_orig(w_dest,z_orig->p->p);
			}
		}
		else
		{
			y_orig=z_orig->p->p->left;
			if(y_orig->col=='r')
			{
				z_orig->p->col='n';
				y_orig->col='n';
				z_orig->p->p->col='r';
				z_orig=z_orig->p->p;
			}
			else
			{
				if(z_orig==z_orig->p->left)
				{
					z_orig=z_orig->p;
					right_rotate_orig(w_dest,z_orig);
				}
				z_orig->p->col='n';
				z_orig->p->p->col='r';
				left_rotate_orig(w_dest,z_orig->p->p);
			}
		}
	}
	w_dest->orig->col='n';
	//end_of_orig_insert_fixup(z_orig);*/
	
	
	w_dest->numorig++;
	if(w_tipi->valid==1)
	{	if(w_dest->numorig > w_tipi->max)
		{
			w_tipi->max=w_dest->numorig;
			w_tipi->nummax=1;
		}
		else
		{
			if(w_dest->numorig == w_tipi->max)
				w_tipi->nummax++;
		}
	}
}
void delrel(char *id_orig,char *id_dest,char *id_rel)
{
	//if(FLAG_DEBUG==1)printf("**** DELREL %s to %s, %s \n",id_orig,id_dest,id_rel);
	struct tipi *x_tipo;
	struct dest *x_dest;
	struct orig *x,*y,*z;
	
	if(testa_ent==nil_ent)
		return;
	
	// cerco il tipo
	x_tipo=tipi_search(testa_tipi,id_rel);
	if(x_tipo==nil_tipi)
		return;
	
	//il tipo esiste ed è x_tipo: cerco la destinazione
	x_dest=dest_search(x_tipo->dest,id_dest);
	if(x_dest==nil_dest)
		return;
	
	//la destinazione esiste ed è x_dest: cerco l'origine e la rimuovo
	z=orig_search(x_dest->orig,id_orig);
	if(z==nil_orig)
		return;
	
	if(z->left==nil_orig || z->right==nil_orig)
	{
		y=z;
	}
	else
	{
		y=orig_successor(z);
	}
	if(y->left!=nil_orig)
	{
		x=y->left;
	}
	else
	{
		x=y->right;
	}
	x->p=y->p;
	if(y->p==nil_orig)
	{
		x_dest->orig=x;
	}
	else
	{
		if(y==y->p->left)
		{
			y->p->left=x;
		}
		else
		{
			y->p->right=x;
		}
	}
	if(y!=z)
	{
		strcpy(z->id_orig,y->id_orig);
	}
	/*if(y->col=='n')
	{
		//adjacents_delete_fixup(ent,x);
		while(x!=x_dest->orig && x->col=='n')
		{
			if(x==x->p->left)
			{
				z=x->p->right;
				if(z->col=='r')
				{
					z->col='n';
					x->p->col='r';
					left_rotate_orig(x_dest,x->p);
					z=x->p->right;
				}
				if(z->left->col=='n' && z->right->col=='n')
				{
					z->col='r';
					x=x->p;
				}
				else
				{
					if(z->right->col=='n')
					{
						z->left->col='n';
						z->col='r';
						right_rotate_orig(x_dest,z);
						z=x->p->right;
					}
					z->col=x->p->col;
					x->p->col='n';
					z->right->col='n';
					left_rotate_orig(x_dest,x->p);
					x=x_dest->orig;
				}
			}
			else
			{
				z=x->p->left;
				if(z->col=='r')
				{
					z->col='n';
					x->p->col='r';
					right_rotate_orig(x_dest,x->p);
					z=x->p->left;
				}
				if(z->right->col=='n' && z->left->col=='n')
				{
					z->col='r';
					x=x->p;
				}
				else
				{
					if(z->left->col=='n')
					{
						z->right->col='n';
						z->col='r';
						left_rotate_orig(x_dest,z);
						z=x->p->left;
					}
					z->col=x->p->col;
					x->p->col='n';
					z->left->col='n';
					right_rotate_orig(x_dest,x->p);
					x=x_dest->orig;
				}
			}
		}
		x->col='n';
		//end_of_adjacents_delete_fixup(ent,x);
	}*/
	
	free(y); //elimino definitivamente
	
	if(x_tipo->valid==1)
	{
		if(x_tipo->max==x_dest->numorig)
		{
			if(x_tipo->nummax<=1)
			{
				x_tipo->valid=0;
			}
			else
			{
				x_tipo->nummax--;
			}
		}
	}
	x_dest->numorig--;
}
void report()
{
	//if(FLAG_DEBUG==1)printf("**** REPORT\n");
	
	//controllo se sono presenti entità e relazioni
	if(testa_tipi==nil_tipi || testa_ent==nil_ent)
	{
		puts("none"); //puts aggiunge \n
		return;
	}
	flag_tipi=0;
	inorder_tipi_walk(testa_tipi);
	if(flag_tipi==0)
	{
		puts("none"); //puts aggiunge \n
	}
	else
	{
		puts(""); //puts aggiunge \n
	}
}
