#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fv[256], n, m;
char dictionar[256], textComprimat[100000];

typedef struct Nod{
	char caracter;
	int frecventa;
	struct Nod *stg, *dr;
}Nod;

typedef struct Cod{
	char caracter;
	char cod[8];
	int n;
}Cod;

struct Cod coduri[100];

int fromBinary(const char *s) {
  return (int) strtol(s, NULL, 2);
}

char *int2bin(int n) {
    int nbits = sizeof(n) * 8;
    char *s = malloc(nbits+1);
    s[nbits] = '\0';
    unsigned int u = *(unsigned int*)&n;
    int i;
    unsigned int mask = 1 << (nbits-1);
    for (i = 0; i < nbits; i++, mask >>= 1)
        s[i] = ((u & mask) != 0) + '0';
    return s;
}
Nod* creareNod(char c, int frecventa){
	struct Nod *r = (struct Nod*) malloc(sizeof(struct Nod));
	r->caracter = c;
	r->frecventa = frecventa;
	r->stg = NULL;
	r->dr = NULL;
	return r;
}

void citire(char *nume){
	char c;
	FILE *file;
	file = fopen(nume, "r");
	if(file){
		while((c = fgetc(file)) != EOF){
			if(fv[c] == 0){
				dictionar[n] = c;
				n++;
			}
			fv[c]++;
		}
		fclose(file);
	}
}

void sortareNoduri(Nod* noduri[], int n){
	struct Nod* aux;
	for(int i=0;i<n-1;i++)
		for(int j=i+1;j<n;j++)
			if(noduri[i]->frecventa > noduri[j]->frecventa){
				aux = noduri[i];
				noduri[i] = noduri[j];
				noduri[j] = aux;
			}
}

void inordine(Nod* r){
	if(!r)
		return;
	if(r->stg)
		inordine(r->stg);
	//if(r->caracter != '*')
	printf("%c ", r->caracter);
	if(r->dr)
		inordine(r->dr);
}

Nod* creareArbore(Nod* noduri[], int n){
	sortareNoduri(noduri, n);
	if(n==2){
		struct Nod* nodNou1 = creareNod('*', noduri[0]->frecventa + noduri[1]->frecventa);
			nodNou1->stg = noduri[0];
			nodNou1->dr = noduri[1];
		return nodNou1;
	}
	struct Nod* nodNou1 = creareNod('*', noduri[0]->frecventa + noduri[1]->frecventa);
			nodNou1->stg = noduri[0];
			nodNou1->dr = noduri[1];
	noduri[0] = nodNou1;
	for(int i=1;i<n-1;i++)
		noduri[i] = noduri[i+1];
	n = n-1;
	return creareArbore(noduri, n);
}

Nod* creareArboreDecomprimare(Cod coduri[], Nod* noduri[]){
	Nod* root = creareNod('*', 0);
	for(int i=0;i<m;i++){
		Nod *r = root;
		for(int j=0;j<strlen(coduri[i].cod)-1;j++){
			if(coduri[i].cod[j] == '0'){
				if(r->stg==NULL){
					r->stg = creareNod('*', 0);
					r = r->stg;
				}
				else
					r = r->stg;
			}
			else if(coduri[i].cod[j] == '1')
				if(r->dr == NULL){
					r->dr = creareNod('*', 0);
					r = r->dr;
				}
				else
					r = r->dr;		
		}
		if(coduri[i].cod[strlen(coduri[i].cod)-1] == '0')
			r->stg = creareNod(noduri[i]->caracter, 0);
		else
			r->dr = creareNod(noduri[i]->caracter, 0);
	}
	return root;
}

void creareCoduri(Nod* r, char arr[], int top){
	if(r->stg){
		arr[top] = '0';
		creareCoduri(r->stg, arr, top+1);
	}
	if(r->dr){
		arr[top] = '1';
		creareCoduri(r->dr, arr, top+1);
	}
	if(r->caracter != '*'){
		coduri[m].caracter = r->caracter;
		for(int i=0;i<top;i++)
			coduri[m].cod[i] = arr[i];
		m++;
	}
}

void afisareCoduri(){
	for(int i=0;i<m;i++){
		printf("%c-%s\n", coduri[i].caracter,coduri[i].cod);
	}
}

