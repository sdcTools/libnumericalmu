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

#if !defined RankSwap_h
#define RankSwap_h

// =========================================================================
// -------------------------------------------------------------------------
//                     RANK SWAPPING HEADER FILE
//                                      Author: Josep Domingo-Ferrer
//                                      	Francesc Sebe
//                                      	Josep M. Mateo
//                                      	Antoni Martinez-Balleste
//                                      	Angel Torres
//                                      	Narcis Macia
//                                              Universitat Rovira i Virgili
//                                              July 2001
// -------------------------------------------------------------------------
// =========================================================================

#define RNK_OK	        1      // Rankswap ended OK
#define RNK_ERR_PER 	0      // Percent value must be in [0, 100]
#define RNK_ERR_RGS     -1     // Number of registers wrongly defined
#define RNK_ERR_COL     -2     // Number of columns wrongly defined

class CRankSwap 
{
public:
	long rankswap(double **data,long n_regs,long n_columns,long percent, long *prog);
private:
	void quicksort_with_info(double *vector,long inf,long sup,long *sort_info);
	void swap_f(double *a, double *b);
	void swap_i(long *a, long *b);
	void partition_with_info(double *vector,long inf,long sup, double x, long *k, long *sort_info);
	void rankswap_vector(double vector[],long n_columns, long n_regs,long percent, long *prog);
	void generate_swap(long swap[],long n_columns, long n_regs,long percent, long *prog );
	void unsort_with_info(double vector[],long n_regs,long sort_info[]);
	long long_min(long a,long b);
	long long_max(long a,long b);
};


#endif
