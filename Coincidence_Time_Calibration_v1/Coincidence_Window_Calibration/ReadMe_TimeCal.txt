

#1########### Coincidence time window calibration############################################################
The measurement output is analysis by BinaryRead_Measurement-LAB240401_TimeCal.cpp
This program will give: 2D back to back reconstruction (h2Map), 1D projection, time vs detections, total eneryg spectrum, and Coincidence time window for each of half-sticID correllation between the 2 boards.
All the analysis results in .root files will be located in a new folder (similar name as the input .root file, with the extension of TimeCal) in the same directory as the input file

This program required the energy calibration file to set a precise energy threshold.
 each of the two board is divided into 16 half-sticID sections
The coincidence time window for each of the 16 half-sticID section between 2 boards is reconigzed (256 correlation)
(x,y)=(32,16)
(8)(9)(10)(11)(12)(13)(14)(15)  
(0) (1) (2)  (3)   (4)   (5)  (6)   (7)

To compile the program:

#open terminal in the same directory

g++ BinaryRead_Measurement-LAB240401_TimeCal.cpp -o BinaryRead_Measurement-LAB240401_TimeCal `root-config --cflags --glibs` -lSpectrum

There are 7 arguments: 1. file path (required), 2. file name (required), 3. Calibration file .txt (required), 4.  Energy threshold [keV](optional),
 5. Minimum ADC value for display (optional), 6. Maximum ADC value for display (optional), 7. Approximated measurement time [sec], 8. Number of bin for the coincidence time window (250), 9. Peaks finding parmeter (0.2)
The arguments 8 and 9 are adjusted to so that the program can find the coincidence peak in the coincidence time window.
The maximum number of peaks are 7. Any correlation has more than 7 peaks are reconigzed as a faule syncronized correlation. 


The execution file is created. To use the execution file for the analysis: (example)


./BinaryRead_Measurement-LAB240401_TimeCal data 100104.298_A3-2MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 200 200 800 165 250 0.2

The output of the time calibration is Time_Window_B0_B1.txt: 16 numbers are given for each of the 256 correlations. The first 2 number are the correlation ID for board 0 and 1. 
The other 14 following numbers are the position of the coincident time peaks and their width.
The half-sticID of each board are defined as: int(int(x2/4)+8*int(y2/8)).
half-sticID running from 0-15.
All output histogram from .root file can be read and reanalyzed by Root macro program.


