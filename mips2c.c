/*

	mips2c

	todo:
		- right now, this doesn't care about portability really
		  (asides from specifying uint32_t(?)), try to test
		  on  different systems and see how it goes!
		- wrap malloc function to check return is not null
		- what does the stack pointer do?
		- how to more accurately emulate pc?
		- get some more complex programs to test out and see what else we need to do!
		- recursive calls?
		- incorporate ncurses to make everything look better!
		- better parsing engine! the current method is pretty meh.
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
			- 32 new registers to add
			- instructions that have to do with floating point
		- error reporting! can we get it to report a line number?
		- clean up makefile - depend on headers
		- figure out whats up with incompatible pointer types with linked list for labels
		- probably memory leaks EVERYWHERE! need to make sure we clean them up!
		- make it so we can store pointers to memory addresses and put this information
		  in memory somewhere!
			- ie, keep a linked list of labels to areas of memory where we've stored
			  this information. if we get a string, just crawl memory until we find a \0??
			  (HOW DOES MIPS KNOW HOW LONG A STRING IS?)

	references used:
		https://www.student.cs.uwaterloo.ca/~cs241/mips/mipsref.pdf
		https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_help.html
		https://github.com/MIPT-ILab/mipt-mips/wiki/MIPS-Instruction-Set
*/

/*
    0x00400000 - Text segment - program instructions 
    0x10000000 - Data segment 
    0x7FFFFFFF, and decreasing addresses - Stack segment
*/

#include "mips2c.h"

// private functions
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
	display_memory = false;

	parse_arguments(argc, argv, &filename);

	program = get_program(filename);
	
	// data_segment = malloc(MEMORY_SIZE);
	data_segment_offset = 0;
	// heap = malloc(MEMORY_SIZE);
	// stack = malloc(MEMORY_SIZE);
	clear_memory();						// also sets $SP and data offset for dataseg

	pc = 0;
	bool finished = false;
	char* statement;
	parsed_instruction* p;
	char* mem_ptr;

	if (debug) printf("Parsing file: %s\n", program.filename);

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
	if (display_memory) print_memory();

	return 0;
}

