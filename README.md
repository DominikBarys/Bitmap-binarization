# Bitmap binarization

## Description

Program for binarize bitmaps. The project was written in C++ language using Qt framework for graphical user interface. The binarization algorithm itself was written in two dynamically added libraries while the program is running. One dynamic library is written in C++ and the other in assembly language to compare the times of the two algorithms.

##

## How to use the program

After running the program, the user will see the following window:

![App Screenshot](https://github.com/DominikBarys/Bitmap-binarization/blob/main/Screenshots/Screenshot1.png?raw=true)

* First, press the **Input** button and select a bitmap.
* Use the slider to select the binarization threshold and the number of threads on which the program will work
* Choose which algorithm to use for binarization: **C++** algorithm or **ASM** algorithm
* Afterward that you can press the **START** button

![App Screenshot](https://github.com/DominikBarys/Bitmap-binarization/blob/main/Screenshots/Screenshot2.png?raw=true)

After these steps, the binarized photo will be shown on the screen and the **binarized.bmp** file will be generated in the folder where the program was started. 

When the program finishes binarization, the algorithm execution time in milliseconds will be displayed on the interface and details of the binarized bitmap can be displayed after pressing **Show details** button.

##

## How to run a program

Run the BitmapBinarization.exe file located in the "BitmapBinarizationExecutable" directory. There are all the necessary .dll libraries required by Qt and libraries with algorithms.
