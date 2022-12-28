# TODO
import math

text = input("Text: ")

letters = 0
words = 0
sentences = 0

for i in range(len(text)):
    if text[i].isalpha():
        letters += 1
    if text[i] == ' ':
        words += 1
    if text[i] == '!' or text[i] == '.' or text[i] == '?':
        sentences += 1
words += 1
#print(letters, words, sentences)

l = 100 * letters / words
s = 100 * sentences / words

index = round(0.0588 * l - 0.296 * s - 15.8)

if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")