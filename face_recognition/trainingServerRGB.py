import os, cv2
import numpy as np
import matplotlib.pyplot as plt
import itertools

from sklearn.utils import shuffle
from sklearn.cross_validation import train_test_split
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

from keras.models import model_from_json
from keras.models import load_model

def plot_confusion_matrix(cm, classes,
                          normalize=False,
                          title='Confusion matrix',
                          cmap=plt.cm.Blues):
    """
    This function prints and plots the confusion matrix.
    Normalization can be applied by setting `normalize=True`.
    """
    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    plt.title(title)
    plt.colorbar()
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=45)
    plt.yticks(tick_marks, classes)

    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        print("Normalized confusion matrix")
    else:
        print('Confusion matrix, without normalization')

    print(cm)

    thresh = cm.max() / 2.
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, cm[i, j],
                 horizontalalignment="center",
                 color="white" if cm[i, j] > thresh else "black")

    plt.tight_layout()
    plt.ylabel('True label')
    plt.xlabel('Predicted label')

######## LOAD THE DATA #########
PATH = os.getcwd()
data_path = PATH + '/bigRGB'
data_dir_list = os.listdir(data_path)

SAVE_PATH = "bigNewRGB/"

num_epoch = 10

img_rows=200
img_cols=200
num_channel=3

num_classes = 8

img_data_list=[]
class_size=[]
class_name=[]

for dataset in data_dir_list:
	img_list=os.listdir(data_path+'/'+ dataset)
	print ('Loaded the ' + str(len(img_list)) + ' images of dataset-'+'{}'.format(dataset))
	class_size.append(len(img_list))
	class_name.append(dataset)
	for img in img_list:
		input_img=cv2.imread(data_path + '/'+ dataset + '/'+ img, 1)
		img_data_list.append(input_img)

img_data = np.array(img_data_list)
img_data = img_data.astype('float32')
img_data /= 255
print ('Shape of the dataset : {' + str(img_data.shape) + '}')
#print(class_size)
#(nb_images,size_x, size_y, nb_chanel) si RGB
#(nb_images,size_x, size_y) si gray scale


#format theano : (nb_images, nb_chanel, size_x, size_y)
#format tensorflow : (nb_images,size_x, size_y, nb_chanel)
if num_channel==1:
	if K.image_dim_ordering()=='th':
		img_data= np.expand_dims(img_data, axis=1)
	else:
		img_data= np.expand_dims(img_data, axis=4)

else:
	if K.image_dim_ordering()=='th':
		img_data=np.rollaxis(img_data,3,1)
print (img_data.shape)

#labels
num_of_samples = img_data.shape[0]
labels = np.ones((num_of_samples,),dtype='int64')


labels[0:class_size[0]] = 0
print('{0}:{', class_size[0],'} -> {0} -> ', class_name[0])
somme = class_size[0]

for i in range(1,num_classes):
	print('{', somme,'}:{', somme + class_size[i],'} -> {', i, '} -> ', class_name[i])
	labels[somme:somme+class_size[i]] = i
	somme += class_size[i]

Y = np_utils.to_categorical(labels, num_classes)
#print(Y)
#print(Y.shape)

######## SHUFFLE DATA #########
x, y = shuffle(img_data, Y, random_state=2)
X_train, X_test, y_train, y_test = train_test_split(x, y, test_size=0.5, random_state=4)


####### MODEL ######
input_shape = img_data[0].shape

model = Sequential()

model.add(Convolution2D(32, 3, 3,input_shape=input_shape))
model.add(Activation('relu'))
model.add(Convolution2D(32, 3, 3))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.5))

model.add(Convolution2D(64, 3, 3))
model.add(Activation('relu'))
model.add(Convolution2D(64, 3, 3))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.5))

model.add(Flatten())
model.add(Dense(64))
model.add(Activation('relu'))
model.add(Dropout(0.5))
# model.add(Dense(128))
# model.add(Activation('relu'))
# model.add(Dropout(0.5))
model.add(Dense(num_classes))
model.add(Activation('softmax'))

model.compile(loss='categorical_crossentropy', optimizer='rmsprop',metrics=["accuracy"])


model.summary()
model.get_config()
model.layers[0].get_config()
model.layers[0].input_shape
model.layers[0].output_shape
model.layers[0].get_weights()
np.shape(model.layers[0].get_weights()[0])
model.layers[0].trainable

