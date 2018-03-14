from __future__ import print_function
import matplotlib.pyplot as plt
import numpy as np
import os
import sys
import tarfile
from IPython.display import display, Image
from scipy import ndimage
from sklearn.linear_model import LogisticRegression
from six.moves.urllib.request import urlretrieve
from six.moves import cPickle as pickle
import time
import hashlib

url = 'https://commondatastorage.googleapis.com/books1000/'
last_percent_reported = None
data_root = '.' # Change me to store data elsewhere

classes_nb = 17

def download_progress_hook(count, blockSize, totalSize):
  """A hook to report the progress of a download. This is mostly intended for users with
  slow internet connections. Reports every 5% change in download progress.
  """
  global last_percent_reported
  percent = int(count * blockSize * 100 / totalSize)

  if last_percent_reported != percent:
    if percent % 5 == 0:
      sys.stdout.write("%s%%" % percent)
      sys.stdout.flush()
    else:
      sys.stdout.write(".")
      sys.stdout.flush()

    last_percent_reported = percent

def maybe_download(filename, expected_bytes, force=False):
  """Download a file if not present, and make sure it's the right size."""
  dest_filename = os.path.join(data_root, filename)
  if force or not os.path.exists(dest_filename):
    print('Attempting to download:', filename)
    filename, _ = urlretrieve(url + filename, dest_filename, reporthook=download_progress_hook)
    print('\nDownload Complete!')
  statinfo = os.stat(dest_filename)
  if statinfo.st_size == expected_bytes:
    print('Found and verified', dest_filename)
  else:
    raise Exception(
      'Failed to verify ' + dest_filename + '. Can you get to it with a browser?')
  return dest_filename

def maybe_extract(filename, force=False):
  """Extract the images from the downloaded files and save them in diffrent directory"""
  root = os.path.splitext(os.path.splitext(filename)[0])[0]  # remove .tar.gz
  if os.path.isdir(root) and not force:
    # You may override by setting force=True.
    print('%s already present - Skipping extraction of %s.' % (root, filename))
  else:
    print('Extracting data for %s. This may take a while. Please wait.' % root)
    tar = tarfile.open(filename)
    sys.stdout.flush()
    tar.extractall(data_root)
    tar.close()
  data_folders = [
    os.path.join(root, d) for d in sorted(os.listdir(root))
    if os.path.isdir(os.path.join(root, d))]
  if len(data_folders) != num_classes:
    raise Exception(
      'Expected %d folders, one per class. Found %d instead.' % (
        num_classes, len(data_folders)))
  print(data_folders)
  return data_folders

image_size = 100  # Pixel width and height.
pixel_depth = 255.0  # Number of levels per pixel.

def load_letter(folder, min_num_images):
  """Load the data for a single letter label."""
  image_files = os.listdir(folder)
  dataset = np.ndarray(shape=(len(image_files), image_size, image_size),
                         dtype=np.float32)
  print(folder)
  num_images = 0
  for image in image_files:
    image_file = os.path.join(folder, image)
    try:
      image_data = (ndimage.imread(image_file).astype(float) -
                    pixel_depth / 2) / pixel_depth
      if image_data.shape != (image_size, image_size):
        raise Exception('Unexpected image shape: %s' % str(image_data.shape))
      dataset[num_images, :, :] = image_data
      num_images = num_images + 1
    except (IOError, ValueError) as e:
      print('Could not read:', image_file, ':', e, '- it\'s ok, skipping.')

  dataset = dataset[0:num_images, :, :]
  if num_images < min_num_images:
    raise Exception('Many fewer images than expected: %d < %d' %
                    (num_images, min_num_images))

  print('Full dataset tensor:', dataset.shape)
  print('Mean:', np.mean(dataset))
  print('Standard deviation:', np.std(dataset))
  return dataset

def maybe_pickle(data_folders, min_num_images_per_class, force=False):
  """Transform the images folder into .pickle files by transfoming .jpg
  into 3D array aka tensor"""
  dataset_names = []
  for folder in data_folders:
    set_filename = folder + '.pickle'
    dataset_names.append(set_filename)
    if os.path.exists(set_filename) and not force:
      # You may override by setting force=True.
      print('%s already present - Skipping pickling.' % set_filename)
    else:
      print('Pickling %s.' % set_filename)
      dataset = load_letter(folder, min_num_images_per_class)
      try:
        with open(set_filename, 'wb') as f:
          pickle.dump(dataset, f, pickle.HIGHEST_PROTOCOL)
      except Exception as e:
        print('Unable to save data to', set_filename, ':', e)

  return dataset_names

def make_arrays(nb_rows, img_size):
  """Create the array before merge all"""
  if nb_rows:
    dataset = np.ndarray((nb_rows, img_size, img_size), dtype=np.float32)
    labels = np.ndarray(nb_rows, dtype=np.int32)
  else:
    dataset, labels = None, None
  return dataset, labels

