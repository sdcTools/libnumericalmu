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

#include <math.h>
#include "Numerical.h"
#include "MicroAggregation.h"
#include "recordSwap.h"
#include <float.h>
#include <locale.h>
#include <string>

/////////////////////////////////////////////////////////////////////////////
// Targeted record swapping

void Numerical::DoTargetedRecordSwap(/*[in]*/ std::string inFileName, /*[in]*/ std::string outFileName, 
                          /*[in]*/ std::string separator, /*[in]*/ int numVar, /*[in]*/ double swaprate,
                          /*[in]*/ int* similar, /*[in]*/ int nSim, /*[in]*/ int* hierarchy, /*[in]*/ int nHier, 
                          /*[in]*/ int* riskVars, /*[in]*/ int nRisk, /*[in]*/ int* carry, /*[in]*/ int nCarry,
                          /*[in]*/ int hhID, /*[in]*/ int k_anonymity, /*[in]*/ int seed, /*[in,out]*/ long* errorCode){
    
    long numberOfLines;
    std::vector< std::vector<int> > inputdata = ReadFromFileForTRS(inFileName, separator, numVar, &numberOfLines, errorCode);
    if (errorCode[0] != 0)
        return;
    
    //std::vector<int> similarRS(nSim);
    std::vector< std::vector<int> > similarRS(1); // Only one similarity profile for now
    similarRS[0].resize(nSim);                    // Only one similarity profile for now

    std::vector< std::vector<double> > risk;      // not yet implemented => risk.size() = 0
    double risk_threshold = 0;                    // not yet implemented => risk_threshold = 0
    
    std::vector<int> hierarchyRS(nHier);
    std::vector<int> riskRS(nRisk);
    std::vector<int> carryRS(nCarry);
    
    for (int i=0; i<nSim; i++) similarRS[0][i] = similar[i]; // Only one similarity profile for now

    for (int i=0; i<nHier; i++) hierarchyRS[i] = hierarchy[i]; // Hierarchy variables (regions for swapping)

    for (int i=0; i<nRisk; i++) riskRS[i] = riskVars[i];  // Risk variables for k-anonymity
    
    for (int i=0; i<nCarry; i++) carryRS[i] = carry[i];   // Variables to be "carried along" while swapping

/*    data.resize(numVar);
    for (int i=0; i<numVar; i++)
    {
        data[i].resize(numberOfLines);
        for (int j=0; j<numberOfLines; j++){
            data[i][j] = (int) inputdata[j][i];   // Need to transpose the data for the time being
        }
    }
*/  
    //inputdata = recordSwap(inputdata, similarRS, hierarchyRS, riskRS, hhID, th, swaprate, carryRS, seed);
//    recordSwap(std::vector< std::vector<int> > data, 
//                                           int hid,
//                              std::vector<int> hierarchy, 
//               std::vector< std::vector<int> > similar,
//                                        double swaprate,
//            std::vector< std::vector<double> > risk,                 
//                                        double risk_threshold,      
//                                           int k_anonymity, 
//                               std::vector<int> risk_variables,  
//                               std::vector<int> carry_along,
//                                           int seed = 123456)
    
    inputdata = recordSwap(inputdata, hhID, hierarchyRS, similarRS, swaprate, risk, risk_threshold, k_anonymity, riskRS, carryRS, seed);
    
    WriteOutputTRS(outFileName, separator, numVar, numberOfLines, inputdata);
}

/////////////////////////////////////////////////////////////////////////////
// CCatalaanCtrl



//void CCatalaanCtrl::FireUpdateProgress (short Perc)
//{ Fire_UpdateProgress(Perc); }

