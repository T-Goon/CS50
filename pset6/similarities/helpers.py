from nltk.tokenize import sent_tokenize

def lines(a, b):
    """Return lines in both a and b"""

    # TODO

    linesA=a.splitlines()
    linesB=b.splitlines()
    same=list(set(linesA).intersection(set(linesB)))

    return same


def sentences(a, b):
    """Return sentences in both a and b"""

    # TODO
    listSentencesA=sent_tokenize(a)
    listSentencesB=sent_tokenize(b)
    same=list(set(listSentencesA).intersection(set(listSentencesB)))

    return same


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    # TODO
    i=n
    j=0
    listA=[]
    listB=[]
    while i<=len(a):
        listA.append(a[j:i])
        i+=1
        j+=1
    i=n
    j=0
    while i<=len(b):
        listB.append(b[j:i])
        i+=1
        j+=1
    same=list(set(listA).intersection(set(listB)))
    return same
