#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int PrintFortune(int index, char *filename){
//    printf("come in PrintFortune!\n");
    FILE *fp;
    fp = fopen(filename,"r");
    if (fp == NULL) {
        printf("ERROR: Can't open fortune file\n");
        exit(1);
    }
    char temp[20];
    char temp2[20];
//    fgets(temp,20,fp);
    if(fgets(temp,20,fp) == NULL){
        printf("ERROR: Fortune File Empty\n");
        exit(1);
    }
    fgets(temp2,20,fp);
    int totalnum, maxlength;
    totalnum = atoi(temp);
    maxlength = atoi(temp2);
    if(index > totalnum || index <= 0){
        printf("ERROR: Invalid Fortune Number\n");
        return(1);
    }

    int cnt = 0;
    char *buffer = (char *) malloc(sizeof(char) * (maxlength+1));
    while(fgets(buffer, maxlength+1, fp) && (cnt <= index)){
        if(buffer[0] == '\%'){
            cnt += 1;
            continue;
        }
        if(cnt == index){
            printf("%s",buffer);
        }
    }
//    printf("go out PrintFortune!\n");
    free(buffer);
    fclose(fp);
    return(0);
}

int FileFortune(int index, char *filename, char *outputFile){
//    printf("come in FileFortune!\n");
    FILE *fp, *fo;
    fp = fopen(filename,"r");
    if (fp == NULL) {
        printf("ERROR: Can't open fortune file\n");
        exit(1);
    }
    fo = fopen(outputFile,"a");
    char temp[20];
    char temp2[20];
    if(fgets(temp,20,fp) == NULL){
        printf("ERROR: Fortune File Empty\n");
        exit(1);
    }
//    fgets(temp,20,fp);
    fgets(temp2,20,fp);
    int totalnum, maxlength;
    totalnum = atoi(temp);
    maxlength = atoi(temp2);
    if(index > totalnum || index <= 0){
        printf("ERROR: Invalid Fortune Number\n\n");
        return(1);
    }

    int cnt = 0;
    char *buffer = (char *) malloc(sizeof(char) * (maxlength+1));
    while(fgets(buffer, maxlength+1, fp) && (cnt <= index)){
        if(buffer[0] == '\%'){
            cnt += 1;
            continue;
        }
        if(cnt == index){
            fputs(buffer, fo);
        }
    }
    fclose(fo);
    fclose(fp);
    free(buffer);
//    printf("go out FileFortune!\n");
    return(0);
}




