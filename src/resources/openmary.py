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
            self.port = 57118;
            try:
                self.socket.connect((self.host , self.port))
            except socket.error, ex:
                totalDelay = 0.0
                delay = 0.25
                while totalDelay < 25.0:
                    try:
                        self.socket.connect((self.host, self.port))
                        return
                    except socket.error, ex:
                        time.sleep(delay)
                        totalDelay = totalDelay + delay
                raise IOError

        def run(self):
            while not self.timeToDie:
                reply = self.socket.recv(4)
                if len(reply) > 0:
                    if reply != "":
                        self.myList[0] = int(float(reply))


        def quit(self):
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

    class sendCommand():
        def __init__(self):
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(None)
            self.socket = socket.create_connection(('localhost', 57117))

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
        self.sendCommand = SynthDriver.sendCommand()
        self.myRate = 50


    name = "openmary"
    description = "Emily - Open Mary"
    supportedSettings = (SynthDriver.RateSetting(), SynthDriver.VoiceSetting())
    voices = ('emily-16-hmm-norm','emily-16-hmm-norm')

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

        if text:
            text = "(NVDA Index)" + str(self.index) + "#" + text
            self.sendText.write(text)

    def _get_voice(self):
        return self.curvoice

    def _getAvailableVoices(self):
        o = OrderedDict()
        for v in self.voices:
            o[v] = synthDriverHandler.VoiceInfo(v, v)
        return o

    def _get_lastIndex(self):
        return self.indexList[0]


    def cancel(self):
        self.sendCommand.write("Cancel")

    def pause(self, switch):
        if switch:
            self.sendCommand.write("Pause")
        else:
            self.sendCommand.write("Start")

    def _set_rate(self, rate):
        self.myRate = rate
        self.sendCommand.write("Rate " + str(self.myRate))

    def _get_rate(self):
        return self.myRate


    def terminate(self):
        self.thread.quit()
        self.thread.join()
        self.stopServer()


    def stopServer(self):
        try:
            self.sendText.socket.shutdown(socket.SHUT_RDWR)
            self.sendCommand.socket.shutdown(socket.SHUT_RDWR)
        except socket.error:
            pass
        self.sendText.socket.close()
        self.sendCommand.socket.close()
