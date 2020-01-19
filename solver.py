# -*- coding: utf-8
#!/usr/bin/python

import RPi.GPIO as GPIO
import sys
import datetime

seekLimit = 20

GPIO.setmode(GPIO.BCM)
GPIO.setup(17, GPIO.OUT)

GPIO.output(17, 0);

puzzle = [
    [0, -1, -1, -1],
    [1, 2, 3, 4],
    [5, 6, 7, 8],
    [9, 10, 11, 12],
    [-1, -1, -1, 0]
    ]

sol = [
    [0, -1, -1, -1],
    [12, 11, 10, 9],
    [8, 7, 6, 5],
    [4, 3, 2, 1],
    [-1, -1, -1, 0]
    ]

dt_now = datetime.datetime.now()
print(dt_now.strftime('%H:%M:%S, START!'))
GPIO.output(17, 1);

f = open("result.txt", 'w')
f.write(dt_now.strftime('%H:%M:%S, START!\n'))
f.close()

depth = []
debug = False
seek = 0
minDist = 50
maxCor = 0

def printPuzzle(puzzle, file=False):
    for i in range(len(puzzle)):
        for j in range(len(puzzle[0])):
            print(puzzle[i][j], end=" ")
        print("")

    if file:
        f = open("result.txt", 'a')
        for i in range(len(puzzle)):
            for j in range(len(puzzle[0])):
                f.write(str(puzzle[i][j]))
                f.write(" ")
            f.write("\n")
        f.write("\n")
        f.close()
    return

def minWrite(puzzle):
    f = open("min.txt", 'a')
    for i in range(len(puzzle)):
        for j in range(len(puzzle[0])):
            f.write(str(puzzle[i][j]))
            f.write(" ")
        f.write("\n")
    f.write("\n")
    f.close()
    return

def distance(puzzle):
    global sol
    dist = 0
    solxy = [
        [3, 3],
        [3, 2],
        [3, 1],
        [3, 0],
        [2, 3],
        [2, 2],
        [2, 1],
        [2, 0],
        [1, 3],
        [1, 2],
        [1, 1],
        [1, 0]
        ]
    
    for row in range(len(puzzle)):
        for col in range(len(puzzle[0])):
            num = puzzle[row][col]
            if num > 0:
                dist += (abs(row-solxy[num-1][0]) + abs(col-solxy[num-1][1]))
    return dist

def correct(puzzle):
    global sol
    cor = 0
    for row in range(len(puzzle)):
        for col in range(len(puzzle[0])):
            if puzzle[row][col] == sol[row][col]:
                cor += 1
    return cor
                
    

