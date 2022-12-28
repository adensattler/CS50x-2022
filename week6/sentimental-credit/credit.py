from cs50 import get_int

def main():
    card_num = get_card_num()

    card_len = len(str(card_num))
    card_num = str(card_num)

    if (card_len == 13 or card_len == 15 or card_len == 16) and checksum(card_num) == 0:
        print_card_provider(card_num)
    else:
        print("INVALID\n")


def checksum(cc_num):
    sum = 0
    for i in range(-2, -len(cc_num)-1, -2):
        for digit in str(int(cc_num[i])*2):
            sum+= int(digit)
    for i in range(-1, -len(cc_num)-1, -2):
        sum += int(cc_num[i])

    return sum%10

# 378282246310005

#checks the provider of a credit card based on identifing characteristics
def print_card_provider(ccn):
    if int(ccn[0]) == 3 and (int(ccn[1]) == 4 or int(ccn[1]) == 7):
        print("AMEX\n")
    elif int(ccn[0]) == 5 and int(ccn[1]) >=1 and int(ccn[1])<=5:
        print("MASTERCARD\n")
    elif int(ccn[0]) == 4:
        print("VISA\n")
    else:
        print("INVALID\n")

# GREEN
def get_card_num():
    while True:
        card_num = input("Number: ")
        try:
            if len(card_num) > 0 and int(card_num):
                break
        except ValueError:
            continue
    card_num = int(card_num)
    return card_num



if __name__ == "__main__":
    main()