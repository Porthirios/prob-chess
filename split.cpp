#include <stdio.h>
#include <string.h>
#include <stdlib.h>

main(int argc, const char** argv) {
  const char* fname=NULL, *foutfmt="%s.part%d", *outname=NULL;
  int maxsize=40*0x100000;
  size_t size=0;
  char* str=NULL;
  for(int i=1; i<argc; i++)
    if(!strcmp(argv[i],"-s"))
      maxsize=atoi(argv[++i]);
    else if(!strcmp(argv[i],"-o"))
      outname=argv[++i];
    else if(!strcmp(argv[i],"-f"))
      foutfmt=argv[++i];
    else fname=argv[i];
  FILE* file=(fname?fopen(fname,"r"):stdin);
  if(fname && !file) {
    fprintf(stderr,"Не могу открыть файл %s\n", fname);
    return 1;
  }
  if(!outname) outname=fname?fname:"noname";
  int l=0,cnt=1,ls;
  char buf[1024];
  sprintf(buf,foutfmt,outname,cnt);
  FILE* fout=fopen(buf,"w");
  if(!fout) {
    fprintf(stderr,"Не могу открыть файл %s для записи\n",buf);
    return 2;
  }
  while((ls=getline(&str,&size,file))>0) {
    if(l && l+ls>maxsize) {
      sprintf(buf,foutfmt,outname,++cnt);
      fclose(fout);
      fout=fopen(buf,"w");
      if(!fout) {
	fprintf(stderr,"Не могу открыть файл %s для записи\n",buf);
	return 2;
      }
      l=0;
    }
    fputs(str,fout);
    l+=ls;
  }
  fclose(file);
  return 0;
}