void comprimare(char *nume){
	FILE *file = fopen("comprimat.txt","w");
	FILE *file2 = fopen(nume,"r");
	FILE *file3 = fopen("compressed_.txt", "w");
	//FILE *file4 = fopen("compressed_to_1and0.txt","w");
	char c;
	char sirBiti[100000];
	sirBiti[0]='\0';
	if(!file){
		printf("Nu s-a putut deschide fisierul pentru scriere. ");
	}
	if(!file3){
		printf("Nu s-a putut deschide fisierul pentru scriere. ");
	}
	fprintf(file, "%d\n", m);
	for(int i=0;i<m;i++){
		if(coduri[i].caracter==' ')
			fprintf(file, "-_-");
		else if(coduri[i].caracter=='\n')
			fprintf(file, "-;-");
		else
		fprintf(file, "-%c-", coduri[i].caracter);
		fprintf(file, "%s\n", coduri[i].cod);
	}
	if(file2){
		while((c = fgetc(file2)) != EOF){
			for(int i=0;i<m;i++)
				if(coduri[i].caracter == c)
					strcat(sirBiti, coduri[i].cod);
		}
		fprintf(file, "%s", sirBiti);
		//fprintf(file4, "%s", sirBiti);
		char s[9]="";
		int n = strlen(sirBiti)/8;
		char *a = sirBiti;
		for(int i=0;i<=n;i++){
			strncpy(s,a,8);
			fprintf(file3, "%c", fromBinary(s));
			a = a + 8;
		}
	}
	fclose(file);
	fclose(file2);
	fclose(file3);
	//fclose(file4);
}

void sirComprimat(char *nume){
	FILE *file = fopen(nume,"r");
	char sir[50];
	if(file){
		fscanf(file,"%d",&m);
		for(int i=0;i<m;i++){
			fscanf(file,"%s",sir);
			if(sir[1]=='_')
				coduri[i].caracter = ' ';
			else if(sir[1]==';')
			coduri[i].caracter = '\n';
			else
				coduri[i].caracter = sir[1];
			for(int j=3;j<strlen(sir);j++)
				coduri[i].cod[j-3] = sir[j];
			coduri[i].cod[strlen(sir)] = '\0';
		}
	}
	fscanf(file, "%s", textComprimat);
	textComprimat[strlen(textComprimat)] = '\0';
}

void decomprimare(Nod* d){
	char textDecomprimat[]="";
	int l = 0;
	Nod* root = d;
	FILE *file = fopen("decomprimat.txt","w");
	if(!file)
		printf("Nu s-a putut deschide fisierul pentru scriere. ");
	for(int i=0;i<strlen(textComprimat);i++){
		if(textComprimat[i]=='0')
			root = root->stg;
			else
			root = root->dr;
		if(root->caracter!='*'){
			fprintf(file,"%c",root->caracter);
			root = d;
		}
	}
}

void decomprimareFisierCompressed_(){
	FILE *file = fopen("compressed_.txt","r");
	char c;
	int n=0;
	char sir[1000];
	sir[0] = '\0';
	if(file){
		while((c = fgetc(file)) != EOF){
			int ascii = (int)c;
			if(ascii<0)
				ascii = 256+ascii;
			char *ci = int2bin(ascii);
			ci = ci + 24;
			strcat(sir,ci);
		}
		strcpy(textComprimat,sir);
		textComprimat[strlen(textComprimat)] = '\0';
		fclose(file);
	}

}

int main(int argc, char *argv[]) {
	char optiune;
	optiune = argv[1][0];
	switch(optiune){
		case 'C': 
			citire(argv[2]);
			struct Nod* noduri[100];
			for(int i=0;i<n;i++){
				noduri[i] = creareNod(dictionar[i], fv[dictionar[i]]);
			}
			struct Nod *t = creareArbore(noduri, n);
			char arr[15];
			creareCoduri(t, arr, 0);
			comprimare(argv[2]);
			break;
		case 'D':
			sirComprimat(argv[2]);
			struct Nod* noduri2[100];
			for(int i=0;i<m;i++)
				noduri2[i] = creareNod(coduri[i].caracter, 0);
			struct Nod *d = creareArboreDecomprimare(coduri, noduri2);
			decomprimare(d);
			break;
		//am incercat in cazul E, sa obtin sirul comprimat din fisierul compressed_.txt, 
		//dar codurile tot din comprimat.txt sunt luate
		case 'E': 								
			sirComprimat(argv[2]); 				
			decomprimareFisierCompressed_(); 	
			struct Nod* noduri3[100];
			for(int i=0;i<m;i++)
				noduri3[i] = creareNod(coduri[i].caracter, 0);
			struct Nod *e = creareArboreDecomprimare(coduri, noduri3);
			decomprimare(e);
		break;
	}
	return(0);
}