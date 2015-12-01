/*SERVER*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include<stdlib.h>

#define MAXLEN 70
#define PORT 8989
extern int errno;		/* eroarea returnata de unele apeluri */

void StergeMelodie(int client);
void Vote(int client);
void AddComment(int client);
void GuestMenu(int client);
void PoateVota(int client, char* num);
void StergeVot(int client);
char * conv_addr (struct sockaddr_in address)/* functie de convertire a adresei IP a clientului in sir de caractere */
{
  static char str[25];
  char port[7];
  strcpy (str, inet_ntoa (address.sin_addr));	 /* adresa IP a clientului */
  bzero (port, 7);/* portul utilizat de client */
  sprintf (port, ":%d", ntohs (address.sin_port));	
  strcat (str, port);
  return (str);
}

//Comanda 1-GATA
void Register(int client)
{
	printf("[server]Cineva vrea sa se inregistreze!!\n");
	FILE *f;
	int biti, already_exists = 0;
	int len_user = 0;
	int len_pass = 0;
	char line[60];
	char* user_from_file = (char*)malloc(30);
	char* user = (char*)malloc(30);
	char* del = (char*)malloc(30);
	char* pass = (char*)malloc(30);
	f = fopen("userparola.txt", "r");
	read(client, &len_user, sizeof(len_user));
	read(client, user, len_user);
	read(client, &len_pass, sizeof(len_pass));
	biti = read(client, pass, len_pass);



while (fgets(line, 60, f) != NULL)// && i<lineno)
{
	del = NULL;
	del = strtok(line, "$");
	strcpy(user_from_file, del);
	

	if (strcmp(user_from_file, user) == 0)
	
		already_exists = 1;
}
fclose(f);

write(client, &already_exists, sizeof(already_exists));
if (already_exists)
{
	printf("user deja luat!!!!");
	GuestMenu(client);
}
else{
	f = fopen("userparola.txt", "a");
	biti = fprintf(f, "%s$%s$%d", user, pass,1);
	fprintf(f, "\n");
	fclose(f);
	write(client, &biti, sizeof(biti));
	
	if (biti)
	{
		printf("Nou utilizator inregistrat!\n");
		GuestMenu(client);
	}
	else perror("Nu am reusit inregistrarea\n");

	

}
	
}
//GATA
void Schimba(char* x, char* y)
{
	char* aux = (char*)malloc(30);

	strcpy(aux, x);
	strcpy(x, y);
	strcpy(y, aux);


}
//Gata
void ShowGeneralTop(int client)
{
	printf("Sunt in ShowGeneralTop!\n");
	char line[1000];
	FILE* f;
	char* del = (char*)malloc(200);
	f = fopen("melodii.txt", "r");
	char clas[1000][6][1000];
	int i = 0;
	int w = 0;
	int k = 0;
	int n = 0;
	int j = 0;
	int nrmel = 0;
	while (fgets(line, 60, f) != NULL)
	{
		del = NULL;
		
		del = strtok(line, "^");
		strcpy(clas[i][0], del);
		clas[i][0][strlen(clas[i][0])] = 0;
		
		
		del = strtok(NULL, "^");
		strcpy(clas[i][1], del);
		clas[i][1][strlen(clas[i][1])] = 0;
		
		
		del = strtok(NULL, "^");
		strcpy(clas[i][2], del);
		clas[i][2][strlen(clas[i][2])] = 0;
		
		
		del = strtok(NULL, "^");
		strcpy(clas[i][3], del);
		
		clas[i][3][strlen(clas[i][3])] = 0;
		
		
		del = strtok(NULL, "^");
		strcpy(clas[i][4], del);
		clas[i][4][strlen(clas[i][4])] = 0;
		
		
		
		del = strtok(NULL, "\n");
		strcpy(clas[i][5], del);
		clas[i][5][strlen(clas[i][5])] = 0;
		
		i++;
		
	}
	for (j = 0; j < i - 1; j++)
	{
		for (k = j + 1; k < i; k++)
		{
			if (atoi(clas[j][3]) < atoi(clas[k][3]))
				for (w = 0; w < 6; w++)

					Schimba(clas[j][w], clas[k][w]);
		}
	}
	nrmel = i;
	
	write(client, &nrmel, sizeof(nrmel));
	for (i = 0; i < nrmel; i++)
	{
		
		n = strlen(clas[i][0]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][0], n);
		
		n = strlen(clas[i][1]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][1], n);
		
		n = strlen(clas[i][2]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][2], n);
		
		n = strlen(clas[i][3]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][3], n);
		
		n = strlen(clas[i][4]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][4], n);
		
		n = strlen(clas[i][5]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][5], n);

	}
	fclose(f);
}
//GATA
void ShowGenreTop(int client)
{

	FILE* f;
	int n,j,k,w, nrmel;
	char line[256];
	char* del = (char*)malloc(50);
	char clas[1000][6][1000];
	char* gen_client = (char*)malloc(30);
	char* piesa = (char*)malloc(30);
	char* artist = (char*)malloc(30);
	char* desc = (char*)malloc(30);
	char* vot = (char*)malloc(30);
	char* gen = (char*)malloc(30);
	char* link = (char*)malloc(30);
	char genuri[10][15];
//	char* gen_copy = (char*)malloc(30);
	f = fopen("melodii.txt", "r");
	if (f == NULL)
		printf("Eroare la deschiderea fisierului melodii.txt!\n");
	read(client, &n, sizeof(n));
	read(client, gen_client, n);
	printf("sunt in server si am citit genul %s\n", gen_client);
	int i = 0;
	int contor = 0;
	while (fgets(line, 60, f) != NULL)
	{
		i = 0;
		del = NULL;
		del = strtok(line, "^");
		strcpy(piesa, del);
		piesa[strlen(piesa)] = 0;


		del = strtok(NULL, "^");
		strcpy(artist, del);
		artist[strlen(artist)] = 0;


		del = strtok(NULL, "^");
		strcpy(desc, del);
		desc[strlen(desc)] = 0;


		del = strtok(NULL, "^");
		strcpy(vot, del);

		vot[strlen(vot)] = 0;


		del = strtok(NULL, "^");
		strcpy(gen, del);
		gen[strlen(gen)] = 0;



		del = strtok(NULL, "\n");
		strcpy(link, del);
		link[strlen(link)] = 0;

		//printf(" %s ** %s ** %s ** %s ** %s **%s\n", piesa, artist, desc, vot, gen, link);

		
		char* ptr=NULL;
		ptr = strstr(gen, gen_client);
		if (ptr != NULL)// && gen[&ptr - 1] == ',' && gen[&ptr + strlen(gen_client)] == ',')

		
			{
				strcpy(clas[i][0], piesa);
				strcpy(clas[i][1], artist);
				strcpy(clas[i][2], desc);
				strcpy(clas[i][3], vot);
				strcpy(clas[i][4], gen);
				strcpy(clas[i][5], link);
				i++;
			}
		
	}
	//am terminat citirea din fisier. acum sortez//asta-i bun
	for (j = 0; j < i - 1; j++)
	{
		for (k = j + 1; k < i; k++)
		{
			if (atoi(clas[j][3]) < atoi(clas[k][3]))
				for (w = 0; w < 6; w++)

					Schimba(clas[j][w], clas[k][w]);
		}
	}
	nrmel = i;

	write(client, &nrmel, sizeof(nrmel));
	for (i = 0; i < nrmel; i++)
	{

		n = strlen(clas[i][0]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][0], n);

		n = strlen(clas[i][1]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][1], n);

		n = strlen(clas[i][2]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][2], n);

		n = strlen(clas[i][3]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][3], n);

		n = strlen(clas[i][4]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][4], n);

		n = strlen(clas[i][5]) + 1;
		write(client, &n, sizeof(n));
		write(client, clas[i][5], n);

	}
	fclose(f);
	

}
void AddSong(int client)
{
	FILE* f;
	int biti = 0;
	int len;
	int already_exists = 0;
	char line[300];
	char* num = (char*)malloc(30);
	char* art = (char*)malloc(30);
	char* des = (char*)malloc(30);
	char* gen = (char*)malloc(30);
	char* lnk = (char*)malloc(30);
	char* song_from_file = (char*)malloc(30);
	char* art_from_file = (char*)malloc(30);
	char* complet = (char*)malloc(300);

	read(client, &len, sizeof(len));
	read(client, num, len);
	num[len] = 0;
	read(client, &len, sizeof(len));
	read(client, art, len);
	art[len] = 0;
	read(client, &len, sizeof(len));
	read(client, des, len);
	des[len] = 0;
	read(client, &len, sizeof(len));
	read(client, gen, len);
	gen[len] = 0;
	read(client, &len, sizeof(len));
	read(client, lnk, len);
	lnk[len] = 0;
	 printf(" %s ** %s ** %s ** %s ** %s **%s\n", num, art, des, gen, lnk);
	f = fopen("melodii.txt", "r");
	char* del = (char*)malloc(50);
	while (fgets(line, 200, f) != NULL)
		{
			
			del = NULL;
			del = strtok(line, "^");
			strcpy(song_from_file, del);
			del = strtok(NULL, "^");
			strcpy(art_from_file, del);
			if (strncmp(song_from_file, num, strlen(num)) == 0 && strncmp(art_from_file, art, strlen(art)) == 0)
				already_exists = 1;
}
	fclose(f);
	write(client, &already_exists, sizeof(already_exists));
	if (already_exists)
	{
		
		printf("Nu s-a adaugat nimic in top\n");
	//	AddSong(client);
		exit(1);
	}
	else
	{
	
		f = fopen("melodii.txt", "a");
		biti=fprintf(f, "%s^%s^%s^%d^%s^%s", num, art, des, 0, gen, lnk);
		
		fprintf(f, "\n");
		fclose(f);
		write(client, &biti, sizeof(biti));
		//AddSong(client);
	}
	

}
void AdminMenu(int client)
{
	int op;
	read(client, &op, sizeof(op));
	printf("clientul a dat comanda %d\n", op);
	switch (op)
	{
	case 1:
	{
		printf("Afisez clasamentul general\n");
		ShowGeneralTop(client);
		AdminMenu(client);
		break;
	}
	case 2:
	{
		printf("Afisez clasamentul dupa gen\n");
		ShowGenreTop(client);
		AdminMenu(client);
		break;
	}
	case 3:
	{
		printf("Votez o melodie");
		Vote(client);
		AdminMenu(client);
		break;
	}
	case 4:
	{
		printf("Adaug o melodie la top\n");
		AddSong(client);
		AdminMenu(client);
		break;
	}
	case 5:
	{
		printf("Utilizatorul vrea sa adauge un comentariu!\n");
		AddComment(client);
		AdminMenu(client);
		break;
	}
	case 6:
	{
		printf("sterg vot\n");
		StergeVot(client);
		AdminMenu(client);
		break;
	}
	case 7:
	{
		printf("Sterg melodie\n");
		StergeMelodie(client);
		AdminMenu(client);
		break;
	}
	case 8:
	{
		printf("ma deloghez\n");
		GuestMenu(client);
		break;
	}
	default:
	{

		printf("Utilizatorul nu stie sa foloseasca aplicatia!\n");
		AdminMenu(client);
		break;
	}
	}
}
void ClientMenu(int client)
{
	int op;
	read(client, &op, sizeof(op));
	printf("clientul a dat comanda %d\n", op);
	switch (op)
	{
	case 1:
	{
		printf("Afisez clasamentul general\n");
		ShowGeneralTop(client);
		ClientMenu(client);
		break;
	}
	case 2:
	{
		printf("Afisez clasamentul dupa gen\n");
		ShowGenreTop(client);
		ClientMenu(client);
		break;
	}
	case 3:
	{
		printf("Votez o melodie");
		Vote(client);
		ClientMenu(client);
		break;
	}
	case 4:
	{
		printf("Adaug o melodie la top\n");
		AddSong(client); 
		ClientMenu(client);
		break;
	}
	case 5:
	{
		printf("Utilizatorul vrea sa adauge un comentariu!\n");
		AddComment(client);
		ClientMenu(client);
		break;
	}
	case 6:
	{
		printf("Utilizatorul s-a delogat!\n");
		GuestMenu(client);
		break;
	}

	default:
	{
		printf("Utilizatorul nu stie sa foloseasca aplicatia!\n");
		GuestMenu(client);
		break;
	}
	}
}