def merge_datasets(pickle_files, train_size, valid_size=0):
  """Merge all the .pickle file in 1 big dataset + 1 for labels"""
  num_classes = len(pickle_files)
  valid_dataset, valid_labels = make_arrays(valid_size, image_size)
  train_dataset, train_labels = make_arrays(train_size, image_size)
  vsize_per_class = valid_size // num_classes
  tsize_per_class = train_size // num_classes

  start_v, start_t = 0, 0
  end_v, end_t = vsize_per_class, tsize_per_class
  end_l = vsize_per_class+tsize_per_class
  for label, pickle_file in enumerate(pickle_files):
    try:
      with open(pickle_file, 'rb') as f:
        letter_set = pickle.load(f)
        # let's shuffle the letters to have random validation and training set
        np.random.shuffle(letter_set)
        if valid_dataset is not None:
          valid_letter = letter_set[:vsize_per_class, :, :]
          valid_dataset[start_v:end_v, :, :] = valid_letter
          valid_labels[start_v:end_v] = label
          start_v += vsize_per_class
          end_v += vsize_per_class

        train_letter = letter_set[vsize_per_class:end_l, :, :]
        train_dataset[start_t:end_t, :, :] = train_letter
        train_labels[start_t:end_t] = label
        start_t += tsize_per_class
        end_t += tsize_per_class
    except Exception as e:
      print('Unable to process data from', pickle_file, ':', e)
      raise

  return valid_dataset, valid_labels, train_dataset, train_labels

def randomize(dataset, labels):
  """Randomize the dataset & labels for better learning"""
  permutation = np.random.permutation(labels.shape[0])
  shuffled_dataset = dataset[permutation,:,:]
  shuffled_labels = labels[permutation]
  return shuffled_dataset, shuffled_labels

def save_dataset(train_dataset, train_labels, valid_dataset, valid_labels, test_dataset, test_labels):
  pickle_file = os.path.join(data_root, 'faceKeyBW.pickle')

  try:
    f = open(pickle_file, 'wb')
    save = {
      'train_dataset': train_dataset,
      'train_labels': train_labels,
      'valid_dataset': valid_dataset,
      'valid_labels': valid_labels,
      'test_dataset': test_dataset,
      'test_labels': test_labels,
      }
    pickle.dump(save, f, pickle.HIGHEST_PROTOCOL)
    f.close()
  except Exception as e:
    print('Unable to save data to', pickle_file, ':', e)
    raise
  return pickle_file

def overlap_rate(train_dataset, train_labels, valid_dataset, valid_labels, test_dataset, test_labels):
  t1 = time.time()
  train_hashes = [hashlib.sha1(x).digest() for x in train_dataset]
  valid_hashes = [hashlib.sha1(x).digest() for x in valid_dataset]
  test_hashes  = [hashlib.sha1(x).digest() for x in test_dataset]

  valid_in_train = np.in1d(valid_hashes, train_hashes)
  test_in_train  = np.in1d(test_hashes,  train_hashes)
  test_in_valid  = np.in1d(test_hashes,  valid_hashes)

  valid_keep = ~valid_in_train
  test_keep  = ~(test_in_train | test_in_valid)

  valid_dataset_clean = valid_dataset[valid_keep]
  valid_labels_clean  = valid_labels [valid_keep]

  test_dataset_clean = test_dataset[test_keep]
  test_labels_clean  = test_labels [test_keep]

  t2 = time.time()

  print("Time: %0.2fs" % (t2 - t1))
  print("valid/train overlap: %d samples" % valid_in_train.sum())
  print("test/train overlap: %d samples" % test_in_train.sum())
  print("test/valid overlap: %d samples" % test_in_valid.sum())

def train_logistic_regression(train_dataset, train_labels, test_dataset, test_labels, train_size, valid_size , test_size):
  """Train with logistic regression + display and save filter + return accurency"""
  (samples, width, height) = train_dataset.shape
  X_tr = np.reshape(train_dataset,(samples, width*height))[0:num_samples]
  Y_tr = train_labels[0:num_samples]

  lr = LogisticRegression(multi_class='multinomial', solver='lbfgs',
                               random_state=42, verbose=0, max_iter=10000,
                               penalty='l2', C=penalty)
  lr.fit(X_tr, Y_tr)

  (samples, width, height) = test_dataset.shape
  X_tst = test_dataset.reshape(samples, width * height)
  y_tst = test_labels
  # h_tst = lr.predict(X_tst)
  print('# Samples:%s, Penalty:%s, Accuracy:%f%%' %
            (num_samples, penalty, lr.score(X_tst, y_tst)*100))

  fig = plt.figure()
  fig.set_size_inches(classes_nb, 2)
  fig.suptitle('Coefficients at Penalty:%s, Samples:%s' %
               (penalty, num_samples), fontsize=14)
  filters = np.ndarray(shape=(num_classes, width, height), dtype=np.float32)
  for class_i in range(0,(num_classes-1)):
      filters[class_i, :, :] = lr.coef_.reshape(num_classes, width, height)[class_i]
      a = fig.add_subplot(1, classes_nb, (class_i+1))
      a.set_title('class:%s' % (class_i+1))
      plt.imshow(filters[class_i])
      plt.axis('off')
  fig.savefig('./filter/logiticRegression.png')
  plt.show()
  return lr.score(X_tst, y_tst)*100

