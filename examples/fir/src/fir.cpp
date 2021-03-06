/*****************************************************************************

  Licensed to Accellera Systems Initiative Inc. (Accellera) under one or
  more contributor license agreements.  See the NOTICE file distributed
  with this work for additional information regarding copyright ownership.
  Accellera licenses this file to you under the Apache License, Version 2.0
  (the "License"); you may not use this file except in compliance with the
  License.  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
  implied.  See the License for the specific language governing
  permissions and limitations under the License.

 *****************************************************************************/

/*****************************************************************************
 
  fir.cpp -- 
 
  Original Author: Rocco Jonack, Synopsys, Inc.
 
 *****************************************************************************/
 
/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation: Teodor Vasilache and Dragos Dospinescu,
                         AMIQ Consulting s.r.l. (contributors@amiq.com)
                   Date: 2018-Feb-20

  Description of Modification: Added sampling of the covergroup created in the 
  "fir.h" file.
            
 *****************************************************************************/

/*****************************************************************************
 
  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.
 
      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#include <systemc.h>
#include "fir.h"

void fir::entry() {

  sc_int<8>  sample_tmp;
  sc_int<17> pro;
  sc_int<19> acc;
  sc_int<8> shift[16];

  // reset watching
  /* this would be an unrolled loop */
  for (int i=0; i<=15; i++) 
    shift[i] = 0;
  result.write(0);
  output_data_ready.write(false);
  wait();

  // main functionality
  while(1) {
    output_data_ready.write(false);
    do { wait(); } while ( !(input_valid == true) );
    sample_tmp = sample.read();
    acc = sample_tmp*coefs[0];

    for(int i=14; i>=0; i--) {
      /* this would be an unrolled loop */
      pro = shift[i]*coefs[i+1];
      acc += pro;
    };

    for(int i=14; i>=0; i--) {
      /* this would be an unrolled loop */
      shift[i+1] = shift[i];
    };

    shift[0] = sample_tmp;
    
    // sample the shift value  
    shift_cg.sample(shift[0]);

    // write output values
    result.write((int)acc); 
    output_data_ready.write(true);
    wait();
  };
}
