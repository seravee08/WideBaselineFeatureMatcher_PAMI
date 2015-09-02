Compile instructions for CODE: A Wide Baseline Feature Matcher

We provide two libraries under lib folder:
CODE_VS10: compiled using Microsoft Visual Studio 2010 under Windows7 64bit
CODE_VS13: compiled using Microsoft Visual Studio 2013 under Windows7 64bit

In order to use the provided libraries, you need to:
(1) Download Eigen and add the path to Visual Studio's "Additional Include Directories"
	For example: C:\Eigen3.2.3\eigen
(2) Download newer version of OpenCV and add the path to "Additional Include Directories"
	For example: C:\opencv\build\include\opencv
	Add the path to OpenCV libraries to "Additional Library Directories"
	For example: C:\opencv\build\x64\vc10\lib
	and add necessary libraries
	For example: opencv_core2411.lib
	Add the path to corresponding .dll files to environment variables and make it effective
	Important: OpenCV 3.0 is not supported !!
(3) Download CUDA 7.0 and install it
	Add the path containing libraries to "Additional Library Directories"
	For example: C:\Program Files\NVIDIA GPU COMPUTING Toolkit\CUDA\v7.0\lib\x64
	and add necessary libraries
	For example: cuda.lib
	Important: At the time of writing this document, CUDA 7.5RC is just released. We don't test
	our libraries on CUDA 7.5RC. If you encounter any problem using it, please report to the authors.
(4) Add the path to the headers we provide to "Additional Include Directories" and also remember
	to add our library to your project depending on the version of Visual Studio you use.

The libraries can also be used only for CUDA Affine-SIFT feature detection. We also provide a
main.cpp as an usage example, you will find more details inside it. If you have any query, please
drop an email to fan.gordon.wang@gmail.com. Keep checking back, we will release the source for CUDA
Affine-SIFT soon.

If some .dll are missing when running the .exe, please goto our .exe version folder at GitHub and download
the necessary .dll files. The program should run smoothly when you put those .dll files under the same
folder with .exe file.