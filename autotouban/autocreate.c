#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DATA_MAX 512

typedef struct data_template {
  int room;
  int grade;
  int num;
}Data;

void set(int set_grade, int set_num, int *grade, int *num) {
  grade[0] = set_grade / 10000;
  grade[1] = (set_grade / 1000) % 10;
  grade[2] = (set_grade / 100) % 10;
  grade[3] = (set_grade / 10) % 10;
  grade[4] = set_grade % 10;

  num[0] = set_num / 1000000;
  num[1] = (set_num / 100000) % 10;
  num[2] = (set_num / 10000) % 10;
  num[3] = (set_num / 1000) % 10;
  num[4] = (set_num / 100) % 10;
  num[5] = (set_num / 10) % 10;
  num[6] = set_num % 10;
}

int day_of_month(int month) {
  time_t t;
  int year;
  int day;

  time(&t);
  year = localtime(&t)->tm_year;

  switch(month) {
  case 1:
  case 3:
  case 5:
  case 7:
  case 8:
  case 10:
  case 12:
    day = 31;
    break;
  default:
    day = 30;
    break;
  }

  if(month == 2) {
   if(month % 400 == 0)
      day = 29;
    else if(month % 100 == 0)
      day = 28;
    else if(month % 4)
      day = 29;
    else
      day = 28;
  }

  return day;
}

int main(int argc, char **argv)
{
  int i, j, k, mon, day, wday;
  int set_grade, set_num, start, current;
  int grade[5], num[7];
  int data_count;
  time_t t;
  FILE *ifp, *ofp, *setfp;
  char ifile_name[] = "room.txt";
  char ofile_name[] = "output.txt";
  char setfile_name[] = "setting.txt";
  Data d[DATA_MAX];

// ##### ##### ##### ##### ##### ##### ##### ##### #####
  if((ifp = fopen(ifile_name, "r")) == NULL) {
    printf("\"%s\" can't open.\n", ifile_name);
    exit(1);
  }
  if((ofp = fopen(ofile_name, "w")) == NULL) {
    printf("\"%s\" can't open.\n", ofile_name);
    exit(1);
  }
  if((setfp = fopen(setfile_name, "r")) == NULL) {
    printf("\"%s\" can't open.\n", setfile_name);
    exit(1);
  }
// ##### ##### ##### ##### ##### ##### ##### ##### #####

  fscanf(setfp, "%d\t%d", &mon, &wday);
  fscanf(setfp, "%d", &set_grade);
  fscanf(setfp, "%d", &set_num);
  fscanf(setfp, "%d", &start);
  set(set_grade, set_num, grade, num);
  fclose(setfp);

  time(&t);
  day = day_of_month(mon);

  i = 0;
  while(fscanf(ifp, "%d\t%d\t%d", &d[i].room, &d[i].grade, &d[i].num) != EOF)
    i++;
  data_count = i;

  current = 0;
  while(d[current].room != start)
    current++;

  k = wday;
  i = 0;
  while(i < day) {
    j = 0;
    while(j < num[k]) {
      if(grade[d[current].grade - 1] == 1) {
        fprintf(ofp, "%d\t", d[current].room);
        j += d[current].num;
        if(k > 6)
          k = 0;
      }
      current++;
      if(current >= data_count)
        current = 0;
    }
    fprintf(ofp, "\n");
    k++;
    i++;
  }


  if((setfp = fopen(setfile_name, "w")) == NULL) {
    printf("\"%s\" can't open.\n", setfile_name);
    exit(1);
  }
  fprintf(setfp, "%d\t%d\n%d\n%d\n%d\n", mon, wday, set_grade, set_num, d[current].room);

  fclose(ifp);
  fclose(ofp);
  fclose(setfp);

  return 0;
}
