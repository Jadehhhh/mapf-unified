#include "Args.h"
#include "LaCamPlanner.h"

int main(int argc, char** argv){
  LaCAMArgs args;
  if(!ParseLaCAM(argc, argv, args)) return 1;
  LaCamPlanner p;
  return p.run(args)?0:2;
}
