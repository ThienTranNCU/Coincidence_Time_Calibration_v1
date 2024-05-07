#2########### Analyzing measurement output adopting coincidence time calibration############################################################
The measurement output is analysis by BinaryRead_Measurement-LAB240402_TWFilted.cpp
This program will give: 2D back to back reconstruction (h2Map), 1D projection, time vs detections, and total eneryg spectrum.
All the analysis results in .root files will be located in a new folder (similar name as the input .root file) in the same directory as the input file

To compile the program:

#open terminal

g++ BinaryRead_Measurement-LAB240402_TWFilted.cpp -o BinaryRead_Measurement-LAB240402_TWFilted `root-config --cflags --glibs` -lSpectrum


There are 7 arguments: 1. file path (required), 2. file name (required), 3. Calibration file .txt (required), 4.  Energy threshold [keV](optional),
 5. Minimum ADC value for display (optional), 6. Maximum ADC value for display (optional), 7. Approximated measurement time [sec] , 8. Coincidence time calibration file in .txt.

The execution file is created. To use the execution file for the analysis: 

>./BinaryRead_Measurement-LAB230303 [file path (required)] [file name (required)] [Calibration file .txt (required)] [Energy threshold [keV] (optional-default 400)]
 [Minimum ADC value for display (optional- default 0)] [Maximum ADC value for display (optional-default 32800)] [Approximated measurement time [sec] (optional-default 1200)] [Coincidence time calibration (TimeCalWin_A3.txt)]

Example:


./BinaryRead_Measurement-LAB240402_TWFilted Data 091448.906_A1-2MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 160 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 093919.795_A2-2MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 160 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 100104.298_A3-2MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 160 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 110053.316_D1-2MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 160 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 113900.803_B1-2MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 160 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 123809.707_E1-2MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 160 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 125755.548_F1-2MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 160 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 121102.047_B2-2MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 160 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 103806.825_C1-a Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 40 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 121102.047_B2-2MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 40 TimeCalWin_A3.txt


./BinaryRead_Measurement-LAB240402_TWFilted Data 091650.765_A1-13MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 800 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 094121.170_A2-13MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 800 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 100351.413_A3-13MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 800 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 110255.258_D1-13MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 800 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 114102.159_B1-13MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 800 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 121303.778_B2-13MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 800 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 125958.489_F1-11MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 800 TimeCalWin_A3.txt
./BinaryRead_Measurement-LAB240402_TWFilted Data 124021.355_E1-13MINS Energy_Calibration_v2_ASPET_CGMH220312.txt 350 200 800 800 TimeCalWin_A3.txt




