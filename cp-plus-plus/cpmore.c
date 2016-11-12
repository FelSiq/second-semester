#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define ARG_NUM 3
#define BITS_IN_BYTE 8
#define BYTES_IN_KBYTES 1024

/*
 * Created by Felipe Alves Siqueira - 9847706
 */

enum{
	ARG_PROGNAME,
	ARG_FILENAME,
	ARG_DESTINY
};

typedef struct{
    unsigned long int size_actual, size_remaining, time_total;
} info_struct;

void *func_time(void *pointer){
	info_struct *data = (info_struct *) pointer;
	unsigned long int size_current = data->size_remaining;
	clock_t clk = clock();
	double sec;

	while(data->size_remaining > 0){
		if (sec >= 1){//Messages every 1 sec
			if (size_current - data->size_remaining != 0)//check if data transfer stalled
				printf("time remaining: %lu seconds (%.1lf%% completed @ %luKbps)\n", 
					((size_current - 1)/(size_current - data->size_remaining)),
					100 - (data->size_remaining*100)/(data->size_actual * 1.0), 
					(size_current - data->size_remaining)/(BYTES_IN_KBYTES));
			else
				printf("data transfer stalled...\n");
			size_current = data->size_remaining;
			sec--;
			data->time_total++;
			clk = clock();
		}
		sec = (clock() - clk)/(CLOCKS_PER_SEC * 1.0);
	}
	return NULL;
}

int main(int argc, char const *argv[]){
	if (argc == ARG_NUM){
		//File to cppy
		FILE *file_cp = fopen(argv[ARG_FILENAME], "r");
		if(file_cp != NULL){
			if (access(argv[ARG_DESTINY], F_OK) == -1){//check if destiny file already exists
				FILE *file_destiny = fopen(argv[ARG_DESTINY], "w");
				if (file_destiny != NULL){
					
					// basic information gathering
					info_struct *data = (info_struct *) malloc(sizeof(info_struct));
					fseek(file_cp, 0, SEEK_END);
					data->size_actual = ftell(file_cp);
					data->size_remaining = data->size_actual;
					fseek(file_cp, 0, SEEK_SET); 
					printf("data size: %lu bytes (%llu bits) \n", data->size_actual, 
						(unsigned long long int) data->size_actual * BITS_IN_BYTE);

					//threads
					pthread_t t_time;
					pthread_create(&t_time, NULL, func_time, (void *) data);

					//file copy
					unsigned char c;
					for(unsigned long int i = 0; i < data->size_actual; i++){
						c = fgetc(file_cp);
						fprintf(file_destiny, "%c", c);
						//fread(&c, 1, 1, file_cp);
						//fwrite(&c, 1, 1, file_destiny);
						data->size_remaining--;
					}
					//Ending message
					printf("time total elapsed: %ld seconds\nfinished.\n", data->time_total);
					//preparing to exit and shutting down
					pthread_exit(NULL);
					free(data);
					fclose(file_destiny);
					file_destiny = NULL;
				} else 
					printf("e: unable to create new file on destiny.\n");
			} else 
				printf("e: file '%s' already exists, give me another location or name, or remove it first.\n", argv[ARG_DESTINY]);
			fclose(file_cp);
			file_cp = NULL;
		} else
			printf("e: unable to open file '%s'.\n", argv[ARG_FILENAME]);
	} else
		printf("usage: %s <filename> <cp destiny>\n", argv[ARG_PROGNAME]);
	return 1;
}