//comanda 2- GATA
void Login(int client)
{
	printf("[server]Cineva vrea sa se autentifice!!\n");
	FILE *f;
	int biti, already_exists = 0;
	int len_user = 0;
	int len_pass = 0;
	char line[60];
	int poate_vota = 0;
	char* user_from_file = (char*)malloc(30);
	char* pass_from_file = (char*)malloc(30);
	char* user = (char*)malloc(30);
	char* pass = (char*)malloc(30);
	char* del = (char*)malloc(30);

	read(client, &len_user, sizeof(len_user));
	read(client, user, len_user);
	//read(sd, &poate_vota, 4);
	user[len_user] = 0;
	read(client, &len_pass, sizeof(len_pass));
	biti = read(client, pass, len_pass);
	pass[len_pass] = 0;
	PoateVota(client, user);//se trimite si poate_vota
	printf("Vreau sa ma inregistrez cu user-ul %s si parola %s\n", user, pass);
	f = fopen("userparola.txt", "r");
	user[strlen(user)] = 0;
	pass[strlen(pass)] = 0;
	//write(client, &poate_vota, 4);
	while (fgets(line, 60, f) != NULL)// && i<lineno)
	{
		del = NULL;
		del = strtok(line, "$");
		strcpy(user_from_file, del);
		user_from_file[strlen(user_from_file)] = 0;
		del = strtok(NULL, "$");
		strcpy(pass_from_file, del);
		pass_from_file[strlen(pass_from_file)] = 0;


		if (strcmp(user_from_file, user) == 0 && strcmp(pass_from_file, pass) == 0)
		{
			printf("Am gasit!\n");
			printf("NUmele extras este %s\n", user_from_file);
			printf("Parola extrasa este %s\n", pass_from_file);
			already_exists = 1;
			
		}
	}
	
		printf("already_exists=%d\n", already_exists);
		write(client, &already_exists, sizeof(already_exists));
		if (already_exists)

		{
			printf("Autentificare reusita!\n");
			ClientMenu(client);
		}
		else
			GuestMenu(client);
	
}




