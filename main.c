#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>


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


int match(Drug drugs[], char *name) {
  char pattern[STRMAX];
  int matched = 0;
  regex_t preg;

  // create the pattern
  sprintf(pattern, ".*%s.*", name);

  // compile the regex
  int r = regcomp(&preg, pattern, 0);
  if (r != 0) {
    perror("regex not compiled/n");
  }

  // match drug's name with regex
  for(int i = 0; i < count() -1; i++) {
    Drug drug = drugs[i];
    int r = regexec(&preg, drug.name, 0, NULL, 0);
    if (r == 0) {
      matched = 1;
      printf("id: %d, name: %s, count: %d\n", drug.id, drug.name, drug.count);
    }
  }

  if (matched == 0) {
    printf("their is no match..!\n");
  }
  
  return matched;
}


void issue(Drug drugs[]) {
  int id;
  int c; // count
  char name[STRMAX];

  printf("Enter the drug name: ");
  scanf("%199s", name);

  if (match(drugs, name) == 1) {
    printf("Enter the id: ");
    scanf("%d", &id);

    if (valid(drugs, id) == 1) {
      printf("Enter the count want to issue: ");
      scanf("%d", &c);

      if (c >= 1) {
        for (int i = 0; i < count() -1; i++) {
          if (drugs[i].id == id) {
            if (drugs[i].count - c > 0) {
              drugs[i].count -= c;
              printf("issue %d items from %s\n", c, drugs[i].name);
              printf("remaining count of %s is: %d\n", drugs[i].name, drugs[i].count);
            } else {
              printf("Not enough items to issue...\n");
            }
            break;
          }
        }
        save(drugs); // save data 
      } else {
        printf("Invalid count, can't issue any items ...\n");
      }
    }
  }

}


void store(Drug drugs[]) {
  int id;
  int c; // count
  char name[STRMAX];

  printf("Enter the drug name: ");
  scanf("%199s", name);

  if (match(drugs, name) == 1) {
    printf("Enter the id: ");
    scanf("%d", &id);

    if (valid(drugs, id) == 1) {
      printf("Enter the count you want to add: ");
      scanf("%d", &c);

      if (c >= 1) {
        for (int i = 0; i < count() -1; i++) {
          if (drugs[i].id == id) {
            drugs[i].count += c;
            printf("Issue %d items from %s\n", c, drugs[i].name);
            printf("New item count of %s is: %d\n", drugs[i].name, drugs[i].count);
            break;
          }
        }
        save(drugs); // save data
      } else {
        printf("Invalid count, can't add any items to store ...\n");
      }
    }
  }
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
  create(drugs);
  issue(drugs);
  store(drugs);
  return 0;
}
