from synthDriverHandler import SynthDriver
import synthDriverHandler
from collections import OrderedDict
import speech
import os

class SynthDriver(SynthDriver):
 name = "openmary"
 description = "Emily - Open Mary"
 supportedSettings = (SynthDriver.RateSetting(), SynthDriver.VoiceSetting())
 voices = ('emily-16-hmm-norm','emily-16-hmm-norm')
 @classmethod
 def check(cls):
  return True
 
 def __init__(self):
  self.voice = 'emily-16-hmm-norm'
  self.curvoice = 'emily-16-hmm-norm'

 def speak(self, speechSequence):
  text_list = []
  index = None
  for item in speechSequence:
   if isinstance(item, basestring):
    text_list.append(item)
   elif isinstance(item, speech.IndexCommand):
    index = item.index
  text = u" ".join(text_list)
  if text:
   user_profile = os.environ['USERPROFILE']
   user_temp = user_profile + '/AppData/Local/Temp/emily.txt'
   f = open(user_temp, 'w')
   f.write(text.encode('utf-8'))
   f.close()

 def _get_voice(self):
  return self.curvoice

 def _getAvailableVoices(self):
  o = OrderedDict()
  for v in self.voices:
   o[v] = synthDriverHandler.VoiceInfo(v, v)
  return o