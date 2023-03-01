#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char*argv[]){
  
  char passwort[20], servername[30], server_ip[20], ch;
  char localfile[30];
  char string[200];
  int size = 0;
  int size2 = 0;
  int size3 = 0;
  int help = 0;
  FILE *fp;
  FILE *fp1;
  FILE *fp2;
  fp = fopen ("config", "a+");
  if (argc == 2 && strcmp(argv[1],"--help") == 0)
  {
    help = 1;
  }

  while((ch = fgetc(fp)) != EOF)
  {
    size++;
  }
  size--;
  fclose(fp);

  if (help == 0 && (size == 0 || size == -1))
  {
    printf("Geben sie der IP-Adresse des Zielservers.\n");
    scanf("%s", server_ip);
    printf("Geben sie ihre Servername.\n");
    scanf("%s", servername);
    printf("Geben sie ihre Passwort.\n");
    scanf("%s", passwort);
    fp = fopen ("config", "w+");
    fprintf(fp, "%s %s %s",server_ip, servername, passwort);
    fclose(fp);
  }
  else
  {
    fp = fopen ("config", "r");
    fscanf(fp, "%s %s %s",server_ip, servername, passwort);
    fclose(fp);
    if (argc == 3 && strcmp(argv[1],"--file") == 0)
    {
      strcpy(localfile, argv[2]);
      printf("„Dateiname“ wird übertragen...\n");
      fp1 = fopen(localfile, "r");
      while( (ch = fgetc(fp1)) != EOF)
      {
        size2++;
      }
      fclose(fp1);
      size2--;
      

      sprintf(string, "sshpass -p %s ssh %s@%s 'cat remotefile' | diff -u - %s >diff.txt", passwort, servername, server_ip, localfile);
      system(string);

      fp2 = fopen("diff.txt", "r");

      while( (ch = fgetc(fp2)) != EOF)
      {
        size3++;
      }
      fclose(fp2);
      size3--;
      if (size3 == size2)
      {
        printf("„%s“ existiert nicht auf dem Zielserver, es wird vollständig übertragen (%d byte)...\n", localfile, size2);
      }
      else
      {
        printf("„%s“ existiert bereits auf dem Zielserver, berechne die Differenz...\n", localfile);
      }
      printf("„%s“ Differenz beträgt %d byte (Gesamtdateigröße ist %d byte).\n", localfile, size3, size2);
      sprintf(string,"sshpass -p %s scp diff.txt %s@%s:diff", passwort, servername, server_ip);
      system(string);
      sprintf(string,"sshpass -p %s ssh %s@%s patch remotefile -p0 <diff.txt\n", passwort, servername, server_ip);
      system(string);
      printf("Fertig.\n");
    }

    if (argc == 4 && strcmp(argv[1],"--file") == 0 && strcmp(argv[3],"--recover") == 0)
    {
      strcpy(localfile, argv[2]);
      printf("„%s“ wird wiederhergestellt...\n", localfile);
      printf("„%s“ existiert bereits auf dem lokalen Server, berechne die Differenz...\n", localfile);
      sprintf(string, "cat %s | sshpass -p %s ssh %s@%s diff -u - remotefile >diff.txt", localfile, passwort, servername, server_ip);
      system(string);

      fp1 = fopen(localfile, "r");
      while( (ch = fgetc(fp1)) != EOF)
      {
        size2++;
      }
      fclose(fp1);
      size2--;

      fp2 = fopen("diff.txt", "r");
      while( (ch = fgetc(fp2)) != EOF)
      {
        size3++;
      }
      fclose(fp2);
      size3--;

      printf("„%s“ Differenz beträgt %d byte (Gesamtdateigröße ist %d byte).\n", localfile, size3, size2);
      sprintf(string,"patch %s -p0 <diff.txt", localfile);
      system(string);
      printf("Fertig.\n");
    }
  }

  if (argc == 2 && strcmp(argv[1],"--help") == 0)
    {
      printf("Führen Sie das Programm zum ersten Mal aus, um Ihren servernamen, Ihr Passwort und der IP-Adresse des Ziel-servers zu speichern.\n");
      printf("Später kann es mit dem folgenden Befehl auf dem Terminal ausgeführt werden:\n");
      printf("./programmName --file Dateiname\n");
      printf("Das Programm sollte auch in der Lage sein, Dateien vom Server herunterzuladen. Der Download-Vorgang kann wie folgt ausgeführt werden:\n");
      printf("./programmName --file Dateiname --recover\n");
    } 
} 