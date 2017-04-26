#ifndef RETURNFILELIST_H
#define RETURNFILELIST_H

#include <iostream>

#include <vector>
#include <string>

#include "dirent.h"
#include "sys/stat.h"

#include "UserCode/diall/include/checkMakeDir.h"

const int listSizeLimit = 10000;

std::vector<std::string> returnFileList(std::string dirPath, const std::string filterStr = ".txt", int currentSize = 0)
{
  bool doFilter = true;
  if(filterStr.size() == 0) doFilter = false;

  std::vector<std::string> fileList;

  DIR *dpdf = opendir(dirPath.c_str());
  struct dirent *epdf;

  std::string interString = "";
  if(dirPath.substr(dirPath.size()-1, 1).find("/") == std::string::npos) interString = "/";

  if(dpdf != NULL){
    epdf = readdir(dpdf);
    while(epdf){
      std::string tempStr = epdf->d_name;

      if(tempStr.find(".") != std::string::npos && tempStr.size() == 1){
	epdf = readdir(dpdf);
	continue;
      }
      else if(tempStr.find("..") != std::string::npos && tempStr.size() == 2){
	epdf = readdir(dpdf);
	continue;
      }

      std::string fullStr = dirPath + interString + tempStr;

      if(checkDir(fullStr)){
	std::vector<std::string> tempStrVect = returnFileList(fullStr, filterStr, currentSize + fileList.size());
        for(int iter = 0; iter < (int)tempStrVect.size(); iter++){
          if(tempStrVect.at(iter).find(filterStr.c_str()) != std::string::npos || !doFilter) fileList.push_back(tempStrVect.at(iter));
        }

	if(fileList.size() + currentSize > listSizeLimit){
	  std::cout << "Exceeded limit on number of files (" << listSizeLimit << "). Return." << std::endl;
	  return fileList;
	}
      }
      else{
        if(fullStr.find(filterStr.c_str()) != std::string::npos || !doFilter) fileList.push_back(fullStr);

	if(fileList.size() + currentSize > listSizeLimit){
	  std::cout << "Exceeded limit on number of files (" << listSizeLimit << "). Return." << std::endl;
	  return fileList;
	}
      }

      epdf = readdir(dpdf);
    }
  }
  else{
    std::cout << "NULL PATH" << std::endl;
    return fileList;
  }

  return fileList;
}

#endif
