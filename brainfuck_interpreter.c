#include <stdio.h>
#include <sysexits.h>
#include <string.h>
#include <stdlib.h>

#define EX_SUCCESS 0

#define str1_is_less_than_str2 (strcmp("bf", file_extension) < 0)
#define str2_is_less_than_str1 (strcmp("bf", file_extension) > 0)

#define MEMSIZE 30000
#define NUMBER_OF_BF_INSTRUCTIONS 8

#define FIRST_ELEMENT_NOT_EXISTS (*head_ptr == NULL)

struct bf_instruction_node
{
	int bf_instruction;
	struct bf_instruction_node *next_element;
};

struct stack_node
{
	struct bf_instruction_node *bf_instr_ptr;
	struct stack_node *link;
};

void inc_ptr(int **values_ptr)
{
	++(*values_ptr);
}

void dec_ptr(int **values_ptr)
{
	--(*values_ptr);
}

void check_if_value_is_correct(int *values_ptr)
{
	if( *values_ptr < 0 ) 
	{
		*values_ptr = 255;
	}
	else if( *values_ptr > 255 )
	{
		*values_ptr = 0;
	}
}

void inc_value(int *values_ptr)
{
	++(*values_ptr);
	check_if_value_is_correct(values_ptr);
}

void dec_value(int *values_ptr)
{
	--(*values_ptr);
	check_if_value_is_correct(values_ptr);
}

void print_value(int *values_ptr)
{
	putchar(*values_ptr);
}

void input_value(int *values_ptr)
{
	*values_ptr = getchar();
}

void push(struct stack_node **esp_ptr, struct bf_instruction_node *current_instr_ptr)
{
	struct stack_node *new_element_on_the_stack;
	
	new_element_on_the_stack = (struct stack_node *)malloc(sizeof(struct stack_node));
	new_element_on_the_stack->bf_instr_ptr = current_instr_ptr;
	new_element_on_the_stack->link = *esp_ptr;
	*esp_ptr = new_element_on_the_stack;
}

void pop(struct stack_node **esp_ptr)
{
	struct stack_node *tmp;

	tmp = *esp_ptr;
	*esp_ptr = (*esp_ptr)->link;
	free(tmp);
}

void start_loop(int *values_ptr, struct stack_node **esp_ptr, struct bf_instruction_node **current_instr_ptr)
{
		if( *values_ptr != 0 ) 
		{
			push(esp_ptr, *current_instr_ptr);
		}
		else
		{
			int balance = 0;

			do
			{
				if( (*current_instr_ptr)->bf_instruction == '[' ) 
				{
					balance++;
				}
				else if ( (*current_instr_ptr)->bf_instruction == ']' ) 
				{
					balance--;
				}
				if( balance != 0 ) 
				{
					*current_instr_ptr = (*current_instr_ptr)->next_element;
				}
			} while( balance > 0 );
		}
}

void end_loop(int *values_ptr, struct bf_instruction_node **current_instr_ptr, struct stack_node **esp_ptr)
{
	if( *values_ptr != 0  )
	{
		check_if_value_is_correct(values_ptr);
		*current_instr_ptr = (*esp_ptr)->bf_instr_ptr;
	}
	else
	{
		pop(esp_ptr);
	}
}

void execute_instructions(int **values_ptr, struct bf_instruction_node *head_ptr, struct bf_instruction_node **current_instr_ptr)
{
	struct stack_node *esp_ptr = NULL;

	char brainfuck_instruction;
	*current_instr_ptr = head_ptr;

	while( *current_instr_ptr != NULL )
	{
		brainfuck_instruction = (*current_instr_ptr)->bf_instruction;

		switch( brainfuck_instruction )
		{
			case '>': inc_ptr(values_ptr);									break;
			case '<': dec_ptr(values_ptr);									break;
			case '+': inc_value(*values_ptr);								break;
			case '-': dec_value(*values_ptr);								break;
			case '.': print_value(*values_ptr);								break;
			case ',': input_value(*values_ptr);								break;
			case '[': start_loop(*values_ptr, &esp_ptr, current_instr_ptr);	break;
			case ']': end_loop(*values_ptr, current_instr_ptr, &esp_ptr);	break;
		}
		*current_instr_ptr = (*current_instr_ptr)->next_element;
	}
}

