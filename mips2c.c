/*

	mips2c

	todo:
		- todo eventually: instructions gets interpreted into
		  machine code and loaded and run from virtual memory
		- rearrange structure of source code, headers, and example programs
		- LOTS of refactoring to do, especially those gross giant function switch blocks
		  to get information about them! store it in some sort of dictionary or hashmap or
		  something!
		- lines that are formatted a bit differently
			- ie handle LABEL: INSTRUCTIONS
		- need a way to handle memory reads/writes, ie 40($s0)
			- memory addresses should just be pointers into a memory array?
		- what to do with pseudoinstructions? process first into atomic expr? or just use direct?
		- handle .data and .text sections
		- floating point coprocessor (CP1)
		- error reporting! (Macro in mips2.h? Or just print to stderr?)
		- clean up makefile - depend on headers
		- figure out whats up with incompatible pointer types with linked list for labels
		- probably memory leaks EVERYWHERE! need to make sure we clean them up!

	references used:
		https://www.student.cs.uwaterloo.ca/~cs241/mips/mipsref.pdf
		https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_help.html
		https://github.com/MIPT-ILab/mipt-mips/wiki/MIPS-Instruction-Set
*/

#include "mips2c.h"

// functions only used here
void parse_arguments(int argc, char* argv[], char** filename);

int main(int argc, char *argv[])
{
	printf(ANSI_COLOR_BRIGHT_BLUE "MIPS2C\n" ANSI_COLOR_RESET);
	printf("--------------------------\n");
	if (argc == 1)
	{
		display_usage();
	}
	// parse command line inputs	

	char* filename;
	program program;

	verbose = false;					// do we need verbose/debug?
	debug = false;						// print everything!
	display_instructions = false;		// show individual parsed instructions
	display_registers = false;			// display registers at end
	display_warnings = false;

	parse_arguments(argc, argv, &filename);

	program = get_program(filename);
	
	memory = malloc(MEMORY_SIZE);
	clear_memory();

	pc = 0;
	bool finished = false;
	char* statement;
	parsed_instruction* p;
	char* mem_ptr;

	if (debug) printf("Parsing file: %s\n", program.filename);

	// // -----------------------
	// // TODO:
	// printf("writing some mem!\n");

	// mem_ptr = memory;
	// int memtest = 23;
	// char chtest[4] = {'h','i','\n','\0'};
	// printf("writing int %i\n", memtest);
	// write_memory(&memtest, mem_ptr, sizeof(int));
	// mem_ptr += 4;
	// printf("writing string %s\n", chtest);
	// write_memory(chtest, mem_ptr, sizeof(chtest));
	// printf("now getting vars\n");
	// int retrieve_int;
	// print_memory();
	// char* retrieve_char = malloc(4);
	// get_memory(retrieve_char, memory + 4, 4);
	// get_memory(&retrieve_int, memory, 4);
	// printf("got int %d\n", retrieve_int);
	// printf("got str %s\n", retrieve_char); 
	// exit(1);
	// // TODO:
	// // -------------------------

	do {
		while (pc < program.lines)
		{
			if (verbose) printf(ANSI_COLOR_YELLOW "%d: " ANSI_COLOR_RESET, pc);
			statement = strdup(program.source[pc]);

			pc++;	// do we do this here or does it have to be somewhere else?
			p = parse_instruction(statement);
			if (p != NULL)
			{
				if (display_instructions) print_instruction(p);
				execute_instruction(p);
			}
		}

		finished = true;

	} while (!finished);

	if (display_registers) print_registers(registers);

	return 0;
}

void parse_arguments(int argc, char* argv[], char** filename)
{
	int opt;
	while ((opt = getopt(argc, argv, "l:vdhriw")) != -1)
	{
		switch(opt)
		{
			case 'v':
				verbose = true;
				break;
			case 'w':
				display_warnings = true;
				break;
			case 'r':
				display_registers = true;
				break;
			case 'h':
				// todo: display help here
				// move this to a helper, build a display
				display_usage();
				break;
			case 'i':
				display_instructions = true;
				break;
			case 'd':
				debug = true;
				break;
			case 'l':
				// get program name and load it
				*filename = optarg;
				break;
			default:
				// print command line args here
				break;
		}
	}
}

