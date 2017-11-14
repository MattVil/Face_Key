from imgaug import augmenters as iaa
from os import listdir, makedirs
from os.path import isfile, join, exists
import numpy as np
import cv2



# Sometimes(0.5, ...) applies the given augmenter in 50% of all cases,
# e.g. Sometimes(0.5, GaussianBlur(0.3)) would blur roughly every second image.
sometimes = lambda aug: iaa.Sometimes(1.0, aug)

possibleChanges = [
  sometimes(iaa.Add(-25)),
  sometimes(iaa.Add(-45)),
  sometimes(iaa.Add(+25)),
  sometimes(iaa.Add(+45)),
  sometimes(iaa.Multiply(0.5)),
  sometimes(iaa.Multiply(1.25)),
  sometimes(iaa.Fliplr(0.5)), # horizontally flip 50% of the images
  sometimes(iaa.GaussianBlur(0.25)),
  sometimes(iaa.AdditiveGaussianNoise(0.03*255)),
  sometimes(iaa.Dropout(0.01)),
  sometimes(iaa.ContrastNormalization(1.2)),
  sometimes(iaa.ContrastNormalization(0.8)),

]
seq = iaa.OneOf(possibleChanges)

roots = ["origin/"]
output_path = "augmented/"

totalImages = 0
index = 0
def generateRandomImages(directory):
  global index
  output_images = seq.augment_images(images)
  for image in output_images:
    cv2.imwrite(output_path + "/" + directory + "/" + str(index) + ".jpg", image)
    index = index + 1

for root in roots:
  path = root
  directories = [f for f in listdir(path) if not isfile(join(path, f))]
  for directory in directories:
    images = []
    folder = path + "/" + directory
    files = [f for f in listdir(folder) if isfile(join(folder, f))]

    print "Reading: " + root + "/" + directory + " (" + str(len(files)) + ")."

    if not exists(output_path + "/" + directory):
      makedirs(output_path + "/" + directory)

    for f in files:
      image = cv2.imread(folder + "/" + f, cv2.cv.CV_LOAD_IMAGE_COLOR)
      images.append(image)

      filename = root.replace("/", "_") + "_" + str(index)
      cv2.imwrite(output_path + "/" + directory + "/" + filename + ".jpg", image)
      index = index + 1

    for i in range(len(possibleChanges)):
      generateRandomImages(directory)

print 'Got ' + str(index) + ' images.'
