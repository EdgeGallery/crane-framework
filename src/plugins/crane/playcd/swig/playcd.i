%module craneplaycd 

//////////////////////////////////////////////////
%include "std_string.i"

%{
#include "playImplCD.h"
#include "wrapplaycd.h"
%}

//////////////////////////////////////////////////

//extern unsigned playcd_play(PlayerImplCD* playcd, std::string str);
extern unsigned playcd_play(crane::PluginBaseInterface* playcd, std::string str);
