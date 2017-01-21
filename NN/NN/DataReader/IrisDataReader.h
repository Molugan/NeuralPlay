//
//  IrisDataReader.h
//  NN
//
//  Created by Morgane Riviere on 18/01/17.
//  Copyright © 2017 Morgane Riviere. All rights reserved.
//

#ifndef IrisDataReader_h
#define IrisDataReader_h

#include <stdio.h>
#include <string>
#include <vector>

bool LoadIrisDataSet(const std::string pathFile, std::vector< std::vector<float> >& inputDataSet, std::vector<int> &inputLabels);

int GetLabelFromName(const std::string name);

#endif /* IrisDataReader_h */
