#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

void alpha_arr_val(char pos, string key)
    string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < strlen(alpha); i++)
    {
        if (islower(pos))
        {
            if (pos == tolower(alpha[i]))
            {
                printf("%c", tolower(key[i]));
            }
        }
        else
        {
            if (pos == toupper(alpha[i]))
            {
                printf("%c", toupper(key[i]));
            }
        }
    }
}


void Do_substitute(string key)
{
    string p = get_string("plaintext: ");

    printf("ciphertext: ");

    for (int i = 0; i < strlen(p); i++)
    {
        if (isalpha(p[i]))
        {
            char x = p[i];
            if (islower(p[i]))
            {
                alpha_arr_val(tolower(x), key);
            }
            else
            {
                alpha_arr_val(toupper(x), key);
            }


        }
        else
        {
            printf("%c", p[i]);
        }
    }

    printf("\n");

}

int main(int argc, string argv[])
{
    if (argc == 2) // number of commands in terminal
    {
        if (strlen(argv[1]) == 26)
        {
            for (int i = 0; i < strlen(argv[1]) ; i++) // checking each element of string
            {

                if (! isalpha(argv[1][i])) // if contain non-alphabet
                {
                    printf("Key must contain 26 characters.\n");
                    return 1;
                }

                for (int j = i + 1 ; j < strlen(argv[1]) ; j++) // checking to the next element of arg[i]
                {

                    if (toupper(argv[1][j]) == toupper(argv[1][i])) // checking repeated element
                    {
                        printf("Key must not contain repeated alphabets.\n");
                        return 1;
                    }
                }
            }

            Do_substitute(argv[1]);

        }
        else
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }

    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    return 0;
}
