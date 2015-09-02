# WideBaselineFeatureMatcher_PAMI
New: We have just released the libraries for wide baseline feature matcher, check them out under GitHub release tag and find header files and examples under CODE-Libs folder !!

(1) How to run the program <br />
The .exe file contained in this repo was compiled under Windows7 64bit using Microsoft Visual Studio 2010 and nvcc of CUDA 7.0. In order to run the .exe file, you need first to download and install CUDA 7.0. The program can only be ran under Windows system at this time and an Nvidia card supporting CUDA is also a necessity.

(2) How to use the program
  - executing the CODE.exe from commandline without any parameters will display all parameter options
  - "-sift" will perform affine-sift feature detection. The program takes either a .txt file containing full paths to images     or a full path to an image. Currently, .png, .jpg, .tif, and .bmp are supported. Example: C:/CODE -sift C:/image_list.txt
  - "-mach" will do matching on two .sift files, and two full paths to .sift files are used as inputs. A temporary .mat          binary matching file will be output in the same folder as two .sift files. Example: C:/CODE -mach -indb C:/Image1.sift       -inqy C:/Image2.sift
  - "-blat" will do the filtering and output filtered matches assuming that you've already got the .mat file and two .sift       files in the folder. It still takes two .sift files as inputs and assume that the .mat file is in the same folder.           Example: C:/CODE -blat -indb C:/Image1.sift -inqy C:/Image2.sift
  - "-mach" + "-blat" will perform the previous two steps altogether. The output is the filtered matchings which is a .txt       file. The temporary .mat file will not be generated. Example: C:/CODE -mach -blat -indb C:/Image1.sift -inqy                 C:/Image2.sift
  - "-pwis" will take a .txt file as input and perform full matching between all pairs of images in the input list. Assuming     image_list.txt is as follows: <br />
    C:/Image1.png <br />
    C:/Image2.png <br />
    C:/Image3.png <br />
    The program will do the matching in this order: (Image1.png, Image2.png), (Image1.png, Image3.png), and (Image2.png,         Image3.png). <br />
    Example: C:/CODE -pwis -indb C:/image_list.txt
  - "-full" will take two .txt files as inputs and perform full matching between every image in the first list and every         image in the second list. Example: C:/CODE -full -indb C:/image_list1.txt -inqy C:/image_list2.txt
  - "-srdy" can be used in combination with "-pwis" or "-full". This option tells program that you've had the .sift files        ready in the same folder with the images listed in the .txt file. Example: C:/CODE -pwis -srdy -indb C:/image_list.txt
  - "-stpt" can be used in combination with "pwis". In case an all pair matching is interrupted in the middle of execution,      this option tells the program to restart from wherever it stops. Example: C:/CODE -pwis -srdy -stpt 5 -indb                  C:/image_list.txt
  - "-disp" enable display function in the program. It displays matchings as points and connecting lines. Example: C:/CODE       -pwis -indb C:/CODE/image_list.txt -disp
  - "-noaf" means no affine. It turns off the affine simulation during sift detection and performs the standard sift feature     detection only. Example: C:/CODE -sift -noaf -indb C:/image_list.txt
  - "-inqy" can be a .txt file containing query images or a path to a single image. The image(s) immediately following this      parameter will be used as query (on the left).
  - "-indb" can be a .txt file containing compare images or a path to a single image. The image(s) immediately following this     parameter will be used as compare (on the right). Note that most operations listed above accept only one input, in this      case, only parameter immediately after "-indb" is used as input
  - "-rept" turns on the full timing report. Example: C:/CODE -pwis C:/image_list.txt -rept
  - "-nmal" tells the program that you do not want the VSFM compatible matches, you just need normal matching ouput. Example:     C:/CODE -pwis -indb C:/image_list.txt -nmal
  
(3) Compatibility with VisualSFM <br />
The default outputs from this program including .sift and matchings.txt are all VSFM compatible. You can directly feed the outputs from the program to VSFM without any trouble. You may notice that there is also a file called .aux. These files store the information about affine and must be put in the same folder as .sift files.

(4) Parameter tuning <br />
We have two sets of parameters ready for tuning: sift feature detection and bilateral function. You can find bilateral_settings.txt and siftParam_settings.txt in the unzipped folder. <br />
siftParam_settings.txt 
 - tilt: how many tilts you want to simulate in the affine process <br />
The rest of the parameters are the same with Changchang Wu's SiftGPU http://cs.unc.edu/~ccwu/siftgpu/manual.pdf

The parameters inside bilateral_settings.txt are currently not recommended for tuning. Further instructions will be coming soon.
