/* This file attempts to test the predicted correctly branches */
/* performance counter on various architectures, as            */
/* implemented by the PAPI_BR_PRC counter.                     */

/* by Vince Weaver, vweaver1@eecs.utk.edu                      */


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "papiStdEventDefs.h"
#include "papi.h"

#include "papi_test.h"

#include "branches_test.h"
#include "helper.h"


int main(int argc, char **argv) {
   
   int retval;

   int num_runs=100;
   long long high=0,low=0,average=0,expected=1000000;
   double error;
   
   retval = PAPI_library_init(PAPI_VER_CURRENT);
   if (retval != PAPI_VER_CURRENT) {
      test_fail( __FILE__, __LINE__, "PAPI_library_init", retval);
   }

   retval = PAPI_query_event(PAPI_BR_PRC);
   if (retval != PAPI_OK) {
      test_fail( __FILE__, __LINE__ ,"PAPI_BR_PRC not supported", retval);
   }

   printf("\n");   

   printf("Testing a loop with %lld branches (%d times):\n",
          expected,num_runs);

   average=branches_test(PAPI_BR_PRC, num_runs,
			 &high,&low);


   error=handle_result(average,high,low,expected);

   if ((error > 1.0) || (error<-1.0)) {

      test_fail( __FILE__, __LINE__, "Instruction count off by more than 1%", 0);
   }
   printf("\n");

   /*******************/

   high=0; low=0;



   average=random_branches(PAPI_BR_CN, num_runs,
			 &high,&low);

   expected=average;

   printf("\n\nTesting a function that branches based on a random number\n");
   printf("   The loop has %lld conditional branches, 500,000 which are random\n",expected);

   average=random_branches(PAPI_BR_PRC, num_runs,
			 &high,&low);

   printf("\n\n%lld correct, %lld not correct\n",
	  average, expected-average);

   if ( (average<100000) || (expected-average > 500000)) {

     test_fail( __FILE__, __LINE__, "Mistpredicts too high\n",0);
   }
   printf("\n");

   PAPI_shutdown();

   test_pass( __FILE__ , NULL, 0);
   
   return 0;
}