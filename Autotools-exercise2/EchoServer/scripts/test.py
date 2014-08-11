##
# @file test.py
# @comment
# the integrated tests of project EchoServer
#
# @author niexw
# @email xiaowen.nie.cn@gmail.com
#

from itframe import TestSuite, TestCase, TestCmd

#
# declare the test cases
#
class HelloTestCases(TestCase):
    def __init__(self, d):
        TestCase.__init__(self, d)
        super(HelloTestCases, self).addcase(self.testHello)
        super(HelloTestCases, self).addcase(self.testWorld)

    def testHello(self):
        cmd = TestCmd()
        cmd.spawn('ls -al')
        cmd.wait();
        if cmd.search('test.py') == False: return False
        if cmd.search('itframe.py') == False: return False
        return True

    def testWorld(self):
        return False

class SshTestCases(TestCase):
    def __init__(self, d):
        TestCase.__init__(self, d)
        super(SshTestCases, self).addcase(self.testLogin)

    def testLogin(self):
        cmd = TestCmd()
        cmd.spawn('ssh 192.168.75.111')
        cmd.wait('password:')
        cmd.sendline('sNipper')

        cmd1 = TestCmd()
        cmd1.spawn('ssh 192.168.75.111')
        cmd1.wait('password:')
        cmd1.sendline('sNipper')

        cmd1.setprompt('niexw@ubuntu11')
        cmd1.wait()
        cmd1.sendline('exit')
        cmd1.wait('logout')

        cmd.setprompt('niexw@ubuntu11')
        cmd.wait()
        cmd.sendline('exit')
        cmd.wait('logout')
        return True

#
# declare the main suite
#
class MainSuite(TestSuite):
    def __init__(self, d):
        TestSuite.__init__(self, d)
        super(MainSuite, self).addsuite(HelloTestCases('hello, world'))
        super(MainSuite, self).addsuite(SshTestCases('ssh test cases'))

#
# run all suites
#
suites = MainSuite('project EchoServer test suite')
suites.begin()
suites.run()
suites.conclude()

