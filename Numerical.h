/*
* Argus Open Source
* Software to apply Statistical Disclosure Control techniques
* 
* Copyright 2014 Statistics Netherlands
* 
* This program is free software; you can redistribute it and/or 
* modify it under the terms of the European Union Public Licence 
* (EUPL) version 1.1, as published by the European Commission.
* 
* You can find the text of the EUPL v1.1 on
* https://joinup.ec.europa.eu/software/page/eupl/licence-eupl
* 
* This software is distributed on an "AS IS" basis without 
* warranties or conditions of any kind, either express or implied.
*/

// CatalaanCtrl.h : Declaration of the CCatalaanCtrl

#ifndef __CATALAANCTRL_H_
#define __CATALAANCTRL_H_

#include <string>
#include <vector>
#include "RankSwap.h"
#include "IProgressListener2.h"
#define MAXRECORDLENGTH  10000      // main symbols

// errors

#define MIC_OK        1      // Everything is OK
#define MIC_OUT_MEM   0      // The routine has run out of memory
#define MIC_ERR_NEL   -1     // Number of records wrongly defined
#define MIC_ERR_NVAR  -2     // Number of variables wrongly defined
#define MIC_ERR_MEG   -3     // Minimum number of records in a group wrongly defined
#define MIC_ERR_GOV   -4     // Number of groups of variables wrongly defined
#define MIC_ERR_VPG   -5     // Grouping of variables is wrongly defined
#define MIC_ERR_COL   -6     // Bad definition of column sorting
#define MIC_ERR_STD   -7     // There was an error while calculating STDEV

/////////////////////////////////////////////////////////////////////////////
// CCatalaanCtrl
class Numerical
{
public:
	Numerical()
	{
	}

// ICatalaanCtrl
public:
        void DoTargetedRecordSwap(/*[in]*/ std::string inFileName, /*[in]*/ std::string outFileName, 
                                  /*[in]*/ std::string separator, /*[in]*/ int numVar, /*[in]*/ double swaprate,
                                  /*[in]*/ int* similar, /*[in]*/ int nSim, /*[in]*/ int* hierarchy, /*[in]*/ int nHier, 
                                  /*[in]*/ int* risk, /*[in]*/ int nRisk, 
                                  /*[in]*/ int hhID, /*[in]*/ int th, /*[in]*/ int seed, /*[in,out]*/ long* errorCode);
	void DoRankSwap(/*[in]*/ std::string inFileName, /*[in]*/ std::string outFileName, /*[in]*/  std::string separator, /*[in]*/ long numVar, /*[in]*/ long perc, /*[in,out]*/ long *errorCode);
	void DoMicroAggregation(/*[in]*/ std::string inFileName, /*[in]*/ std::string outFileName,  /*[in]*/ std::string separator, /*[in]*/ long nVar, /*[in]*/ long nMinRecPerGr, /*[in]*/ long nGroupsPerVar, /*[in]*/ long typeSolution, /*[in,out]*/  long  *nColumnsPerGr, /*[in,out]*/  long  *errorCode);
        void SetProgressListener(IProgressListener2* progressListener);
private:
	typedef struct
	{
	double   *costs;
	double   cost;
	long      link;
	} t_node;

typedef struct
{
  long      n_nodes;
  t_node   *nodes;
} t_graph;

	bool ConvertNumeric(const char* code, double &d);
        double** ReadFromFile(std::string inFileName,std::string separator,long nVar, long* numberOfLines, long* errorCode);
        std::vector< std::vector<int> > ReadFromFileForTRS(std::string inFileName, std::string separator, long nVar, long* numberOfLines, long* errorCode);
	void WriteOutput(std::string outFileName, std::string separator, long nVar, long nRec, double **out_Data);
        void WriteOutputTRS(std::string outFileName, std::string seperator, long nVar, long nRec, std::vector< std::vector<int> > out_Data);
	/*
        long Microaggregation(long  n_var, long n_elements,
					 long elms_p_group, long group_var, 
					 long *how_many,long *var, 
					 double **out_data, int optim);
	long Do_Opt_Mic ( long n_el, long n_var, long k, double **out_data, long * prog);
	void Quick_Sort(double *vector, long *bo, long inf, long sup);
	long Opt_Mic ( t_graph g, long k, long *bo, double *v, double **out_data, long var, long vars, long *prog);
	long Graph ( long k, long n, double *v, long *bo, t_graph *g);
	double Sum_Quad_Err ( double *fp, long y, long n);
	double Mean ( double *fp, long y, long n);
*/
	
};

#endif //__CATALAANCTRL_H_
