#include <stdio.h>
#include <stdlib.h>
void dailywaste();
void perhouse();
void total_avg();
union numberofdays{
    double degwaste[200];
    double plastwaste[200];
    double electwaste[200];
} nod[100];
struct processdata{
    double degwasteperhouse[100];
    double plastwasteperhouse[100];
    double electwasteperhouse[100];
    double totalwasteperhouse[100];
    double costperhouse[100];
    double avgdegwaste[10];
    double avgelectwaste[10];
    double avgplastwaste[10];
    double allareaswaste[3];
} *pd;

int main(){
    int areas, month, numofdays, presentday, b, c, d, e, f, totalnumofhouses, choice, cont = 1, numofhouses[10];
    double lowest, totalearning;
    pd = (struct processdata *)malloc(sizeof(struct processdata));
    FILE *fp;
    fp = fopen("waste.txt", "w");
    if (fp == NULL){
        printf("The file is not created");
        return 0; 
    }
    fprintf(fp, "\t\t\t\t\t\t\t\tWASTE MANAGEMENT");
    
    //Yearly loop
    do{
        month = 1;
        //Monthly loop
        while (month <= 12 && cont){
            totalnumofhouses = 0;
            printf("\nMonth %d  : ", month);
            printf("\nEnter num of areas : ");
            scanf("%d", &areas);
            fprintf(fp, "\n\n\t%s %d\n\t%s", "Month", month, "~~~~~~~");
            fprintf(fp, "\n\t%s %d", "No. of areas :", areas);
            for (int i = 0; i < areas; i++){
                printf("Enter number of houses in area %d : ", i + 1);
                scanf("%d", &numofhouses[i]);
                fprintf(fp, "\n\t%s %d %s %d", "No. of houses in area", i+1, ":", numofhouses[i]);
                totalnumofhouses += numofhouses[i];
            }
            printf("Enter number of days in this month: ");
            scanf("%d", &numofdays);

            //Printing data in file
            fprintf(fp, "\n\t%s %d", "Number of days in this month :", numofdays);
            fprintf(fp, "\n\t%s", "-------------------------------------");
            fprintf(fp, "\n\t%s\n\t\t\t\t%s", "Days of wastes :","Degradable : 1,4,7,10,...");
            fprintf(fp, "\n\t\t\t\t%s", "Plastic    : 2,5,8,11,...");
            fprintf(fp, "\n\t\t\t\t%s\n\n\t", "Electronic : 3,6,9,12,...");
            for (int i = 1; i <= totalnumofhouses; i++)
                fprintf(fp, "%s", "________________");
            fprintf(fp, "\n\t%s\n\t%s", "| \\\\House|", "|  \\\\    |");
            for (int i = 1; i <= totalnumofhouses; i++)
                fprintf(fp, "\t\t\t%d", i);
            fprintf(fp, "\n\t%s\n\t", "|Day\\\\   |");
            for (int i = 1; i <= totalnumofhouses; i++)
                fprintf(fp, "%s", "----------------");

            //Daily loop - Input data loop
            d = 0, e = 0, f = 0;
            for (int presentday = 1; presentday <= numofdays; presentday += 3){
                printf("\nDay %d", presentday);
                fprintf(fp, "\n\t%s\t%d\t%s\t", "|", presentday, " |");
                dailywaste(nod[presentday - 1].degwaste, &totalnumofhouses, "degradable",fp);
                if ((presentday != 28 || numofdays != 28) && (presentday != 31 || numofdays != 31)){
                    printf("\nDay %d", presentday + 1);
                    fprintf(fp, "\n\t%s\t%d\t%s\t", "|", presentday + 1, " |");
                    dailywaste(nod[presentday].plastwaste, &totalnumofhouses, "plastic", fp);
                }
                if ((presentday != 28 || numofdays != 28) && ((presentday + 1) != 29 || numofdays != 29) && (presentday != 31 || numofdays != 31)){
                    printf("\nDay %d", presentday + 2);
                    fprintf(fp, "\n\t%s\t%d\t%s\t", "|", presentday + 2, " |");
                    dailywaste(nod[presentday + 1].electwaste, &totalnumofhouses, "electronic", fp);
                }
            }
             fprintf(fp, "%s\n\t", "");
            for (int i = 1; i <= totalnumofhouses; i++)
                fprintf(fp, "%s", "________________");
            printf("\nYou have collected monthly data  ");
            //end of input data loop

            //Calculating wasteperhouse
            perhouse(&totalnumofhouses, nod, pd->degwasteperhouse, &numofdays, 0);
            perhouse(&totalnumofhouses, nod, pd->plastwasteperhouse, &numofdays, 1);
            perhouse(&totalnumofhouses, nod, pd->electwasteperhouse, &numofdays, 2);

            // Common loop for choice 1,4 - totalwasteperhouse,costperhouse
            for (b = 0; b < totalnumofhouses; b++){
                pd->totalwasteperhouse[b] = pd->degwasteperhouse[b] + pd->plastwasteperhouse[b] + pd->electwasteperhouse[b];
                pd->costperhouse[b] = pd->degwasteperhouse[b] * 5 + pd->plastwasteperhouse[b] * 10 + pd->electwasteperhouse[b] * 15;
            }

            // Choice 2,3,5 - average waste, waste percentage
            total_avg(&areas, numofhouses, pd->degwasteperhouse, pd->allareaswaste, pd->avgdegwaste, 0);     //degwaste in all areas
            total_avg(&areas, numofhouses, pd->plastwasteperhouse, pd->allareaswaste, pd->avgplastwaste, 1); //plastwaste in all areas
            total_avg(&areas, numofhouses, pd->electwasteperhouse, pd->allareaswaste, pd->avgelectwaste, 2); //electwaste in all areas

            // Common loop of choice 2,3,4,5 - finding house with lowest amount of waste
            f = 0;
            lowest = pd->totalwasteperhouse[0];
            for (int b = 0; b < totalnumofhouses; b++){
                if (lowest > pd->totalwasteperhouse[b]){
                    lowest = pd->totalwasteperhouse[b];
                    f = b;
                }
            }
            pd->costperhouse[f] = 0;

            fprintf(fp, "\n\n%s\n\t%s", "Index : ", "~ Statistics");
            fprintf(fp, "\n\t%s", "~ House number of house which gave less quantity of waste");
            fprintf(fp, "\n\t%s", "~ Processing fee of each house and total earning for municipality");
            fprintf(fp, "\n\t%s", "~ Percentage of each waste");

            //Choices for preferred details
            do{
                printf("\n\nEnter you choice:\n1 for statistics");
                printf("\n2 to know house number of house which gave less quantity of waste");
                printf("\n3 to know processing fee of each house and monthly earning for municipality");
                printf("\n4 to know percentage of waste from each category");
                printf("\n5 to know monthly expenses of a particular house\nChoice input: ");
                scanf("%d", &choice);

                switch (choice){
                case 1:
                    printf("\nStatistics : Average waste collection of this month");
                    fprintf(fp, "\n\n\n%s","Statistics : Average waste collection");
                    for (b = 0; b < areas; b++){
                        printf("\nArea %d", b + 1);
                        printf("\ndegradable waste :  %.2f kg", pd->avgdegwaste[b]);
                        printf("\nplastic waste    :  %.2f kg", pd->avgplastwaste[b]);
                        printf("\nelectronic waste :  %.2f kg", pd->avgelectwaste[b]);
                        printf("\nTotal waste      :  %.2f kg", (pd->avgdegwaste[b] + pd->avgplastwaste[b] + pd->avgelectwaste[b]) / 3);
                        fprintf(fp, "\n\t%s%d", "Area ", b + 1);
                        fprintf(fp, "\n\t%s  %.2f %s", "Degradable waste :", pd->avgdegwaste[b], "kg");
                        fprintf(fp, "\n\t%s  %.2f %s", "Plastic waste    :", pd->avgplastwaste[b], "kg");
                        fprintf(fp, "\n\t%s  %.2f %s", "Electronic waste :", pd->avgelectwaste[b], "kg");
                        fprintf(fp, "\n\t%s  %.2f %s", "Total waste      :", ((pd->avgdegwaste[b] + pd->avgplastwaste[b] + pd->avgelectwaste[b]) / 3), "kg");
                    }
                    break;

                case 2:
                    printf("\nThe house number %d gave %.2f kg of waste which is least of all", f + 1, lowest);
                    printf("\nSo the municipality decided to refund the processing fee of that house\n");
                    fprintf(fp, "\n\n\n%s\n\t%s %d %s %.2f %s", "House with Least Waste :", "The house number", f + 1, "gave", lowest, "kg of waste which is least of all");
                    fprintf(fp, "\n\t%s", "So the municipality decided to refund the processing fee of that house");
                    break;

                case 3:
                    totalearning = 0;
                    printf("\nProcessing fee of waste per house ");
                    fprintf(fp, "\n\n\n%s", "Processing fee of waste per house :");
                    for (b = 0, d = 0; b < areas; b++) {
                        printf("\nArea %d", b + 1);
                        fprintf(fp, "\n\t%s %d", "Area", b + 1);
                        for (c = 0; c < numofhouses[b]; c++, d++){
                            printf("\nHouse number %d : %.2f rupees", d + 1, pd->costperhouse[d]);
                            fprintf(fp, "\n\t%s %d %s %.2f %s", "House number", d + 1, ":", pd->costperhouse[d], "rupees");
                            totalearning += pd->costperhouse[d];
                        }
                    }
                    printf("\nTotal earning for municipality from all areas in this month: %.2f rupees\n", totalearning);
                    fprintf(fp, "\n\t%s  %.2f %s", "Total earning for municipality from all areas in this month:", totalearning, "rupees");
                    break;

                case 4:
                    printf("\nPercentage of wastes in all areas");
                    printf("\nDegradable waste  : %.2f %%", pd->allareaswaste[0] * 100 / (pd->allareaswaste[0] + pd->allareaswaste[1] + pd->allareaswaste[2]));
                    printf("\nPlastic waste     : %.2f %%", pd->allareaswaste[1] * 100 / (pd->allareaswaste[0] + pd->allareaswaste[1] + pd->allareaswaste[2]));
                    printf("\nElectronic waste  : %.2f %%", pd->allareaswaste[2] * 100 / (pd->allareaswaste[0] + pd->allareaswaste[1] + pd->allareaswaste[2]));
                    fprintf(fp, "\n\n\n%s", "Percentage of wastes in all areas :");
                    fprintf(fp, "\n\t%s  %.2f %s", "Degradable waste  :", pd->allareaswaste[0] * 100 / (pd->allareaswaste[0] + pd->allareaswaste[1] + pd->allareaswaste[2]), "%%");
                    fprintf(fp, "\n\t%s  %.2f %s", "Plastic  waste    :", pd->allareaswaste[1] * 100 / (pd->allareaswaste[0] + pd->allareaswaste[1] + pd->allareaswaste[2]), "%%");
                    fprintf(fp, "\n\t%s  %.2f %s", "Electronic waste  :", pd->allareaswaste[2] * 100 / (pd->allareaswaste[0] + pd->allareaswaste[1] + pd->allareaswaste[2]), "%%");
                    break;

                case 5:
                    printf("\nTo know a particular house expenses");
                    printf("\nEnter House number :");
                    scanf("%d", &b);
                    if (b <= totalnumofhouses && b > 0)
                        printf("Expenses of house number %d : %.2f rupees", b, pd->costperhouse[b - 1]);
                    else
                        printf("Invalid House number !");
                    break;
                default:
                    printf("\nInvalid choice !");
                    break;
                }
                printf("\n\nDo you want to choose another option: \nEnter 1 to continue\n      0 to stop\nOption input : ");
                scanf("%d", &cont);
            } while (cont); //end of choices

            fprintf(fp,"\n\n%s","End of the month !");
            printf("\nEnd of the month !\nDo you want to start with next month :");
            printf("\nEnter 1 to continue\n      0 to stop\nOption input : ");
            scanf("%d", &cont);
            month++;
        } //end of monthly loop

        fprintf(fp, "\n\n%s", "End of the year !");
        printf("\nEnd of the year ! \nDo you want to start with next year :");
        printf("\nEnter 1 to continue \n 0 to stop\nOption input : ");
        scanf("%d", &cont);
    } while (cont); //end of yearly loop

    printf("\nThanks for using our program  !");
    fclose(fp);
    return 0;
}
void dailywaste(double waste[], int *totalnumofhouses, char g[],FILE *fp){
    for (int c = 0; c < *totalnumofhouses; c++){
        printf("\nEnter the weight of %s waste collected in house number %d :", g, c % (*totalnumofhouses) + 1);
        scanf("%lf", &waste[c]);
        fprintf(fp,"\t\t%.2f", waste[c]);
    }
}
void perhouse(int *totalnumofhouses, union numberofdays waste[], double perhouse[], int *numofdays, int day){
    for (int b = 0, d = 0; b < *totalnumofhouses; b++){
        d = 0;
        for (int c = day; c < *numofdays; c += 3){
            d += waste[c].degwaste[b];
            perhouse[b] = d;
        }
    }
}
void total_avg(int *areas, int numofhouses[], double wasteperhouse[], double allareaswaste[], double avgwaste[], int index){
    for (int b = 0, d = 0, totwaste = 0; b < *areas; b++){
        totwaste = 0;
        for (int c = 0; c < numofhouses[b]; c++, d++)
            totwaste += wasteperhouse[d];
        allareaswaste[index] += totwaste;
        avgwaste[b] = totwaste / numofhouses[b];
    }
}