//Comanda 3 - GATA
void LoginAdmin(int client)
{
	printf("[server]Adminul vrea sergergerga se autentifice!!\n");
	FILE *f;
	int biti, already_exists = 0;
	int len_user = 0;
	int len_pass = 0;
	char line[60];
	char* user_from_file = (char*)malloc(30);
	char* pass_from_file = (char*)malloc(30);
	char* user = (char*)malloc(30);
	char* pass = (char*)malloc(30);
	char* del = (char*)malloc(30);

	read(client, &len_user, sizeof(len_user));
	read(client, user, len_user);
	read(client, &len_pass, sizeof(len_pass));
	biti = read(client, pass, len_pass);
	printf("Vreau sa ma inregistrez cu user-ul %s si parola %s\n", user, pass);
	
	f = fopen("admini.txt", "r");
	if (f == 0)
	{
		perror("Eroare in Login la dechiderea fisierului!");
		exit(2);
	}
	while (fgets(line, 60, f) != NULL)// && i<lineno)
	{

		del = NULL;
		del = strtok(line, "$");
		strcpy(user_from_file, del);
		del = strtok(NULL, "\n");
		strcpy(pass_from_file, del);


		if (strncmp(user_from_file, user, strlen(user) - 1) == 0 && strncmp(pass_from_file, pass, strlen(pass) - 1) == 0)
		{
			printf("Am gasit!\n");
			printf("NUmele extras este %s\n", user_from_file);
			printf("Parola extrasa este %s\n", pass_from_file);
			already_exists = 1;
		}
	}
	printf("already_exists=%d\n", already_exists);
	write(client, &already_exists, sizeof(already_exists));
	if (already_exists)
	{


		printf("Admin:Autentificare reusita!\n");
		AdminMenu(client);
	}
	
	{
		ClientMenu(client);
	}
	

}

