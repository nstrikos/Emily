from synthDriverHandler import SynthDriver
import synthDriverHandler
from collections import OrderedDict
import speech
import os
from logHandler import log
import synthDrivers
import codecs
import threading
import string
import socket
import sys
import time

class SynthDriver(SynthDriver):

    class myThread (threading.Thread):
        def __init__(self, threadID, name, myList):
            threading.Thread.__init__(self)
            self.threadID = threadID
            self.name = name
            self.myList = myList
            self.timeToDie = False
            self.readData = ""
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(None)
            self.host = 'localhost';
            self.port = 57121;
            #self.server_address = ('localhost', 57000)
            #self.socket.bind(self.server_address)
            try:
                self.socket.connect((self.host , self.port))
            except socket.error, ex:
                totalDelay = 0.0
                delay = 0.25
                while totalDelay < 25.0:
                    try:
                        self.socket.connect((self.host, self.port))
                        #self.initServer()
                        #self.log(DEBUG_MESSAGE, "Connect succeeded")
                        return
                    except socket.error, ex:
                        time.sleep(delay)
                        totalDelay = totalDelay + delay
                #self.log(DEBUG_MESSAGE, "Connect failed\n")
                raise IOError
            #self.socket.settimeout(2)

            #self.socket.listen(1)


        def run(self):
            while not self.timeToDie:
                #user_profile = os.environ['USERPROFILE']
                #user_temp3 = user_profile + '/AppData/Local/Temp/emily2.txt'
                #file = open(user_temp3, 'r')
                #line = file.read()
                #line.decode('utf-8')
                #file.close()
                #line.replace(" ", "")
                #if line != "":
                    #self.myList[0] = int(float(line))
                    #if self.myList[0] > 0:
                    #    log.info("Read Index: %d"%self.myList[0])
                #self.socket.listen(1)
                #connection, client_address = self.socket.accept()
                #self.readData = connection.recv(1024)
                #try:
                reply = self.socket.recv(4)
                if len(reply) > 0:
                    if reply != "":
                        self.myList[0] = int(float(reply))
                        #log.info("Read Index: %d"%self.myList[0])


        def quit(self):
            #self.socket.settimeout(2)
            #self.socket(socket.AF_INET,socket.SOCK_STREAM).connect( (self.host, self.port))
            self.socket.shutdown(socket.SHUT_RDWR)
            self.socket.close()
            self.timeToDie = True

    class sendText:
        def __init__(self):
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(None)
            self.socket = socket.create_connection(('localhost', 57116))

        def write(self, data):
            self.socket.sendall(data.encode('utf-8'))

        def quit(self):
            self.sock.close()

    class sendIndex():
        def __init__(self):
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(None)
            self.socket = socket.create_connection(('localhost', 57117))

        def write(self, data):
            self.socket.sendall(data.encode('utf-8'))

        def quit(self):
            self.sock.close()

    class sendCommand():
        def __init__(self):
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket = socket.create_connection(('localhost', 57118))

        def write(self, data):
            self.socket.sendall(data.encode('utf-8'))

        def quit(self):
            self.sock.close()

    def __init__(self):
        self.voice = 'emily-16-hmm-norm'
        self.curvoice = 'emily-16-hmm-norm'
        self.index = 0
        self.indexList = [0]
        self.thread = SynthDriver.myThread(1, "Thread", self.indexList)
        self.thread.timeToDie = False
        self.thread.start()
        self.sendText = SynthDriver.sendText()
        self.sendIndex = SynthDriver.sendIndex()
        self.sendCommand = SynthDriver.sendCommand()


    name = "openmary"
    description = "Emily - Open Mary"
    supportedSettings = (SynthDriver.RateSetting(), SynthDriver.VoiceSetting())
    voices = ('emily-16-hmm-norm','emily-16-hmm-norm')
    #index = 0;
    #indexList = [0]
    #thread = myThread(1, "Thread", indexList)
    #myWriteLine = writeLine()
    #myWriteLine2 = SynthDriver.writeLine2()
    #thread.start()

    @classmethod
    def check(cls):
        return True



    def speak(self, speechSequence):
        text = ""
        for item in speechSequence:
            if isinstance(item, basestring):
                text = text + item
            elif isinstance(item, speech.IndexCommand):
                self.index=item.index
                #text = text + "(NVDA Index)" + str(self.index) + "#"

        if text:
            self.sendIndex.write("Index:" + str(self.index))
            text = "(NVDA Index)" + str(self.index) + "#" + text
            self.sendText.write(text)
            #self.index = 0

    def _get_voice(self):
        return self.curvoice

    def _getAvailableVoices(self):
        o = OrderedDict()
        for v in self.voices:
            o[v] = synthDriverHandler.VoiceInfo(v, v)
        return o

    def _get_lastIndex(self):
        #log.info("Client checked for last index mark (%s)"%self.indexList[0])
        return self.indexList[0]


    def cancel(self):
        #user_profile = os.environ['USERPROFILE']
        #user_temp4 = user_profile + '/AppData/Local/Temp/emily4.txt'
        #file1 = open(user_temp4, 'w')
        #file1.write("Cancel")
        #file1.close
        self.sendCommand.write("Cancel")

    def pause(self, switch):
        if switch:
            self.sendCommand.write("Pause")
        else:
            self.sendCommand.write("Start")


    def terminate(self):
        self.thread.quit()
        self.thread.join()
        self.stopServer()


    def stopServer(self):
        try:
            self.sendText.socket.shutdown(socket.SHUT_RDWR)
            self.sendIndex.socket.shutdown(socket.SHUT_RDWR)
            #self.thread.socket.shutdown(socket.SHUT_RDWR)
        except socket.error:
            pass
        self.sendText.socket.close()
        self.sendIndex.socket.close()
        #self.thread.socket.close()
