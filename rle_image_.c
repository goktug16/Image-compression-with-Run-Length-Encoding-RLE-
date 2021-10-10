#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Alaaddin Goktug AYAR
// Yildiz Technical University Computer Engineering Student

struct pgm_file{
	int width;
	int height;
	int max;
	int * Data;
};

void print_matrix(int *Data , int size);
int* apply_rle(int * data, int w , int h); // applies rle to the array
int ret_size(int * data, int w, int h);    // returns the what encoded size should be of a array
int* decode(int *encoded_data ,int w , int h, int encoded_size);  // decodes the given encoded 1d array
void change_color(int * encoded_data, int size,int old_col ,int new_col);  // change spesific color to another color
int change_bit(int *encoded_data,int*, int size, int coordinate, int new_bit); // changes bit at a given spesific coordinate
int is_valid_size(int * encoded_data, int h, int w,int encoded_size);  // is valid size control before decoding process starts 
int is_valid_colors(int *encoded_data,int encoded_size, int max);  // is colors in the array are valid compare to max value that is stated in the pgm file
int is_valid_rle(int * encoded_data, int encoded_size);           // is rle encoding valid, it checks whether there are some consecutive same numbers in a decoded array 
int is_single(int *encoded_data , int encoded_size, int target_index);  // helper function for the coordinate given bit change process
void create_hist(int *histogram, int his_size, int * encoded_data, int encoded_size);  // creates histogram
 