#Download
#print("\n-------------------------------------------------------------------")
#print("Downloading files ...")
#train_filename = maybe_download('notMNIST_large.tar.gz', 247336696)
#test_filename = maybe_download('notMNIST_small.tar.gz', 8458043)
#print("Done !")


#Extract
print("\n-------------------------------------------------------------------")
print("Extracting files ...")
num_classes = 17
np.random.seed(133)
train_folders = ['./fullLearningSetBW/a', './fullLearningSetBW/b', './fullLearningSetBW/c', './fullLearningSetBW/d', './fullLearningSetBW/e', './fullLearningSetBW/f', './fullLearningSetBW/g', './fullLearningSetBW/h', './fullLearningSetBW/i', './fullLearningSetBW/j', './fullLearningSetBW/k', './fullLearningSetBW/l', './fullLearningSetBW/n', './fullLearningSetBW/o', './fullLearningSetBW/p', './fullLearningSetBW/q', './fullLearningSetBW/r']
test_folders = ['./testSetBW/a', './testSetBW/b', './testSetBW/c', './testSetBW/d', './testSetBW/e', './testSetBW/f', './testSetBW/g', './testSetBW/h', './testSetBW/i', './testSetBW/j', './testSetBW/k', './testSetBW/l', './testSetBW/n', './testSetBW/o', './testSetBW/p', './testSetBW/q', './testSetBW/r']
print("Done !")

#Print a exemple of image
#img = cv2.imread('notMNIST_small/B/Q2FsaWd1bGEgUmVndWxhci50dGY=.png', 0)
#cv2.imshow('Source',img)

#Convert all dataset into 3D array + zero mean normalization + standard deviation
print("\n-------------------------------------------------------------------")
print("Convert to pickle ...")
train_datasets = maybe_pickle(train_folders, 7000)
test_datasets = maybe_pickle(test_folders, 120)
print("Done !")

#Print images from pickle files
print("\n-------------------------------------------------------------------")
print("Show a sample ...")
#pickle_file = train_datasets[0]  # index 0 should be all As, 1 = all Bs, etc.
#with open(pickle_file, 'rb') as f:
#    letter_set = pickle.load(f)  # unpickle
#    sample_idx = np.random.randint(len(letter_set))  # pick a random image index
#    sample_image = letter_set[sample_idx, :, :]  # extract a 2D slice
    #cv2.imshow('Normalized',sample_image)
print("Done !")

#Check the balance
print("\n-------------------------------------------------------------------")
print("Check the size of each classes ...")
for i in range(classes_nb):
    with open(train_datasets[i], 'rb') as fp:
        letter_set = pickle.load(fp)
        print(letter_set.shape)
print("Done !")

#Merge all in 1 dataset
print("\n-------------------------------------------------------------------")
print("Merge all .pickle files ...")
train_size = 100000
valid_size = 30000
test_size = 2520

valid_dataset, valid_labels, train_dataset, train_labels = merge_datasets(train_datasets, train_size, valid_size)
_, _, test_dataset, test_labels = merge_datasets(test_datasets, test_size)

print('Training:', train_dataset.shape, train_labels.shape)
print('Validation:', valid_dataset.shape, valid_labels.shape)
print('Testing:', test_dataset.shape, test_labels.shape)

print("Randomize ...")
train_dataset, train_labels = randomize(train_dataset, train_labels)
test_dataset, test_labels = randomize(test_dataset, test_labels)
valid_dataset, valid_labels = randomize(valid_dataset, valid_labels)
print("Done !")

#Save the merged .pickle file
print("\n-------------------------------------------------------------------")
print("Save ...")
pickle_file = save_dataset(train_dataset, train_labels, valid_dataset, valid_labels, test_dataset, test_labels)
print("Compress ...")
statinfo = os.stat(pickle_file)
print('Compressed pickle size:', statinfo.st_size)
print("Done !")

#Overlap rate of the shuffled_dataset
print("\n-------------------------------------------------------------------")
print("Compute overlape ...")
overlap_rate(train_dataset, train_labels, valid_dataset, valid_labels, test_dataset, test_labels)
print("Done !")

#Training with logistic LogisticRegression
print("\n-------------------------------------------------------------------")
print("Train with Logistic Regression ...")
num_samples = 50000
n_classes = 17
penalty = 0.01

accurency = train_logistic_regression(train_dataset, train_labels, test_dataset, test_labels, train_size, valid_size , test_size)
print("Done !")



