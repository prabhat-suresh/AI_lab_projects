from collections import Counter
import csv
import re

def pr_word_spam(word,spam_words_dict,spam_words,spam_words_len,non_spam_words_len):
    if word.lower() in spam_words:
        return spam_words_dict[word.lower()]/spam_words_len
    return 1/(spam_words_len+non_spam_words_len)

def pr_word_nonspam(word,non_spam_words_dict,non_spam_words,spam_words_len,non_spam_words_len):
    if word.lower() in non_spam_words:
        return non_spam_words_dict[word.lower()]/non_spam_words_len
    return 1/(spam_words_len+non_spam_words_len)

def spam_classifier(message,spam_words_dict,spam_words,spam_words_len,non_spam_words_dict,non_spam_words,non_spam_words_len):
    pr_msg_spam,pr_msg_non_spam=1,1
    words=message.lower().split()
    for word in words:
        pr_msg_spam*=pr_word_spam(word,spam_words_dict,spam_words,spam_words_len,non_spam_words_len)
        pr_msg_non_spam*=pr_word_nonspam(word,non_spam_words_dict,non_spam_words,spam_words_len,non_spam_words_len)

    pr_spam=spam_words_len/(spam_words_len+non_spam_words_len)
    
    if pr_spam*pr_msg_spam>(1-pr_spam)*pr_msg_non_spam:
        return True     #return True if spam and False if non_spam
    return False

spam_words,non_spam_words=[],[]

with open('sms.csv','r') as file:
    csv_reader=csv.reader(file,delimiter='\t')
    for row in csv_reader:
        if row[0]=='spam':
            spam_words.extend(re.sub("[^A-Z]","", row[1].lower(),0,re.IGNORECASE).split())
        else:
            non_spam_words.extend(re.sub("[^A-Z]","", row[1].lower(),0,re.IGNORECASE).split())

remove_words=["a","an","the",'as','at','by','in','of','on','to','up','bar','for','off','per','pro','via','amid','anti','atop','down','from','into','less','like','near','onto','over','past','plus','save','than','till','upon','eith']
for word in remove_words:
    try:
        spam_words.remove(word)
    except:
        pass

    try:
        non_spam_words.remove(word)
    except:
        pass

spam_words_dict=Counter(spam_words)
non_spam_words_dict=Counter(non_spam_words)
spam_words_len=len(spam_words)
non_spam_words_len=len(non_spam_words)

if spam_classifier("I'm really not up to it still tonight babe",spam_words_dict,spam_words,spam_words_len,non_spam_words_dict,non_spam_words,non_spam_words_len):
    print("spam")
else:
    print("not spam")
