/*

	mips2c
	Tyler Weston, 2019/2020
	"I should probably be studying..."

	todo:
		- doesn't display text accurately rn when parsing
	    - should replace , with ' '
		- bubble sort still isn't working? why? try re-downloading,
		  why does some syntax have to change? implement that next!
		  such as wordd 12:0 or whatever it was!
		  How is the SP working out?
		- need to be able to store arrays like 0 : 12 to declare
		  enough space for ie arrays
		- if there is more than one space between var name
		  and quotation
		- problem with lines that have both # and : in them, being interpreted as labels!
		  ignore lines that the first char is a # !
		- this memory stuff is getting better but still not great, can't read or
		  write stuff efficiently! spin this off into a smaller test file and
		  play around with it more!
		- .align directive
		- test SLT
		- switch all error messages over to error function
		- Make error message be able to accept formatted text!
		- parsing data section still seems brittle! look into this and test more!
		- right now, this doesn't care about portability really
		  (asides from specifying uint32_t(?)), try to test
		  on  different systems and see how it goes!
		- wrap malloc function to check return is not null
		- what does the stack pointer do? (NEED TO ALLOCATE MEMORY FOR THIS AND 
		  ALLOW ARBITRARY WRITES TO SP)
		- how to more accurately emulate pc?
		- get some more complex programs to test out and see what else we need to do!
		- recursive calls?
			- this will be dealt with through the jump and link instructions?
			- store pc in $31 to be used later
			- will also need to implement stack and $sp to do this?
		- incorporate ncurses to make everything look better!
			- split the screen into two halves? have one display
			  current running source and the other display infromation
			  about our program?
		- better parsing engine! the current method is pretty meh.
		- todo eventually: instructions gets interpreted into
		  machine code and loaded and run from virtual memory
		- rearrange structure of source code, headers, and example programs
		- LOTS of refactoring to do, especially those gross giant function switch blocks
		  to get information about them! store it in some sort of dictionary or hashmap or
		  something!
		- what to do with pseudoinstructions? process first into atomic expr? or just use direct?
		- floating point coprocessor (CP1)
			- 32 new registers to add
			- instructions that have to do with floating point
		- clean up makefile - depend on headers
		- figure out whats up with incompatible pointer types with linked list for labels
		- probably memory leaks EVERYWHERE! need to make sure we clean them up!

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

// Big ol' header
#include "mips2c.h"
#include "mips2c_instructions.h"


// private functions
void parse_arguments(int argc, char* argv[], char** filename);
void display_usage(bool full);

int flags = 0;	// init flags
int pc = 0;
int max_steps;
label_list* labels;

// Main
int main(int argc, char *argv[])
{
	printf(ANSI_COLOR_BRIGHT_BLUE "MIPS2C\n" ANSI_COLOR_RESET);
	printf("--------------------------\n");
	if (argc == 1)
	{
		display_usage(false);
	}
	// parse command line inputs	

	char* filename;
	program program;	// hmm... a variable named program of type program

	parse_arguments(argc, argv, &filename);

	if (check_flag(f_debug)) printf("Setting up memory\n");
	clear_memory();						// also sets $SP and data offset for dataseg

	if (check_flag(f_debug)) printf("Parsing file: " ANSI_COLOR_BLUE "%s\n" ANSI_COLOR_RESET, filename);
	program = get_program(filename);
	
	int step_number = 0;

	bool finished = false;
	char* statement;
	parsed_instruction* p;

	while (pc < program.lines)	//is this a good way to check this?
	{
		if (check_flag(f_break_max))
		{
			if (step_number + 1 > max_steps)
			{
				break;
			}
		}
		
		if (check_flag(f_display_step_number)) printf("[%d]", step_number);
		step_number++;
		if (check_flag(f_verbose)) printf(ANSI_COLOR_YELLOW "%d: " ANSI_COLOR_RESET, pc);
		
		statement = strdup(program.source[pc]);

		pc++;	// do we do this here or does it have to be somewhere else?
				// do this here so if we jump somewhere else while interpreting
				// our instruction, we don't increase pc again
		p = parse_instruction(statement);
		if (p != NULL)
		{
			if (check_flag(f_display_instructions)) print_instruction(p);
			execute_instruction(p);
		}
	}

	exit_info();
	return 0;
}

void exit_info() {
	// call this before program ends to do any clean-up,
	// diagnostic stuff, etc.
	if (check_flag(f_display_registers)) print_registers(registers); 
	if (check_flag(f_display_memory)) print_memory();
	printf(ANSI_COLOR_RESET "\n");	// flush stuff out just in case
}


void parse_arguments(int argc, char* argv[], char** filename)
{
	// check command line arguments and set appropriate flags
	const char* arg_flags = "l:s:vdhpriwnm";
	int opt;
	while ((opt = getopt(argc, argv, arg_flags)) != -1)
	{
		switch(opt)
		{
			case 'p':
				set_flag(f_print_stack_pointer);
				break;
			case 's':
				max_steps = str_to_int(optarg);
				set_flag(f_break_max);
				break;
			case 'n':
				set_flag(f_display_step_number);
				break;
			case 'm':
				set_flag(f_display_memory);
				break;
			case 'v':
				set_flag(f_verbose);
				break;
			case 'w':
				set_flag(f_display_warnings);
				break;
			case 'r':
				set_flag(f_display_registers);
				break;
			case 'h':
				display_usage(true);
				break;
			case 'i':
				set_flag(f_display_instructions);
				break;
			case 'd':
				set_flag(f_debug);
				break;
			case 'l':
				// get program name and load it
				*filename = optarg;
				break;
			default:
				display_usage(false);
				break;
		}
	}
}

program get_program(char* filename) 
{
	parser_state p_state;
	p_state = TEXT_STATE;

	int data_segment_offset = 0;	// track where we are in memory

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
	    program.source[i] = malloc(MAX_LINE_LENGTH);
	    line_parse = malloc(MAX_LINE_LENGTH);

	    if (program.source[i]==NULL)
        {
            error("Out of memory");
        }

	    if (fgets(line_parse, MAX_LINE_LENGTH-1, fp) == NULL)
	    {
	        break;
	    }

	    // remove white space from start of line:
	    while (isspace(line_parse[0]))
	    {
	    	line_parse++;
	    }

	    if (line_parse[0] == '#')	// this entire line is a comment, ignore it
	    {
	    	continue;
	    }

	    // todo: make sure we haven't tried to store the same label twice,
	    //		 this would probably boink things up!
	    if (strchr(line_parse, ':') != NULL)
	    {
	    	if (isalpha(line_parse[0]) == 0)
	    	{
    			char err_msg[128];
				sprintf(err_msg, "Label %s must start with alphabetic character", line_parse);
				error(err_msg);
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
		    	char* value = malloc(MAX_STR_LENGTH);
		    	p++;	// p should now point to a space
		    	int d_state = 0;
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

		    	while(1)	// memory label parse state machine
		    	{
		    		if (d_state == 0)
		    		{
		    			if (*p == ' ' || *p == '\t')
		    			{
		    				// d_state++;
		    				p++;
		    			}
		    			else
		    			if (*p == '.')
		    			{
		    				d_state = 2;
		    				p++;
		    			}
		    			else
		    			{
		    				error("Expected space or . ");		    				
		    			}
		    		}
		    		else 
		    		if (d_state == 1)
		    		{
		    			if (*p != '.')
		    			{
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
	    				// we're assuming the label will be lowercase
	    				// TODO: do we need to cast this? 
	    				// error check data type
	    				if (STR_EQ(data_type, "ascii"))
	    				{
	    					d_type = _ASCII;
	    				}
	    				else if (STR_EQ(data_type, "asciiz"))
	    				{
	    					d_type = _ASCIIZ;
	    				}
	    				else if (STR_EQ(data_type, "byte"))
	    				{
	    					d_type = _BYTE;
	    				}
	    				else if (STR_EQ(data_type, "half"))
	    				{
	    					d_type = _HALF;
	    				}
	    				else if (STR_EQ(data_type, "word"))
	    				{
	    					// this can also initialize an array
	    					// THERE IS SPECIAL SYNTAX THAT CAN BE
	    					// USED HERE
	    					d_type = _WORD;
	    				}
	    				else if (STR_EQ(data_type, "float"))
	    				{
	    					d_type = _FLOAT;
	    				}
	    				else if (STR_EQ(data_type, "double"))
	    				{
	    					d_type = _DOUBLE;
	    				}
	    				else if (STR_EQ(data_type, "space"))
	    				{
	    					d_type = _SPACE;
	    				}
	    				else
	    				{
	    					error("Invalid data type");
	    				}
	    				d_state++;
	    			}
	    			else
	    			{
	    				value[k++] = *p;
		    			if (*p == '\n' || *p == '\r')
		    			{
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
		    	if (check_flag(f_debug)) printf(ANSI_COLOR_GREEN "%s " ANSI_COLOR_RESET, node->label);

		    	int32_t v;
		    	// remove comments if they exist
		    	bool gotQuote = false;
		    	for (int k = strlen(value); k >= 0; k--)
		    	{
		    		if (value[k - 1] == '\"')
		    		{
		    			value[k] = '\0';
		    			gotQuote = true;
		    			break;
		    		}
		    	}
		    	k = strlen(value) - 1;

		    	if (!gotQuote)
		    	{
		    		error("String declaration goes in double quotes!");
		    	}

		    	switch (d_type)
		    	{
		    		case _ASCII:
		    		case _ASCIIZ:
		    			// remove start and finish quotes
		    			if (value[k] != '\"')
		    			{
		    				error("String must end with quotation mark");
		    			}
		    			value[k] = 0;

		    			if (value[0] != '\"')
		    			{
		    				error("Strings must start with quotation mark");
		    			}

		    			value++;

		    			// write to mem
		    			node->mem_address = data_segment_offset;
		    			int wlen = strlen(value) + 1;	//+1 to account for null terminator
		    			if (check_flag(f_debug)) printf("> string > %s\n", value);
		    			for (int wi = 0; wi < wlen; wi++)
		    			{
		    				char c = value[wi];
		    				if (c == 0 && d_type == _ASCII) break;
		    				write_memory(&c, memory + data_segment_offset + wi, 1);
		    			}
		    			data_segment_offset += wlen;
		    			break;

		    		case _BYTE:
		    		case _WORD:
		    		case _HALF:
		    			// todo: test this, I think it's working?
		    			v = str_to_int(value);
		    			if (check_flag(f_debug)) printf("> word > %i\n", v);
		    			node->mem_address = data_segment_offset;
		    			int* store_loc = (int*)(&memory[data_segment_offset]);
		    			*store_loc = v;

		    			break;

		    		default:
		    			error("Type not supported yet");
		    			break;
		    	}

		    	data_segment_offset = align4(data_segment_offset);
		    	free(data_type);
		    	// free(value);		// how do we free this now that it's changed?

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
	    		program.source[i][k++] = ' ';
	    		continue;
	    	}
	    	program.source[i][k++] = line_parse[j];
	    }

	    // free(line_parse);		// deal with memory leaks if we can

	    program.source[i][k] = '\0';

	    // todo: does this do anythin useful or nah?
	    // clean up line endings
	    for (j = strlen(program.source[i]) - 1; j >= 0 && 
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

	if (check_flag(f_debug))
	{
		printf("Got labels:\n");
		print_labels();
	}

	if (check_flag(f_display_memory))
	{
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

void display_usage(bool full)
{
	// todo: add more info about command line arguments here
	// todo: make this const char* or something??
	printf(" Usage:\n");
	printf("   ./mip2c -l filename\n");
	printf("        loads and executes a file.\n");
	printf("		-h to see all commands\n");

	if (!full)	// display just basic help
	{ 
		exit(0);
	}
	printf("\nFull help:\n");
	printf(" -m\t\tdisplay memory\n");
	printf(" -i\t\tdisplay parsed instructions\n");
	printf(" -v\t\tverbose mode\n");
	printf(" -r\t\tdisplay registers\n");
	printf(" -s steps\thalt after steps iterations\n");
	printf(" -d\t\tdebug mode\n");
	printf("\n");
	printf(ANSI_COLOR_RED "Tyler Weston, 2019/2020\n" ANSI_COLOR_RESET);
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
		printf(ANSI_COLOR_BRIGHT_YELLOW "> " ANSI_COLOR_RESET);
		if (curr->data_type == _PC)
		{
			printf("%s\t\tline number: %d\n", curr->label, curr->source_line);
		}
		else
		{
			printf("%s\t\ttype: %s\n address: %d\n", curr->label, 
				data_type_labels[curr->data_type], curr->mem_address);
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