
#include "stats.h"

stats::stats(PNG & im){

/* your code here */
	sumHueX.reserve(im.width());
	sumHueY.reserve(im.width());
	sumSat.reserve(im.width());
	sumLum.reserve(im.width());

	hist.reserve(im.width());

	for(unsigned int x=0; x<im.width(); x++){
		sumHueX[x].reserve(im.height());
		sumHueY[x].reserve(im.height());
		sumSat[x].reserve(im.height());
		sumLum[x].reserve(im.height());

		hist[x].reserve(im.height());

		for(unsigned int y=0; y<im.height(); y++){
			double sumHX = 0;
			double sumHY = 0;
			double sumS = 0;
			double sumL = 0;

			hist[x][y].reserve(36);

			for(unsigned int i=0; i<=x; i++){
				for(unsigned int j=0; j<=y; j++){
					HSLAPixel *pixel = im.getPixel(i,j);
					sumS+=pixel->s;
					sumL+=pixel->h;
					if(i==x){
						sumHY+=pixel->h;
					}
					if(j==y){
						sumHX+=pixel->h;
					}

					for(int k=0; k<36; k++){
						if(pixel->h >= (10*k) && pixel->h < (10*k+10)){
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


}

long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

/* your code here */

	return (lr.first - ul.first +1) * (lr.second - ul.second+1);
}

HSLAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr){

/* your code here */
	HSLAPixel average;

	long count = rectArea(ul,lr);
	cout<< sumSat[lr.first][lr.second]<<endl;
	cout<< sumSat[ul.first][ul.second]<<endl;
	cout<< sumSat[lr.first][ul.second]<<endl;
	cout<< sumSat[ul.first][lr.second]<<endl;
	double sumS = sumSat[lr.first][lr.second] + sumSat[ul.first][ul.second] - sumSat[ul.first][lr.second] - sumSat[lr.first][ul.second];
	average.s = sumS/count;

	double sumL = sumLum[lr.first][lr.second] + sumLum[ul.first][ul.second] - sumLum[ul.first][lr.second] - sumLum[lr.first][ul.second];
	average.l = sumL/count;

	double sumHX = sumHueX[lr.first][lr.second] + sumHueX[ul.first][ul.second] - sumHueX[ul.first][lr.second] - sumHueX[lr.first][ul.second];
	double X = sumHX/count;

	double sumHY = sumHueY[lr.first][lr.second] + sumHueY[ul.first][ul.second] - sumHueY[ul.first][lr.second] - sumHueY[lr.first][ul.second];
	double Y = sumHY/count;

	average.h = atan2(X,Y)*180/PI;

	average.a = 1.0;

	return average;
}

vector<int> stats::buildHist(pair<int,int> ul, pair<int,int> lr){

/* your code here */
	vector<int> newHist(36);
	for(int k=0; k<36; k++){
		newHist[k] = hist[ul.first][ul.second][k] + hist[lr.first][lr.second][k] - hist[ul.first][lr.second][k] - hist[lr.first][ul.second][k];
	}
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