struct bf_instruction_node *create_new_element(struct bf_instruction_node *head_ptr, struct bf_instruction_node **current_instr_ptr, int char_from_file)
{
	struct bf_instruction_node *new_element;

	*current_instr_ptr = head_ptr;
	
	while( (*current_instr_ptr)->next_element != NULL )
		*current_instr_ptr = (*current_instr_ptr)->next_element;

	new_element = (struct bf_instruction_node *)malloc(sizeof(struct bf_instruction_node));

	return new_element;
}

void add_instruction_to_the_list(struct bf_instruction_node **head_ptr, struct bf_instruction_node **current_instr_ptr, int char_from_file)
{
	if( FIRST_ELEMENT_NOT_EXISTS )
	{
		*head_ptr = (struct bf_instruction_node *)malloc(sizeof(struct bf_instruction_node));

		if( *head_ptr == NULL )
		{
			perror("Memory allocation failed");
			exit(EXIT_FAILURE);
		}
		else
		{
			(*head_ptr)->bf_instruction = char_from_file;
			(*head_ptr)->next_element = NULL;
			*current_instr_ptr = *head_ptr;
		}
	}
	else
	{
		struct bf_instruction_node *new_element = create_new_element(*head_ptr, current_instr_ptr, char_from_file);

		if( new_element == NULL )
		{
			perror("Memory allocation failed.");
			exit(EXIT_FAILURE);
		}
		else
		{
			new_element->bf_instruction = char_from_file;
			new_element->next_element = NULL;
			(*current_instr_ptr)->next_element = new_element;
				*current_instr_ptr = new_element;
			}
	}
}

void print_instructions(struct bf_instruction_node *head_ptr, struct bf_instruction_node **current_instr_ptr)
{
	*current_instr_ptr = head_ptr;

	while( *current_instr_ptr != NULL )
	{
		printf("%c", (*current_instr_ptr)->bf_instruction);
		*current_instr_ptr = (*current_instr_ptr)->next_element;
	}
}

void clear_the_memory(struct bf_instruction_node *head_ptr, struct bf_instruction_node **current_instr_ptr)
{
	struct bf_instruction_node *earlier_element;

	*current_instr_ptr = head_ptr;

	while( (*current_instr_ptr) != NULL )
	{
		earlier_element = *current_instr_ptr;
		*current_instr_ptr = (*current_instr_ptr)->next_element;
		free(earlier_element);
	}

	 puts("Memory is cleared.");
}

const char *is_bf_instruction(int char_from_file)
{
	const char bf_alphabet[NUMBER_OF_BF_INSTRUCTIONS] = {'>', '<', '+', '-', ',', '.', '[', ']'};
	return memchr(bf_alphabet, char_from_file, sizeof(bf_alphabet));
}

const char *get_file_extension(const char *filename)
{
	const char *dot = strchr(filename, '.');

	if( dot == NULL )
		return NULL;

	const char *file_extension = dot + 1;
	return file_extension;
}

int main(int argc, char **argv)
{
	if( argc != 2 )
	{
		fprintf(stderr, "File not specified.\n");
		puts("usage: ./bf_interpreter <filename.bf>");
		return EX_USAGE;
	}

	const char *filename = argv[1];
	const char *file_extension = get_file_extension(filename);

	if( file_extension == NULL || str1_is_less_than_str2 || str2_is_less_than_str1 )
	{
		fprintf(stderr, "Incorrect file extension.\n");
		puts("usage: ./bf_interpreter <filename.bf>");
		return EX_DATAERR;
	}

	FILE *file_with_bf_code = fopen(filename, "r");

	if( file_with_bf_code == NULL )
	{
		perror(filename);
		return EX_NOINPUT;
	}

	int values[MEMSIZE] = {0};
	int *values_ptr = values;
	int char_from_file;

	struct bf_instruction_node *head_ptr = NULL, *current_instr_ptr;

	while( (char_from_file = fgetc(file_with_bf_code)) != EOF )
	{
		if( is_bf_instruction(char_from_file) != NULL )
			add_instruction_to_the_list(&head_ptr, &current_instr_ptr, char_from_file);
	}

	execute_instructions(&values_ptr, head_ptr, &current_instr_ptr);
	clear_the_memory(head_ptr, &current_instr_ptr);

	return EX_SUCCESS;
}