//GATA
void GuestMenu(int client)
	{
		printf("hurr durr sunt in server\n");
		int op = 0;
		int len = 0;
		if (read(client, &op, sizeof(op)) == -1)//primesc comanda din registerclient-client
			perror("[server]Eroare la citirea comenzii!");
		switch (op)
		{
		case 1:
		{
			printf("Urmeaza sa inregistram clientul!\n");
			Register(client);
			break;
		}
		case 2:
		{
			printf("Clientul vrea sa se autentifice\n");
			Login(client);
			break;
		}
		case 3:
		{
			LoginAdmin(client);
			printf("Adminul vrea sa se autentifice!\n");
			break;
		}
		case 4:
		{
			printf("Clientul se deconecteaza de la server!\n");
			close(client);
			break;
		}
		default:
		{
			printf("nu stiu sa folosesc aplicatia\n");
			GuestMenu(client);
			break;
		}
		}
	}
//GATA
void StergeMelodie(int client)
{
	FILE* f1;
	FILE* f2;
	char* melodie = (char*)malloc(50);
	char * art = (char*)malloc(50);
	char* mel_from_client = (char*)malloc(50);
	char* art_from_client = (char*)malloc(50);
	char* line_copy = (char*)malloc(256);
	char* del = (char*)malloc(50);
	char line[256];
	int len;

	read(client, &len, 4);
	read(client, mel_from_client, len);
	mel_from_client[len] = 0;
	
	read(client, &len, 4);
	read(client, art_from_client, len);
	art_from_client[len] = 0;
	
	int already_exists = 0;
	f1 = fopen("melodii.txt","r");
	f2 = fopen("temp3.txt", "w");
	printf("inainte de while %s %s\n", mel_from_client,art_from_client);
	while (fgets(line, 256, f1) != NULL)
	{
		already_exists = 0;
		printf("in while\n");
		strcpy(line_copy, line);
		del = NULL;
		del = strtok(line, "^");
		strcpy(melodie, del);
		melodie[strlen(melodie)] = 0;
		del = strtok(NULL, "^");
		strcpy(art, del);
		art[strlen(art)] = 0;
		printf("%s %s %s\n", melodie, art, line_copy);
		if (strcmp(melodie, mel_from_client) == 0 && strcmp(art, art_from_client) == 0)
		{
			already_exists = 1;
		}
		else
		{
			fputs(line_copy, f2);
			already_exists = 0;
		}
		
	}
	write(client, &already_exists, 4);
	fclose(f1);
	fclose(f2);
	f1 = fopen("melodii.txt", "w");
	f2 = fopen("temp3.txt", "r");
	while (fgets(line, 256, f2) != NULL)
	{
		fputs(line, f1);
	

	}
	fclose(f1);
	fclose(f2);
	

}

