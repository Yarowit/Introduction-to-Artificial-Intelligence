import os

import tensorflow as tf
from keras.datasets import mnist
from keras.utils import to_categorical

import matplotlib.pyplot as plt

import numpy as np

import cv2 as cv

import os

(x_train,y_train),(x_test,y_test)=mnist.load_data()

print("x_train shape: ",x_train.shape)
print("y_train shape: ",y_train.shape)
print("x_test shape: ",x_test.shape)
print("y_test shape: ",y_test.shape)

y_train_enc=to_categorical(y_train)
y_test_enc=to_categorical(y_test)

# Zmniejszenie wymiaru zdjęcia
# x_train_rs=np.reshape(x_train,(60000,784))

# x_test_rs=np.reshape(x_test,(10000,784))
x_train_rs = x_train
x_test_rs = x_test
# print("x_train reshaped: ",x_train_rs.shape)
# print("x_test reshaped: ",x_test_rs.shape)

# Standaryzacja
x_mean=np.mean(x_train_rs)
x_mean2=np.mean(x_test_rs)
x_std=np.std(x_train_rs)
x_std2=np.std(x_test_rs)
x_train_std=(x_train_rs-x_mean)/x_std
x_test_std=(x_test_rs-x_mean2)/x_std2

# print("Standardized training set: ",set(x_train_std[0]))
# print("Standardized testing set: ",set(x_test_std[0]))

from keras.models import Sequential
from keras.layers import Dense, Flatten, Conv2D,MaxPooling2D
# import keras.layers as s


# model = Sequential([
#     # Flatten(input_shape=(28, 28)),
#     Dense(532, activation = 'relu', input_shape = (28*28,)),
#     Dense(128, activation = 'relu'),
#     Dense(10, activation = 'softmax')
# ])
# model = Sequential([
#     Dense(532, activation = 'relu', input_shape = (784,)),
#     Dense(532, activation = 'relu'),
#     Dense(10, activation = 'softmax')
# ])

model = Sequential([
    Conv2D(32,(3,3),activation='relu', input_shape=(28,28,1)),
    MaxPooling2D((2,2)),
    Conv2D(64,(3,3), activation='relu'),
    MaxPooling2D((2,2)),
    Flatten(),
    Dense(64, activation = 'relu'),
    Dense(10, activation = 'softmax'),
])

model.compile(
    optimizer = 'sgd',
    loss = 'categorical_crossentropy',
    metrics = ['accuracy']
)

model.summary()

model.fit(
    x_train_std,
    y_train_enc,
    epochs = 10
)

loss1, accuracy1 = model.evaluate(x_test_std, y_test_enc)
loss2, accuracy2 = model.evaluate(x_train_std, y_train_enc)
print('test set accuracy: ', accuracy1 *100)
print('train set accuracy: ', accuracy2 * 100)
    
fig, axs = plt.subplots(3, len(os.listdir("images")))
y = 0
for folder in os.listdir("images"):
    x = 0
    for file in os.listdir(f"images/{folder}"):
        file = f"images/{folder}/{file}"

        image = cv.imread(file, cv.IMREAD_GRAYSCALE)
        image = cv.resize(image, (28, 28))
        image = image.astype('float32')
        image = image.reshape(1, 28, 28, 1)
        image = 255-image
        image /= 255

        pred = model.predict(image.reshape(1, 28, 28, 1), batch_size=1)

        axs[x,y].imshow(image.reshape(28, 28),cmap='Greys')
        axs[x,y].set_title(f"Predicted: {pred.argmax()}")
        

        # print(pred.argmax())
        x = x + 1
    y = y + 1


plt.show()