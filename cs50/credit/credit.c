#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    long cc;

    do
    {
        cc = get_long("Enter credit card number: ");
    }
    while (cc < 0);

    long ccNUM = cc;
    int sum = 0;
    int count = 0;
    int digit;
    bool is_second = false;

    while (cc > 0)
    {
        digit = cc % 10;
        if (is_second)
        {
            int product = digit * 2;
            sum += product / 10 + product % 10;
        }
        else
        {
            sum += digit;
        }
        is_second = !is_second;
        cc /= 10;
        count++;
    }

    long start = ccNUM;
    while (start >= 100)
    {
        start /= 10;
    }

    if (sum % 10 == 0)
    {
        if ((start / 10 == 4) && (count == 13 || count == 16))
        {
            printf("VISA\n");
        }
        else if ((start >= 51 && start <= 55) && count == 16)
        {
            printf("MASTERCARD\n");
        }
        else if ((start == 34 || start == 37) && count == 15)
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}
