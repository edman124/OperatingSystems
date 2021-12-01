#include "fat_internal.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
using namespace std;
Fat32BPB *bpb;
char *addr;
map<int, DirEntry> usedFD;
set<int> availableFD;
// char **FAT_table;
bool fat_mount(const std::string &path) {
	const char* filepath = path.c_str();
	int fd = open(filepath, 0);
	char* buffer = (char *)malloc(512);
	int r = read(fd, (void *)buffer, 512);
	// uint bytesPerCluster = bpb->BPB_BytsPerSec*bpb->BPB_SecPerClus;
	bpb = (Fat32BPB *)buffer;
	if(r!=512)
	{
		printf("%d\n", r);
		return false;
	}
	// printf("%d\n", bpb->BPB_TotSec32*bpb->BPB_BytsPerSec);
	if((addr = (char *)mmap((void *)0, 
		bpb->BPB_TotSec32*bpb->BPB_BytsPerSec, 
		PROT_WRITE | PROT_READ | PROT_EXEC, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		printf("failed mapping\n");
		return false;
	}
	for (int i=0; i<128; i++)
	{
		availableFD.insert(i);
	}
	// FAT_table = (char **)malloc(bpb->BPB_BytsPerSec*4);
	// for (uint i = 0; i<bpb->BPB_BytsPerSec; i++ )
	// {
	// 	FAT_table[i] = (char *)malloc(4);
	// 	memcpy(FAT_table[i], (const void *)(addr + bpb->BPB_RsvdSecCnt*bpb->BPB_BytsPerSec + 4*i), 4);
	// }
	// printf("rsvp sectors: %d\n", bpb->BPB_RsvdSecCnt);
	// printf("root clus: %d\n", bpb->BPB_RootClus);
	// printf("newAddr: %x\n",*addr);

    return true;
}

int fat_open(const std::string &path) {
	size_t found = path.find_last_of('/');
	string tempPath;
	if (found != 0)
		tempPath = path.substr(0, found);
	else
		tempPath = path.substr(0, found+1);
	string fileName = path.substr(found+1);
	vector<AnyDirEntry> curDirectory = fat_readdir(tempPath);
	if(curDirectory.size()==0 || fileName.size()==0)
	{
		return -1;
	}
    transform(fileName.begin(), fileName.end(), fileName.begin(), ::toupper);
    DirEntry foundFile;
    int foundIt = false;
	for (const AnyDirEntry &entry : curDirectory) 
	{
		if (entry.dir.DIR_Attr & DirEntryAttributes::DIRECTORY)
		{
			continue;
		}
		std::string file_name = std::string(entry.dir.DIR_Name, entry.dir.DIR_Name + 8); //from shell
		while (file_name.back() == ' ') file_name.pop_back();
        if (entry.dir.DIR_Name[8] != ' ') {
            file_name += '.';
            file_name += std::string(entry.dir.DIR_Name + 8, entry.dir.DIR_Name + 11);
            while (file_name.back() == ' ') file_name.pop_back();
        }
        transform(file_name.begin(), file_name.end(), file_name.begin(), ::toupper);
        if (file_name.compare(fileName)==0)
        {
        	foundFile = entry.dir;
        	foundIt = true;
        	break;
        }
	}
	if(!foundIt || availableFD.size()==0)
	{
    	return -1;
    }
    //allocate file descriptor
    //find available fd
    auto it = availableFD.begin();
    usedFD.insert({*it, foundFile});
    int tempRet = *it;
    availableFD.erase(it);
    return tempRet;
}

bool fat_close(int fd) {
	auto it = usedFD.find(fd);
	if(it == usedFD.end())
	{
		return false;
	}
	availableFD.insert(fd);
	usedFD.erase(it);
    return true;
}

int fat_pread(int fd, void *buffer, int count, int offset) {
	auto it = usedFD.find(fd);
	if(it == usedFD.end())
	{
		return -1;
	}
	DirEntry entry = usedFD[fd];
	uint fileSize = entry.DIR_FileSize;
	if((uint)offset > fileSize || count == 0)
	{
		return 0;
	}
	uint curCluster = entry.DIR_FstClusLO | (entry.DIR_FstClusHI << 16);
	uint rootDirSectors = ((bpb->BPB_rootEntCnt*32) + (bpb->BPB_BytsPerSec - 1))/bpb->BPB_BytsPerSec;
	uint firstDataSector = bpb->BPB_RsvdSecCnt + bpb->BPB_FATSz32*bpb->BPB_NumFATs + rootDirSectors;
	uint minSize = min(fileSize-(uint)offset, (uint)count);
	uint bytesPerCluster = bpb->BPB_BytsPerSec*bpb->BPB_SecPerClus;
	uint temp = minSize;
	// uint tempOffset = offset;
	// int iteration = 0;
	while((uint)offset > bytesPerCluster)
	{
		uint fat_offset = curCluster*4;
		// // uint fat_sector = first_fat_sector + (fat_offset/bpb->BPB_BytsPerSec);
		// uint ent_offset = fat_offset % bpb->BPB_BytsPerSec;
		curCluster = *(uint*)(addr + (bpb->BPB_RsvdSecCnt)*bpb->BPB_BytsPerSec + fat_offset) & 0x0FFFFFFF;
		if (curCluster >= 0x0FFFFFF7)
		{
			return 0;
		}
		offset -= bytesPerCluster;
	}
	char *tempBuffer = (char *)buffer;
	while(temp)
	{
		char* memAddr = (addr + ((curCluster - 2)*bpb->BPB_SecPerClus + firstDataSector)*bpb->BPB_BytsPerSec);
		memAddr += offset;
		uint byteToCopy = temp;
		if (byteToCopy > bytesPerCluster - offset)
		{
			byteToCopy = bytesPerCluster - offset;
			// temp-= bytesPerCluster;
		}
		offset = 0;
		temp -= byteToCopy;
		memcpy((void *)tempBuffer, (void *)memAddr, byteToCopy);
		tempBuffer = (char *)(tempBuffer + byteToCopy);
		uint fat_offset = curCluster*4;
		// // uint fat_sector = first_fat_sector + (fat_offset/bpb->BPB_BytsPerSec);
		// uint ent_offset = fat_offset % bpb->BPB_BytsPerSec;
		curCluster = *(uint*)(addr + (bpb->BPB_RsvdSecCnt)*bpb->BPB_BytsPerSec + fat_offset) & 0x0FFFFFFF;
		if (curCluster >= 0x0FFFFFF7)
		{
			break;
		}
	}
    return minSize;
}
	
void getDir(vector<AnyDirEntry> &directories, uint startCluster)
{
	// uint bytesPerCluster = bpb->BPB_BytsPerSec*bpb->BPB_SecPerClus;
	// uint first_fat_sector = bpb->BPB_RsvdSecCnt;

	uint curCluster = startCluster;
	uint rootDirSectors = ((bpb->BPB_rootEntCnt*32) + (bpb->BPB_BytsPerSec - 1))/bpb->BPB_BytsPerSec;
	uint firstDataSector = bpb->BPB_RsvdSecCnt + bpb->BPB_FATSz32*bpb->BPB_NumFATs + rootDirSectors;
	while(true)
	{
		// printf("memaddr: %x\n", *(addr + ((curCluster - 2)*bpb->BPB_SecPerClus + firstDataSector)*bpb->BPB_BytsPerSec));
		// printf("memaddr diff: %x\n", (curCluster*bytesPerCluster));
		for(char *memAddr = (addr + ((curCluster - 2)*bpb->BPB_SecPerClus + firstDataSector)*bpb->BPB_BytsPerSec); 
			memAddr < (addr + ((curCluster - 1)*bpb->BPB_SecPerClus + firstDataSector)*bpb->BPB_BytsPerSec); memAddr += sizeof(DirEntry))
		{
			if (memAddr[0] == 0)
				return;
			char buffer[sizeof(DirEntry)];
			memcpy(buffer, (const void *)(memAddr), sizeof(DirEntry));
			// printf("%s\n",buffer );
		    DirEntry *root = (DirEntry *)buffer;
		    AnyDirEntry temp;
		    temp.dir = *root;
		    directories.push_back(temp);
		}
		// char *buffer = (char *)malloc(bytesPerCluster);
		// buffer = addr + curCluster*bytesPerCluster;
		// DirEntry *temp = (DirEntry *)buffer;
		// directories = temp;
		// directories.insert(directories.begin(), temp, temp+bytesPerCluster);
		uint fat_offset = curCluster*4;
		// // uint fat_sector = first_fat_sector + (fat_offset/bpb->BPB_BytsPerSec);
		// uint ent_offset = fat_offset % bpb->BPB_BytsPerSec;
		curCluster = *(uint*)(addr + (bpb->BPB_RsvdSecCnt)*bpb->BPB_BytsPerSec + fat_offset) & 0x0FFFFFFF;
		if (curCluster >= 0x0FFFFFF7)
		{
			return;
		}
		// return;
	}
}
vector<AnyDirEntry> fat_readdir(const std::string &path) {
	// const char* filepath = path.c_str();
	// int fd = open(filepath, 0);
	// char *buffer[512];
    vector<AnyDirEntry> result;
    if(!bpb)
    {
    	return result;
    }
    // printf("readdir: %d\n", bpb->BPB_RsvdSecCnt);
    // printf("readdir addr: %x\n", *addr);
	// uint bytesPerCluster = bpb->BPB_BytsPerSec*bpb->BPB_SecPerClus;
    // char buffer[sizeof(DirEntry)];
    // buffer[0]='\0';
    // printf("%s\n", buffer);
    // memcpy(buffer, (const void *)(addr + bpb->BPB_RootClus*bytesPerCluster), sizeof(DirEntry));
    // DirEntry *root = (DirEntry *)buffer;
    // AnyDirEntry temp;
    // temp.dir = *root;
    // result.push_back(temp);
    if(path[0] != '/')
    {
    	return result;
    }
    getDir(result, bpb->BPB_RootClus);
    std::vector<string> pathItems;
    size_t pos = 0;
    string token;
    string tempPath = path.substr(1,string::npos) + "/";

    // printf("dirs: \n")	;
    while((pos = tempPath.find("/")) != string::npos)
    {
    	token = tempPath.substr(0, pos);
		// printf("%s, ", token.c_str());
    	pathItems.push_back(token);
    	tempPath.erase(0, pos + 1);
    }
    // printf("%s\n", tempPath.substr(0, string::npos).c_str());
    for(uint i=0; i< pathItems.size(); i++)
    {
    	uint nextClus = 0;
    	for (const AnyDirEntry &entry : result) 
    	{
    		if (!(entry.dir.DIR_Attr & DirEntryAttributes::DIRECTORY))
    		{
    			continue;
    		}
    		std::string file_name = std::string(entry.dir.DIR_Name, entry.dir.DIR_Name + 8); //from shell
    		while (file_name.back() == ' ') file_name.pop_back();
	        if (entry.dir.DIR_Name[8] != ' ') {
	            file_name += '.';
	            file_name += std::string(entry.dir.DIR_Name + 8, entry.dir.DIR_Name + 11);
	            while (file_name.back() == ' ') file_name.pop_back();
	        }
	        transform(pathItems[i].begin(), pathItems[i].end(), pathItems[i].begin(), ::toupper);
	        transform(file_name.begin(), file_name.end(), file_name.begin(), ::toupper);
	        if (file_name.compare(pathItems[i])==0)
	        {
	        	nextClus = entry.dir.DIR_FstClusLO | (entry.dir.DIR_FstClusHI << 16);
	        	if(nextClus == 0)
	        	{
	        		nextClus = bpb->BPB_RootClus;
	        	}
	        	break;
	        }
    	}

    	if(i != pathItems.size()-1 || nextClus != 0)
    	{
    		// printf("clearing\n");
    		result.clear();
    	}
    	if(nextClus)
    	{
    		getDir(result, nextClus);
    	}
    }
    return result;
}