void Numerical::DoMicroAggregation(std::string inFileName, std::string outFileName,
        std::string separator, long nVar,
        long nMinRecPerGr, long nGroupsPerVar,
        long typeSolution, long *nColumnsPerGr,
        long *errorCode) {
    long numberOfRecords;

    double** inputData = ReadFromFile(inFileName, separator, nVar, &numberOfRecords, errorCode);
    if (inputData == NULL) {
        return;
    }

    long* var;
    //The variables are already sorted in the inputfile
    var = new long [nVar];
    for (int i = 0; i < nVar; i++) {
        var[i] = i;
    }

    CMicroAggregation microAggr;
    long result = microAggr.Microaggregation(nVar, numberOfRecords, nMinRecPerGr,
            nGroupsPerVar, nColumnsPerGr, var,
            inputData, typeSolution);

    if (result != MIC_OK) {
        *errorCode = result;
        return;
    }
        
    WriteOutput(outFileName, separator, nVar, numberOfRecords, inputData);

    //Do The deletes
    //NB: var was already deleted in the MicroAggregation function;
    for (int i = 0; i < numberOfRecords; i++) {
        delete[] inputData[i];
    }
    delete[] inputData;
    *errorCode = 0;
}



void Numerical::SetProgressListener(IProgressListener2* progressListener) {
    //TODO
    ;
}

void Numerical::DoRankSwap(std::string inFileName,
        std::string outFileName,
        std::string separator,
        long numVar,
        long perc,
        long *errorCode) {
    long numberOfLines;
    double** inputdata = ReadFromFile(inFileName, separator, numVar, &numberOfLines, errorCode);
    if (inputdata == NULL)
        return;

    CRankSwap oRankSwap;
    long progress = 0;
    long result = oRankSwap.rankswap(inputdata, numberOfLines, numVar, perc, &progress);
    if (result != MIC_OK) {
        *errorCode = result;
        return;
    }
    
    WriteOutput(outFileName, separator, numVar, numberOfLines, inputdata);

    for (int i = 0; i < numberOfLines; i++) {
        delete inputdata[i];
    }
    delete inputdata;
    *errorCode = 0;
}   

double** Numerical::ReadFromFile(std::string inFileName,
        std::string separator,
        long nVar,
        long* numberOfLines,
        long* errorCode) {
    
	FILE *fdin;
	char str[MAXRECORDLENGTH];
	std::string stempstr;
        std::string stemp;
	int iseppos;
	double **inputdata;
	double d;
	
        
        setlocale(LC_NUMERIC, "C");
        
	fdin = fopen (inFileName.c_str(), "r");
	if (fdin == 0) {
		*errorCode = -1;
		return NULL;
	}
	
        
	str[0] = 0;
	fgets((char *)str, MAXRECORDLENGTH, fdin);
	if (str[0] == 0) {
            *errorCode = -1;
            return NULL;
	}
	
	rewind(fdin);
	long icount = 0;  // length of file
	while (!feof(fdin)) {
		fgets((char *) str, MAXRECORDLENGTH, fdin);
		icount ++;
	}

	*numberOfLines = icount;

	inputdata = new double*[*numberOfLines];
	for (long i=0; i<*numberOfLines; i++)  {
	
		inputdata[i] = new double [nVar];
		if (!inputdata[i])
		{
                    *errorCode = MIC_OUT_MEM;
                    return NULL;
		}
        }
    

	rewind(fdin);
	long recnr = 0;
	while (!feof(fdin)) {
		fgets((char *)str, MAXRECORDLENGTH, fdin);
                
		stempstr = std::string(str);
		iseppos = stempstr.find(separator,0);
		int pos = 0;
		icount = 0;
		while (iseppos != -1)  {
			stemp=stempstr.substr(pos,iseppos);
			//sVariableCode.SetAt(icount,stemp);
			//Check if it is a numerical value 
			//inputdata[recnr][icount] = strtod(stemp,&stop);
			if (!ConvertNumeric(stemp.c_str(), d) ) {
				//return ISNOTNUMERIC;   // is not numeric! (variabelenummer meegeven voor gebruiker)
			}

			inputdata[recnr][icount] = d;

			icount ++;
                        pos = iseppos+1;
			iseppos = stempstr.find(separator,pos);

		}
                stemp = stempstr.substr(pos);

		if ((stempstr.length() == 0) || (icount < nVar-1) || (icount > nVar))
		{
			//return false;

		}
		else
		{
		//	sVariableCode.SetAt(icount,stempstr);
			if (!ConvertNumeric(stemp.c_str(), d) ) {
				//return ISNOTNUMERIC;   // is not numeric! (variabelenummer meegeven voor gebruiker)
			}

			inputdata[recnr][icount] = d;

		}
		recnr++;
	}

	
	fclose(fdin);
        *errorCode = 0;
        return inputdata;
    
}

