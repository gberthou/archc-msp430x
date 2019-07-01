const char *project_name="msp430x";
const char *project_file="msp430x.ac";
const char *archc_version="2.4.1";
const char *archc_options="-abi ";

#include  <iostream>
#include  <systemc.h>
#include  "ac_stats_base.H"
#include  "msp430x.H"

#include "sytare-syscalls/syscalls.h"

int sc_main(int ac, char *av[])
{

  //!  ISA simulator
  msp430x msp430x_proc1("msp430x");

#ifdef AC_DEBUG
  ac_trace("msp430x_proc1.trace");
#endif 

  msp430x_proc1.init(ac, av);
  msp430x_proc1.set_prog_args();
  cerr << endl;

  Syscalls syscalls;
  syscalls.print();

  sc_start();

  msp430x_proc1.PrintStat();
  cerr << endl;

#ifdef AC_STATS
  ac_stats_base::print_all_stats(std::cerr);
#endif 

#ifdef AC_DEBUG
  ac_close_trace();
#endif 

  return msp430x_proc1.ac_exit_status;
}
