#include <unistd.h>
#include <stdlib.h>
#include "cwrap.h"

int main(){
	void *x = get_events();
	if(x==NULL)
		return 1;
	sleep(5);
	end_events(x);
	return 0;
}
