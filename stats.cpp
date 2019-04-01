
#include "stats.h"

stats::stats(PNG & im){

/* your code here */
	sumHueX.resize(im.width());
	sumHueY.resize(im.width());
	sumSat.resize(im.width());
	sumLum.resize(im.width());

	hist.resize(im.width());
	//cout<<"width:"<<im.width()<<endl;
	//cout<<"height:"<<im.height()<<endl;

	for(unsigned int x=0; x<im.width(); x++){
		//cout<<"x:"<<x<<endl;
		sumHueX[x].resize(im.height());
		sumHueY[x].resize(im.height());
		sumSat[x].resize(im.height());
		sumLum[x].resize(im.height());

		hist[x].resize(im.height());

		for(unsigned int y=0; y<im.height(); y++){
			//cout<<"y:"<<y<<endl;
			double sumHX = 0;
			double sumHY = 0;
			double sumS = 0;
			double sumL = 0;


			hist[x][y].resize(36);
/*			HSLAPixel *pixel = im.getPixel(x,y);


if(x>0 && y >0){
   sumHueX[x][y] = sumHueX[x-1][y] + sumHueX[x][y-1] + pixel->s * cos((pixel->h)*PI/180) - sumHueX[x-1][y-1];
   sumHueY[x][y] = sumHueY[x-1][y] + sumHueY[x][y-1] + pixel->s * sin((pixel->h)*PI/180) - sumHueY[x-1][y-1];
   sumSat[x][y] = sumSat[x-1][y] + sumSat[x][y-1]+ pixel->s - sumSat[x-1][y-1];
   sumLum[x][y] = sumLum[x-1][y] + sumLum[x][y-1] +pixel->l - sumLum[x-1][y-1];
   for(int k = 0; k<35; k++){
      if(pixel->h >= (10 * k) && (pixel->h < (10 * k + 10))){
         hist[x][y][k] = hist[x-1][y][k] + hist[x][y-1][k] - hist[x-1][y-1][k] + 1;
      }
   }
}
else if(x>0 && y==0){
   sumHueX[x][y] = sumHueX[x-1][y] + pixel->s * cos((pixel->h)*PI/180);
   sumHueY[x][y] = sumHueY[x-1][y] + pixel->s * sin((pixel->h)*PI/180);
   sumSat[x][y] = sumSat[x-1][y] + pixel->s;
   sumLum[x][y] = sumSat[x-1][y] + pixel->l;
   for(int k = 0; k<35; k++){
      if(pixel->h >= (10 * k) && (pixel->h < (10 * k + 10))){
         hist[x][y][k] = hist[x-1][y][k] + 1;
      }
   }

}
else if (y>0 && x==0){
   sumHueX[x][y] = sumHueX[x][y-1] + pixel->s * cos((pixel->h)*PI/180);
   sumHueY[x][y] = sumHueY[x][y-1] + pixel->s * sin((pixel->h)*PI/180);
   sumSat[x][y] = sumSat[x][y-1] + pixel->s;
   sumLum[x][y] = sumLum[x][y-1] + pixel->l;
   for(int k = 0; k<35; k++){
      if(pixel->h >= (10 * k) && (pixel->h < (10 * k + 10))){
         hist[x][y][k]= hist[x][y-1][k]+1;
      }

   }

}
else{
   sumHueX[x][y] = pixel->s * cos(pixel->h);
   sumHueY[x][y] = pixel->s * sin(pixel->h);
   sumSat[x][y] = pixel->s;
   sumLum[x][y] = pixel->l;
   for(int k = 0; k<35; k++){
      if(pixel->h >= (10 * k) && (pixel->h < (10 * k + 10))){
         hist[x][y][k]++;
      }
   }
}*/

			for(unsigned int i=0; i<=x; i++){
				for(unsigned int j=0; j<=y; j++){
					HSLAPixel *pixel = im.getPixel(i,j);
					sumS+=pixel->s;
					sumL+=pixel->l;
					//cout<<"pixel h:"<<pixel->h<<endl;
					sumHX+=cos((pixel->h)*PI/180);
					sumHY+=sin((pixel->h)*PI/180);

					for(int k=0; k<36; k++){
						if(pixel->h >= (10*k) && pixel->h < (10*k+10)){
							/*cout<<"should count"<<endl;
							cout<<"k:"<<k<<endl;*/
							hist[x][y][k]++;
						}
					}				
				}
			}

			sumHueX[x][y] = sumHX;
			sumHueY[x][y] = sumHY;
			sumSat[x][y] = sumS;
			sumLum[x][y] = sumL;

		}

	}
/*		for(int k=0; k<36; k++){
			cout<<"first array:"<<"k: "<<k<<"   "<<hist[1][1][k]<<endl;
		}*/


}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