int main()
{
	FILE * file;  			// file pointers for the creating and reading files 
	FILE * en_file;
	FILE * pgm_write;
	FILE * new_file;
	struct pgm_file* pgm;  	// structure to read pgm file info into 
	int i = 0; 				// loop variables 
	int j = 0;
	int z = 0;
	int encoded_size = 0;  	// to calculate size of an encoded array
	int * encoded_data;  	// hold encoded data
	int * decoded_data; 	// holds decode
	int dec_flag = 0;  		// control variables 
	char read[255];		
	char file_name[40];
	int values [255];
	int old_col,new_col,add,user;  // variables to send parameter to functions
	int x,y,val_size,val_col,val_rle;  // to hold results of the valid functions 
	int * new_array;
	pgm = (struct pgm_file*) malloc(sizeof(struct pgm_file));  // allocating memory for the structure
	int *histogram;
	int his_size;
	printf("File format for the .pgm file should be\n P2 \n #comment \n column row numbers \n max value \n 2d array\n\n");
	
	printf("Encoding a pgm file -> 1\nDecoding a txt file -> 2\nExit Program -> 3\n\nEnter a number: ");
	scanf("%d",&user);
	
	while(user != 3)
	{
		dec_flag = 1;
		if(user == 1)
		{
			printf("\nEnter name of the file to be encoded: ");
			scanf("%s",file_name);  // getting file name
			file = fopen(file_name,"r"); 
			fscanf(file,"%s",read);
			// reading and encoding file 
			if(strcmp(read, "P2") == 0)
			{	
				fscanf (file, "%s", read);
				while(read[0] == '#')
				{
					fgets(read, 255, file); // getting comment 
					fscanf(file,"%s",read); // getting width
				}
				
				sscanf(read, "%d", &pgm->width);
				fscanf(file,"%d",&pgm->height);
				fscanf(file,"%d",&pgm->max);
				
				pgm->Data = (int *) malloc(sizeof(int) * pgm->width * pgm->height); // allocationg memory for data
				
				for(i=0; i < pgm->height * pgm->width; i++) // reading data into array from .pgm file
					fscanf(file,"%d", &pgm->Data[i]);
				
				encoded_data = apply_rle(pgm->Data,pgm->width,pgm->height);  // encode the file.
				
				encoded_size = ret_size(pgm->Data,pgm->width,pgm->height) + 2; 
				en_file = fopen("test_encoded.txt","w");
				fprintf(en_file,"Row and Column Numbers : %d %d \n",pgm->width,pgm->height);
				fprintf(en_file,"Encoded File : \n");
				printf("Encoded file : ");
				for(i=2 ; i< encoded_size; i+=2)
				{
					printf("[%d  %d] ",encoded_data[i],encoded_data[i+1]);
					fprintf(en_file,"%d %d ",encoded_data[i],encoded_data[i+1]);
				}
				fclose(en_file);
			}
		}
		else if(user == 2)
		{
			printf("Enter name of the file to be decoded(test_encoded.txt)\n");
			scanf("%s",file_name);
			en_file = fopen(file_name,"r"); // uncomment for decoding section
			i = 1;
			fgets(read,26,en_file); // reading file sentence
			fscanf(en_file, "%d %d", &pgm->width,&pgm->height); // reading width and height		
			pgm->max = 255;   // max pixel value				
			fscanf(en_file,"%s",read);		
			fgets(read,255,en_file);		
			fscanf(en_file,"%s",read);
			sscanf(read,"%d",&values[0]);
			
			while(fscanf(en_file,"%d",&values[i]) != EOF) // reading encoded data into array 
				i++;
				
			encoded_size = i;				
			pgm->Data = (int *) malloc(sizeof(int) * encoded_size);
			
			for(i=0; i<encoded_size; i++)
				pgm->Data[i] = values[i];
			fclose(en_file);
			
			while(dec_flag != -1)
			{
				printf("Before decoding file do you want to change anything ?\n");
				printf("\t Change a given color : 1\n");
				printf("\t Change a pixel value with exact coordinates : 2\n");
				printf("\t Calculate histogram : 3\n");
				printf("\t Decode file : 4\n");
				printf("Enter a number(-1 terminates decoding options) : ");
				scanf("%d",&dec_flag);
				
				// decoding a valid txt file
				if(dec_flag == 1)
				{
					printf("Enter color you want to change : ");
					scanf("%d",&old_col);
					printf("Enter new color: ");
					scanf("%d",&new_col);
				
					change_color(pgm->Data,encoded_size,old_col,new_col);  
			
					new_file = fopen("test_encoded.txt","w");
					fprintf(new_file,"Row and Column Numbers : %d %d \n",pgm->height,pgm->width);  // write necessary infos into file 
					fprintf(new_file,"Encoded File : \n");
						
					printf("New encoded version after color change : ");
						
					for(i=0 ; i< encoded_size; i+=2) // write new array with the changed bit into file 
					{
						printf("[%d  %d] ",pgm->Data[i],pgm->Data[i+1]);
						fprintf(new_file,"%d %d ",pgm->Data[i],pgm->Data[i+1]);
					}
					printf("\n");
					fclose(new_file);
				}
				else if(dec_flag == 2)
				{
					printf("Enter row and col coordinates to change(x y): ");
					scanf("%d %d",&x,&y);
					printf("Enter new pixel value : ");
					scanf("%d",&old_col);   
		
					new_col = ( x * pgm->width ) + y + 1;
					add = is_single(pgm->Data,encoded_size,new_col);               // it makes changes according to given coordinates
						
					new_array = (int*) malloc(sizeof(int) * (encoded_size+add));
					encoded_size = change_bit(pgm->Data,new_array,encoded_size,new_col,old_col);
							
					pgm->Data = new_array;
					encoded_size = encoded_size + add; 
					new_file = fopen("test_encoded.txt","w");
					fprintf(new_file,"Row and Column Numbers : %d %d \n",pgm->height,pgm->width);
					fprintf(new_file,"Encoded File : \n");
					
					printf("\n\nNew encoded version after color change : ");
						
					for(i=0 ; i< encoded_size; i+=2)
					{
						printf("[%d  %d] ",pgm->Data[i],pgm->Data[i+1]);
						fprintf(new_file,"%d %d ",pgm->Data[i],pgm->Data[i+1]);
					}
					printf("\n");
					fclose(new_file);
				}
				else if(dec_flag == 3)
				{
					his_size = find_unique(pgm->Data,encoded_size);
					histogram = (int *) malloc(sizeof(int) * his_size * 2);
					create_hist(histogram,his_size*2,pgm->Data,encoded_size); // creating histogram
					
					en_file = fopen("test_encoded.txt","a");
					printf("\n\nHistogram for the encoded pgm file: ");
					for(i=0 ; i< his_size*2; i+=2)
					{
						printf("[%d  %d] ",histogram[i],histogram[i+1]);
					}
					fclose(en_file);
					printf("\n\n");
					// call calculate histogram func
				}
				else if (dec_flag == 4)
				{
					printf("\nTests are starting before decoding process.\n\n");
					printf("Checking if the row * column numbers are match with the decoded file....\n\n");
					val_size = is_valid_size(pgm->Data, pgm->height, pgm->width, encoded_size);
					if(val_size == 1)
						printf("\trow * column values match, it is a valid decoded file.\n\n");
					else
						printf("\trow * column values does not match, it is an invalid decoded file.\n\n");
						
					printf("Checking if some value in the decoded file above the max....\n\n");
					val_col = is_valid_colors(pgm->Data,encoded_size,pgm->max);
					if(val_col == 1)
						printf("\tAll values in the decoded file are valid.\n\n");
					else
						printf("\tSome values in the decoded file are above the max value. It is an invalid file.\n\n");
						
					printf("Checking if there are any consecutive run lengths....\n\n");
					val_rle = is_valid_rle(pgm->Data,encoded_size);
					if(val_rle == 1)
						printf("\tAll values in the decoded file are valid.\n\n");
					else
						printf("\tThere are some consecutive run length in decoded file. It is an invalid file.\n\n");
						
					if((val_size && val_col && val_rle) == 1)  // it checks did decoded file pass all valid tests
					{
						z = 0;
						pgm_write = fopen("test_decoded.pgm","w");	
						decoded_data = decode(pgm->Data,pgm->width, pgm->height, encoded_size);
						// creating pgm format file for decoded data
						fprintf(pgm_write,"P2\n");
						fprintf(pgm_write,"# This is an decoded version of an encoded file. Data structures and algorithms term project.\n");
						fprintf(pgm_write,"%d %d\n255\n",pgm->width,pgm->height);

						for(i=0 ; i<pgm->height; i++)
						{
							for(j=0; j<pgm->width; j++)
							{
								fprintf(pgm_write,"%d ",decoded_data[z]);
								z++;
							}
							fprintf(pgm_write,"\n");
						}
						fclose(pgm_write);
						printf("Encoded file sucessfully decoded into test_decoded.pgm file.\n\n");
					}
					else
						printf("\nFile can not be decoded since it is not a valid encoded file.\n\n");
			
						// end of creating a pgm format for decoded data
				}
			}
		}
		printf("\nEncoding a pgm file -> 1\nDecoding a txt file -> 2\nExit Program -> 3\n\nEnter a number: ");
		scanf("%d",&user);
	}
return 0;
}