int main(int argc, char *argv[]) {
//Input: 5 or 7 args that may be different at the 4th and the 5th.
//If the 4th is -n, then the 5th should be exactly 1 number indicating the index of the fortune
//If the 4th is -b, then the 5th should be a file that we need to read
//The 6th and the 7th are optional, if exists, in another word, the 6th is -o,
//then the 7th should be the output file name

//Output: STDOUT or In a file, depending on the argc

//TODO: First check the argc, and decide whether we need a output file.
//      Then look at the 4th arg to decide which mode.
//      After that read in the file name(3rd arg).
//      Finally, start for loop to output.
    if(argc < 5){
        printf("USAGE: \n\tbadger-fortune -f <file> -n <number> (optionally: -o <output file>) \n\t\t OR \n\tbadger-fortune -f <file> -b <batch file> (optionally: -o <output file>)\n");
        exit(1);
    }
    else{
        //TODO: Check Error
        if((argc == 5) || (argc ==7)) {
            if(argc == 5){
                if((!strcmp(argv[1],"-f"))){
                    int strLength = (int)strlen(argv[2]);
                    if((strcmp(argv[2]+strLength-4,".txt")) || strLength<=4){
                        printf("ERROR: No fortune file was provided\n");
                        exit(1);
                    }
                }
                if((!strcmp(argv[2],"-f"))){
                    int strLength = (int)strlen(argv[3]);
                    if((strcmp(argv[3]+strLength-4,".txt")) || strLength<=4){
                        printf("ERROR: No fortune file was provided\n");
                        exit(1);
                    }
                }
                if((!strcmp(argv[3],"-f"))){
                    int strLength = (int)strlen(argv[4]);
                    if((strcmp(argv[4]+strLength-4,".txt")) || strLength<=4){
                        printf("ERROR: No fortune file was provided\n");
                        exit(1);
                    }
                }
                if((!strcmp(argv[4],"-f"))){
                    printf("ERROR: No fortune file was provided\n");
                    exit(1);
                }
                if((strcmp(argv[1],"-f")) && (strcmp(argv[1],"-o")) && (strcmp(argv[1],"-b")) && (strcmp(argv[1],"-n"))){
                    printf("ERROR: Invalid Flag Types\n");
                    exit(1);
                }
                if((strcmp(argv[3],"-f")) && (strcmp(argv[3],"-o")) && (strcmp(argv[3],"-b")) && (strcmp(argv[3],"-n"))){
                    printf("ERROR: Invalid Flag Types\n");
                    exit(1);
                }
                if((strcmp(argv[1],"-f")) && (strcmp(argv[2],"-f")) && (strcmp(argv[3],"-f")) && (strcmp(argv[4],"-f"))){
                    printf("ERROR: No fortune file was provided\n");
                    exit(1);
                }
                //TODO: -n -b
                if((!strcmp(argv[1],"-n")) && (!strcmp(argv[2],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-n")) && (!strcmp(argv[3],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-n")) && (!strcmp(argv[4],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-n")) && (!strcmp(argv[3],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-n")) && (!strcmp(argv[4],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[3],"-n")) && (!strcmp(argv[4],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                //TODO: -b -n
                if((!strcmp(argv[1],"-b")) && (!strcmp(argv[2],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-b")) && (!strcmp(argv[3],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-b")) && (!strcmp(argv[4],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-b")) && (!strcmp(argv[3],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-b")) && (!strcmp(argv[4],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[3],"-b")) && (!strcmp(argv[4],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                //TODO: Normal Cases
                if(!strcmp(argv[3],"-n")){
                    //TODO: Number Mode
                    int index;
                    index = atoi(argv[4]);
                    int returnCode = PrintFortune(index, argv[2]);
                    if(returnCode){
                        exit(1);
                    }
                }
                else if(!strcmp(argv[3],"-b")){
                    //TODO: Batch mode
                    FILE *batchfp;
                    batchfp = fopen(argv[4], "r");
                    if (batchfp == NULL) {
                        printf("ERROR: Can't open batch file\n");
                        exit(1);
                    }
                    //Start to get index one by one
                    char *currentIndex = (char *) malloc((sizeof(char)) * 20);
                    if(fgets(currentIndex, 20, batchfp) == NULL){
                        printf("ERROR: Batch File Empty\n");
                        exit(1);
                    }
                    do{
                        int Index = atoi(currentIndex);
//                        printf("Current Index is %d\n",Index);
                        int returnCode = PrintFortune(Index, argv[2]);
                        if (returnCode){
                            printf("\n");
                            continue;
                        }
                        printf("\n\n");
                    }while(fgets(currentIndex, 20, batchfp));
                    fclose(batchfp);
                    free(currentIndex);
                    return(0);
                }
            }
            else if(argc == 7){
                if((!strcmp(argv[1],"-f"))){
                    int strLength = (int)strlen(argv[2]);
                    if((strcmp(argv[2]+strLength-4,".txt")) || strLength<=4){
                        printf("ERROR: No fortune file was provided\n");
                        exit(1);
                    }
                }
                if((!strcmp(argv[2],"-f"))){
                    int strLength = (int)strlen(argv[3]);
                    if((strcmp(argv[3]+strLength-4,".txt")) || strLength<=4){
                        printf("ERROR: No fortune file was provided\n");
                        exit(1);
                    }
                }
                if((!strcmp(argv[3],"-f"))){
                    int strLength = (int)strlen(argv[4]);
                    if((strcmp(argv[4]+strLength-4,".txt")) || strLength<=4){
                        printf("ERROR: No fortune file was provided\n");
                        exit(1);
                    }
                }
                if((!strcmp(argv[4],"-f"))){
                    int strLength = (int)strlen(argv[5]);
                    if((strcmp(argv[5]+strLength-4,".txt")) || strLength<=4){
                        printf("ERROR: No fortune file was provided\n");
                        exit(1);
                    }
                }
                if((!strcmp(argv[5],"-f"))){
                    int strLength = (int)strlen(argv[6]);
                    if((strcmp(argv[6]+strLength-4,".txt")) || strLength<=4){
                        printf("ERROR: No fortune file was provided\n");
                        exit(1);
                    }
                }
                if((!strcmp(argv[6],"-f"))){
                    printf("ERROR: No fortune file was provided\n");
                    exit(1);
                }
                if((strcmp(argv[1],"-f")) && (strcmp(argv[1],"-o")) && (strcmp(argv[1],"-b")) && (strcmp(argv[1],"-n"))){
                    printf("ERROR: Invalid Flag Types\n");
                    exit(1);
                }
                if((strcmp(argv[3],"-f")) && (strcmp(argv[3],"-o")) && (strcmp(argv[3],"-b")) && (strcmp(argv[3],"-n"))){
                    printf("ERROR: Invalid Flag Types\n");
                    exit(1);
                }
                if((strcmp(argv[5],"-f")) && (strcmp(argv[5],"-o")) && (strcmp(argv[5],"-b")) && (strcmp(argv[5],"-n"))){
                    printf("ERROR: Invalid Flag Types\n");
                    exit(1);
                }
                if((strcmp(argv[1],"-f")) && (strcmp(argv[2],"-f")) && (strcmp(argv[3],"-f")) && (strcmp(argv[4],"-f")) && (strcmp(argv[5],"-f")) && (strcmp(argv[6],"-f"))){
                    printf("ERROR: No fortune file was provided\n");
                    exit(1);
                }
                //TODO: -n -b
                if((!strcmp(argv[1],"-n")) && (!strcmp(argv[2],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-n")) && (!strcmp(argv[3],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-n")) && (!strcmp(argv[4],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-n")) && (!strcmp(argv[5],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-n")) && (!strcmp(argv[6],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-n")) && (!strcmp(argv[3],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-n")) && (!strcmp(argv[4],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-n")) && (!strcmp(argv[5],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-n")) && (!strcmp(argv[6],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[3],"-n")) && (!strcmp(argv[4],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[3],"-n")) && (!strcmp(argv[5],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[3],"-n")) && (!strcmp(argv[6],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[4],"-n")) && (!strcmp(argv[5],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[4],"-n")) && (!strcmp(argv[6],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                if((!strcmp(argv[5],"-n")) && (!strcmp(argv[6],"-b"))){
                    printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
                    exit(1);
                }
                //TODO: -b -n
                if((!strcmp(argv[1],"-b")) && (!strcmp(argv[2],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-b")) && (!strcmp(argv[3],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-b")) && (!strcmp(argv[4],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-b")) && (!strcmp(argv[5],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[1],"-b")) && (!strcmp(argv[6],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-b")) && (!strcmp(argv[3],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-b")) && (!strcmp(argv[4],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-b")) && (!strcmp(argv[5],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[2],"-b")) && (!strcmp(argv[6],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[3],"-b")) && (!strcmp(argv[4],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[3],"-b")) && (!strcmp(argv[5],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[3],"-b")) && (!strcmp(argv[6],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[4],"-b")) && (!strcmp(argv[5],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[4],"-b")) && (!strcmp(argv[6],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                if((!strcmp(argv[5],"-b")) && (!strcmp(argv[6],"-n"))){
                    printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
                    exit(1);
                }
                //TODO: Write into File
                if(!strcmp(argv[3],"-n")){
                    //TODO: Number Mode
                    int index;
                    index = atoi(argv[4]);
                    int returnCode = FileFortune(index, argv[2], argv[6]);
                    if(returnCode){
                        exit(1);
                    }
                }
                else if(!strcmp(argv[3],"-b")){
                    //TODO: Batch mode
                    FILE *batchfp;
                    batchfp = fopen(argv[4], "r");
                    if (batchfp == NULL) {
                        printf("ERROR: Can't open batch file\n");
                        exit(1);
                    }
                    //Start to get index one by one
                    char *currentIndex = (char *) malloc((sizeof(char)) * 20);
                    while(fgets(currentIndex, 20, batchfp)){
                        int Index = atoi(currentIndex);
//                        printf("Current Index is %d\n",Index);
                        int returnCode = FileFortune(Index, argv[2], argv[6]);
                        if (returnCode){
                            continue;
                        }
                        FILE *fo;
                        fo = fopen(argv[6],"a");
                        fputs("\n\n",fo);
                        fclose(fo);
                    }
                    fclose(batchfp);
                    free(currentIndex);
                    return(0);
                }
            }
        }
    }
}