void parse_arguments(int argc, char* argv[], char** filename)
{
	int opt;
	while ((opt = getopt(argc, argv, "l:vdhriwm")) != -1)
	{
		switch(opt)
		{
			case 'm':
				display_memory = true;
				break;
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
	parser_state p_state;
	p_state = TEXT_STATE;
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
	int lines_allocated = 128;			// start with 128 lines of space and increase if needed
	int max_line_len = 100;				// cap line limit at 100 chars

	// Allocate lines of text
	program.source = (char **)malloc(sizeof(char*)*lines_allocated);
	if (program.source == NULL)
    {
    	error("Out of memory");
    }

	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
    {
    	error("Error opening file");
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
	        new_size = lines_allocated * 2;
	        program.source = (char **) realloc(program.source, sizeof(char*)*new_size);
	        if (program.source == NULL)
	            {
	            error("Out of memory");
	            }
	        lines_allocated = new_size;
        }

	    // alloc line for space
	    program.source[i] = malloc(max_line_len);
	    line_parse = malloc(max_line_len);

	    if (program.source[i]==NULL)
        {
            error("Out of memory");
        }

	    if (fgets(line_parse, max_line_len-1, fp) == NULL)
	    {
	        break;
	    }

	    if (line_parse[0] == '#')	// this entire line is a comment, ignore it
	    {
	    	continue;
	    }

	    // todo: RIGHT NOW THIS ONLY HANDLES LABELS THAT ARE ON A SEPARATE LINE!!
	    // Lines can have format LABEL: INSTRUCTIONS #COMMENTS
	    // ALSO todo, we want to strip white space from start of line!
	    // todo: make sure we haven't tried to store the same label twice,
	    //		 this would probably boink things up!
	    if (strchr(line_parse, ':') != NULL)
	    {
	    	if (isalpha(line_parse[0]) == 0)
	    	{
	    		printf(ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET": Label %s must start with alphabet (not number/symbol)\n", line_parse);
	    		exit(1);
	    	}

	    	// every thing underneath here is only if we are in text section, we match up a label with a line number!
	    	// so use a state machine here to check if we are in .data or .text section

	    	if (p_state == TEXT_STATE)
	    	{
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
		    	label_get[pi] = 0;	// null terminate the string
		    	// ok we got a label

		    	// todo: IF THERE IS MORE OF A LINE HERE, WE NEED TO PARSE IT!!
		    	// see what else happens here


		    	// now add it to a linked list along with it's line number!	    	
		    	label_list *node = malloc(sizeof(label_list));
		    	node->label = malloc(sizeof(MAX_LABEL_LENGTH));
		    	node->label = strdup(label_get);
		    	node->data_type = _PC;
		    	node->source_line = i;
		    	node->next = NULL;
		    	
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

		    if (p_state == DATA_STATE)
		    {
		    	// todo: FIRST, make sure our next thing starts with '.'
		    	// and is a proper data type!
		    	// options are 
		    	char* data_type = malloc(MAX_LABEL_LENGTH);
		    	// data types
		    	// ascii, asciiz, byte, halfword, word, float, double
		    	// halfword = 16bit
		    	// word = 32bit
		    	// byte = 8bit.
		    	// Integer = 32bit
		    	// character = 4 bits
		    	int k = 0;
		    	data_types d_type;
		    	char* p = strdup(strchr(line_parse, ':'));
		    	char* value = malloc(MAX_LABEL_LENGTH);
		    	p++;	// p should now point to a space
		    	int d_state = 0;
		    	int data_size = 0;	// in bits! (/8 for size in char)
		    	// get label
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
		    	label_get[pi] = 0;	// null terminate the string

		    	while(1)
		    	{
		    		if (d_state == 0)
		    		{
		    			if (*p != ' ')
		    			{
		    				// todo: throw error here
		    				// this should be a space before label
		    				warning("Expected space here");
		    			}
		    			d_state++;
		    			p++;
		    		}
		    		else 
		    		if (d_state == 1)
		    		{
		    			if (*p != '.')
		    			{
		    				// todo: this throws an error
		    				// we need a .
		    				error("Expected . before data type");

		    			}
		    			d_state++;
		    			p++;
		    		} 
		    		else
	    			if (d_state == 2)
		    		{	
		    			// grab data type
		    			data_type[k++] = *p;
		    			if (*p == ' ')
		    			{
		    				data_type[k - 1] = 0;
		    				k = 0;	// reset k so we can use it again
		    				d_state++;
		    			}
		    			p++;
		 
		    		}
	    			else
	    			if (d_state == 3)
	    			{
	    				// error check data type
	    				if (strcmp(data_type, "ascii") == 0)
	    				{
	    					d_type = _ASCII;
	    				}
	    				else if (strcmp(data_type, "asciiz") == 0)
	    				{
	    					d_type = _ASCIIZ;
	    				}
	    				else if (strcmp(data_type, "byte") == 0)
	    				{
	    					d_type = _BYTE;
	    				}
	    				else if (strcmp(data_type, "half") == 0)
	    				{
	    					d_type = _HALF;
	    				}
	    				else if (strcmp(data_type, "word") == 0)
	    				{
	    					// this can also initialize an array
	    					d_type = _WORD;
	    				}
	    				else if (strcmp(data_type, "float") == 0)
	    				{
	    					d_type = _FLOAT;
	    				}
	    				else if (strcmp(data_type, "double") == 0)
	    				{
	    					d_type = _DOUBLE;
	    				}
	    				else if (strcmp(data_type, "space") == 0)
	    				{
	    					d_type = _SPACE;
	    				}
	    				else
	    				{
	    					// printf(ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET ": Invalid data type\n");
	    					// exit(1);
	    					error("Invalid data type");
	    				}
	    				d_state++;
	    			}
	    			else
	    			{
	    				value[k++] = *p;
		    			if (*p == '\n' || *p == '\r')
		    			{
		    				// value[k] = 0;	//don't use this
		    				break;
		    			}
		    			p++;
	    			}
		    	}

		    	label_list *node = malloc(sizeof(label_list));
		    	node->label = malloc(sizeof(MAX_LABEL_LENGTH));
		    	node->label = strdup(label_get);
		    	node->data_type = d_type;
		    	node->source_line = i;		// we don't care about this!
		    	node->next = NULL;

		    	int32_t v;
		    	switch (d_type)
		    	{
		    		case _ASCII:
		    		case _ASCIIZ:
		    			// remove start and finish quotes
		    			if (value[k - 2] != '\"')
		    			{
		    				error("String must end with quotation mark");
		    			}
		    			value[k - 2] = 0;

		    			if (value[0] != '\"')
		    			{
		    				error("Strings must start with quotation mark");
		    			}

		    			value++;

		    			// write to mem
		    			node->mem_ptr = data_segment + data_segment_offset;
		    			int wlen = strlen(value);
		    			if (d_type == _ASCII)
		    			{
		    				wlen--;	// don't write final 0 if we're just an ascii
		    			}
		    			printf("trying to write ascii to mem...");
		    			write_memory(value, node->mem_ptr, wlen);
		    			printf("success\n");
		    			data_segment_offset += wlen;
		    			// for (int wr=0; wr < strlen(value); wr++)
		    			// {
		    			// 	write_memory(value[wr], data_segment + data_segment_offset, 1);
		    			// 	data_segment_offset++;
		    			// }
		    			// if (d_type == _ASCIIZ)
		    			// {
		    			// 	write_memory(0, data_segment + data_segment_offset, 1);
		    			// 	data_segment_offset++;
		    			// }
		    			break;

		    		case _BYTE:
		    		case _WORD:
		    		case _HALF:
		    			// check if we're a char
		    			v = str_to_int(value);
		    			node->mem_ptr = data_segment + data_segment_offset;
		    			write_memory(&v, node->mem_ptr, sizeof(v));
		    			data_segment_offset += sizeof(v);
		    			break;

		    		default:
		    			error("Type not supported yet");
		    			break;
		    	}
		    	// todo: round data_Segment_offset to nearest multiple of 4
		    	data_segment_offset = align4(data_segment_offset);
		    	free(data_type);
		    	// free(value);		// how do we free this now that it's changed?

		    	// TODO:
		    	// write out stuff to memory and store a pointer to that
		    	// memory under mem_ptr!
		    	// we need a way to keep track of where we are writing to
		    	// in our data section!


		    	
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
		    if (p_state == GLOBAL_STATE)
		    {
		    	// todo: what happens here??
		    }
		    // do we need to check if (p_state == GLOBAL_STATE) here? this is probably only for compiled progrs?

	    }

	    int k = 0;
	    int len = strlen(line_parse);

		for (int j=0; j<len; j++)	
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

	    // todo: does this do anythin useful or nah?
	    // clean up line endings
	    for (j = strlen(program.source[i])-1; j >= 0 && 
			    	   (program.source[i][j] == '\n' || 
			    		program.source[i][j] == '\t' ||
			    		program.source[i][j] == '\r' ||
			    		program.source[i][j] == '#' || 

			    		program.source[i][j] == ' '); j--);
	    	program.source[i][j+1] = '\0';

	    // switch between different parser states
	    if (program.source[i][0] == '.')
	    {
	    	if (strcmp(program.source[i], ".data") == 0)
	    	{
	    		p_state = DATA_STATE;
	    	}
	    	if (strcmp(program.source[i], ".text") == 0)
	    	{
	    		p_state = TEXT_STATE;
	    	}
	    	if (strcmp(program.source[i], ".global") == 0)
	    	{
	    		p_state = GLOBAL_STATE;
	    	}
	    }

	    i++;

    }

	/* Close file */
	fclose(fp);

	program.lines = i;

	labels = head;

	if (verbose) 
	{
		printf("Got labels:\n");
		print_labels();
		printf("Memory:\n");
		print_memory();
	}

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
		// eventually use this to release mem alloc'd
// }

void display_usage()
{
	// todo: add more info about command line arugments here
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
	// display a linked list of labels (this will eventually hold source_lines OR mem pointers?)
	// or should the mem pointer be different??
	const char * data_type_labels[] = {
	    "Program Counter",
	    "ascii",
	    "asciiz",
	    "byte",
	    "halfword",
	    "word",
	    "float",
	    "double",
	    "space"
	};

	label_list *curr = labels;
	while (curr != NULL)
	{
		if (curr->data_type == _PC)
		{
			printf("label: %s\t\tline number: %d\n", curr->label, curr->source_line);
		}
		else
		{
			printf("label: %s\t\ttype: %s\n", curr->label, 
				data_type_labels[curr->data_type]);
		}
		curr = curr->next;
	}

}

inline int align4(int num) {
	// return num rounded up to closest multiple of 4
	int remainder = num % 4;
    if (remainder != 0) num += 4 - remainder;
    return num;
}