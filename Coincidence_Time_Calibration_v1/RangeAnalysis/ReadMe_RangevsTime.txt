
#3##################Example reading processed data from the analysis program

 To run this program:
 
#open terminal

> root .x  Compare_Simulation_Measurement_230307_NormRange.cc

The Read_LOR_Range_v4_DAQ.cpp and Read_LOR_RangeSingle_v5.cpp will read the output from the BinaryRead_Measurement-LAB240402_TWFilted.cpp and analyze the PAG range with time.
The Read_LOR_Range_v4_DAQ.cpp will analyze and combine 2 output files.

# To compile the program:
g++ Read_LOR_Range_v4_DAQ.cpp -o Read_LOR_Range_v4_DAQ `root-config --cflags --glibs` -lSpectrum

g++ Read_LOR_RangeSingle_v5.cpp -o Read_LOR_RangeSingle_v5 `root-config --cflags --glibs` -lSpectrum

There are 7 arguments for Read_LOR_Range_v4_DAQ.cpp: 1. first read in output file, 2. second read in output file, 3. The exact measured time in second of the first output file, 4. 5. the approximate entrance range(in channel),
 6. 7. the approximate PAG range (channel).

# to run the program:

./Read_LOR_Range_v4_DAQ Data/121102.047_B2-2MINS_Processed3/Data-LOR.txt Data/121303.778_B2-13MINS_Processed3/Data-LOR.txt C1BG_1D.txt 121 1 6 18 26 Correction64_1D_21cm_DAQ_220312.txt
./Read_LOR_Range_v4_DAQ Data/125755.548_F1-2MINS_Processed3/Data-LOR.txt Data/125958.489_F1-11MINS_Processed3/Data-LOR.txt C1BG_1D.txt 121 1 6 18 26 Correction64_1D_21cm_DAQ_220312.txt
./Read_LOR_Range_v4_DAQ Data/113900.803_B1-2MINS_Processed3/Data-LOR.txt Data/114102.159_B1-13MINS_Processed3/Data-LOR.txt C1BG_1D.txt 121 1 6 18 26 Correction64_1D_21cm_DAQ_220312.txt
./Read_LOR_Range_v4_DAQ Data/123809.707_E1-2MINS_Processed3/Data-LOR.txt Data/124021.355_E1-13MINS_Processed3/Data-LOR.txt C1BG_1D.txt 131 1 7 21 27 Correction64_1D_21cm_DAQ_220312.txt

./Read_LOR_Range_v4_DAQ Data/100104.298_A3-2MINS_Processed3/Data-LOR.txt Data/100351.413_A3-13MINS_Processed3/Data-LOR.txt C1BG_1D.txt 121 1 6 18 26 Correction64_1D_7cm_DAQ_220312.txt
./Read_LOR_Range_v4_DAQ Data/091448.906_A1-2MINS_Processed3/Data-LOR.txt Data/091650.765_A1-13MINS_Processed3/Data-LOR.txt C1BG_1D.txt 120 1 7 9 15 Correction64_1D_7cm_DAQ_220312.txt
./Read_LOR_Range_v4_DAQ Data/093919.795_A2-2MINS_Processed3/Data-LOR.txt Data/094121.170_A2-13MINS_Processed3/Data-LOR.txt C1BG_1D.txt 120 1 6 13 20 Correction64_1D_7cm_DAQ_220312.txt
./Read_LOR_Range_v4_DAQ Data/110053.316_D1-2MINS_Processed3/Data-LOR.txt Data/110255.258_D1-13MINS_Processed3/Data-LOR.txt C1BG_1D.txt 121 1 7 20 28 Correction64_1D_7cm_DAQ_220312.txt
./Read_LOR_Range_v4_DAQ Data/100104.298_A3-2MINS_Processed3/Data-LOR.txt Data/100351.413_A3-13MINS_Processed3/Data-LOR.txt C1BG_1D.txt 121 1 6 18 26 Correction64_1D_7cm_DAQ_220312.txt


./Read_LOR_RangeSingle_v5 Data/110255.258_D1-13MINS_Processed2/Data-LOR.txt C1BG_1D.txt 480 780 1 6 20 28
./Read_LOR_RangeSingle_v5 Data/100351.413_A3-13MINS_Processed2/Data-LOR.txt C1BG_1D.txt 480 780 1 6 18 26
