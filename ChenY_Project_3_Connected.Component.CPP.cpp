#include<iostream>
#include<string>
#include<fstream>
using namespace std;

//ImageProcessing Class
class ImageProcessing {
	public:
	int** imgAry;
	int** zeroFramedAry;
	int*  eqAry; 
	int   newLabel;
	int   numRows, numCols;
	int   minVal, maxVal;
	
	ImageProcessing(int numRows, int numCols, int minVal, int maxVal);
	void loadImage( ifstream& inFile ); 
	void zeroFramed(); 
	void firstPass8Conn();
	void secondPass8Conn();
	void thirdPass8Conn();
	void manageEqAry();
	void prettyPrintZeroFramedAry( ofstream& outFile );
	void printCurrentEqAry( ofstream& outFile );
	void createBinaryImageFromZeroFrameAry( ofstream& outFile );

};

//ImageProcessing Constructor
ImageProcessing::ImageProcessing(int numRows, int numCols, int minVal, int maxVal) 
{
	this->numRows  = numRows;
	this->numCols  = numCols;
	this->minVal   = minVal;
	this->maxVal   = maxVal;
	this->eqAry	   = new int[numRows*numCols/4] {} ; 
	this->newLabel = 0;
	
	this->imgAry   = new int*[numRows];
		
	for (int i = 0; i<numRows; i++){
		this->imgAry[i] = new int[numCols] {};
	}
}
//load the image from textfile into variable imgAry**
void ImageProcessing::loadImage( ifstream& inFile )
{
	for(int i =0; i<numRows; i++ ){
		for(int j =0; j<numCols; j++){
			inFile>>imgAry[i][j];
		}
	}		
};

//frame the imgAry with 0's on all edges and store it to zeroFramedAry**
void ImageProcessing::zeroFramed()
{
	zeroFramedAry = new int*[numRows+2];
	for(int i =0; i<numRows+2; i++ ){
		zeroFramedAry[i] = new int[numCols+1] {};
	}	
   //filling the array	 	
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			zeroFramedAry[i][j] = imgAry[i-1][j-1];
		}
	}		
};//zeroFramed()

//first pass scan of 8connected component algorithm
void ImageProcessing::firstPass8Conn() 
{
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
		
			if( zeroFramedAry[i][j] > 0){
		
				int min = 0;
				int max = 0;
				
				if( zeroFramedAry[i-1][j-1] > 0 ){
					if(min < 1) min = 99999;
					if( zeroFramedAry[i-1][j-1] < min ) min = zeroFramedAry[i-1][j-1];
					if( zeroFramedAry[i-1][j-1] > max ) max = zeroFramedAry[i-1][j-1];
				}
				
				if( zeroFramedAry[i-1][j] > 0 ){
					if(min < 1) min = 99999;
					if( zeroFramedAry[i-1][j] < min ) min = zeroFramedAry[i-1][j];
					if( zeroFramedAry[i-1][j] > max ) max = zeroFramedAry[i-1][j];
				}
				
				if( zeroFramedAry[i-1][j+1] > 0 ){
					if(min < 1) min = 99999;
					if( zeroFramedAry[i-1][j+1] < min ) min = zeroFramedAry[i-1][j+1];
					if( zeroFramedAry[i-1][j+1] > max ) max = zeroFramedAry[i-1][j+1];
				}
			
				if( zeroFramedAry[i][j-1] > 0 ){
					if(min < 1) min = 99999;
					if( zeroFramedAry[i][j-1] < min ) min = zeroFramedAry[i][j-1];
					if( zeroFramedAry[i][j-1] > max ) max = zeroFramedAry[i][j-1];
				}
				
				if( max <= 0 )
				{
					newLabel++;
					eqAry[newLabel] = newLabel;
					zeroFramedAry[i][j] = newLabel;
				}
				else if( max == min )
				{
					zeroFramedAry[i][j] = min;
				}
				else 
				{
					zeroFramedAry[i][j] = min;
				}	
					
			}//if( zeroFramedAry[i][j] > 0)
		}//for(int j =1; j<numCols+1; j++){
	}//for(int i =1; i<numRows+1; i++ ){	
};//firstPass8Conn()

//second pass scan of 8connected component algorithm
void ImageProcessing::secondPass8Conn() 
{
	for(int i =numRows+1; i >= 1; i-- ){
		for(int j =numCols+1; j >= 1; j--){
		
			if( zeroFramedAry[i][j] > 0)
			{
				int min = zeroFramedAry[i][j];
				int max = zeroFramedAry[i][j];
				
				if( zeroFramedAry[i][j+1] > 0 ){
					if( zeroFramedAry[i][j+1] < min ) min = zeroFramedAry[i][j+1];
					if( zeroFramedAry[i][j+1] > max ) max = zeroFramedAry[i][j+1];
				}
				
				if( zeroFramedAry[i+1][j-1] > 0 ){
					if( zeroFramedAry[i+1][j-1] < min ) min = zeroFramedAry[i+1][j-1];
					if( zeroFramedAry[i+1][j-1] > max ) max = zeroFramedAry[i+1][j-1];
				}
				
				if( zeroFramedAry[i+1][j] > 0 ){
					if( zeroFramedAry[i+1][j] < min ) min = zeroFramedAry[i+1][j];
					if( zeroFramedAry[i+1][j] > max ) max = zeroFramedAry[i+1][j];
				}
				
				if( zeroFramedAry[i+1][j+1] > 0 ){
					if( zeroFramedAry[i+1][j+1] < min ) min = zeroFramedAry[i+1][j+1];
					if( zeroFramedAry[i+1][j+1] > max ) max = zeroFramedAry[i+1][j+1];
				}
				
				if( max <= 0 ){
					newLabel++;
					zeroFramedAry[i][j] = newLabel;
				}else{
					eqAry[zeroFramedAry[i][j]] = min;
					zeroFramedAry[i][j] = min;
				}	
					
			}//if( zeroFramedAry[i][j] > 0)
		}//for(int j =1; j<numCols+1; j++){
	}//for(int i =1; i<numRows+1; i++ ){	
};//secondPass8Conn() 

