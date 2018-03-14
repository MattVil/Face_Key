import numpy as np
import cv2

import os, cv2
import numpy as np
import matplotlib.pyplot as plt
import itertools

from keras.models import model_from_json
from keras.models import load_model

from sklearn.metrics import classification_report, confusion_matrix

import keras
from keras import callbacks
from keras.utils import np_utils
from keras.models import Sequential
from keras.layers.core import Dense, Dropout, Activation, Flatten
from keras.layers.convolutional import Convolution2D, MaxPooling2D
from keras.optimizers import SGD,RMSprop,adam
from keras import backend as K
K.set_image_dim_ordering('tf')

face_cascade = cv2.CascadeClassifier('file/haarcascade_frontalface_alt.xml')
eye_cascade = cv2.CascadeClassifier('file/haarcascade_eye.xml')

if(face_cascade.empty()):
    print("error loading faces")

path = "coloredmodel/"

model = load_model(path + 'new_model.h5')
print("Loaded model from disk")

names = ['Romane', 'Guillaume', 'Bruno', 'Julien', 'Solene', 'Pierre', 'Clemence', 'Quentin', 'JP', 'Phillipe', 'Louis', 'Matthieu', 'Mihail', 'Orphyse', 'Eugenie', 'Sofiane', 'Mathilde']
names2 = ['Kierian', 'Valentin', 'Julien', 'Jerome', 'quentin', 'remi', 'louis', 'matthieu']

num_channel = 3
LOW_DB = False
IMG_SIZE = 100

cap = cv2.VideoCapture(0)

while 1:
    ret, img = cap.read()
    img = cv2.flip(img, 1)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    for (x,y,w,h) in faces:
        cv2.rectangle(img,(x,y),(x+w,y+h),(255,0,0),2)

        face_r = img[y:y+h, x:x+w]

        face = cv2.resize(face_r, (IMG_SIZE, IMG_SIZE))

        if(num_channel == 1):
            gray = cv2.cvtColor(face, cv2.COLOR_BGR2GRAY)
        else:
            gray = face

        img_2_detect = np.array(gray)
        img_2_detect = img_2_detect.astype('float32')
        img_2_detect /= 255

        if(num_channel == 1):
            if K.image_dim_ordering()=='th':
            	img_2_detect = np.expand_dims(img_2_detect, axis=0)
            	img_2_detect = np.expand_dims(img_2_detect, axis=0)
            else:
            	img_2_detect = np.expand_dims(img_2_detect, axis=3)
            	img_2_detect = np.expand_dims(img_2_detect, axis=0)
        else:
            if K.image_dim_ordering() == 'th':
                img_2_detect = np.rollaxis(img_2_detect, 2, 0)
                img_2_detect = np.expand_dims(img_2_detect, axis=0)
            else:
                img_2_detect = np.expand_dims(img_2_detect, axis=0)


        class_num = model.predict_classes(img_2_detect)[0]
        nom = names[model.predict_classes(img_2_detect)[0]]
        if(LOW_DB):
            nom = names2[model.predict_classes(img_2_detect)[0]]
        score = str(round((model.predict(img_2_detect)[0])[class_num] * 100, 2))
        cv2.putText(img, nom, (x+w+10,y+h+10), cv2.FONT_HERSHEY_SIMPLEX, 1,(255,0,0),2)
        cv2.putText(img, score + "%", (x+w+10,y+h+40), cv2.FONT_HERSHEY_SIMPLEX, 1,(255,0,0),2)
        #print(names[model.predict_classes(img_2_detect)[0]])

    cv2.imshow('img',img)
    k = cv2.waitKey(30) & 0xff
    if k == 27:
        break

cap.release()
cv2.destroyAllWindows()
