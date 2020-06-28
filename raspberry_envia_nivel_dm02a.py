#Código exemplo para Rasberry criado e fornecido pelo nosso amigo Leonardo Oliveira - knowdbtech (https://github.com/knowdbtech). Obrigado pela contribuição Leonardo.

import RPi.GPIO as GPIO
import time,sys

if len (sys.argv) <= 1: sys.exit('Parametro entre 0 e 70 eh necessario!')

nivel = 0

try:
    nivel = int(sys.argv[1])
except:
  print("O parametro deve ser numerico")
  sys.exit()

if nivel < 0 or nivel > 70: sys.exit('Parametro fora do range 0-70')

usleep = lambda x: time.sleep(x/1000000.0)

refval = int(sys.argv[1]) * 500;

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(10, GPIO.OUT)
GPIO.output(10,0)


GPIO.setup(9, GPIO.OUT)

GPIO.output(9,GPIO.LOW)
usleep(refval)
GPIO.output(9,GPIO.HIGH)
