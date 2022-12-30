import multiprocessing
import requests
from termcolor import colored
from colorama import init
init(autoreset=True)

if __name__ == ('__main__'):
    f1 = open("outputresults.txt", 'w')

    # on-screen inputs for desired plate len
    print("1 <- All 1 letter/character combinations")
    print("2 <- All 2 letter/character combinations")
    print("3 <- All 3 letter/character combinations")
    print("4 <- All 4 letter words")
    print("5 <- All 5 letter words")
    choice = int(input("Please enter what you want to check: "))

    lines = []
    alphabet = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"]
    numbers = ["1", "2", "3", "4", "5", "6", "7", "8", "9"]
    index = 0
    #parsething = ""
    urlfront = "https://bmvonline.dps.ohio.gov/bmvonline/oplates/PlatePreview?plateNumber="
    urlback = "&vehicleClass=PC&organizationCode=0"

    # makes an array of all of the one letter/character combinations
    if choice == 1:
        for letter in alphabet:
            lines.append(letter)
        for num in numbers:
            lines.append(num)

    elif choice == 2:
        for i in range(len(alphabet)):
            for j in range(len(alphabet)):
                lines.append(alphabet[i]+alphabet[j])
            for num in numbers:
                lines.append(alphabet[i] + num)
    
    elif choice == 3:
        for i in alphabet:
            for j in alphabet:
                for k in alphabet:
                    lines.append(i+j+k)

    for plate in lines:
        url = urlfront + plate + urlback
        response = requests.get(url)
        

        if "This plate number is currently available." not in str(response.content):
                print(colored("PLATE UNAVAILABLE: " + url[74:len(url)-35], "red"))

        else:
            print(colored("PLATE AVAILABLE: " + url[74:len(url)-35], "green"))
            f = open("outputresults.txt", "a")
            f.write(url[74:len(url)-35] + "\n")
            f.close()


# class Worker(multiprocessing.Process):
    
#     def __init__(self, job_queue):
#         super().__init__()
#         self._job_queue = job_queue

#     def run(self) -> None:
#         while True:
#             url = self._job_queue.get()
#             if url is None:
#                 break

#             request = requests.get(url)
#             if "This plate number is currently available." not in str(request):
#                 print(colored("PLATE UNAVAILABLE: " + url[74:len(url)-35], "red"))

#             else:
#                 print(colored("PLATE AVAILABLE: " + url[74:len(url)-35], "green"))
#                 f = open("outputresults.txt", "a")
#                 f.write(url[74:len(url)-35] + "\n")
#                 f.close()

        # url = "https://raw.githubusercontent.com/averywit/LicensePlateChecker/main/3letterwords.txt"
        # r = requests.get(url)
        # for line in r.iter_lines():
        #     print(line)



