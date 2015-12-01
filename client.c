 /*CLIENT*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>


extern int errno;
int sd;
int port;

//numa printf-uri ce-s aicea!
void ClientMenu(int poate_vota);
void ShowGenreTop(void);
void ShowGeneralTop(void);
int ClientCommander(void);
void AddSong(void);
void AdminMenu(void);
void StergeVot(void);
void StergeMelodie(void);

void ShowAdminMenu()
{
	printf("~~Meniu Admin~~\n");
	printf("1.Afisare clasament general\n");
	printf("2.Afisare clasament dupa gen\n");
	printf("3.Voteaza o melodie\n");
	printf("4.Adauga o melodie la top\n");
	printf("5.Adauga un comentariu\n");
	printf("6.Restrictioneaza drept de vot\n");
	printf("7.Sterge melodie\n");
	printf("8.Logout\n");
	printf("\n");
	printf("Introduceti comanda:\n");
	AdminMenu();
}
void ShowGuestMenu()
{
printf("~~ Guest Menu ~~\n");
printf("1.Register client\n");
printf("2.Login client\n");
printf("3.Login administrator\n");
printf("4.Quit\n");
printf("Introduceti comanda:");
ClientCommander();
} 
void ShowClientMenu(int poate_vota)
{
	printf("~~Meniu Client~~\n");
	printf("1.Afisare clasament general\n");
	printf("2.Afisare clasament dupa gen\n");
	printf("3.Voteaza o melodie\n");
	printf("4.Adauga o melodie la top\n");
	printf("5.Adauga un comentariu\n");
	printf("6.Logout\n");
	printf("\n");
	printf("Introduceti comanda:\n");
	ClientMenu(poate_vota);
}

//comanda 1 - GATA
int RegisterClient()
{
int already_exists;
char * nume=(char *)malloc(30);
char * parola=(char *)malloc(30);
int biti;
int len_user=0;
int len_pass=0;

printf("Introduceti numele de utilizator:");
fgets(nume, 30, stdin);
printf("Introduceti parola:");
fgets(parola, 30, stdin);
 len_pass= strlen(parola)-1;
 len_user= strlen(nume)-1;

printf("ma inregistrez cu userul %s si parola %s\n", nume, parola);
printf("lungimea useurlui este %d si lungimea parolei este %d\n", len_user, len_pass);

write(sd,&len_user,4);
write(sd,nume,len_user);
write(sd,&len_pass,4);
biti=write(sd,parola,len_pass);

if (biti==0)
perror("nu\n");

read(sd, &already_exists, sizeof(already_exists));
printf("already_exists= %d\n", already_exists);
if (already_exists)
{
	printf("Username deja luat! Incercati altul.\n");
	ShowGuestMenu();
	exit(1);
	
}
else
{
	read(sd, &biti, sizeof(biti));

	if (biti)
	{
		printf("SUccesssss\n");
		ShowGuestMenu();
	}
	else printf("u're dumb as fuck\n");
	
}


return 0;
}
//GATA
void ShowGeneralTop()
{
	int nrmel,i,n;
	char clas[1000][6][30];
	read(sd, &nrmel, sizeof(nrmel));
	for (i = 0; i < nrmel; i++)
	{
		read(sd, &n, sizeof(n));
		read(sd, clas[i][0], n);
		read(sd, &n, sizeof(n));
		read(sd, clas[i][1], n);
		read(sd, &n, sizeof(n));
		read(sd, clas[i][2], n);
		read(sd, &n, sizeof(n));
		read(sd, clas[i][3], n);
		read(sd, &n, sizeof(n));
		read(sd, clas[i][4], n);
		read(sd, &n, sizeof(n));
		read(sd, clas[i][5], n);
		
	}
	for (i = 0; i < nrmel; i++)
	{
	printf("~~Locul %d~~\n", i+1);
	printf("melodie:%s\n", clas[i][0]);
	printf("artist:%s\n", clas[i][1]);
	printf("voturi:%s\n", clas[i][3]);
	printf("gen:%s\n", clas[i][4]);
	printf("link:%s\n", clas[i][5]);
	printf("\n");

	}

}

//GATA
void ShowGenreTop()
{
	char* gen = (char*)malloc(30);
	printf("Introduceti genul muzical:");
	scanf("%s", gen);
	int n;
	int i,nrmel;
	char clas[1000][6][1000];
	n = strlen(gen) + 1;
	write(sd, &n, sizeof(n));
	write(sd, gen, n);
	read(sd, &nrmel, sizeof(nrmel));
	for (i = 0; i < nrmel; i++)
	{
		read(sd, &n, sizeof(n));
		read(sd, clas[i][0], n);
		
		read(sd, &n, sizeof(n));
		read(sd, clas[i][1], n);
		
		read(sd, &n, sizeof(n));
		read(sd, clas[i][2], n); 
		
		read(sd, &n, sizeof(n));
		read(sd, clas[i][3], n);

		read(sd, &n, sizeof(n));
		read(sd, clas[i][4], n);

		read(sd, &n, sizeof(n));
		read(sd, clas[i][5], n);

	}
	for (i = 0; i < nrmel; i++)
	{
		printf("~~Locul %d~~\n", i + 1);
		printf("melodie:%s\n", clas[i][0]);
		printf("artist:%s\n", clas[i][1]);
		printf("voturi:%s\n", clas[i][3]);
		printf("gen:%s\n", clas[i][4]);
		printf("link:%s\n", clas[i][5]);
		printf("\n");

	}

	

}

//GATA

void Vote()
{
	char* num = (char*)malloc(50);
	char* art = (char*)malloc(50);
	int raspuns = 0;
	printf("Introduceti melodia:");
	fgets(num, 50, stdin);
	printf("Introduceti artistul:");
	fgets(art, 50, stdin);
	int len = 0;
	len = strlen(num) - 1;
	write(sd, &len, 4);
	write(sd, num, len);
	len = strlen(art) - 1;
	write(sd, &len, 4);
	write(sd, art, len);

	read(sd, &raspuns, 4);
	if (raspuns)
		printf("melodie votata cu succes!");
	else printf("Melodia pe care ati introdus-o nu exista!");

}


//GATA
void AddComment()
{
	char* comment = (char*)malloc(150);
	char* num = (char*)malloc(50);
	char* art = (char*)malloc(50);
	int len = 0;
	int biti = 0;
	int already_exists = 0;
	printf("Introduceti melodia:sdcdsvc");
	fgets(num, 50, stdin);
	printf("Introduceti artistul:");
	fgets(art, 50, stdin);
	len = strlen(num)-1;
	write(sd, &len, sizeof(len));
	write(sd, num, len);
	len = strlen(art)-1;
	write(sd, &len, sizeof(len));
	write(sd, art, len);
	read(sd, &already_exists, sizeof(already_exists));
	if (already_exists)
	{
		printf("Introduceti comentariul:");
		fgets(comment, 150, stdin);
		len = strlen(comment)-1;
		printf("%s\n", comment);
		write(sd, &len, sizeof(len));
		write(sd, comment, len);
		read(sd, &biti, sizeof(biti));
		if (biti)
		{
			printf("Comentariu adaugat cu succes!\n");
		}
		else printf("Eroare la adaugarea comentariului!\n");
	}
	else printf("Melodia cautata nu exista!\n");
	
}

//GATA
void AddSong()
{
	int biti;
	int len;
	int already_exists = 0;
char* temp = (char*)malloc(30);

	printf("Introduceti numele:");
	fgets(temp,50, stdin);	
	
	len = strlen(temp)-1;

	write(sd, &len, sizeof(len));
	write(sd, temp, len);

	printf("Introduceti artistul:");
	fgets(temp, 50 ,stdin);
	
	len = strlen(temp) -1;
	write(sd, &len, sizeof(len));
	write(sd, temp, len);

	printf("Introduceti o descriere:");
	fgets(temp, 50 ,stdin);

	len = strlen(temp)-1 ;
	write(sd, &len, sizeof(len));
	write(sd, temp, len);

	printf("Introduceti genul/genurile muzical/e(daca sunt mai multe, vor fi separate prin virgula!):");
	fgets(temp, 50 ,stdin);

	len = strlen(temp)-1 ;
	write(sd, &len, sizeof(len));
	write(sd, temp, len);

	printf("Introduceti linkul:");
	fgets(temp, 50 ,stdin);
	
	len = strlen(temp)-1 ;
	write(sd, &len, sizeof(len));
	write(sd, temp, len);
	

	read(sd, &already_exists, sizeof(already_exists));
	if (already_exists == 1)
	{
		printf("Melodia exista deja!\n");
		//ShowClientMenu();
		//AddSong();
	}
	else
	{
		read(sd, &biti, sizeof(biti));
		if (biti)
			printf("Melodie adaugata cu succes!\n");
		else printf("Eroare la adaugarea melodiei in fisier!\n");
		//AddSong();
	}
	
}

void AdminMenu()
{
	int com=0;
	char term;
	

	if (scanf("%d%c", &com, &term) != 2 || term != '\n')
	{
		printf("Comanda necunosuta!Incercati din nou!\n");
		ShowAdminMenu();
	}

	printf("am citit com=%d\n", com);
	write(sd, &com, sizeof(com));
	switch (com)
	{
	case 1:{
		ShowGeneralTop();
		ShowAdminMenu();
		break;
	}
	case 2:
	{
		ShowGenreTop();
		ShowAdminMenu();
		break;
	}
	case 3:
	{
		Vote();
		ShowAdminMenu();
		break;
	}
	case 4:
	{
		AddSong();
		ShowAdminMenu();
		break;
	}
	case 5:
	{
		AddComment();
		ShowAdminMenu();
		break;
	}
	case 6:
	{
		StergeVot();
		ShowAdminMenu();
		break;
	}
	case 7:
	{
		StergeMelodie();
		ShowAdminMenu();
		break;
	}

	case 8:
	{
		printf("Logout successful\n");
		ShowGuestMenu();
		break;
	}
	default:
	{
		printf("Comanda necunoscuta! Incercati din nou\n");
		ShowAdminMenu();
		break;
	}
	}
}
void StergeMelodie()
{
	char* num = (char*)malloc(50);
	char* art = (char*)malloc(50);
	int already_exists = 0;
	printf("Introduceti melodia:");
	fgets(num, 50, stdin);
	printf("Introduceti artistul:");
	fgets(art, 50, stdin);

	

	//num[strlen(num)-1] = 0;
	//art[strlen(art)-1] = 0;
	printf("%s %s\n", num, art);

	int len1 = strlen(num)-1;
	int len2 = strlen(art)-1;
	write(sd, &len1, 4);
	write(sd, num, len1);
	
	
	write(sd, &len2, 4);
	write(sd, art, len2);
	
	read(sd, &already_exists, 4);
	printf("aiciiiiii\n");
	if (already_exists)
		printf("Melodie stearsa cu succes!\n");
	else printf("Nu am gasit melodia!\n");
	

}
void ClientMenu(int poate_vota)
{
	int com;
	char term;
	

	if (scanf("%d%c", &com, &term) != 2 || term != '\n')
	{
		printf("Comanda necunosuta!Incercati din nou!\n");
		ClientMenu(poate_vota);
	}
		
	printf("am citit com=%d\n", com);
	write(sd, &com, sizeof(com));
	switch (com)
	{
	case 1:{
		ShowGeneralTop();
		ShowClientMenu(poate_vota);
		break;
	}
	case 2:
	{
		ShowGenreTop();
		ShowClientMenu(poate_vota);
		break;
	}
	case 3:
	{
		if (poate_vota == 0)
			printf("Nu ai voie sa votezi!");
		else
		Vote();
		ShowClientMenu(poate_vota);
		break;
	}
	case 4:
	{
		AddSong();
		ShowClientMenu(poate_vota);
		break;
	}
	case 5:
	{
		AddComment();
		ShowClientMenu(poate_vota);
		break;
	}
	case 6:
	{
		printf("Logout successful\n");
	ShowGuestMenu();
	break;
	}
	default:
	{
		printf("Comanda necunoscuta! Incercati din nou\n");
		ClientMenu(poate_vota);
		break;
	}
	}
}
//comanda 2- mai trebuie continuat cu meniu client, dar te poti autentifica
int LoginClient()
{
	int already_exists;
	char * nume = (char *)malloc(30);
	char * parola = (char *)malloc(30);
	int biti;
	int poate_vota = 0;
	int len_user = 0;
	int len_pass = 0;
    printf("Introduceti numele de utilizator:");
	fgets(nume,30,stdin);
	printf("Introduceti parola:");
	fgets(parola, 30, stdin);
	len_pass = strlen(parola)-1;
	len_user = strlen(nume)-1;
	
	write(sd, &len_user, 4);
	write(sd, nume, len_user);
	write(sd, &len_pass, 4);
	biti = write(sd, parola, len_pass);
	read(sd, &poate_vota, 4);
	read(sd, &already_exists, sizeof(already_exists));

	if (already_exists==0)
	{
		printf("Username sau parola incorecta!Incercati din nou!\n");
		ShowGuestMenu();
		exit(1);
	}
	else
	{
		printf("Autentificare reusita!\n");
		printf("Te-ai autentificat cu user-ul %s si parola %s\n", nume, parola);
		ShowClientMenu(poate_vota);
	}

}


//comanda 3-GATA
void LoginAdmin()
{
	int already_exists;
	char * nume = (char *)malloc(30);
	char * parola = (char *)malloc(30);
	int biti;
	int len_user = 0;
	int len_pass = 0;
	printf("Introduceti numele de utilizator:");
	scanf("%s", nume);
	printf("Introduceti parola:");
	scanf("%s", parola);
	len_pass = strlen(parola) ;
	len_user = strlen(nume) ;

	write(sd, &len_user, 4);
	write(sd, nume, len_user);
	write(sd, &len_pass, 4);
	biti = write(sd, parola, len_pass);

	read(sd, &already_exists, sizeof(already_exists));
	if (already_exists == 0)
	{
		printf("Username sau parola incorecta!Incercati din nou!\n");
		ShowGuestMenu();
		exit(1);
	}
	else
	{
		printf("Autentificare reusita!\n");
		printf("Te-ai autentificat cu user-ul %s si parola %s\n", nume, parola);
		ShowAdminMenu();
	}

}
void StergeVot()
{
	char* num=(char*)malloc(50);
	printf("Introduceti numele de utilizator:");
	fgets(num, 50, stdin);
	int len;
	len = strlen(num)-1;
	write(sd, &len, sizeof(len));
	write(sd, num, len);
	
}
int ClientCommander()
{

	int com;
	char term;

	if (scanf("%d%c", &com, &term) != 2 || term!= '\n')
	{
		printf("Comanda necunoscuta! Incercati din nou!\n");

		ClientCommander();
	}
	write(sd, &com, sizeof(com));
	printf("Am trimis la server comanda %d\n", com);
	
		switch (com)
		{
		case 1:
		{

			RegisterClient();
			//trimit comanda de register in main-server
			break;
		}
		case 2:
		{

			LoginClient();
			break;
		}
		case 3:{
			LoginAdmin();
			break;
		}
		case 4:
		{
			printf("Aplicatia a fost inchisa!\n");
			close(sd);
			break;
		}
		default:{
			printf("Comanda necunoscuta!Incercati din nou\n");
			ShowGuestMenu();
			break;
		}
		
	}
}
int main (int argc, char *argv[])
{
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char* msg=(char*)malloc(1000);		// mesajul trimis
  if (argc != 3)/* exista toate argumentele in linia de comanda? */
    {
      printf ("[client] Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }
  port = atoi (argv[2]);
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[client] Eroare la socket().\n");
      return errno;
    }
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

 ShowGuestMenu();
 

 
 return 0;
}