program get_program(char* filename) 
{
	get_program_state p;
	p = TEXT_STATE;
	// TODO: This will become a state machine that switches between
	// .data and .text mode, since : means different things in different states
	// ie, when we are data mode, we want to store labels as pointers or something
	// like that so that we can use them directly

	// TODO: MAYBE move all prints to stderr to stdout (??)
	// taken & edited from stackoverflow!
	program program;
	program.filename = (char*) malloc(strlen(filename) + 1);
	char* line_parse;

	// what is not working right about this linked list?
	label_list* head = NULL;
	label_list* tail = NULL;

	strcpy(program.filename, filename);
	int lines_allocated = 128;
	int max_line_len = 100;

	// Allocate lines of text
	program.source = (char **)malloc(sizeof(char*)*lines_allocated);
	if (program.source==NULL)
	    {
		    fprintf(stderr,"Out of memory (1).\n");
		    exit(1);
	    }

	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	    {
		    fprintf(stderr,"Error opening file.\n");
		    exit(2);
	    }

	int i = 0;	// line index
	while(1)
    {
	    int j;	// character index

	    // alloc more space if needed
	    if (i >= lines_allocated)
	        {
	        int new_size;

	        // double alloc'd space
	        new_size = lines_allocated*2;
	        program.source = (char **)realloc(program.source,sizeof(char*)*new_size);
	        if (program.source==NULL)
	            {
	            fprintf(stderr,"Out of memory.\n");
	            exit(3);
	            }
	        lines_allocated = new_size;
	        }
	        
	    // alloc line for space
	    program.source[i] = malloc(max_line_len);
	    line_parse = malloc(max_line_len);

	    if (program.source[i]==NULL)
	        {
	        fprintf(stderr,"Out of memory (3).\n");
	        exit(4);
	        }

	    if (fgets(line_parse,max_line_len-1,fp)==NULL)
	        break;

	    if (line_parse[0] == '#')	// this entire line is a comment, ignore it
	    {
	    	continue;
	    }

	    // todo: RIGHT NOW THIS ONLY HANDLES LABELS THAT ARE ON A SEPARATE LINE!!
	    // Lines can have format LABEL: INSTRUCTIONS #COMMENTS
	    // ALSO todo, we want to strip white space from start of line!
	    if (strchr(line_parse, ':') != NULL)	// is : ONLY in a line when it is a label??
	    {

	    	if (isalpha(line_parse[0]) == 0)
	    	{
	    		printf(ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET": Label %s must start with alphabet (not number/symbol)\n", line_parse);
	    		exit(1);
	    	}

	    	char* label_get = malloc(MAX_LABEL_LENGTH);
	    	int pi;
	    	for (pi = 0; ; pi++)
	    	{
	    		if (line_parse[pi] == ':')
	    		{
	    			break;
	    		}
	    		label_get[pi] = line_parse[pi];
	    	}
	    	label_get[pi] = 0;
	    	// ok we got a label

	    	// todo: IF THERE IS MORE OF A LINE HERE, WE NEED TO PARSE IT!!
	    	
	    	label_list *node = malloc(sizeof(label_list));
	    	node->label = malloc(sizeof(MAX_LABEL_LENGTH));
	    	node->label= strdup(label_get);
	    	node->source_line = i;
	    	node->next = NULL;
	    	
	    	// now add it to a linked list along with it's line number!

	    	if (head == NULL)
	    	{
	    		head = node;
	    	}
	    	
	    	if (tail == NULL)
	    	{
	    		tail = node;
	    	}
	    	else
	    	{
	    		tail->next = node;
	    		tail = node;
	    	}
	    }



	    int k = 0;
	    int len = strlen(line_parse);

		for (int j=0;j<len;j++)	
	    {
	    	if (line_parse[j] == '#') 	// rest of line is comment, ignore it
	    	{
	    		program.source[i][k] = '\0';
	    		break;
	    	}
	    	if (line_parse[j] == ',')	// remove pesky commas, deal with it
	    	{
	    		continue;
	    	}
	    	program.source[i][k++] = line_parse[j];
	    }

	    free(line_parse);		// deal with memory leaks if we can

	    program.source[i][k] = '\0';

	    // clean up line endings
	    for (j = strlen(program.source[i])-1;j >= 0 && 
	    		(program.source[i][j] == '\n' || 
	    		program.source[i][j] == '\r' ||
	    		program.source[i][j] == '#' || 
	    		program.source[i][j] == '\t' ||
	    		program.source[i][j] == ' ');j--);
	    	program.source[i][j+1] = '\0';
	    i++;
    }

	/* Close file */
	fclose(fp);

	program.lines = i;

	labels = head;

	return program;
}

// todo: for now assume lowercase
// todo: note. don't make .data lower case!
// char* lower_case(char* str)
// {
// 	for ( ; *str; ++str) *str = tolower(*str);
// 	return str;
// }

// int free_program(char** program)
// {

// }

void display_usage()
{
	printf(" Usage:\n");
	printf("   ./mip2c -l filename\n");
	printf("        loads and executes a file.\n");
	exit(0);
}

int str_to_int(char* str)
{
	// todo: maybe clean this up later? for now atoi works
	return atoi(str);
}

void free_label_list(label_list* head)
{
	// free our label list
	// or do we need to do this since the program is probably ending?
}

void print_labels()
{
	label_list *curr = labels;
	while (curr != NULL)
	{
		printf("label: %s line number: %d\n", curr->label, curr->source_line);
		curr = curr->next;
	}

}