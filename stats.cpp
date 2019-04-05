#include "stats.h"

stats::stats(PNG & im) {

/* your code here */
	sumHueX.resize(im.width());
	sumHueY.resize(im.width());
	sumSat.resize(im.width());
	sumLum.resize(im.width());

	hist.resize(im.width());

	for (unsigned int x = 0; x < im.width(); x++) {
		//cout<<"x:"<<x<<endl;
		sumHueX[x].resize(im.height());
		sumHueY[x].resize(im.height());
		sumSat[x].resize(im.height());
		sumLum[x].resize(im.height());

		hist[x].resize(im.height());

		for (unsigned int y = 0; y < im.height(); y++) {
			//cout<<"y:"<<y<<endl;

			HSLAPixel *pixel = im.getPixel(x, y);

			hist[x][y].resize(36);
			if (x > 0 && y > 0) {
				sumHueX[x][y] = sumHueX[x - 1][y] + sumHueX[x][y - 1] + cos((pixel->h) * PI / 180) -
								sumHueX[x - 1][y - 1];
				sumHueY[x][y] = sumHueY[x - 1][y] + sumHueY[x][y - 1] + sin((pixel->h) * PI / 180) -
								sumHueY[x - 1][y - 1];
				sumSat[x][y] = sumSat[x - 1][y] + sumSat[x][y - 1] + pixel->s - sumSat[x - 1][y - 1];

				sumLum[x][y] = sumLum[x - 1][y] + sumLum[x][y - 1] + pixel->l - sumLum[x - 1][y - 1];


				for (int k = 0; k < 36; k++) {
					if (pixel->h >= (10 * k) && (pixel->h < (10 * k + 10))) {
						hist[x][y][k] = hist[x - 1][y][k] + hist[x][y - 1][k] - hist[x - 1][y - 1][k] + 1;
					}
					else{
						hist[x][y][k] = hist[x - 1][y][k] + hist[x][y - 1][k] - hist[x - 1][y - 1][k];
					}
				}
			} else if (x > 0 && y == 0) {
				sumHueX[x][y] = sumHueX[x - 1][y] + cos((pixel->h) * PI / 180);
				sumHueY[x][y] = sumHueY[x - 1][y] + sin((pixel->h) * PI / 180);
				sumSat[x][y] = sumSat[x - 1][y] + pixel->s;
				sumLum[x][y] = sumLum[x - 1][y] + pixel->l;

				for (int k = 0; k < 36; k++) {
					if (pixel->h >= (10 * k) && (pixel->h < (10 * k + 10))) {
						hist[x][y][k] = hist[x - 1][y][k] + 1;
					}
					else{
						hist[x][y][k] = hist[x - 1][y][k];
					}
				}

			} else if (y > 0 && x == 0) {
				sumHueX[x][y] = sumHueX[x][y - 1] + cos((pixel->h) * PI / 180);
				sumHueY[x][y] = sumHueY[x][y - 1] + sin((pixel->h) * PI / 180);
				sumSat[x][y] = sumSat[x][y - 1] + pixel->s;
				sumLum[x][y] = sumLum[x][y - 1] + pixel->l;
				
				for (int k = 0; k < 36; k++) {
					if (pixel->h >= (10 * k) && (pixel->h < (10 * k + 10))) {
						hist[x][y][k] = hist[x][y - 1][k] + 1;
					}
					else{
						hist[x][y][k] = hist[x][y - 1][k];
					}

				}
			} else {
				sumHueX[x][y] = cos((pixel->h)*PI/180);
				sumHueY[x][y] = sin((pixel->h)*PI/180);
				sumSat[x][y] = pixel->s;
				sumLum[x][y] = pixel->l;
				//cout << "sumlum4: "<<sumLum[x][y] << endl;
				for (int k = 0; k < 36; k++) {
					if (pixel->h >= (10 * k) && (pixel->h < (10 * k + 10))) {
						hist[x][y][k]++;
					}
					else{
						hist[x][y][k] = 0;
					}
				}
			}
/*		for(int k=0; k<36; k++){
			cout<<"first array:"<<"k: "<<k<<"   "<<hist[1][1][k]<<endl;
		}*/

		}
	}
}


