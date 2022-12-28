#include <cs50.h>
#include <stdio.h>

//function declarations
int cc_len(long long ccn);
bool check_valid(long long ccn);
int checksum(long long ccn);
void print_card_provider(long long ccn);

//main executable
int main(void)
{
    long long cc_num;
    do
    {
        cc_num = get_long("Number: ");
    }
    while (cc_num < 1);

    if (check_valid(cc_num) == true)
        print_card_provider(cc_num);
    else
        printf("INVALID\n");

    return 0;
}

// FUNCTION DEFINITIONS!

// finds the length (digits) of a provided credit card number
int cc_len(long long ccn)
{
    int len = 0;
    while (ccn > 0)
    {
        ccn /= 10;
        len++;
    }
    return len;
}

// checks that the provided ccn is the appropriate length and passes Luhn's Algorithim
bool check_valid(long long ccn)
{
    int len = cc_len(ccn);
    return (len == 13 || len == 15 || len == 16) && checksum(ccn) == 0;
}

//checks that the provided ccn is compatible with Luhn's Algorithim
int checksum(long long ccn)
{
    int sum = 0;
    for(int i = 0; ccn != 0; i+=1, ccn /= 10)
    {
        if (i % 2 != 0)
        {
            int digit = 2 * (ccn % 10);
            sum += (digit / 10) + (digit % 10);
        }
        else
            sum += ccn % 10;
    }
    return (sum % 10);
}

//checks the provider of a credit card based on identifing characteristics
void print_card_provider(long long ccn)
{
    if ((ccn >= 34e13 && ccn <35e13) || (ccn >= 37e13 && ccn < 38e13))
        printf("AMEX\n");
    else if ((ccn >= 51e14 && ccn <56e14))
        printf("MASTERCARD\n");
    else if ((ccn >= 4e12 && ccn <5e12) || (ccn >= 4e15 && ccn < 5e15))
        printf("VISA\n");
    else
        printf("INVALID\n");

}