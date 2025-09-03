/* Buffer procedures used by buffer bomb */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

/* Function declarations */
char *Gets(char *, char *, int);

/* Buffer size for getbuf */
#define NORMAL_BUFFER_SIZE 32

/* Buffer size for getbufn */
#define KABOOM_BUFFER_SIZE 512
#define INPUT_STRING_SIZE 4096
#define BYTE_BUFFER_INITIAL_SIZE 1024
#define COMMENT_START "/*"
#define COMMENT_END   "*/"
//////////////////////////////////////////////////////////////////////
//
// @fn convert_to_hex_value Parses a hex string and returns specified byte.
// @param input The hex string. Should not have the leading "0x". 
//    For  example, "ab" is a valid value of input, but "0xab" is not.
// @return Parsed value.
//
//////////////////////////////////////////////////////////////////////
unsigned char convert_to_hex_value(char *input)
{
	unsigned val;
	sscanf(input, "%x", &val);
	return (unsigned char)val;
}

unsigned char* convert_to_byte_string(FILE *file_in, int *size)
{
	int comment_level = 0;
	int  input_string_size = INPUT_STRING_SIZE;
	char input[input_string_size];

  
	int byte_buffer_size = BYTE_BUFFER_INITIAL_SIZE;
	int byte_buffer_offset = 0;

	unsigned char* byte_buffer = (unsigned char*)malloc( BYTE_BUFFER_INITIAL_SIZE * sizeof(*byte_buffer) );
	if ( byte_buffer == NULL )
		{
			return NULL;
		}


	// scan loop
	while ( fscanf(file_in, "%s", input) > 0 )
		{
			// Case 1: Enter a comment block
			if ( !strcmp(input, COMMENT_START) )
				{
					comment_level++;
					continue;
				}

			// Case 2: Leave a comment block
			if ( !strcmp(input, COMMENT_END) )
				{
					if ( comment_level <= 0 )
						{
							// make sure we are in a comment-block
							fprintf(stderr, "Error: stray %s found.\n", COMMENT_END);
							free( byte_buffer );
							return NULL;
						}
					comment_level--;
					continue;
				}

			// Case 3: Convert data to hex value and store
			if ( comment_level == 0 )
				{
					// we should have read a hex value and print it out.
					if ( !isxdigit(input[0]) ||
						 !isxdigit(input[1]) ||
						 (input[2] != '\0') )
						{
							fprintf(stderr,
									"Invalid hex value [%s]. "
									"Please specify only single byte hex values separated by whitespace.\n",
									input);
							free( byte_buffer );
							return NULL;
						}

					unsigned char b = convert_to_hex_value(input);


					// see if we have enough room in the buffer...
					if ( byte_buffer_offset == byte_buffer_size )
						{
							byte_buffer = (unsigned char *)realloc( byte_buffer, 2 * byte_buffer_size );
							if ( byte_buffer == NULL )
								{
									return NULL;
								}
							byte_buffer_size *= 2;
						}
					byte_buffer[ byte_buffer_offset++ ] = b;
				}
		}


	*size = byte_buffer_offset;
	return byte_buffer;
}

/*
 * getbuf - Has the buffer overflow bug exploited by levels 0-3
 */
int getbuf(char *src, int len)
{
    char buf[NORMAL_BUFFER_SIZE];
	printf("buf%p\n", &buf);
    Gets(buf,src,len);
    return 1;
}

/* 
 * getbufn - Has the buffer overflow bug exploited by level 4.
 */
int getbufn(char *src, int len)
{
    char buf[NORMAL_BUFFER_SIZE];	
	printf("bufn:%p\n", &buf);
    Gets(buf, src, len);
    return 1;
}

