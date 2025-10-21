#include "Args.h"
#include "RHCRPlanner.h"

int main(int argc, char** argv){
  RHCRArgs args;
  if(!ParseRHCR(argc, argv, args)) return 1;
  RHCRPlanner p;
  return p.run(args)?0:2;
}
