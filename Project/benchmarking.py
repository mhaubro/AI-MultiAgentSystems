import glob, os, zipfile, datetime, shutil, sys

def didSolve(filename):
    with open(filename) as f:
        lines = f.readlines()
        for i in range(0, len(lines)):
            line = lines[i]            
            if 'unsuccessful' in line:
                continue
            if 'successful' in line:
                return [lines[i+1].rstrip("\n\r"), lines[i+2].rstrip("\n\r")]
    return None

def average(list):
    sum = 0
    for n in list:
        sum += n
    return int(sum/len(list))

if __name__ == "__main__":
    if len(sys.argv) > 0:
        output = sys.argv[1]
    else:
        output = 'output.out'

    path = os.getcwd()
    solves = 0
    time = []
    actions = []

    with zipfile.ZipFile(os.path.join(path, output),"r") as zip_ref:
        zip_ref.extractall("results")

    for f in glob.glob(os.path.join(path, 'results/*.log')):
        ret = didSolve(f)
        if(didSolve(f)):
            print("Solved: " + os.path.basename(f).replace(".log", "") + " in " + ret[0] + " milliseconds and " + ret[1] + " actions")
            time.append(int(ret[0]))
            actions.append(int(ret[1]))
            solves += 1
    print("\n\n")
    print("======================================================")
    print("Summary - solved " +  str(solves) + " levels in total!")
    print("Average of " + str(average(time)) + " milliseconds and " + str(average(actions)) + " actions")
    print("======================================================")
    shutil.rmtree("results")
        