//third pass scan of 8connected component algorithm
void ImageProcessing::thirdPass8Conn() 
{
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			if( zeroFramedAry[i][j] > 0 ){
				zeroFramedAry[i][j] = eqAry[ zeroFramedAry[i][j] ];	
			}
		}//for(int j =1; j<numCols+1; j++)
	}//for(int i =1; i<numRows+1; i++ )
};//thirdPass8Conn() 

//Yan's label management algorithm for eqAry*
void ImageProcessing::manageEqAry()
{
	int counter   = 0;
	int eqArySize = numRows*numCols/4; 
	
	int num = 1;
	while(eqAry[num] >0){
		num++;
	}	
	
	for(int i =0; i<eqArySize; i++){
		if( eqAry[i] > eqAry[eqAry[i]] ){
			eqAry[i] = eqAry[eqAry[i]];
		}
	}	

	for(int i =0; i<eqArySize; i++){			
		if(eqAry[i] == counter){
			counter++;
		}
		if(eqAry[i] > counter ){
			int temp = eqAry[i];
			for(int j = i; j<eqArySize; j++){
				if( eqAry[j] == temp ){ 
					eqAry[j] = counter;  
				}
			}
			counter++;
		}	
	}
	
};//manageEqAry()

//print the Current state of eqAry*
void ImageProcessing::printCurrentEqAry( ofstream& outFile )
{
	int eqArySize = numRows*numCols/4; 

	for(int i =0; i<newLabel+1; i++){
		outFile<<i<<"\t";
	}	
	outFile<<endl;
	for(int i =0; i<newLabel+1; i++){
		outFile<<eqAry[i]<<"\t";
	}
	outFile<<endl<<endl;
};//printCurrentEqAry()

//print the current state of zeroFramedAry**
void ImageProcessing::prettyPrintZeroFramedAry( ofstream& outFile )
{
	for(int i =0; i<numRows+2; i++ ){
		for(int j =0; j<numCols+2; j++){
			if(zeroFramedAry[i][j] == 0 )
				outFile<<" ";
			else
				outFile<<zeroFramedAry[i][j];
		}
		outFile<<endl;
	}	
	outFile<<endl<<endl;
};

//save zeroFramedAry** to a file WITHOUT the zero border frames
void ImageProcessing::createBinaryImageFromZeroFrameAry( ofstream& outFile )
{
	outFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
	for(int i =1; i<numRows+1; i++ ){
		for(int j =1; j<numCols+1; j++){
			if(zeroFramedAry[i][j] == 0 )
				outFile<<0<<" ";
			else
				outFile<<zeroFramedAry[i][j]<<" ";
		}
		outFile<<endl;
	}	
	outFile<<endl<<endl;
};

int main(int argc, char* argv[]) 
{
	
	ifstream inFile    (argv[1]);
	ofstream outFile1  (argv[2]);
	ofstream outFile2  (argv[3]);
	
	int rowSize, colSize, min, max;
	
	/*
	step 0: read the image header & dynamically allocate zeroFramedAry	*/   
	inFile>>rowSize;
	inFile>>colSize;
	inFile>>min;
	inFile>>max; 
    ImageProcessing* img1 = new ImageProcessing(rowSize, colSize, min, max);
   
    /*
    step 1: load the input image onto zeroFramedAry	*/
   	img1->loadImage(inFile);
   	img1->zeroFramed();
  	inFile.close();   

	/*step 2: firstPass8conn // algorithm taught in class
	   - prettyprint the result // with caption
	   - print EQAry	// with index up to newLable with caption	*/
	img1->firstPass8Conn();	
	outFile1<<"prettyPrint of firstPass8conn():"<<endl;	
	img1->prettyPrintZeroFramedAry( outFile1 );
	outFile1<<"EqAry after firstPass8conn():"<<endl;
	img1->printCurrentEqAry( outFile1 );
		
	/*
	step 3: secondpass8conn // algorithm taught in class
	   - prettyprint the result // with caption
	   - print EQAry // with index up to newLable with caption	*/
	img1->secondPass8Conn();
	outFile1<<"prettyPrint of secondPass8Conn():"<<endl;		
	img1->prettyPrintZeroFramedAry( outFile1 );
	outFile1<<"EqAry after secondPass8Conn():"<<endl;
	img1->printCurrentEqAry( outFile1 );
	
	/*
	step 4: manageEQAry // 
 	   - print EQAry // with index up to newLable with caption	*/
	img1->manageEqAry();
	outFile1<<"EqAry after manageEqAry():"<<endl;
	img1->printCurrentEqAry( outFile1 );

	/*
	step 5: thirdPass8conn // algorithm taught in class
	   - prettyprint the result // with caption	*/
	img1->thirdPass8Conn();	
	outFile1<<"prettyPrint of thirdPass8Conn():"<<endl;		
	img1->prettyPrintZeroFramedAry( outFile1 );
	outFile1.close();
	
	//create a binary image from the result of Pass-3 with header for future processing
	img1->createBinaryImageFromZeroFrameAry( outFile2 );
	outFile2.close();
	
	cout<<"Program successful!"<<endl;

}//main