long stats::rectArea(pair<int,int> ul, pair<int,int> lr){

/* your code here */

	if(ul.first > lr.first && ul.second >lr.second){
		return (hist.size() - ul.first + lr.first +1) * (hist[0].size()- ul.second + lr.second +1);
	}

	else if(ul.first > lr.first){
		return (hist.size() - ul.first + lr.first +1) * (lr.second - ul.second +1);
	}

	else if(ul.second > lr.second){
		return (lr.first - ul.first + 1) * (hist[0].size()- ul.second + lr.second+1);
	}
	else{
		return (lr.first - ul.first +1) * (lr.second - ul.second+1);
	}

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

	if(ul.first>lr.first && ul.second>lr.second){
		//cout<<"1"<<endl;
		for(int k=0; k<36; k++){
			pair<int, int> whole;
			whole.first = hist.size() -1;
			whole.second = hist[0].size() -1;

			pair<int, int> right;
			right.first = hist.size() -1;
			right.second = lr.second;

			pair<int ,int> up;
			up.first = ul.first;
			up.second = 0;

			pair<int, int> left;
			left.first = 0;
			left.second = ul.second;

			pair<int, int> down;
			down.first = lr.first;
			down.second = hist[0].size() -1;

			newHist[k] = hist[lr.first][lr.second][k] + buildHistHelper(ul, whole, k) + buildHistHelper(up,right, k) + buildHistHelper(left, down, k);
		}
	}

	else if(ul.first > lr.first){
		//cout<<"2"<<endl;
		for(int k=0; k<36; k++){
			pair<int, int> right;
			right.first = hist.size()-1;
			right.second = lr.second;

			pair<int, int> left;
			left.first = 0;
			left.second = ul.second;

			newHist[k] = buildHistHelper(ul, right, k) + buildHistHelper(left, lr, k); 
		}

	}

	else if(ul.second > lr.second){
		//cout<<"3"<<endl;
		for(int k=0; k<36; k++){
			pair<int, int> up;
			up.first = ul.first;
			up.second = 0;

			pair<int, int> down;
			down.first = lr.first;
			down.second = hist[0].size()-1;

			newHist[k] = buildHistHelper(up, lr, k) + buildHistHelper(ul, down, k);

		}
	}

	else{
/*		cout<<"4"<<endl;
		cout<<"ul: "<<ul.first<<", "<<ul.second<<endl;
		cout<<"lr: "<<lr.first<<", "<<lr.second<<endl;
		cout<<"Image height: " << hist[0].size()<<endl;*/
		for(int k=0; k<36; k++){
			newHist[k] = buildHistHelper(ul, lr, k);
		}
	}

/*	for(int k=0; k<36; k++){ 
		cout<<"k: "<<k<<"   "<<newHist[k]<<endl;
	}*/
	return newHist;
}
int stats::buildHistHelper(pair<int,int> ul, pair<int,int>lr, int k){
	int helper;
	if(ul.first>0 && ul.second>0){
		helper = hist[lr.first][lr.second][k] + hist[ul.first-1][ul.second-1][k] - hist[ul.first-1][lr.second][k] - hist[lr.first][ul.second-1][k];
	}
	else if(ul.second > 0){
		helper = hist[lr.first][lr.second][k] - hist[lr.first][ul.second-1][k];
	}
	else if(ul.first > 0){
		helper = hist[lr.first][lr.second][k] - hist[ul.first-1][lr.second][k];
	}
	else{
		helper = hist[lr.first][lr.second][k];
	}
	return helper;
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

    //cout<<"entropy: "<<-1* entropy<<endl;

    return  -1 * entropy;

}

double stats::entropy(pair<int,int> ul, pair<int,int> lr){

/* your code here */
	vector<int> distn = buildHist(ul,lr);
	long area = rectArea(ul,lr);
	return entropy(distn, area);

}
