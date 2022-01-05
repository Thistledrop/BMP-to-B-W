#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
//https://csfieldguide.org.nz/en/interactives/pixel-viewer/

/*	Hailey Carter
*	BMP to Black and White Converter
*	Code takes command line input, the name of a file (without extension)
*	first and an integer (1-3) second to indicate the method to convert the given
*	image to Black and White.
*
*
*/

using namespace std;
typedef unsigned char BYTE;

int main(int argc, char *argv[])
{
	string name = argv[1];				//getting the name of the file from command line
	int method = atoi(argv[2]);			//getting the method from command line
	cout << name << ".bmp" << endl;		//changing the name to a .bmp address
	
	ofstream outf(name + "_Black&White.bmp");	//Writing a new file with the name plus _Black&White_'method'.bmp
	if (!outf)
	{
		cerr <<"unable to create new file!" << endl; 
		exit(1);
	}
 	
	ifstream inf(name + ".bmp");			//reading the file with the name that was givin
	if (!inf)
 	{
		cerr <<"unable to open file for reading!" << endl;
		exit(1);
	}
	
	//TODO: rename the outf file to add the conversion method.

 	BYTE* header = new BYTE[54];		//creating an array of BYTE's to hold the header of the file

 	for (int i = 0; i < 54; i++)		//copying the header to the header array and to the outf file
 	{
		header[i] = inf.get();
		outf.put(header[i]);
	}
	
	if (header[0] != 'B' || header[1] != 'M')
	{
		cerr << "File is not a BMP" << endl;
		exit(1);
	}

	else
		cout << "Signature: " << header[0] << header[1] << endl;	//Signature is bits one and two
 	
	stringstream ss;
	ss << hex << int (+header[5]);
	ss << hex << int (+header[4]);
	ss << hex << int (+header[3]);
	ss << hex << int (+header[2]);
	unsigned int intsize;
	ss >> intsize;							//File Size is bits 2-5
		
	cout << "File Size: " << intsize << " bits" << endl;
	
	stringstream bitbit;
  bitbit << (+header[29]); 
	bitbit << (+header[28]);
	
	unsigned int ByteSize;						//Pixel Size is bits 28 and 29
	bitbit >> ByteSize;
	bool alpha = false;
	
	cout << "Pixel Size: " << ByteSize << " bits" << endl;
	if (ByteSize == 32)
		alpha = true;
		
	stringstream bb;
	bb << hex << int (+header[13]);
	bb << hex << int (+header[12]);
	bb << hex << int (+header[11]);
	bb << hex << int (+header[10]);
	unsigned int address;						//the starting address of the pixel is bits 10-13
	bb >> address;
	
	cout << "Starting Address: " << address << endl;

	stringstream width;
	width << hex << int (+header[21]);
	width << hex << int (+header[20]);
	width << hex << int (+header[19]);
	width << hex << int (+header[18]);
	int columns;								//the number of columns is bits 18-21
	width >> columns;

	stringstream height;
	height << hex << int (+header[25]);
	height << hex << int (+header[24]);
	height << hex << int (+header[23]);
	height << hex << int (+header[22]);
	int rows;								//the number of rows is bits 22-25 UNLESS the file is square
											//then there's an indicator here
	height >> rows;

	if (rows >= 2147483647)						//Check for indicator of a square file.
    {
      cout << "File is Square" << endl;
      cout << "Size: " << columns << "x" << columns << endl;
      rows = columns;
    }
	
	else
	{
		cout << "Size: " << columns << "x" << rows << endl;
	}


	BYTE* colors = new BYTE[3];	
  BYTE* bw = new BYTE[3];


	if (address >= 55)
	{
		for (int i = 0; i < address-54; i++)	//For bigger files: copy the filler text between the end of the header and the beginning of the pixels
		{
			outf.put(inf.get());
		}
	}

  int red; int green; int blue; //tracks the RGB value
	int r; int g; int b;          //tracks the placement in the colors & bw array
	int avg;                      //the int avg 
	char give;                    //the char rendition of avg

  for (int i = 0; i < (rows*columns); i++)
  {
    blue = int (+inf.get());
    green = int (+inf.get());
    red = int (+inf.get());

    if (method == 1)
      avg = max(max(red, green), max(green, blue)) + min(min(red, green), min(green, blue));

    if (method == 2)
      avg = (blue + green + red)/3;

    if (method == 3)
      avg = (.21 * red) + (.72 * green) + (.07 * blue);
    
    give = (char)avg;

    outf.put(give);
    outf.put(give);
    outf.put(give);

    if (alpha)
    {
      inf.get();
      outf.put(give);
    }
  }

	inf.close();				//Close all files
	outf.close();

	cout << "All Done!";		//Confirm the program hasn't crashed half-way through.
}