int find_unique(int *encoded_data, int encoded_size)
{
	int i=0;
	int j;
	int size = encoded_size / 2;
	int flag = 0;
	
	for(i=1; i < encoded_size; i+=2)
	{
		for(j=i+2; j<encoded_size; j+=2)
		{
			if((encoded_data[i] == encoded_data[j]) && flag == 0)  
			{
				size--;
				flag = 1;
			}	
		}
		flag = 0;
	}
	return size;
}

void create_hist(int *histogram, int his_size, int * encoded_data, int encoded_size)
{
	int i,j,z;
	int flag = 0;
	int *copy = (int *) malloc(sizeof(int) * encoded_size);
	
	for(i=0; i<his_size; i++)
		histogram[i] = 0;
	for(i=0; i<encoded_size; i++)
		copy[i] = encoded_data[i];
	
	z = 1;
	
	for(i=1; i < encoded_size; i+=2)
	{
		for(j=i+2; j<encoded_size; j+=2)
		{
			if((copy[i] == copy[j]))  
			{
				copy[j] = -20;
			}	
		}
	}
	i = 1;

	for(z=0 ; z<his_size; z+=2)
	{
		
		if(copy[i] != -20)
		{
			histogram[z] = copy[i];
		}
		else
			z-=2;
		i+=2;
	}
	z= 0;
	while(z<his_size)
	{
		for(i=1; i<encoded_size; i+=2)
		{
			if(histogram[z] == encoded_data[i])
			{
				histogram[z+1] += encoded_data[i-1];			
			}		
		}
		z+=2;
	}
}

