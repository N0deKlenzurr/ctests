#include <iostream>


int binary_search(int dataset[], int lowerb, int upperb, int target)
{
	int mid = (upperb + lowerb) / 2;

	if (lowerb < upperb) {
		if (dataset[mid] == target) {
			return mid;
		} else if (dataset[mid] > target) {
			return binary_search(dataset, 
				lowerb, mid - 1, target);
		} else {
			return binary_search(dataset, 
				mid + 1, upperb, target);
		}
	} else {
		return -1;
	}
}