std::vector< std::vector<int> > Numerical::ReadFromFileForTRS(std::string inFileName,
               std::string separator, long nVar, long* numberOfLines, long* errorCode) {
    
	FILE *fdin;
	char str[MAXRECORDLENGTH];
	std::string stempstr;
        std::string stemp;
	int iseppos;
	std::vector< std::vector<int> > inputdata;
	int d;
	        
        setlocale(LC_NUMERIC, "C");
        
	fdin = fopen (inFileName.c_str(), "r");
	if (fdin == 0) {
		*errorCode = -1;
		return inputdata;
	}
	        
	str[0] = 0;
	fgets((char *)str, MAXRECORDLENGTH, fdin);
	if (str[0] == 0) {
            *errorCode = -1;
            return inputdata;
	}
	
	rewind(fdin);
	long icount = 0;  // length of file
	while (!feof(fdin)) {
		fgets((char *) str, MAXRECORDLENGTH, fdin);
		icount ++;
	}

	*numberOfLines = icount;
        
        // For time being:
        // inputdata is transposed datafile !!!!
        inputdata.resize(*numberOfLines);

	for (long i=0; i<*numberOfLines; i++)  {
            inputdata[i].resize(nVar);
        }
    
	rewind(fdin);
	long recnr = 0;
	while (!feof(fdin)) {
		fgets((char *)str, MAXRECORDLENGTH, fdin);
                
		stempstr = std::string(str);
		iseppos = stempstr.find(separator,0);
		int pos = 0;
		icount = 0;
		while (iseppos != -1)  {
			stemp=stempstr.substr(pos,iseppos);

			inputdata[recnr][icount] = atoi(stemp.c_str());
                        //inputdata[icount][recnr] = atoi(stemp.c_str());    // Transpose matrix for time being, see line 327 as well

			icount ++;
                        pos = iseppos+1;
			iseppos = stempstr.find(separator,pos);
		}
                stemp = stempstr.substr(pos);

		if ((stempstr.length() == 0) || (icount < nVar-1) || (icount > nVar))
		{
			//return false;
		}
		else
		{
    		    inputdata[recnr][icount] = atoi(stemp.c_str());
                    //inputdata[icount][recnr] = atoi(stemp.c_str());   // Transpose matrix for time being
		}
		recnr++;
	}

	
	fclose(fdin);
        *errorCode = 0;
        
        return inputdata;
}


bool Numerical::ConvertNumeric(const char* code, double &d)
{ char *stop;
  d = strtod(code, &stop);

  if (*stop != 0) {
    while (*(stop) == ' ') stop++;
    if (*stop != 0) {
      return false;
    }
  }

  return true;
}

void Numerical::WriteOutput(std::string outFileName, std::string seperator, 
								long nVar, long nRec,
								double **out_Data)
{
	FILE *fdout;
	long i,j;
	fdout = fopen(outFileName.c_str(),"w");
	
	for (i=0; i<nRec; i++) {
	
		for (j=0; j <nVar; j++) {
			fprintf(fdout,"%f", out_Data[i][j]);
			if (j <nVar-1) {
				fprintf(fdout,"%s",seperator.c_str());
			}
	
		}
		if (i<nRec-1) {
			fprintf(fdout,"\n");
		}
	}

	fclose (fdout);
}

void Numerical::WriteOutputTRS(std::string outFileName, std::string seperator, 
								long nVar, long nRec,
								std::vector< std::vector<int> > out_Data)
{
	FILE *fdout;
	long i,j;
	fdout = fopen(outFileName.c_str(),"w");
	
	for (i=0; i<nRec; i++) {
		for (j=0; j <nVar; j++) {
                        fprintf(fdout,"%d", out_Data[i][j]);
			//fprintf(fdout,"%d", out_Data[j][i]);    // Transpose back
			if (j <nVar-1) {
				fprintf(fdout,"%s",seperator.c_str());
			}
	
		}
		if (i<nRec-1) {
			fprintf(fdout,"\n");
		}
	}
	fclose (fdout);
}
