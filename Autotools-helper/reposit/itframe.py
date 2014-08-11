##
# @file itframe.py
# @comment
# integrated test framework
# 
# @author niexw
# @email xiaowen.nie.cn@gmail.com
#
import pexpect
import re

def red(str):
    return '\033[31;49m'+str+'\033[0m'
def green(str):
    return '\033[32;49m'+str+'\033[0m'
def cyan(str):
    return '\033[36;49m'+str+'\033[0m'

class TestCmd(object):
    'the object of command line'
    def __init__(self, p='[#\$]'):
        self.command = ''
        self.outputs = ''
        self.prompt = p
        
    def setprompt(self, p):
        self.prompt = p 
  
    def wait(self, p=''):
        if p == '': p = self.prompt
        while True:
            index = self.child.expect([p, pexpect.EOF, pexpect.TIMEOUT])
            if index == 0 or index == 1: 
                self.outputs = self.child.before
                break
            elif index == 2: 
                pass

    def spawn(self, cmd):
        self.command = cmd
        self.child = pexpect.spawn(cmd)

    def sendline(self, l):
        self.child.sendline(l)

    def sendCtrl(self, c):
        self.child.sendcontrol(c)
    
    def search(self, t):
        group = re.search(t, self.outputs)
        if group is not None:
            return True
        else:
            print(red('error: expect(%s)' % t))
            print(self.outputs)
            return False

    def nsearch(self, f):
        group = re.search(f, self.outputs)
        if group is not None:
            print(red('error: nexpect(%s)' % f))
            print(self.outputs)
            return False
        else:
            return True

    def interact(self):
        self.child.interact()

class TestCase(object):
    'the base class of test cases'
    ok = '[O]'
    fail = '[X]'
    def __init__(self, d):
        self.description = d
        self.cases = []
        self.total = 0
        self.error = 0

    def __stretch(self, left, right):
        return 80-len(left)-len(right)

    def addcase(self, case):
        self.cases.append(case)

    def run(self):
        for case in self.cases:
            ret = case()
            self.total += 1
            desc = self.description + ' : ' + case.func_name
            if ret == True:
                num = self.__stretch(desc, self.ok)
                print(desc + '.'*num + green(self.ok))
            else:
                self.error += 1
                num = self.__stretch(desc, self.fail)
                print(desc + '.'*num + red(self.fail))
        return (self.total, self.error)

class TestSuite(object):
    'the test suite base class'
    def __init__(self, d):
        self.description = d
        self.suites = []
        self.total = 0
        self.error = 0
    
    def addsuite(self, s):
        self.suites.append(s)

    def run(self):
        for suite in self.suites:
            (a, b) = suite.run()
            self.total += a
            self.error += b
        return (self.total, self.error)

    def begin(self):
        print(self.description)
    def conclude(self):
        print('Running total %d test case, %d error detected!' 
        		% (self.total, self.error))
