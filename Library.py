

import random
from queue import Queue


# FIFO algorithme
def fifo(ref, numb):

    data = list()
    fautes = 0
    queue = Queue()

    print(print_frames(numb))
    for p in ref: 
        if(len(data) < numb):
            if(not p in data):
                data.append(p)
                queue.put(p)
                fautes += 1
        else:
            if(not p in  data):
                e = queue.get()
                e_index = data.index(e)
                data.remove(e)
                queue.put(p)
                data.insert(e_index, p)
                fautes += 1
        print(print_data(data, p))

    return fautes


# LRU algorithme
def lru(ref, numb):

    data = list()
    fautes = 0
    lst = list()

    print(print_frames(numb))
    for p in ref:
        if(len(data) < numb):
            if(not p in data):
                data.append(p)
                lst.append(p)
                fautes += 1
            else:
                if(p in lst):
                    lst.remove(p)
                    lst.append(p)
        else: 
            if(not p in data):
                e = lst.pop(0)
                lst.append(p)
                e_index = data.index(e)
                data.remove(e)
                data.insert(e_index, p)
                fautes += 1
            else:
                if(p in lst): 
                    lst.remove(p)
                    lst.append(p)
                    
        print(print_data(data, p))

    return fautes


def print_frames(numb):
    c = "Pages"
    for i in range(numb):
        c += "\t\tFrame " + str(i+1)
    return c


def print_data(data, p):
    print = "P" + str(p) + " =>"
    for i in data:
        print += "\t\t" + str(i)
    return print


def cree_ref(n):
    ref = []
    for i in range(n):
        ref.append(random.randint(0,9))
    return ref


if __name__ == "__main__":

    print("\n---------------------- FIFO & LRU ----------------------\n")

    n_pages =int(input("Entre le nombre total de pages : "))
    ref = cree_ref(n_pages)
    n_frames = int(input("Entre le nombre de frames : "))

    if(not (1 < n_frames < 7)):
        print("Nombre doit être entre 1 et 7!")
        while(not (1 < n_frames < 7)):
            n_frames = int(input("Entre le nombre de frames : "))

    print("Références de page : {}\n".format(ref))
    
    print("\n------------------------- FIFO -------------------------")
    res = fifo(ref, n_frames)
    print("--------------------------------------------------------")
    print("FIFO fautes de page : {}\n".format(res))

    print("\n------------------------- LRU --------------------------")
    res = lru(ref, n_frames)
    print("--------------------------------------------------------")
    print("LRU fautes de page : {}\n".format(res))
