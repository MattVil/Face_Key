from os import listdir, makedirs
from os.path import isfile, join, exists
import numpy as np
import cv2

roots = ["testSet/"]
output_path = "testSetBW/"

totalImages = 0
index = 0

for root in roots:
  path = root
  directories = [f for f in listdir(path) if not isfile(join(path, f))]
  for directory in directories:
    images = []
    folder = path + "/" + directory
    files = [f for f in listdir(folder) if isfile(join(folder, f))]

    print("Reading: " + root + "/" + directory + " (" + str(len(files)) + ").")

    if not exists(output_path + "/" + directory):
      makedirs(output_path + "/" + directory)

    for f in files:
      image = cv2.imread(folder + "/" + f, cv2.cv.CV_LOAD_IMAGE_COLOR)
      images.append(image)

      gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

      filename = root.replace("/", "_") + "_" + str(index)
      cv2.imwrite(output_path + "/" + directory + "/" + filename + ".jpg", gray_image)
      index = index + 1

    

print ('Got ' + str(index) + ' images.')