def seekTile(seek, moved):
    global puzzle
    global debug
    global minDist
    global maxCor
    global seekLimit
    global depth
    global sol

    intoFor = 0
    
    for i in range(len(puzzle)):
        for j in range(len(puzzle[0])):
            if puzzle[i][j] == 0:
                intoFor += 1

                ###################
                if debug:
                    print("intoFor", intoFor)
                    print("i, j = ", i, j)
                #up tile -> down
                if i-1 >= 0 and puzzle[i][j] == 0 and puzzle[i-1][j] > 0 and moved != "up":
                    moved = "down"
                    puzzle[i][j] = puzzle[i-1][j]
                    puzzle[i-1][j] = 0

                    if not puzzle in depth:
                        seek += 1
                        
                        temp = [
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1]
                            ]
                        for row in range(len(puzzle)):
                            for col in range(len(puzzle[0])):
                                temp[row][col] = puzzle[row][col]
                                
                        depth.append(temp)
                        if debug:
                            print("insert 1:")

                        if temp == sol:
                            print("find!")
                            f = open("result.txt", 'a')
                            f.write("find!\n")
                            f.close()
                            for d in depth:
                                printPuzzle(d)
                                print("")
                                
                            for d in depth:
                                printPuzzle(d, True)
                            
                            GPIO.output(17, 0);
                            GPIO.cleanup()
                            sys.exit(1)

                        ##############################
                        if seek >= seekLimit:
                            dt_now = datetime.datetime.now()
                            endTile = depth[-1]
                            
                            dist = distance(endTile)
                            if dist < minDist:
                                f = open("min.txt", "w")
                                f.write(str(dist))
                                f.write("\n")
                                f.close()
                                
                                for d in depth:
                                    minWrite(d)
                                minDist = dist
                            elif dist == minDist:
                                cor = correct(endTile)
                                if cor > maxCor:
                                    f = open("min.txt", "w")
                                    f.write(str(dist))
                                    f.write(", ")
                                    f.write(str(cor))
                                    f.write("\n")
                                    f.close()

                                    for d in depth:
                                        minWrite(d)
                                    maxCor = cor
                                elif cor == maxCor:
                                    f = open("min.txt", "a")
                                    f.write(str(dist))
                                    f.write(", ")
                                    f.write(str(cor))
                                    f.write("\n")
                                    f.close()

                                    for d in depth:
                                        minWrite(d)
                                
                            seek -= 1
                            puzzle[i-1][j] = puzzle[i][j]
                            puzzle[i][j] = 0
                            depth.pop()
                            ###################
                            if len(depth) == 0:
                                moved = ""
                            ###################
                            if debug:
                                print("Depth Pop!", len(depth))
                        else:
                            seekTile(seek, moved)
                            
                            seek -= 1
                            puzzle[i-1][j] = puzzle[i][j]
                            puzzle[i][j] = 0
                            depth.pop()
                            ###################
                            if len(depth) == 0:
                                moved = ""
                            ###################
                            if debug:
                                print("depth 1 return! Depth Pop!")
                                #input()
                    else:
                        if debug:
                            print("dup 1!")
                            
                        puzzle[i-1][j] = puzzle[i][j]
                        puzzle[i][j] = 0
                else:
                    if debug:
                        print("No 1")

                #left tile -> right
                if j-1 >= 0 and puzzle[i][j] == 0 and puzzle[i][j-1] > 0 and moved != "left":
                    moved = "right"
                    puzzle[i][j] = puzzle[i][j-1]
                    puzzle[i][j-1] = 0

                    if not puzzle in depth:
                        seek += 1
                        
                        temp = [
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1]
                            ]
                        for row in range(len(puzzle)):
                            for col in range(len(puzzle[0])):
                                temp[row][col] = puzzle[row][col]
                                
                        depth.append(temp)
                        if debug:
                            print("insert 2:")

                        if temp == sol:
                            print("find!")
                            f = open("result.txt", 'a')
                            f.write("find!\n")
                            f.close()
                            for d in depth:
                                printPuzzle(d)
                                print("")
                                
                            for d in depth:
                                printPuzzle(d, True)

                            GPIO.output(17, 0);
                            GPIO.cleanup()
                            sys.exit(1)

                        ############################
                        if seek >= seekLimit:
                            dt_now = datetime.datetime.now()
                            endTile = depth[-1]
                            
                            dist = distance(endTile)
                            if dist < minDist:
                                f = open("min.txt", "w")
                                f.write(str(dist))
                                f.write("\n")
                                f.close()
                                
                                for d in depth:
                                    minWrite(d)
                                minDist = dist
                            elif dist == minDist:
                                cor = correct(endTile)
                                if cor > maxCor:
                                    f = open("min.txt", "w")
                                    f.write(str(dist))
                                    f.write(", ")
                                    f.write(str(cor))
                                    f.write("\n")
                                    f.close()

                                    for d in depth:
                                        minWrite(d)
                                    maxCor = cor
                                elif cor == maxCor:
                                    f = open("min.txt", "a")
                                    f.write(str(dist))
                                    f.write(", ")
                                    f.write(str(cor))
                                    f.write("\n")
                                    f.close()

                                    for d in depth:
                                        minWrite(d)
                                
                            seek -= 1
                            puzzle[i][j-1] = puzzle[i][j]
                            puzzle[i][j] = 0
                            depth.pop()
                            ###################
                            if len(depth) == 0:
                                moved = ""
                            ###################
                            if debug:
                                print("Depth Pop!", len(depth))
                        else:
                            seekTile(seek, moved)

                            seek -= 1
                            puzzle[i][j-1] = puzzle[i][j]
                            puzzle[i][j] = 0
                            depth.pop()
                            ###################
                            if len(depth) == 0:
                                moved = ""
                            ###################
                            if debug:
                                print("depth 2 return! Depth Pop!")
                                #input()
                    else:
                        if debug:
                            print("dup 2!")
                        puzzle[i][j-1] = puzzle[i][j]
                        puzzle[i][j] = 0
                else:
                    if debug:
                        print("No 2")

                #down tile -> up
                if i+1 <= len(puzzle)-1 and puzzle[i][j] == 0 and puzzle[i+1][j] > 0 and moved != "down":
                    moved = "up"
                    puzzle[i][j] = puzzle[i+1][j]
                    puzzle[i+1][j] = 0

                    if not puzzle in depth:
                        seek += 1
                        
                        temp = [
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1]
                            ]
                        for row in range(len(puzzle)):
                            for col in range(len(puzzle[0])):
                                temp[row][col] = puzzle[row][col]
                                
                        depth.append(temp)
                        if debug:
                            print("insert 3:")
                        else:
                            pass

                        if temp == sol:
                            print("find!")
                            f = open("result.txt", 'a')
                            f.write("find!\n")
                            f.close()
                            for d in depth:
                                printPuzzle(d)
                                print("")
                                
                            for d in depth:
                                printPuzzle(d, True)

                            GPIO.output(17, 0);
                            GPIO.cleanup()
                            sys.exit(1)

                        ##############################
                        if seek >= seekLimit:
                            dt_now = datetime.datetime.now()
                            endTile = depth[-1]
                            
                            dist = distance(endTile)
                            if dist < minDist:
                                f = open("min.txt", "w")
                                f.write(str(dist))
                                f.write("\n")
                                f.close()
                                
                                for d in depth:
                                    minWrite(d)
                                minDist = dist
                            elif dist == minDist:
                                cor = correct(endTile)
                                if cor > maxCor:
                                    f = open("min.txt", "w")
                                    f.write(str(dist))
                                    f.write(", ")
                                    f.write(str(cor))
                                    f.write("\n")
                                    f.close()

                                    for d in depth:
                                        minWrite(d)
                                    maxCor = cor
                                elif cor == maxCor:
                                    f = open("min.txt", "a")
                                    f.write(str(dist))
                                    f.write(", ")
                                    f.write(str(cor))
                                    f.write("\n")
                                    f.close()

                                    for d in depth:
                                        minWrite(d)
                                
                            seek -= 1
                            puzzle[i+1][j] = puzzle[i][j]
                            puzzle[i][j] = 0
                            depth.pop()
                            ###################
                            if len(depth) == 0:
                                moved = ""
                            ###################
                            if debug:
                                print("Depth Pop!", len(depth))
                        else:
                            seekTile(seek, moved)

                            seek -= 1
                            puzzle[i+1][j] = puzzle[i][j]
                            puzzle[i][j] = 0
                            depth.pop()
                            ###################
                            if len(depth) == 0:
                                moved = ""
                            ###################
                            if debug:
                                print("depth 3 return! Depth Pop!")
                                #input()
                    else:
                        if debug:
                            print("dup 3!")
                        puzzle[i+1][j] = puzzle[i][j]
                        puzzle[i][j] = 0
                else:
                    if debug:
                        print("No 3")

                #right tile -> left
                if j+1 <= len(puzzle[0])-1 and puzzle[i][j] == 0 and puzzle[i][j+1] > 0 and moved != "right":
                    moved = "left"
                    puzzle[i][j] = puzzle[i][j+1]
                    puzzle[i][j+1] = 0

                    if not puzzle in depth:
                        seek += 1

                        temp = [
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1],
                            [-1, -1, -1, -1]
                            ]
                        for row in range(len(puzzle)):
                            for col in range(len(puzzle[0])):
                                temp[row][col] = puzzle[row][col]
                                
                        depth.append(temp)
                        if debug:
                            print("insert 4:")
                        else:
                            pass

                        if temp == sol:
                            print("find!")
                            f = open("result.txt", 'a')
                            f.write("find!\n")
                            f.close()
                            for d in depth:
                                printPuzzle(d)
                                print("")
                                
                            for d in depth:
                                printPuzzle(d, True)

                            GPIO.output(17, 0);
                            GPIO.cleanup()
                            sys.exit(1)

                        ###############################
                        if seek >= seekLimit:
                            dt_now = datetime.datetime.now()
                            endTile = depth[-1]
                            
                            dist = distance(endTile)
                            if dist < minDist:
                                f = open("min.txt", "w")
                                f.write(str(dist))
                                f.write("\n")
                                f.close()
                                
                                for d in depth:
                                    minWrite(d)
                                minDist = dist
                            elif dist == minDist:
                                cor = correct(endTile)
                                if cor > maxCor:
                                    f = open("min.txt", "w")
                                    f.write(str(dist))
                                    f.write(", ")
                                    f.write(str(cor))
                                    f.write("\n")
                                    f.close()

                                    for d in depth:
                                        minWrite(d)
                                    maxCor = cor
                                elif cor == maxCor:
                                    f = open("min.txt", "a")
                                    f.write(str(dist))
                                    f.write(", ")
                                    f.write(str(cor))
                                    f.write("\n")
                                    f.close()

                                    for d in depth:
                                        minWrite(d)
                                
                            seek -= 1
                            puzzle[i][j+1] = puzzle[i][j]
                            puzzle[i][j] = 0
                            depth.pop()
                            ###################
                            if len(depth) == 0:
                                moved = ""
                            ###################
                            if debug:
                                print("Depth Pop!", len(depth))
                        else:
                            seekTile(seek, moved)

                            seek -= 1
                            puzzle[i][j+1] = puzzle[i][j]
                            puzzle[i][j] = 0
                            depth.pop()
                            ###################
                            if len(depth) == 0:
                                moved = ""
                            ###################
                            if debug:
                                print("depth 4 return! Depth Pop!")
                                #input()
                    else:
                        if debug:
                            print("dup 4!")
                        puzzle[i][j+1] = puzzle[i][j]
                        puzzle[i][j] = 0
                else:
                    if debug:
                        print("No 4")

        
seekTile(seek, "")
dt_now = datetime.datetime.now()
print(dt_now.strftime('%H:%M:%S, END!'))

f = open("result.txt", 'a')
f.write(dt_now.strftime('%H:%M:%S, END!\n'))
f.close()
GPIO.output(17, 0);
GPIO.cleanup()
