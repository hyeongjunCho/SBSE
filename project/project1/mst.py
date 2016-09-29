from math import *
import sys

def distance(v1, v2):
    return (sqrt(pow((v1[0]-v2[0]),2) + pow((v1[1]-v2[1]),2)))

def read_file(filename):
    f = open(filename, 'r')
    f.readline()
    f.readline()
    f.readline()
    num_vertices = int(f.readline().split()[2])
    f.readline()
    f.readline()
    #print (num_vertices)

    vertices=[]
    edges=[]
    for i in range(num_vertices):
        temp = list(map(str, f.readline().split()))
        vertices.append((float(temp[1]), float(temp[2])))
    for i in range(num_vertices):
        edges.append([])
        for j in range(num_vertices):
            if i < j:
                break
            else:
                edges[i].append(distance(vertices[i],vertices[j]))
    #print (len(vertices))
    return num_vertices, vertices, edges

def mst(vertices):
    pass


if __name__ == "__main__":
    global num_vertices, vertices, edges
    num_vertices, vertices, edges = read_file(sys.argv[1])

print (num_vertices)
print (len(edges))
print (edges[5000][3000])
print (edges[11848][11847])
