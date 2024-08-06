from keras.datasets import mnist

import numpy as np


# [0, 1]
def cosSimPrepr(Asqr,A, B):
    #28x28
    Bsqr = 0
    ABProd = 0
    for x in range(28*28):
        ABProd = ABProd + A[x] * B[x]
        Bsqr = Bsqr + B[x] ** 2
        
    return 1 - ABProd / ((Asqr ** 0.5) * (Bsqr ** 0.5))

def getNeighborIndecies(data, origin, eps):
    OrASqrt = 0
    for x in range(28*28):
        OrASqrt = OrASqrt + data[origin][x] ** 2

    neighs = []
    for point in range(len(data)):
        if point != origin:
            if cosSimPrepr(OrASqrt,data[origin],data[point]) < eps:
                neighs.append(point)

    return neighs

def DBSCAN(data, eps, minPoints):
    #x * 28x28
    c = 0
    label = [0 for i in range(len(data))]
    for point in range(len(data)):
        if label[point] != 0:
            continue
        neighs = getNeighborIndecies(data,point,eps)
        if len(neighs) < minPoints:
            label[point] = -1
            continue
        c = c + 1
        label[point] = c
        i = 0
        while i < len(neighs):
            neigh = neighs[i]
            if label[neigh] == -1:
                label[neigh] = c
            if label[neigh] != 0:
                i = i + 1
                continue

            label[neigh] = c
            neighNeighs = getNeighborIndecies(data,neigh,eps)
            if len(neighNeighs) >= minPoints:
                for index in neighNeighs:
                    if index not in neighs:
                        neighs.append(index)
            
            i = i + 1
    return label
                

            



(x_train,y_train),(x_test,y_test)=mnist.load_data()

x_train_rs=np.reshape(x_train,(60000,784))

# Standaryzacja
x_mean=np.mean(x_train_rs)
x_std=np.std(x_train_rs)
x_train_std=(x_train_rs-x_mean)/x_std

# Zmienne
epsRange = np.arange(0.295,0.3,0.005)
minPointsRange = range(6,7)

dp = 200

def most_frequent(List):
    unique, counts = np.unique(List, return_counts=True)
    index = np.argmax(counts)
    return unique[index]

for eps in epsRange:
    for minPoints in minPointsRange:
        cats = DBSCAN(x_train_std[0:dp],eps,minPoints)
        results = [[] for i in range(max(cats)+1)]
        orphans = 0
        for i in range(len(cats)):
            if cats[i] != -1:
                results[cats[i]].append(y_train[i])
            else:
                orphans = orphans +1

        print("MinPoints:",minPoints,"Eps:",eps)
        print("Categories:",max(cats),"Orphans:",orphans)
        matches = 0.0
        els = 0.0
        for line in results:
            if len(line) == 0:
                continue
            print(line)
            comm = most_frequent(line)
            for el in line:
                els = els + 1
                if el == comm:
                    matches = matches + 1
        print(">> Matched",matches/dp,"<<")
        print(">>     Acc",matches/els,"<<")
        print(results)
