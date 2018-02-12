/*
 * topksorttest.cpp
 *
 *  Created on: 2014-8-24
 *      Author: ggbond
 */




#include "simrankmethod.h"

class topkTest: public SimRankMethod{
public:
	topkTest(int a, int b, double c):SimRankMethod(a,b,c){
	}

	void run(int qv, int k){}
};

int main(){
	topkTest srm(1, 1, 1.0);
	vector<SimRankValue> array;
	for(int i = 0; i < 10; i++){
		array.push_back(SimRankValue(i, i));
	}
	int k;
	while(scanf("%d", &k) != EOF){
		for(int i = 0; i < 10; i++){
			printf("%.1lf ", array[i].getValue());
		}
        printf("\n");
		srm.getKMax(array, 0, 9, k);
		for(int i = 0; i < 10; i++){
			printf("%.1lf ", array[i].getValue());
		}
        printf("\n");
	}
	return 0;
}