####### TRAINING ########


hist = model.fit(X_train, y_train, batch_size=16, nb_epoch=num_epoch, verbose=1, validation_data=(X_test, y_test))

# save the model
model.save(SAVE_PATH + 'new_model.h5')
print("model saved in new_model.h5 !\n")


filename= SAVE_PATH + 'model_train_new.csv'
csv_log=callbacks.CSVLogger(filename, separator=',', append=False)

early_stopping=callbacks.EarlyStopping(monitor='val_loss', min_delta=0, patience=0, verbose=0, mode='min')

filepath= SAVE_PATH + "Best-weights-my_model-{epoch:03d}-{loss:.4f}-{acc:.4f}.hdf5"

checkpoint = callbacks.ModelCheckpoint(filepath, monitor='val_loss', verbose=1, save_best_only=True, mode='min')

callbacks_list = [csv_log,early_stopping,checkpoint]

#hist = model.fit(X_train, y_train, batch_size=16, nb_epoch=num_epoch, verbose=1, validation_data=(X_test, y_test),callbacks=callbacks_list)


########### RESULTS #########

# visualizing losses and accuracy
train_loss=hist.history['loss']
val_loss=hist.history['val_loss']
train_acc=hist.history['acc']
val_acc=hist.history['val_acc']
xc=range(num_epoch)

plt.figure(1,figsize=(7,5))
plt.plot(xc,train_loss)
plt.plot(xc,val_loss)
plt.xlabel('num of Epochs')
plt.ylabel('loss')
plt.title('train_loss vs val_loss')
plt.grid(True)
plt.legend(['train','val'])
#print plt.style.available # use bmh, classic,ggplot for big pictures
plt.style.use(['classic'])

plt.figure(2,figsize=(7,5))
plt.plot(xc,train_acc)
plt.plot(xc,val_acc)
plt.xlabel('num of Epochs')
plt.ylabel('accuracy')
plt.title('train_acc vs val_acc')
plt.grid(True)
plt.legend(['train','val'],loc=4)
#print plt.style.available # use bmh, classic,ggplot for big pictures
plt.style.use(['classic'])

score = model.evaluate(X_test, y_test, verbose=0)
print('Test Loss:', score[0])
print('Test accuracy:', score[1])

#confusion matrix
Y_pred = model.predict(X_test)
print(Y_pred)
y_pred = np.argmax(Y_pred, axis=1)
print(y_pred)

print(classification_report(np.argmax(y_test, axis=1), y_pred, target_names=class_name))

# Plotting the confusion matrix

# Compute confusion matrix
cnf_matrix = (confusion_matrix(np.argmax(y_test,axis=1), y_pred))

np.set_printoptions(precision=2)

plt.figure()

# Plot non-normalized confusion matrix
plot_confusion_matrix(cnf_matrix, classes=class_name,
title='Confusion matrix')

plt.show()

######### TEST #########

test_image = X_test[0:1]
#cv2.imshow("test1", test_image)
print (test_image.shape)

print(model.predict(test_image))
print(model.predict_classes(test_image))
print(y_test[0:1])

# Testing a new image
# test_image = cv2.imread('62.jpg')
# test_image=cv2.cvtColor(test_image, cv2.COLOR_BGR2GRAY)
# test_image=cv2.resize(test_image,(100,100))
# test_image = np.array(test_image)
# test_image = test_image.astype('float32')
# test_image /= 255
#
# if num_channel==1:
# 	if K.image_dim_ordering()=='th':
# 		test_image= np.expand_dims(test_image, axis=0)
# 		test_image= np.expand_dims(test_image, axis=0)
# 	else:
# 		test_image= np.expand_dims(test_image, axis=3)
# 		test_image= np.expand_dims(test_image, axis=0)
#
# else:
# 	if K.image_dim_ordering()=='th':
# 		test_image=np.rollaxis(test_image,2,0)
# 		test_image= np.expand_dims(test_image, axis=0)
# 	else:
# 		test_image= np.expand_dims(test_image, axis=0)
#
# # Predicting the test image
# print((model.predict(test_image)))
# print(model.predict_classes(test_image))
# res = model.predict_classes(test_image)
# print(class_name[res[0]])
