#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define STRMAX 200


typedef struct Drug {
  int id;
  char name[STRMAX];
  int count;
} Drug;


void welcome();                       // show the welcome message
int count();                          // return the record count of the data.txt file
int valid(Drug drugs[], int id);      // check whether given id is in the drugs array
int match(Drug drugs[], char* name);  // check whether given name is in the drugs array
void create(Drug drugs[]);            // create drugs array array with data.txt file content 
void save(Drug drugs[]);              // create a new data.txt file with drugs array elements
void add(Drug drugs[]);               // add a new drug to the store 
void store(Drug drugs[]);             // increase specific drug's count
void issue(Drug drugs[]);             // decrease specific drug's count (issue drug)
void add(Drug drugs[]);               // add a new drug to the store 


int count() {
  FILE *fp;
  int c = 0;
  char val[STRMAX];

  fp = fopen("data.txt", "r");

  while(1) {
    if (feof(fp)) {
      break;
    }

    fgets(val, STRMAX, fp);
    c++;
  }

  fclose(fp);
  return c;
}


void create(Drug drugs[]) {
  FILE *fp;
  char val[STRMAX];
  Drug drug = {};

  fp = fopen("data.txt", "r");

  for(int i = 0; i < count()-1; i++) {
    if (feof(fp)) {
      break;
    }

    fgets(val, STRMAX, fp);
    sscanf(val, "%d_%d_%s\n", &drug.id, &drug.count, drug.name);

    drugs[i] = drug;
  }

  fclose(fp);
}


void save(Drug drugs[]) {
  FILE *fp;
  
  // save new data to temp.txt file
  fp = fopen("temp.txt", "w");
  for (int i = 0; i < count()-1; i++) {
    Drug drug = drugs[i];
    fprintf(fp, "%d_%d_%s\n", drug.id, drug.count, drug.name);
  }
  fclose(fp);

  // remove the data.txt file
  remove("data.txt");

  // rename the temp.txt file as data.txt file
  rename("temp.txt", "data.txt");
}


int valid(Drug drugs[], int id) {
  int stat = 0;
  Drug sDrug = {}; // selected drug

  for (int i = 0; i < count()-1; i++) {
    Drug drug = drugs[i];
    if (drug.id == id) {
      stat = 1;
      sDrug.id = drug.id;
      strncpy(sDrug.name, drug.name, STRMAX); 
      sDrug.count = drug.count;
    }
  }

  if (stat == 0) {
    printf("id %d is not exists..\n", id);
  } else {
    printf("id: %d | name: %s | count: %d\n", sDrug.id, sDrug.name, sDrug.count);
  }

  return stat;
}


void printDrugs(Drug drugs[]) {
  for (int i = 0; i < count() -1; i++) {
    Drug d = drugs[i];
    printf("id: %d, name: %s, count: %d\n", d.id, d.name, d.count);
  }
}


int main() {
  int id;
  Drug drugs[count()];

  printf("file is working..!\n");
  printf("count: %d\n", count());
  create(drugs);
  printDrugs(drugs);
  // save(drugs);

  printf("id: ");
  scanf("%d", &id);
  printf("stat: %d\n", valid(drugs, id));

  return 0;
}
