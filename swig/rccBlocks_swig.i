/* -*- c++ -*- */

#define RCCBLOCKS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "rccBlocks_swig_doc.i"

%{
#include "rccBlocks/rayleighChan_cc.h"
%}


%include "rccBlocks/rayleighChan_cc.h"
GR_SWIG_BLOCK_MAGIC2(rccBlocks, rayleighChan_cc);
