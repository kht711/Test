import sys
import datetime

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

f = open("result.txt", 'w')
f.write(dt_now.strftime('%H:%M:%S, START!\n'))
f.close()

depth = []

debug = False
seek = 0
seekLimit = 50

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

def seekTile(seek, moved):
    global puzzle
    global debug
    global debug2
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
                                
                            sys.exit(1)

                        ##############################
                        if seek >= seekLimit:
                            dt_now = datetime.datetime.now()
                            endTile = depth[-1]
                            rate = 0
                            for row in range(1, 4):
                                for col in range(0, 4):
                                    if endTile[row][col] == sol[row][col]:
                                        rate += 1
                            #if rate > 6:
                            #    print(dt_now.strftime('%H:%M:%S'), end=" ")
                            #    print("Correctly:", rate)
                            
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
                                
                            sys.exit(1)

                        ############################
                        if seek >= seekLimit:
                            dt_now = datetime.datetime.now()
                            endTile = depth[-1]
                            rate = 0
                            for row in range(1, 4):
                                for col in range(0, 4):
                                    if endTile[row][col] == sol[row][col]:
                                        rate += 1
                            #if rate > 6:
                            #    print(dt_now.strftime('%H:%M:%S'), end=" ")
                            #    print("Correctly:", rate)
                                
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
                                
                            sys.exit(1)

                        ##############################
                        if seek >= seekLimit:
                            dt_now = datetime.datetime.now()
                            endTile = depth[-1]
                            rate = 0
                            for row in range(1, 4):
                                for col in range(0, 4):
                                    if endTile[row][col] == sol[row][col]:
                                        rate += 1
                            #if rate > 6:
                            #    print(dt_now.strftime('%H:%M:%S'), end=" ")
                            #    print("Correctly:", rate)
                                
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
                                
                            sys.exit(1)

                        ###############################
                        if seek >= seekLimit:
                            dt_now = datetime.datetime.now()
                            endTile = depth[-1]
                            rate = 0
                            for row in range(1, 4):
                                for col in range(0, 4):
                                    if endTile[row][col] == sol[row][col]:
                                        rate += 1
                            #if rate > 6:
                            #    print(dt_now.strftime('%H:%M:%S'), end=" ")
                            #    print("Correctly:", rate)
                                
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
