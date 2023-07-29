#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdbool.h>
#include <ctype.h>


#define STRMAX 200


typedef struct Drug {
  int id;
  char name[STRMAX];
  int count;
} Drug;


void welcome();                       // show the welcome message
void add_head();                      // header message for add function
void store_head();                    // header message for store function
void issue_head();                    // header message for issue function
int count();                          // return the record count of the data.txt file
int valid(Drug drugs[], int id);      // check whether given id is in the drugs array
int match(Drug drugs[], char* name);  // check whether given name is in the drugs array
void create(Drug drugs[]);            // create drugs array array with data.txt file content 
void save(Drug drugs[]);              // create a new data.txt file with drugs array elements
void store(Drug drugs[]);             // add spedific items to the stock
void issue(Drug drugs[]);             // decrease specific drug's count (issue drug)
void add(Drug drugs[]);               // add a new drug to the store 


void welcome() {
  printf("\n<<-####: PHARMACY :####->>\n");
  printf("1. Add new Medicine\n");
  printf("2. Sell Medicine\n");
  printf("3. New stock\n");
  printf("4. Exit\n");
}


void add_head() {
  system("clear");
  printf("<---####: ADD :####--->\n");
}


void issue_head() {
  system("clear");
  printf("<---####: SELL :####--->\n");
}


void store_head() {
  system("clear");
  printf("<---####: STORE :####--->\n");
}

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
    perror("regex not compiled\n");
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

  issue_head();
  printf("\nEnter the drug name: ");
  scanf("%199s", name);

  issue_head();
  printf("##: matched items :##\n");
  if (match(drugs, name) == 1) {
    printf("\nEnter the id: ");
    scanf("%d", &id);

    issue_head();
    printf("##: selected item :##\n");
    if (valid(drugs, id) == 1) {
      printf("\nEnter the count want to issue: ");
      scanf("%d", &c);

      if (c >= 1) {
        for (int i = 0; i < count() -1; i++) {
          if (drugs[i].id == id) {
            if (drugs[i].count - c > 0) {
              drugs[i].count -= c;
              issue_head();
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

  store_head();
  printf("\nEnter the drug name: ");
  scanf("%199s", name);

  store_head();
  printf("##: matched items :##\n");
  if (match(drugs, name) == 1) {
    printf("\nEnter the id: ");
    scanf("%d", &id);

    store_head();
    if (valid(drugs, id) == 1) {
      printf("\nEnter the count you want to add: ");
      scanf("%d", &c);

      if (c >= 1) {
        for (int i = 0; i < count() -1; i++) {
          if (drugs[i].id == id) {
            drugs[i].count += c;
            store_head();
            printf("stored %d items from %s\n", c, drugs[i].name);
            printf("New item count of %s is: %d\n", drugs[i].name, drugs[i].count);
            break;
          }
        }
        save(drugs); // save data
      } else {
        store_head();
        printf("Invalid count, can't add any items to store ...\n");
      }
    }
  }
}


void add(Drug drugs[]) {
  regex_t preg; 
  bool alreadyUse = false;
  char pattern[STRMAX + 10];
  char name[STRMAX];
  char res;

  // get the new item name
  add_head();
  printf("\nEnter the item name: ");
  scanf("%s", name);

  if (strlen(name) > 3) {
    // show match items
    sprintf(pattern, ".*%s.*", name);

    if (regcomp(&preg, pattern, 0)) {
      printf("regex is not compiled..\n");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count() -1; i++) {
      // check whether new name is already used
      if (!strcmp(name, drugs[i].name)) {
        printf("this name is already using, try with another name..!\n");
        alreadyUse = true;
        break;
      }

      // show the all names that match with given name pattern
      char r = regexec(&preg, drugs[i].name, 0, NULL, 0);
      if (r == 0) {
        add_head();
        printf("id: %d, name: %s, count: %d\n", drugs[i].id, drugs[i].name, drugs[i].count);
      }
    }

    if (!alreadyUse) {
      printf("\nDo you want to add \"%s\" item to the store (Y/n)?: ", name);
      scanf("%s", &res);

      res = toupper(res); 
      if (res == 'Y') {
        FILE *fp = fopen("data.txt", "a");
        fprintf(fp, "%d_%d_%s\n", count(), 0, name);
        fclose(fp);
        add_head();
        printf("%s is added to the store...\n", name);
      } else {
        add_head();
        printf("%s is not added to the store...\n", name);
      }
    }

  } else {
    add_head();
    printf("The name you entered is too short, can't add..\n");
  }
}


void printDrugs(Drug drugs[]) {
  for (int i = 0; i < count() -1; i++) {
    Drug d = drugs[i];
    printf("id: %d, name: %s, count: %d\n", d.id, d.name, d.count);
  }
}


int main() {
  bool run = true;
  while (run) {
    int response;
    Drug drugs[count()]; // declare the drug array
    // system("clear");
    
    create(drugs); // assign to drug array 
    welcome();
    printf("> ");
    scanf("%1d", &response);

    switch(response) {
      case 1:
        add(drugs);
        break;
      case 2:
        issue(drugs);
        break;
      case 3:
        store(drugs);
        break;
      default:
        printf("Program is closing...!\n");
        run = false;
        break;
    }
  }
}
