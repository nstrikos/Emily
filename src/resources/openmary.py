from synthDriverHandler import SynthDriver
import synthDriverHandler
from collections import OrderedDict
import speech
import os
import os.path
from logHandler import log
import synthDrivers
import codecs
import threading
import string
import socket
import sys
import time
import subprocess

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
            self.socket.settimeout(100000000)
            self.host = 'localhost';
            self.port = 57118;
	 
			
            try:
                self.socket.connect((self.host , self.port))
            except socket.error, ex:
                totalDelay = 0.0
                delay = 0.25
                while totalDelay < 250000000.0:
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
	filepath = "c:/Program Files/Emily/emily.exe"
	if os.path.isfile(filepath):
		p = subprocess.Popen("c:/Program Files/Emily/emily.exe", cwd="c:/Program Files/Emily")
	else:
		p = subprocess.Popen("c:/Program Files (x86)/Emily/emily.exe", cwd="c:/Program Files (x86)/Emily")				
	
        self.voice = 'hercules - greek'
        self.myVoice = 'hercules - greek'
        self.voices = ('hercules - greek',)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-emily-v2.0.1-hmm-5.1-SNAPSHOT.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('emily - greek')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-emily-v2.0.1-hmm-5.1-SNAPSHOT.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('emily - greek')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-dfki-spike-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('spike')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-dfki-spike-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('spike')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-cmu-rms-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('rms')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-cmu-rms-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('rms')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-cmu-slt-hsmm-5.1.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('slt')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-cmu-slt-hsmm-5.1.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('slt')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-cori-2740-hsmm-5.2.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('cori - experimental')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-cori-2740-hsmm-5.2.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('cori - experimental')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-dfki-obadiah-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('obadiah')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-dfki-obadiah-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('obadiah')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-dfki-prudence-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('prudence')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-dfki-prudence-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('prudence')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-dfki-poppy-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('poppy')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-dfki-poppy-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('poppy')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-cmu-bdl-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('bdl')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-cmu-bdl-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('bdl')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-dfki-pavoque-neutral-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('pavoque - german')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-dfki-pavoque-neutral-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('pavoque - german')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-dfki-ot-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('ot - turkish ')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-dfki-ot-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('ot - turkish ')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-istc-lucia-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('lucia - italian')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-istc-lucia-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('lucia - italian')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "C:/Program Files/Emily/marytts/lib/voice-upmc-pierre-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('pierre - french')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "C:/Program Files (x86)/Emily/marytts/lib/voice-upmc-pierre-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('pierre - french')
            self.voices = self.voices + (self.tmpVoice,)
        filepath = "c:/Program Files/Emily/marytts/lib/voice-cmu-nk-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('nk - telugu')
            self.voices = self.voices + (self.tmpVoice,)
	filepath = "c:/Program Files (x86)/Emily/marytts/lib/voice-cmu-nk-hsmm-5.1.jar"
        if os.path.isfile(filepath):
            self.tmpVoice = ('nk - telugu')
            self.voices = self.voices + (self.tmpVoice,)

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
    myVoice = 'hercules - greek'
    voices = ('hercules - greek')

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
        return self.myVoice

    def _getAvailableVoices(self):
        o = OrderedDict()
        for v in self.voices:
        	o[v] = synthDriverHandler.VoiceInfo(v, v)
                if o[v] == None:
                        o[v] = 'hercules - greek'
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

    def _set_voice(self, value):
        print(value)
        if value not in self.voices:
            value = 'hercules - greek'
        if (self.myVoice == 'None'):
            self.myVoice = 'hercules - greek'
            value = 'hercules - greek'
        if (self.myVoice != value):
            self.myVoice = value
            self.sendCommand.write("Voice " + str(self.myVoice))

    def terminate(self):
	self.sendCommand.write("Quit")
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
