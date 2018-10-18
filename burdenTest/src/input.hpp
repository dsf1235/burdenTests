//
//  input.hpp
//  burdenTest
//
//  Created by Corin Thummel on 7/6/18.
//  Copyright © 2018 Corin Thummel. All rights reserved.
//

#ifndef input_hpp
#define input_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <map>
#include <gsl/gsl_matrix.h>

using namespace std;

class readInput
{
    
public:
    readInput(string dir, string testType, string vcfType, string vcfFile1, string region, string phenoFile, string covFile);
    
    void readVcfInitialInfo(string filename);
    void readCaseCount(string filename);
    void readPhenotype(string phenoFile);
    void readMaf(string filename);
    void makePositionFile(string filename);
    void mergeData(string filename);
    void bcfInput(string filename);
    void readGenes(string filename);
    
    int getCaseCount(){return caseCount;}
    gsl_matrix* getGslGenotype(){return genotypeGslMatrix;}
    gsl_matrix* getCovariates(){return covariates;}
    gsl_vector* getMaf(){return maf;}
    gsl_vector* getMaf(string geneName){return geneMaf[geneName];}
    gsl_vector* getPheno(){return pheno;}
    map<string, gsl_matrix*> getGeneSubsets(){return genes;}
    

private:
    void parseGenes(string chromosome, vector<string> *geneName);
    int variantCount;
    int subjectCount;
    int caseCount;
    string variantRegion;
    string testType;
    string vcfType;
    string bcftools_loc = "../../externals/bin/bcftools";
    string bgzip_loc = "../../externals/bin/bgzip";
    string externals_loc = "../../externals/bin/";
    string testDir;
    regex gMatch;
    regex altAlleleCountMatch;
    regex mafMatch;
    regex subjectCountMatch;
    regex variantCountMatch;
    regex headerMatch;
    regex posMatch;

    ifstream inputFile;
    
    //vector<string> vcfChrom;
    vector<int> vcfPos;

    map<string, vector<int> > posMap;
    map<string, gsl_matrix *> genes;
    map<string, gsl_vector *> geneMaf;

    gsl_matrix* genotypeGslMatrix;
    gsl_matrix* covariates;
    gsl_vector* maf;
    gsl_vector* pheno;
};

#endif /* input_hpp */