int is_single(int *encoded_data , int encoded_size, int target_index)
{
	int i=0;
	int index = 0;
	int real;
	printf("burdayim\n");
	while(index < target_index)
	{
		index += encoded_data[i];
		if(index >= target_index)
			real = i ;
		i+=2;	
	}
	if(encoded_data[real] == 1)
		return 0;
	else
		return 2;
}

int is_valid_size(int * encoded_data, int h, int w,int encoded_size)
{
	int size = h * w ; 
	int i = 0;
	int add = 0;
	
	for(i=0; i<encoded_size; i+=2)
		add += encoded_data[i];
		
	if(add == size)
		return 1;
	else
		return 0;
}

int is_valid_colors(int *encoded_data,int encoded_size, int max)
{
	int i;
	for(i=1 ; i < encoded_size; i += 2)
		if(encoded_data[i] > max)
			return 0;
	
	return 1;
}

int is_valid_rle(int * encoded_data, int encoded_size)
{
	int i=0;
	int before = encoded_data[1];
	
	for(i=3; i<encoded_size; i+=2)
	{
		if(encoded_data[i] == before)
			return 0;
		else
			before = encoded_data[i];
	}
	return 1;
}

void change_color(int * encoded_data, int size,int old_col ,int new_col)
{
	int i;
	for(i=1; i < size; i+=2)
	{
		if(encoded_data[i] == old_col)
			encoded_data[i] = new_col;
	}
}

int change_bit(int *encoded_data,int* new_array, int size, int coordinate, int new_bit)
{
	int i = 0;
	int index = 0;
	int real;
	int j=0;
	int size_flag = 0;
	while(index < coordinate)
	{
		index += encoded_data[i];
		if(index >= coordinate)
			real = i ;
		i+=2;	
	}
	i = 0;
	while(i < size+2)
	{
		if(i != real)
		{
			if(encoded_data[j] == 0 && j%2 == 0)
			{
				j +=2 ;
				size_flag++;
			}
			new_array[i] = encoded_data[j];
			i++;
			j++;
		}
		else
		{
			encoded_data[i] = encoded_data[i] - 1;
			new_array[i] = 1;
			i++;
			new_array[i] = new_bit;
			i++;
		}
	}
	return size;
}

int* decode(int *encoded_data ,int w , int h, int encoded_size)
{
	int *data = (int*) malloc(sizeof(int) * w * h);
	int i = 0;
	int j = 0;
	int index = 0;
	int rep;
	
	while(i < encoded_size)
	{
		rep = encoded_data[i];
		i++;
		for(j=0; j < rep; j++)
		{
			data[index] = encoded_data[i];
			index++;
		}
		i++;
	}
	return data;
}

int ret_size(int * data, int w, int h)
{
	int i=0;
	int hold=0;
	int count = 0;
	int size = 0;
	
	hold = data[0];
	
	for(i=1; i < h*w; i++)
	{	
		if(hold == data[i])
			count++;
		else
		{
			size += 2;
			hold = data[i];
		}
		if( i == ( h * w ) - 1 )
			size +=2;	
	}
	return size;
}

int * apply_rle(int * data, int w , int h)
{
	int size = ret_size(data,w,h);
	int * encoded_data = (int *) malloc((sizeof(int) * size) + 2);
	int i = 0;
	int j = 2;
	int count = 1 ;
	
	encoded_data[0] = h;
	encoded_data[1] = w;
	
	int hold = data[0];
	
	for(i=1; i < h * w; i++)
	{
		if(hold == data[i])
			count++;
		else
		{
			encoded_data[j] = count;
			encoded_data[j+1] = hold;
			hold = data[i];
			j += 2;
			count = 1;
		}
		if( i == ( h * w ) - 1 )
		{
			encoded_data[j] = count;
			encoded_data[j+1] = hold;	
		}	
	}
	
	return encoded_data;
}

void print_matrix(int *Data , int size)
{
	int i = 0;
	int j = 0;
	printf("\n");
		for(i=0; i<size; i++)
		{
			printf("%d  \n",Data[i]);
		}
}
