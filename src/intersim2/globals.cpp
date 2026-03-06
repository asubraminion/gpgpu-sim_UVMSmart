// Global variables from main.cpp (extracted to avoid main() conflict)
#include <iostream>
#include <cstdio>
using namespace std;

class TrafficManager;
class InterconnectInterface;

InterconnectInterface *g_icnt_interface = nullptr;
TrafficManager * trafficManager = nullptr;

bool gPrintActivity;
int gK;  // radix
int gN;  // dimension
int gC;  // concentration
int gNodes;
bool gTrace;
ostream * gWatchOut;

// cuobjdump_in is needed as a global but cuobjdump_parse is in cuobjdump_parser.o
FILE *cuobjdump_in = nullptr;