//GATA
void PoateVota(int client, char* num)
{
	FILE* f;
	char* del=(char*)malloc(50);
	char* user = (char*)malloc(50);
	char* pass = (char*)malloc(50);
	char* vot = (char*)malloc(50);
	char line[256];
	f = fopen("userparola.txt","r");
	int poate_vota = 0;
	while (fgets(line, 60, f) != NULL)
	{

		del = NULL;
		del = strtok(line, "$");
		strcpy(user, del);
		del = strtok(NULL, "$");
		strcpy(pass, del);
		del = strtok(NULL, "\n");
		strcpy(vot, del);
		if (strcmp(user, num) == 0)
			if (atoi(vot) != 0)
				poate_vota = 1;
			else poate_vota = 0;
			
	}
	fclose(f);
write(client, &poate_vota, 4);
}
//GATA
void Vote(int client)
{
	FILE *f1, *f2;

	int len = 0;
	char* num = (char*)malloc(50);
	char* art = (char*)malloc(50);
	char* num_from_file = (char*)malloc(50);
	char* art_from_file = (char*)malloc(50);
	char* des_from_file = (char*)malloc(50);
	char* lnk_from_file = (char*)malloc(50);
	char* gen_from_file = (char*)malloc(50);
	char* vot_from_file = (char*)malloc(50);
	char* del = (char*)malloc(50);
	char* aux = (char*)malloc(50);
	char line[256];
	int vot;
	int raspuns = 0;
	//printf("aici!!!!!!!!!!");
	read(client, &len, sizeof(len));
	read(client, num, len);
	num[len] = 0;
	//printf("lungimea numelui este %d\n", len);
	
	read(client, &len, sizeof(len));
	read(client, art, len);
	art[len] = 0;
	//printf("lungimea srtistului este %d\n", len);
	//printf("am primit melodia %s si artistul %s", num, art);
	
	
	f1 = fopen("melodii.txt", "r");
	f2 = fopen("temp.txt", "w");

	while (fgets(line, 60, f1) != NULL)
	{
		strcpy(aux, line);
		del = NULL;
	del = strtok(line, "^");
	strcpy(num_from_file, del);
	num_from_file[strlen(num_from_file)] = 0;
	del = strtok(NULL, "^");
	strcpy(art_from_file, del);
	art_from_file[strlen(art_from_file)] = 0;
	//printf("compar %s cu %s si %s cu %s\n", num, num_from_file, art, art_from_file);

		if (strcmp(art_from_file, art) ==0 && strcmp(num_from_file, num) == 0)
		
		{
			//printf("%s si %s sunt egale!!!!!la fel si %s cu %s\n",num,num_from_file,art,art_from_file);
			del = strtok(NULL, "^");
			strcpy(des_from_file, del);

			del = strtok(NULL, "^");
			strcpy(vot_from_file, del);
			//num_from_file[strlen(num_from_file)] = 0;

			del = strtok(NULL, "^");
			strcpy(gen_from_file, del);

			del = strtok(NULL, "\n");
			strcpy(lnk_from_file, del);
			lnk_from_file[strlen(lnk_from_file)] = 0; 

			//strcpy(vot_from_file, del);
			vot = atoi(vot_from_file);
			printf("%s / %s / %s / %s / %s / %s\n", num_from_file, art_from_file, des_from_file, vot_from_file, gen_from_file, lnk_from_file);
			//printf("%s\n", vot_from_file);
			//printf("votul intial este %d \n", vot);
			vot++;
			//("votul final este %d\n", vot);
			fprintf(f2, "%s^%s^%s^%d^%s^%s", num_from_file, art_from_file, des_from_file, vot, gen_from_file, lnk_from_file);
			fprintf(f2, "\n");
			raspuns = 1;
			printf("Melodie votata!");

		}
		else 
		{

			fprintf(f2,"%s",aux);
			//fprintf( f2,"\n");

		}
	}
	fclose(f1);
	fclose(f2);
	
		f1 = fopen("melodii.txt","w");
		f2 = fopen("temp.txt","r");

		while (fgets(line, 256,f2) != NULL)
		{
			fputs(line, f1);

		}
		fclose(f1);
		fclose(f2);
		write(client, &raspuns, 4);

		
		
	

}
//GATA
int SongExists(char* num, char* art)
{
	FILE* f,f2;
	char* song_from_file = (char*)malloc(50);
	char* art_from_file = (char*)malloc(50);
	char line[256];
	char* del = (char*)malloc(50);
	int already_exists=0;
	f = fopen("melodii.txt","r");

	while (fgets(line, 200, f) != NULL)
	{

		del = NULL;
		del = strtok(line, "^");
		strcpy(song_from_file, del);
		del = strtok(NULL, "^");
		strcpy(art_from_file, del);
		if (strncmp(song_from_file, num, strlen(num)) == 0 && strncmp(art_from_file, art, strlen(art)) == 0)
			already_exists = 1;
	}
	fclose(f);
	
	return already_exists;
}
//GATA
void AddComment(int client)
{	
	printf("Sunt in ADDCOMMMENTQ!!!!!\n");
	FILE* f;
	int biti=0;
	char* comment = (char*)malloc(150);
	char* art = (char*)malloc(50);
	char* num = (char*)malloc(50);
	int len=0;
	int already_exists=0;
	read(client, &len, sizeof(len));
	read(client, num, len);
	num[len] = 0;
	read(client, &len, sizeof(len));
	read(client, art, len);
	art[len] = 0;
	already_exists = SongExists(num, art);
	write(client, &already_exists, sizeof(already_exists));
		if (already_exists)
		{
			f = fopen("comentarii.txt", "a");
			read(client, &len, sizeof(len));
			read(client, comment, len);
			printf("comentariu %s\n", comment);
			biti=fprintf(f,"%s/%s/%s",num,art,comment);
			fprintf(f, "\n");
			fclose(f);
			write(client, &biti, sizeof(biti));
		}

}
//GATA
void StergeVot(int client)
{
	FILE* f1;int intvot = 0;
	FILE* f2;
	char line[256];
	char* user = (char*)malloc(30);
	char* pass = (char*)malloc(30);
	char*del = (char*)malloc(30);
	char* vot = (char*)malloc(30);
	char* nume = (char*)malloc(30);
	int len;
	read(client, &len, sizeof(len));
	read(client, nume, len);
	nume[strlen(nume)] = 0;
	printf("sunt in sterge vot\n");
	
	f1 = fopen("userparola.txt", "r");
	f2 = fopen("temp2.txt", "w");
	printf("sunt in sterge vot\n");
	while (fgets(line, 200, f1) != NULL)
	{
		printf("linia este %s\n", line);
		del = NULL;
		del = strtok(line, "$");
		strcpy(user, del);
		del = strtok(NULL, "$");
		strcpy(pass, del);
		del = strtok(NULL, "\n");
		strcpy(vot, del);
		printf("Sterge voT:%s %s %s\n", user, pass, vot);
		printf("compar %s cu %s\n", user, nume);
		int ok = 0;
		ok = strcmp(user, nume);
		printf("ok=%d\n", ok);
		if (ok== 0)
		{
			fprintf(f2, "%s$%s$%d", user, pass, 0);
			fprintf(f2, "\n");
		}
		else
		{
			fprintf(f2, "%s$%s$%s", user, pass, vot);
			fprintf(f2, "\n");
		}
		}
	fclose(f1);
	fclose(f2);
	f1 = fopen("userparola.txt", "w");
	f2 = fopen("temp2.txt", "r");
	while (fgets(line, 256, f2) != NULL)
	{
		fputs(line, f1);

	}
	fclose(f1);
	fclose(f2);
	

}
int main()
	{
		struct sockaddr_in server;	/* structurile pentru server si clienti */
		struct sockaddr_in from;
		int sd, client;		/* descriptori de socket */
		int optval = 1; 			/* optiune folosita pentru setsockopt()*/
		int fd;			/* descriptor folosit pentru*/
		char msg[100];			 /*  parcurgerea listelor de descriptori */
		int nfds;			/* numarul maxim de descriptori */
		int len;			/* lungimea structurii sockaddr_in */
		int pid;
		int length = sizeof(from);
		if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			perror("[server] Eroare la socket().\n");
			return errno;
		}
		setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
		bzero(&server, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		server.sin_port = htons(PORT);
		int yes = 1;
		//char yes='1'; // use this under Solaris
		if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1)
		{
			perror("[server] Eroare la bind().\n");
			return errno;
		}
		if (listen(sd, 5) == -1)
		{
			perror("[server] Eroare la listen().\n");
			return errno;
		}

		printf("[server] Asteptam la portul %d...\n", PORT);

		len = sizeof(from);
		while (1)
		{
			client = accept(sd, (struct sockaddr *) &from, &len);
			if (client < 0)
			{
				perror("[server] Eroare la accept\n");
			}
			pid = fork();
			if (pid < 0)
				perror("Eroare la fork");
			if (pid == 0)
			{
				GuestMenu(client);
				//close(client);
			}
		}
	}
