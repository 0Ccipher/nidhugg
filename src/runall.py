
import datetime
import subprocess
import sys
import tempfile
import os
import time
from termcolor import colored,cprint

curDir = os.getcwd()
#LITMUSDIR = curDir +'/C-litmus'
LOGFILENAME = 'examples/litmus/runall-results.txt'
INPUTFILE = curDir + '/examples/litmus/litmus.txt'
def runall():
    logfile=open(LOGFILENAME,'w')
    t0=time.time()
    tests = open(INPUTFILE)
    tracecount = 0
    n=0
    for tst in tests:
        t = 0
        res = 0
        n=n+1
        tst = tst.split("\n")
        cmd = f"nidhuggc -ccv -extfun-no-race=printf examples/litmus/litmus_tests/{tst[0]}"
        try:
            output=subprocess.check_output(cmd,shell = True).decode("utf-8")
        except Exception as e:
            output = str(e.output)
        print('{0:4}: '.format(n),end='')
        print(tst[0])
        if output.find("Error: Assertion violation at ") >= 0:
            text = colored('CCV UNSAFE' , 'red')
            print(text)
            res = 0
        else:
            parts_0=output.split(":=")
            trace = parts_0[-1].split("\n");
            text = colored('CCV SAFE' , 'green')
            print(text)
            res = 1
        try:
            tracecount+=int(trace[0])
        except Exception as e:
            tracecount += 0
            t = -1
        if t == -1:
            text = colored('Assertion Violation..!' , 'red' , attrs=['reverse','blink'])
            logfile.write(tst[0]+' Assertion Violation \n')
        if res == 1 :
            text = colored('..SAFE' , 'green' , attrs=['reverse','blink'])
            logfile.write(tst[0]+' Forbid \n')
        if res == 0 :
            text = colored('Assertion Violation..!' , 'red' , attrs=['reverse','blink'])
            logfile.write(tst[0]+' Allow \n')
    runtime=time.time()-t0
    logfile.write('# '+str(tracecount)+'\n')
    logfile.write('# Time: {0:.2f} s\n'.format(runtime))
    print('# '+str(tracecount)+'\n')
    print('# Time: {0:.2f} s\n'.format(runtime))
    logfile.close()

runall()

