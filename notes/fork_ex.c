//note that the terminal may look like it's hanging, but it's not, 
//it has printed the terminal prompt before the rest of the print statements


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{

	int count = 0;
	int pid; 
	pid=fork();
	printf("\npid: %d",pid);
    if (pid != 0) {
        while (count < 10){
                count++;
			    printf("\nParent pid: %d sleeping for 1 s",pid); 
			    sleep(1);
		}			    
	}
    else {
    printf("\nChild pid: %d sleeping for 15s ",pid); 
    sleep(15);
    }

    printf("\nCount: %d\n",count);
 
    return 0;

}