/* your code here */

	return (lr.first - ul.first +1) * (lr.second - ul.second+1);
}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){

/* your code here */
	HSLAPixel average;

	long count = rectArea(ul,lr);
	/*cout<< sumHueX[lr.first][lr.second]<<endl;
	cout<< sumLum[ul.first][ul.second]<<endl;
	cout<< sumLum[lr.first][ul.second]<<endl;
	cout<< sumLum[ul.first][lr.second]<<endl;*/
	double sumS;
	double sumL;
	double sumHX;
	double sumHY;
	if(ul.first>0 && ul.second>0){
		sumS = sumSat[lr.first][lr.second] + sumSat[ul.first-1][ul.second-1] - sumSat[ul.first-1][lr.second] - sumSat[lr.first][ul.second-1];
		sumL = sumLum[lr.first][lr.second] + sumLum[ul.first-1][ul.second-1] - sumLum[ul.first-1][lr.second] - sumLum[lr.first][ul.second-1];
		sumHX = sumHueX[lr.first][lr.second] + sumHueX[ul.first-1][ul.second-1] - sumHueX[ul.first-1][lr.second] - sumHueX[lr.first][ul.second-1];
		sumHY = sumHueY[lr.first][lr.second] + sumHueY[ul.first-1][ul.second-1] - sumHueY[ul.first-1][lr.second] - sumHueY[lr.first][ul.second-1];
	}
	else if(ul.second > 0){
		sumS = sumSat[lr.first][lr.second] - sumSat[lr.first][ul.second-1];
		sumL = sumLum[lr.first][lr.second] - sumLum[lr.first][ul.second-1];
		sumHX = sumHueX[lr.first][lr.second] - sumHueX[lr.first][ul.second-1];
		sumHY = sumHueY[lr.first][lr.second] - sumHueY[lr.first][ul.second-1];
	}
	else if(ul.first > 0){
		sumS = sumSat[lr.first][lr.second] - sumSat[ul.first-1][lr.second];
		sumL = sumLum[lr.first][lr.second] - sumLum[ul.first-1][lr.second];
		sumHX = sumHueX[lr.first][lr.second] - sumHueX[ul.first-1][lr.second];
		sumHY = sumHueY[lr.first][lr.second] - sumHueY[ul.first-1][lr.second];
	}
	else{
		sumS = sumSat[lr.first][lr.second];
		sumL = sumLum[lr.first][lr.second];
		sumHX = sumHueX[lr.first][lr.second];
		sumHY = sumHueY[lr.first][lr.second];
	}

	average.s = sumS/count;
	average.l = sumL/count;

	double X = sumHX/count;
	double Y = sumHY/count;

	average.h = atan2(Y,X)*180/PI;

	average.a = 1.0;

	return average;
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){

/* your code here */
	vector<int> newHist(36);
	for(int k=0; k<36; k++){
		if(ul.first>0 && ul.second>0){
			newHist[k] = hist[lr.first][lr.second][k] + hist[ul.first-1][ul.second-1][k] - hist[ul.first-1][lr.second][k] - hist[lr.first][ul.second-1][k];
		}
		else if(ul.second > 0){
			newHist[k] = hist[lr.first][lr.second][k] - hist[lr.first][ul.second-1][k];
		}
		else if(ul.first > 0){
			newHist[k] = hist[lr.first][lr.second][k] - hist[ul.first-1][lr.second][k];
		}
		else{
			newHist[k] = hist[lr.first][lr.second][k];
		}
	}

/*	for(int k=0; k<36; k++){
		cout<<"k: "<<k<<"   "<<newHist[k]<<endl;
	}*/
	return newHist;
}

// takes a distribution and returns entropy
// partially implemented so as to avoid rounding issues.
double stats::entropy(vector<int> & distn,int area){

    double entropy = 0.;

/* your code here */

    for (int i = 0; i < 36; i++) {
        if (distn[i] > 0 ) 
            entropy += ((double) distn[i]/(double) area) 
                                    * log2((double) distn[i]/(double) area);
    }

    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

/* your code here */
	vector<int> distn = buildHist(ul,lr);
	long area = rectArea(ul,lr);
	return entropy(distn, area);

}
