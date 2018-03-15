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

def get_featuremaps(model, layer_idx, X_batch):
	get_activations = K.function([model.layers[0].input, K.learning_phase()],[model.layers[layer_idx].output,])
	activations = get_activations([X_batch,0])
	return activations

path = "coloredmodel/"

num_channel = 3

model = load_model('resultats/' + path + 'new_model.h5')
print("Loaded model from disk")

names = ['Romane', 'Guillaume', 'Bruno', 'Julien', 'Solene', 'Pierre', 'Clemence', 'Quentin', 'JP', 'Phillipe', 'Louis', 'Matthieu', 'Mihail', 'Orphyse', 'Eugenie', 'Sofiane', 'Mathilde']

test_image = cv2.imread('test/46.jpg')
if(num_channel == 1):
	test_image = cv2.cvtColor(test_image, cv2.COLOR_BGR2GRAY)
cv2.namedWindow("test1", cv2.WINDOW_NORMAL)
cv2.imshow("test1", test_image)
image = np.array(test_image)
image = image.astype('float32')
image /= 255

if(num_channel == 1):
	if K.image_dim_ordering()=='th':
		image = np.expand_dims(image, axis=0)
		image = np.expand_dims(image, axis=0)
	else:
		image = np.expand_dims(image, axis=3)
		image = np.expand_dims(image, axis=0)
else:
	if K.image_dim_ordering() == 'th':
		image = np.rollaxis(image, 2, 0)
		image = np.expand_dims(image, axis=0)
	else:
		image = np.expand_dims(image, axis=0)

print(model.predict(image))
#print(model.predict_classes(image)[0])
print(names[model.predict_classes(image)[0]])
#cv2.waitKey(0)

layer_num=6
filter_num=0

activations = get_featuremaps(model, int(layer_num),image)

feature_maps = activations[0][0]

if K.image_dim_ordering()=='th':
	feature_maps=np.rollaxis((np.rollaxis(feature_maps,2,0)),2,0)

fig=plt.figure(figsize=(16,16))
plt.imshow(feature_maps[:,:,filter_num])
#plt.savefig(path + "featuremaps-layer-{}".format(layer_num) + "-filternum-{}".format(filter_num)+'.jpg')

num_of_featuremaps=feature_maps.shape[2]
fig=plt.figure(figsize=(16,16))
plt.title("featuremaps-layer-{}".format(layer_num))
subplot_num=int(np.ceil(np.sqrt(num_of_featuremaps)))
for i in range(int(num_of_featuremaps)):
	ax = fig.add_subplot(subplot_num, subplot_num, i+1)
	#ax.imshow(output_image[0,:,:,i],interpolation='nearest' ) #to see the first filter
	ax.imshow(feature_maps[:,:,i])
	plt.xticks([])
	plt.yticks([])
	plt.tight_layout()
plt.show()
#fig.savefig(path + "featuremaps-layer-{}".format(layer_num) + '.jpg')
